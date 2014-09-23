/**
 * \addtogroup CkLdb
*/
/*@{*/

/*
 status:
*/

#include <algorithm>

#include "charm++.h"
#include "ckheap.h"
#include "elements.h"

#include "PowerCapRefineLB.h"
#include "RefinerPower.C"

using namespace std;

CreateLBFunc_Def(PowerCapRefineLB, "Move chares from overloaded processors to underloaded processors.")

PowerCapRefineLB::PowerCapRefineLB(const CkLBOptions &opt): CentralLB(opt)
{
	lbname = "PowerCapRefineLB";
	if (CkMyPe()==0){
		CkPrintf("[%d] PowerCapRefineLB created\n",CkMyPe());
	#if CMK_LB_USER_DATA
		CkpvAccess(_lb_obj_index) = LBRegisterObjUserData(sizeof(LBUserDataStruct));
		CkPrintf("PowerCapLB: size of struct = %lu\n",sizeof( LBUserDataStruct));
		if (_lb_args.debug() >= 1) 
			CkPrintf("In PowerCapRefineLB: LB user data is enabled\n");
	#else
		if (_lb_args.debug() >= 1) 
			CkPrintf("In PowerCapRefineLB: LB user data is disabled\n");
	#endif
	}
}

void PowerCapRefineLB::computePeSpeed(LDStats * stats, int * from_procs)
{
	int n_pes = stats->nprocs();
	int n_objs = stats->n_objs;
	int * workload = new int[n_pes];
	double * abs_speed = new double[n_pes];

	for(int i = 0; i < n_pes; i++){
		workload[i] = 0;
	}

	for(int obj = 0; obj < n_objs; obj++){
		LDObjData & odata = stats->objData[obj];
	#if CMK_LB_USER_DATA
		if (_lb_args.debug() > 0) 
			CkPrintf("PowerRefineLB: Using user defined workload\n");
		LBUserDataStruct * udata = (LBUserDataStruct *)
			odata.getUserData(CkpvAccess(_lb_obj_index));
		workload[ stats->from_proc[obj] ] += udata->idx;
	#else
		if (_lb_args.debug() > 0) 
			CkPrintf("PowerRefineLB: Using default workload\n");
		workload[ stats->from_proc[obj] ] += default_load;
	#endif
	}

	int working_pes = n_pes;
	for(int i = 0; i < n_pes; i++){
		double obj_walltime = stats->procs[i].total_walltime - stats->procs[i].idletime - stats->procs[i].bg_walltime;
		if(obj_walltime <= 0.000001){
			working_pes--;
			abs_speed[i] = -1;
		}
		else{
			abs_speed[i] = workload[i]/obj_walltime;
		}
	}
	//For each pe, calculate pe speed
	double total = 0;
	for(size_t i = 0; i < n_pes; i++){
		if(abs_speed[i] >= 0)
			total += abs_speed[i];
	}
	double ave_speed =  total/working_pes;

	for(int i = 0; i < n_pes; i++){
		if(abs_speed[i] >= 0)
			stats->procs[i].pe_speed = abs_speed[i]/ave_speed;
	}
	delete [] workload;
	delete [] abs_speed;
	//End of calculation of pe_speed
	
  int obj;

  // get original object mapping
  for(obj=0;obj<stats->n_objs;obj++)  {
    int pe = stats->from_proc[obj];
    from_procs[obj] = pe;
  }
}

void PowerCapRefineLB::work(LDStats* stats)
{
	CkPrintf("enter work func\n");
	int obj;
	int n_pes = stats->nprocs();
  	int* from_procs = RefinerPower::AllocProcs(n_pes, stats);

	computePeSpeed(stats, from_procs);

  // Get a new buffer to refine into
  int* to_procs = RefinerPower::AllocProcs(n_pes, stats);

  RefinerPower refiner(1.003);  // overload tolerance=1.05

  refiner.Refine(stats->nprocs(), stats, from_procs, to_procs);

  // Save output
  for(obj=0;obj<stats->n_objs;obj++) {
		int pe = stats->from_proc[obj];
		if (to_procs[obj] != pe) {
			if (_lb_args.debug()>=2) {
				CkPrintf("[%d] Obj %d migrating from %d to %d\n",
				CkMyPe(),obj,pe,to_procs[obj]);
			}
			stats->to_proc[obj] = to_procs[obj];
		}
	}

	if (_lb_args.metaLbOn()) {
		stats->after_lb_max = refiner.computeAverageLoad();
		stats->after_lb_avg = refiner.computeMax();
		stats->is_prev_lb_refine = 0;
		if (_lb_args.debug() > 0)
			CkPrintf("PowerCapRefineLB> After lb max load: %lf avg load: %lf\n", stats->after_lb_max, stats->after_lb_avg);
	}

	//Free up memory
	RefinerPower::FreeProcs(from_procs);
	RefinerPower::FreeProcs(to_procs);
}

#include "PowerCapRefineLB.def.h"

/*@}*/
