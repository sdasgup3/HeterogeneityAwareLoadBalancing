/**
 * \addtogroup CkLdb
*/
/*@{*/

#ifndef _HETEROAWARELB_H_
#define _HETEROAWARELB_H_

#include "CentralLB.h"
#include "Refiner.h"
#include "HeteroAwareLB.decl.h"

class minheap;
class maxheap;

void CreateRefineLB();
BaseLB *AllocateRefineLB();

class HeteroAwareLB : public CentralLB {
protected:
  computeInfo *computes;
  processorInfo *processors;
  minHeap *pes;
  maxHeap *computesHeap;
  int P;
  int numComputes;
  double averageLoad;

  double overLoad;

public:
  HeteroAwareLB(const CkLBOptions &);
  HeteroAwareLB(CkMigrateMessage *m):CentralLB(m) { lbname = (char *)"HeteroAwareLB"; }
  void work(LDStats* stats);
  void calculate(int proc_id,double* wall_times, int* resultant,int max_procs, int max_objs, LDStats* stats);
private:
  bool QueryBalanceNow(int step) { return true; }

protected:
};

#endif /* _HETEROAWARELB_H_ */

/*@}*/
