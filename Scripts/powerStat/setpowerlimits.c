#include "rapl.h"
#include "stdio.h"
#include "stdlib.h"

int main(int argc, char **argv){
  if(argc < 3){
    printf("Usage: ./setpowerlimit <pkg power limit> <dram power limit>");
    exit(1);
  }
  init_rapl();
  pkg_rapl_power_limit_control_t pkg_plc;
  pkg_plc.power_limit_watts_1 = atoi(argv[1]);
  pkg_plc.limit_time_window_seconds_1 = 0.1;
  pkg_plc.limit_enabled_1 = 1;
  pkg_plc.clamp_enabled_1 = 1;
  pkg_plc.lock_enabled = 0;

  set_pkg_rapl_power_limit_control(0, &pkg_plc);

  dram_rapl_power_limit_control_t dram_plc;
  get_dram_rapl_power_limit_control(0, &dram_plc);
  dram_plc.limit_enabled = 1;
  dram_plc.limit_time_window_seconds=0.1;
  dram_plc.power_limit_watts = atoi(argv[2]);
  set_dram_rapl_power_limit_control(0, &dram_plc);

  return terminate_rapl();
}
