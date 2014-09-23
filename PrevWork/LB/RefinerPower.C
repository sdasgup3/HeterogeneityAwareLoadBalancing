/**
 * \addtogroup CkLdb
*/
/*@{*/

/** This code is derived from RefineLB.C, and RefineLB.C should
 be rewritten to use this, so there is no code duplication
*/

#include "RefinerPower.h"

using namespace std;

CkpvExtern(int, _lb_obj_index);

int* RefinerPower::AllocProcs(int count, BaseLB::LDStats* stats)
{
  return new int[stats->n_objs];
}

void RefinerPower::FreeProcs(int* bufs)
{
  delete [] bufs;
}

void RefinerPower::create(int count, BaseLB::LDStats* stats, int* procs)
{
  int i;

  // now numComputes is all the computes: both migratable and nonmigratable.
  // afterwards, nonmigratable computes will be taken off

  numAvail = 0;
  for(i=0; i < P; i++) {
    processors[i].Id = i;
    processors[i].backgroundLoad = 0;
    processors[i].load = 0; // computeLoad/pe_speed
    processors[i].computeLoad = 0; // Absolute load: sum of loads of each chare on this proc
    processors[i].computeSet = new Set();
    processors[i].pe_speed = stats->procs[i].pe_speed;
//    processors[i].utilization = stats->procs[i].utilization;
    processors[i].available = stats->procs[i].available;
    if (processors[i].available) numAvail++;
  }

  for (i=0; i<stats->n_objs; i++)
  {
		LDObjData &odata = stats->objData[i];
		computes[i].Id = i;
		computes[i].id = odata.objID();
	#if CMK_LB_USER_DATA
		LBUserDataStruct * udata = (LBUserDataStruct *)
			stats->objData[i].getUserData(CkpvAccess(_lb_obj_index));
		// was cpuTime, now is passed in by application or constant
		computes[i].load = udata->idx;	
	#else
		computes[i].load = default_load;
	#endif
		computes[i].processor = -1;
		computes[i].oldProcessor = procs[i];
		computes[i].migratable = odata.migratable;
		if (computes[i].oldProcessor >= P)  {
			if (stats->complete_flag)
				CmiAbort("LB Panic: the old processor in RefineLB cannot be found, is this in a simulation mode?");
			else {
				// an object from outside domain, randomize its location
				computes[i].oldProcessor = CrnRand()%P;
			}
		}
  }
}

void RefinerPower::assign(computeInfo *c, int processor)
{
  assign(c, &(processors[processor]));
}

void RefinerPower::assign(computeInfo *c, processorInfo *p)
{
   c->processor = p->Id;
   p->computeSet->insert((InfoRecord *) c);
   p->computeLoad += c->load;
   p->load= p->computeLoad / p->pe_speed;
}

void  RefinerPower::deAssign(computeInfo *c, processorInfo *p)
{
   c->processor = -1;
   p->computeSet->remove(c);
   p->computeLoad -= c->load;
   p->load = p->computeLoad / p->pe_speed;
}

double RefinerPower::computeAverageLoad() {
  computeAverage();
  return averageLoad;
}

void RefinerPower::computeAverage()
{
  int i;
  double total = 0;
  for (i=0; i<numComputes; i++) 
		total += computes[i].load;
  averageLoad = total/P;
}

double RefinerPower::computeMax()
{
  int i;
  double max = -1.0;
  for (i=0; i<P; i++) {
    if (processors[i].available && processors[i].load > max){
      max = processors[i].load;
		}
  }
  return max;
}

int RefinerPower::isHeavy(processorInfo *p)
{
  if (p->available) 
     return p->load > overLoad*averageLoad;
  else {
     return p->computeSet->numElements() != 0;
  }
}

int RefinerPower::isLight(processorInfo *p)
{
  if (p->available) 
     return p->load < averageLoad;
  else 
     return 0;
}

// move the compute jobs out from unavailable PE
void RefinerPower::removeComputes()
{
  int first;
  Iterator nextCompute;

  if (numAvail < P) {
    if (numAvail == 0) 
			CmiAbort("No processor available!");
	  for (first=0; first<P; first++){
			if (processors[first].available == true) 
				break;
			for (int i=0; i<P; i++) {
				if (processors[i].available == false) {
					computeInfo *c = (computeInfo *)
					processors[i].computeSet->iterator((Iterator *)&nextCompute);
					while (c) {
						deAssign(c, &processors[i]);
						assign(c, &processors[first]);
						nextCompute.id++;
						c = (computeInfo *)processors[i].computeSet->next((Iterator *)&nextCompute);
					}
				}
			}
  	}
	}
}

