/**
 * \addtogroup CkLdb
*/
/*@{*/

#ifndef _POWERCAPREFINELB_H_
#define _POWERCAPREFINELB_H_
//#undef CMK_LB_USER_DATA

#include "CentralLB.h"
#include "RefinerPower.h"
#include "PowerCapRefineLB.decl.h"

const double default_load = 1;

void CreatePowerCapRefineLB();
BaseLB * AllocatePowerCapRefineLB();

class PowerCapRefineLB : public CentralLB {

public:
  PowerCapRefineLB(const CkLBOptions &);
  PowerCapRefineLB(CkMigrateMessage *m):CentralLB(m) { lbname = "PowerCapRefineLB"; }
  void work(LDStats* stats);
private:
	void computePeSpeed(LDStats * stats, int * from_procs);
};

#endif /* _HEAPCENTLB_H_ */

/*@}*/
