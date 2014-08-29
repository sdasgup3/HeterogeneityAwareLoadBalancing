CHARMC=../../../bin/charmc -O3  $(OPTS) -tracemode projections #-balancer RefineLB

OBJS = jacobi2d.o

all: jacobi2d

jacobi2d: $(OBJS)
	$(CHARMC)  -language charm++ -o jacobi2d $(OBJS)

jacobi2d.decl.h: jacobi2d.ci
	$(CHARMC)  jacobi2d.ci

clean:
	rm -rf conv-host *.o *.h *.log.gz jacobi2d charmrun *~

jacobi2d.o: jacobi2d.C jacobi2d.decl.h
	$(CHARMC) -c jacobi2d.C
