#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <highlevelmonitorconfigurationapi.h>

#ifndef N_DEBUG
#include <stdio.h>
#endif

#pragma comment(lib, "dxva2.lib")
#pragma comment(lib, "user32.lib")

static void 
SetBrightness(HANDLE hMonitor, DWORD new_brightness) 
{
  DWORD physical_monitors_count = 0;
  LPPHYSICAL_MONITOR physical_monitors = 0;
  BOOL success = FALSE;
  
  success = GetNumberOfPhysicalMonitorsFromHMONITOR(hMonitor, &physical_monitors_count);
  if(success) 
  {
    physical_monitors = (LPPHYSICAL_MONITOR)malloc(physical_monitors_count * sizeof(PHYSICAL_MONITOR));
    if(physical_monitors != 0) 
    {
      success = GetPhysicalMonitorsFromHMONITOR(hMonitor, physical_monitors_count, physical_monitors);
      if(success) 
      {
        for(DWORD phyiscal_monitor_idx = 0; 
            phyiscal_monitor_idx < physical_monitors_count; 
            ++phyiscal_monitor_idx) 
        {
          success = SetMonitorBrightness(physical_monitors[phyiscal_monitor_idx].hPhysicalMonitor, new_brightness);
          if(!success) 
          {
#ifndef N_DEBUG
            printf("Failed to set brightness for montior %d", i);
#endif
          }
        }
        
        success = DestroyPhysicalMonitors(physical_monitors_count, physical_monitors);
      }
      free(physical_monitors);
    }
  }
}

int WINAPI 
WinMain(HINSTANCE instance, HINSTANCE prev_instance, PSTR command_line, int show_code)
{
  HMONITOR hMonitor = MonitorFromWindow(GetDesktopWindow(), MONITOR_DEFAULTTOPRIMARY);
  while(1)
  {
    SYSTEMTIME local_time = {0};
    GetLocalTime(&local_time);
    
    WORD hour = local_time.wHour;
    
    if(hour >= 7 && hour < 18)
    {
      SetBrightness(hMonitor, 80);
    }
    else if(hour >= 18 && hour < 20)
    {
      SetBrightness(hMonitor, 50);
    }
    else if(hour >= 20)
    {
      SetBrightness(hMonitor, 20);
    }
    
    DWORD seconds_to_sleep = 5*60;
    Sleep(seconds_to_sleep*1000);
  }
  return(0);
}