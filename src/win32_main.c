#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <highlevelmonitorconfigurationapi.h>

#ifndef N_DEBUG
#include <stdio.h>
#endif

#pragma comment(lib, "dxva2.lib")
#pragma comment(lib, "user32.lib")

#define ArrayCount(arr) (sizeof(arr)/sizeof(*arr))

typedef struct ConfigEntry ConfigEntry;
struct ConfigEntry
{
  WORD hour;
  DWORD brightness;
};

static HMONITOR monitors[32] = {0};
static int monitors_count;

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

static BOOL 
MonitorEnumCallback(HMONITOR hmonitor, HDC hdc, LPRECT rect, LPARAM user_data)
{
  monitors_count = 0;
  monitors[monitors_count++] = hmonitor;
  BOOL result = TRUE;
  return(result);
}

int WINAPI 
WinMain(HINSTANCE instance, HINSTANCE prev_instance, PSTR command_line, int show_code)
{
  
  ConfigEntry entries[] = 
  {
#define X(hour, brightness) {hour, brightness},
#include "config_entries.c"
#undef X
  };
  
  while(1)
  {
    RECT rect = {0};
    EnumDisplayMonitors(0, 0, MonitorEnumCallback, 0);
    
    SYSTEMTIME local_time = {0};
    GetLocalTime(&local_time);
    
    WORD hour = local_time.wHour;
    
    for(int config_entry_idx = 0; config_entry_idx < ArrayCount(entries); ++config_entry_idx)
    {
      ConfigEntry *entry = entries + (ArrayCount(entries)-config_entry_idx-1);
      if(hour >= entry->hour)
      {
        for(int i = 0; i < monitors_count; ++i)
        {
          SetBrightness(monitors[i], entry->brightness);
        }
        break;
      }
    }
    
    DWORD seconds_to_sleep = 5*60;
    Sleep(seconds_to_sleep*1000);
  }
  return(0);
}