#CHARMC=../../../bin/charmc -O3  $(OPTS)  -balancer  PowerCapRefineLB 
CHARMC=../../../bin/charmc -O3  $(OPTS)  -balancer  HeteroAwareLB 
#CHARMC=../../../bin/charmc -O3  $(OPTS)  -balancer   RefineLB 

OBJS = jacobi2d.o

all: jacobi2d

jacobi2d: $(OBJS)
	$(CHARMC) -tracemode projections  -language charm++ -o jacobi2d $(OBJS) 

jacobi2d.decl.h: jacobi2d.ci
	$(CHARMC)  jacobi2d.ci

clean:
	rm -rf conv-host *.o *.h *.log.gz jacobi2d charmrun *~
	rm -f *.log.gz *.projrc *.topo *.sts log

jacobi2d.o: jacobi2d.C jacobi2d.decl.h
	$(CHARMC) -c jacobi2d.C
