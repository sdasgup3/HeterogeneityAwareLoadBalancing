Abstract:

Different processors across the nodes have different execution times for the same workloads. This performance imbalance is seen only when the CPU power is capped to low power values. This performance imbalance causes increased execution times of the parallel applications. We propose a power aware load balancer (Using Charm++) which minimizes the performance imbalance at the lower power caps by tackling this heterogeneity.


The proposal  is two enhance the number of parameters to decide on the optimum power cap of  nodes and using genetic algorithms to reach the optimal value.
The parameters could be selected from the (1) application under consideration or (2) from the charm++ runtime or (3) the cpu parameters.
The examples of (2) can be found from charm/ tmp/ckgraph.h and of (3) can be found from charm/tmp/TempAware*LB.C

Also two understand the work already done on this we should go through some of the papers from  http://charm.cs.uiuc.edu/people/osmansarood

So in brief the goal is that the proposed lbd will decide on the optimal power distribution,  poser cap the cores accordingly  and based on that will distribute the chares loads on them.
