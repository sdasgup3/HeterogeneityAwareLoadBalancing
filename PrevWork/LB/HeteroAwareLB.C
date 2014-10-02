/**
 * \addtogroup CkLdb
*/
/*@{*/

#include "elements.h"
#include "ckheap.h"
#include "HeteroAwareLB.h"

  typedef struct record {
	double time;
	int id;
	} record;

CreateLBFunc_Def(HeteroAwareLB, "Move objects away from overloaded processor to reach average")

HeteroAwareLB::HeteroAwareLB(const CkLBOptions &opt): CentralLB(opt)
{
  lbname = (char *)"HeteroAwareLB";
  if (CkMyPe() == 0)
    CkPrintf("[%d] HeteroAwareLB created\n",CkMyPe());
}
int compare (const void * a, const void * b)
{
  double  aT = ((record *)a)->time;
  double  bT = ((record *)b)->time;
  if(aT > bT) {
    return 1;
  } else if(aT < bT) {
     return -1;
  } else {
     return 0;
  }
}

void HeteroAwareLB::calculate(int proc_id,double* wall_times, int* resultant,int max_procs, int max_objs, LDStats* stats)
{
  double sum = 0.0;
  int residual;
  record *R =  (record *) malloc(max_procs*sizeof(record)); 

  for(int i=0;i<max_procs;i++){         
    sum += wall_times[proc_id]/wall_times[i];         
  }
  resultant[proc_id] = int ((max_objs)/(sum) + 0.5);
	  
  if(proc_id == max_procs - 1){

    int total_objs_assigned = 0;
    for(int i=0;i<max_procs;i++){
      total_objs_assigned +=resultant[i];
    }
    int residual = total_objs_assigned - max_objs;

    printf("residual count (res(+ more/-less) %d)\n", residual);

    for(int pe; pe < max_procs ; pe++) {
      R[pe].time =  wall_times[pe];
      R[pe].id = pe;
    }
    qsort(R, max_procs, sizeof(R[0]),compare);
    printf("Sorted Record\n");
    for(int pe; pe < max_procs ; pe++) {
      printf("(id - %d time  - %lf  allocated - %d)\n", R[pe].id, R[pe].time, resultant[R[pe].id]);
    }

    if(residual > 0){
      int j=1;
      for(int i=0;i<residual;i++){
        printf("Removing residual load from %d\n",R[j].id);
        resultant[R[j++].id] --;
      }
    } else if(residual < 0) {
      residual = -1*residual;
      int j=max_procs -1;
      for(int i=0;i<residual;i++){
        //printf("Adding residual load to %d\n",R[j].id);
        //resultant[R[j--].id] ++;
        printf("Adding residual load to 0");
        resultant[0] ++;
      }
    }
  }
}

void HeteroAwareLB::work(LDStats* stats)
{
  int obj;
  int n_pes   = stats->nprocs();
  int n_objs  = stats->n_objs;


  int* final_dist     = new int[n_pes];
  int* initial_dist = new int[n_pes];

  /********* Getting Initial Dist ***********/
  for(int pe=0;pe<n_pes;pe++)  {
    initial_dist[pe] = 0;
  }
  for(obj=0;obj<n_objs;obj++)  {
    initial_dist[stats->from_proc[obj]] ++;
  }

  double* bgTime      = new double[n_pes];
  double* objTime     = new double[n_pes];
  double* wall_times  = new double[n_pes];
  double* idleTime    = new double[n_pes];

  for(int i=0;i< n_pes;i++){
    objTime[i]    = 0.0;
  }
  for(int obj = 0 ; obj<n_objs;obj++)  {
    objTime[stats->from_proc[obj]] +=  stats->objData[obj].wallTime;
  }

  for(int i=0;i< n_pes;i++){
    bgTime[i]     = (stats->procs[i]).bg_walltime;
    idleTime[i]   = (stats->procs[i]).idletime;
    //wall_times[i] = (stats->procs[i]).total_walltime; //  - (stats->procs[i]).idletime; 
    wall_times[i] = objTime[i]; //  - (stats->procs[i]).idletime; 
  }

  //wall_times[0] = wall_times[5];
  
  // calculate the new load for each of the proc
  int *resultant = new int[n_pes];
  for(int i=0;i< n_pes;i++){
    calculate(i, wall_times, resultant,n_pes,n_objs,stats);
  }

  printf("After reshuffling\n");
  for(int i=0;i< n_pes;i++){
    final_dist[i]=resultant[i];
    printf("procid %d (w:%lf bg:%lf obj:%lf i:%lf) :%d->%d\n",i,wall_times[i],bgTime[i], objTime[i], idleTime[i],initial_dist[i],final_dist[i]);
  }
  

  int obj_cnt = 0;
  for(int i=0;i< n_pes;i++){
    int k = final_dist[i];
    int j = 0;
    while(j<k){
      stats->to_proc[obj_cnt] = i;
      obj_cnt++;
      j++;
    }
  }
  printf("the total object count after LB is %d\n",obj_cnt);
              

  delete [] final_dist;
  delete [] wall_times;
  delete [] resultant;
}

#include "HeteroAwareLB.def.h"

/*@}*/
