Abstract:

Different processors across the nodes have different execution times for the same workloads. This performance imbalance is seen only when the CPU power is capped to low power values. This performance imbalance causes increased execution times of the parallel applications. We propose a power aware load balancer (Using Charm++) which minimizes the performance imbalance at the lower power caps by tackling this heterogeneity.


The proposal  is two enhance the number of parameters to decide on the optimum power cap of  nodes and using genetic algorithms to reach the optimal value.
The parameters could be selected from the (1) application under consideration or (2) from the charm++ runtime or (3) the cpu parameters.
The examples of (2) can be found from charm/ tmp/ckgraph.h and of (3) can be found from charm/tmp/TempAware*LB.C

Also two understand the work already done on this we should go through some of the papers from  http://charm.cs.uiuc.edu/people/osmansarood

So in brief the goal is that the proposed lbd will decide on the optimal power distribution,  poser cap the cores accordingly  and based on that will distribute the chares loads on them.


Proposal
========

The proposed lbd will Optimize the numbers of nodes (n), the CPU power level
(pc) and memory power level (pm), that minimizes execution
time (t) of an application under a strict power budget (P), and based on the current distribution will migrate the chares on the nodes.

Dynamic Load Redistribution Under Varying Power Budgets
Abstract: An application running on multiple nodes in a cluster is likely to be subjected to varying power caps. Such demands may arise due to non-steady wall power input to the cluster, or due to scheduling of multiple applications on the same cluster. In such a case, the Charm runtime should be able to expand (shrink) the application to different number of nodes, and also regulate the power to those nodes so as to stay within the allocated power budget. With many power states available for the CPU and memory (through RAPL), and with multiple options for the node count, selecting the best performing (throughput) configuration is a challenge. The extremely large search space precludes runtime techniques for finding this configuration and offline methods are usually explored. Our proposal tries to mitigate this problem by using a load redistributor that dynamically selects the close-to-best solution from the search space within realistic timing constraints. Offline methods also suffer from the issue of varying node performance i.e. if the throughput from the nodes varies over time, say due to cluster upgrades, then the data points collected during offline measurements become stale. A dynamic redistributor, once coded in, can saved the manual labor.
Idea: 
<main>
  1. N X P1 X P2 : search space
  2. Load Redistributor picks out *few* of them for sampling (Flicker uses 2 techniques to sample). It runs these on the cluster and gathers throughput data.
  3. Based on these sampled data points, a response surface model is created for T (throughput/performance). See Flicker for details on response surface models. Flicker uses RBF (Radially Baised Functions). Basically these produce function the T(n,p1,p2) after reading in the sampled data points. We can use this function to get the T at any [n,p1,p2] now.
  4. We now need to find the [n,p1,p2] such that T is maximized under the constraint that n*p1*p2 < PowerBudgetAllocated to app. Again we can't search the entire space N X P1 X P2. Flicker uses genetic algorithm to find the close-to-best (n,p1,p2)
</main>