int RefinerPower::refine()
{
  int i;
  int finish = 1;
  maxHeap *heavyProcessors = new maxHeap(P);

  Set *lightProcessors = new Set();
  for (i=0; i<P; i++) {
    if (isHeavy(&processors[i])) {  
      //      CkPrintf("Processor %d is HEAVY: load:%f averageLoad:%f!\n",
      //	       i, processors[i].load, averageLoad);
      heavyProcessors->insert((InfoRecord *) &(processors[i]));
    } else if (isLight(&processors[i])) {
      //      CkPrintf("Processor %d is LIGHT: load:%f averageLoad:%f!\n",
      //	       i, processors[i].load, averageLoad);
      lightProcessors->insert((InfoRecord *) &(processors[i]));
    }
  }
  int done = 0;

  while (!done) {
    double bestSize;
    computeInfo *bestCompute;
    processorInfo *bestP;
    
    processorInfo *donor = (processorInfo *) heavyProcessors->deleteMax();
    if (!donor) 
			break;

    //find the best pair (c,receiver)
    Iterator nextProcessor;
    processorInfo *p = (processorInfo *)
			lightProcessors->iterator((Iterator *) &nextProcessor);
    bestSize = 0;
    bestP = 0;
    bestCompute = 0;

    while (p) {
      Iterator nextCompute;
      nextCompute.id = 0;
      computeInfo *c = (computeInfo *) 
			donor->computeSet->iterator((Iterator *)&nextCompute);
      while (c) {
        if (!c->migratable) {
	  			nextCompute.id++;
				  c = (computeInfo *) 
	    		donor->computeSet->next((Iterator *)&nextCompute);
          continue;
        }
				//CkPrintf("c->load: %f p->load:%f overLoad*averageLoad:%f \n",
				//c->load, p->load, overLoad*averageLoad);
				if ( c->load + p->load < overLoad*averageLoad) {
	  			if(c->load > bestSize) {
				    bestSize = c->load;
				    bestCompute = c;
				    bestP = p;
				  }
				}
				nextCompute.id++;
				c = (computeInfo *) 
				  donor->computeSet->next((Iterator *)&nextCompute);
      }
      p = (processorInfo *) 
			lightProcessors->next((Iterator *) &nextProcessor);
    }

    if (bestCompute) {
      //      CkPrintf("Assign: [%d] with load: %f from %d to %d \n",
      //	       bestCompute->id.id[0], bestCompute->load, 
      //	       donor->Id, bestP->Id);
      deAssign(bestCompute, donor);      
      assign(bestCompute, bestP);
    } else {
      finish = 0;
      break;
    }

    if (bestP->load > averageLoad)
      lightProcessors->remove(bestP);
    
    if (isHeavy(donor))
      heavyProcessors->insert((InfoRecord *) donor);
    else if (isLight(donor))
      lightProcessors->insert((InfoRecord *) donor);
  }  

  delete heavyProcessors;
  delete lightProcessors;

  return finish;
}

int RefinerPower::multirefine()
{
  computeAverage();
  double avg = averageLoad;
  double max = computeMax();

  const double overloadStep = 0.01;
  const double overloadStart = 1.001;
  double dCurOverload = max / avg;
                                                                                
  int minOverload = 0;
  int maxOverload = (int)((dCurOverload - overloadStart)/overloadStep + 1);
  double dMinOverload = minOverload * overloadStep + overloadStart;
  double dMaxOverload = maxOverload * overloadStep + overloadStart;
  int curOverload;
  int refineDone = 0;
  if (_lb_args.debug()>=1)
    CmiPrintf("dMinOverload: %f dMaxOverload: %f\n", dMinOverload, dMaxOverload);
                                                                                
  overLoad = dMinOverload;
  if (refine())
    refineDone = 1;
  else {
    overLoad = dMaxOverload;
    if (!refine()) {
      CmiPrintf("ERROR: Could not refine at max overload\n");
      refineDone = 1;
    }
  }
                                                                                
  // Scan up, until we find a refine that works
  while (!refineDone) {
    if (maxOverload - minOverload <= 1)
      refineDone = 1;
    else {
      curOverload = (maxOverload + minOverload ) / 2;
                                                                                
      overLoad = curOverload * overloadStep + overloadStart;
      if (_lb_args.debug()>=1)
      CmiPrintf("Testing curOverload %d = %f [min,max]= %d, %d\n", curOverload, overLoad, minOverload, maxOverload);
      if (refine())
        maxOverload = curOverload;
      else
        minOverload = curOverload;
    }
  }
  return 1;
}

void RefinerPower::Refine(int count, BaseLB::LDStats* stats, 
		     int* cur_p, int* new_p)
{
  //  CkPrintf("[%d] Refiner strategy\n",CkMyPe());

  P = count;
  numComputes = stats->n_objs;
  computes = new computeInfo[numComputes];
  processors = new processorInfo[count];

  create(count, stats, cur_p);

  int i;
  for (i=0; i<numComputes; i++)
    assign((computeInfo *) &(computes[i]),
           (processorInfo *) &(processors[computes[i].oldProcessor]));

  removeComputes();
  computeAverage();

  if (_lb_args.debug()>2)  {
    CkPrintf("Old PE load (bg load): ");
    for (i=0; i<count; i++) CkPrintf("%d:%f(%f) ", i, processors[i].load, processors[i].backgroundLoad);
    CkPrintf("\n");
  }

  multirefine();

  int nmoves = 0;
  for (int pe=0; pe < P; pe++) {
    Iterator nextCompute;
    nextCompute.id = 0;
    computeInfo *c = (computeInfo *)
      processors[pe].computeSet->iterator((Iterator *)&nextCompute);
    while(c) {
      new_p[c->Id] = c->processor;
      if (new_p[c->Id] != cur_p[c->Id]) nmoves++;
//      if (c->oldProcessor != c->processor)
//      CkPrintf("Refiner::Refine: from %d to %d\n", c->oldProcessor, c->processor);
      nextCompute.id++;
      c = (computeInfo *) processors[pe].computeSet->
	             next((Iterator *)&nextCompute);
    }
  }
  if (_lb_args.debug()>2)  {
    CkPrintf("New PE load: ");
    for (i=0; i<count; i++) CkPrintf("%f ", processors[i].load);
    CkPrintf("\n");
  }
  if (_lb_args.debug()>1) { 
    CkPrintf("RefinerPower: moving %d obejcts. \n", nmoves);
		computeAverage();
		double maxLoad = computeMax();
		CkPrintf("RefinerPower: max load = %lf, ave load = %lf\n", maxLoad, averageLoad);
	}
  delete [] computes;
  delete [] processors;
}

/*@}*/
