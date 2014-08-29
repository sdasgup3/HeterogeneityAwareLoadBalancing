#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#define core_msr   0x639
#define cpu_msr    0x611
#define IA32_PERF_CTL 0x199
#define IA32_PERF_STATUS 0x198
#define MSR_IA32_MPERF          0x000000e7
#define MSR_IA32_APERF          0x000000e8
#define IA32_TIME_STAMP_COUNTER 0x00000010
#define IA32_MISC_ENABLE 0x1A0
#define mem_msr		0x619
#define uncore_msr 0x642
#define energy_unit_msr 0x606
#define ENERGY_UNIT_OFFSET      0x08
#define ENERGY_UNIT_MASK        0x1F00

void mytime_(double *val)
{
        struct timeval end;
        long xx;
        *val =0.0;
        gettimeofday(&end, NULL);
        xx = end.tv_sec * 1000000 + end.tv_usec;
        *val = ((double)xx)/1000000.0;

}


void getpower_()
{
  char path[100];
  char res[256];
  sprintf(path, "/dev/ttyUSB0");

  uint64_t msr_value=-1;
  int retval = 0;

  int fd = open(path, O_RDONLY);
  if (fd >= 0)
{
    
while(1)
{

char mybuf[1000];
//int ret1=100;
int ret1 = read(fd, &mybuf, 256);

//if(ret1>80)
{
//printf("oo xx=%s ret=%d\n",mybuf,ret1);
char delims[] = ",";
char *result = NULL;
double power=-1.0;
//printf("ret=%d 111 xx=%s\n",ret1,mybuf);

result = strtok( mybuf, delims );
int n=0;
while( result != NULL ) {
n++;
    if(n==4)
    {
     power= (double) atof(result)/10; 
//printf("RRRRRRRRRRRR xx=%s pow=%f\n",result,power);
     if(power<20 || power>120) power=0;
//	if(power<40 || power>170) power=0;
//     if(ret1<85 || ret1>90) 
	if(ret1<85 || ret1>97)
     {
  	printf("THE RET CODE IS BAD ret=%d\n",ret1);
 	power=0;
     }

     if(power!=0) 
     {
//       printf( "%.1f\n", power);
printf( "%.1f %d \n", power,ret1);
         FILE *ff=fopen("power","w");
        fprintf(ff,"%.1f\n",power);
        fclose(ff);

     }
     else 
     {
       printf( "%.1f\n", 1000.1);
         FILE *ff=fopen("power","w");
        fprintf(ff,"%.1f\n",power);
        fclose(ff);
       printf( "JUNK VALUE NOT WRITING NEW VALUE\n");
     }

    }
    result = strtok( NULL, delims );
}

}
//usleep(2000000);
}
}
  else printf("++ 77 Can not read MSR id=%d\n",core_msr);
  close(fd);

 printf("Coming -- MACHINE POWER ---------------- valu=%s ret=%d\n",res,retval);
}

uint64_t rdmsr(uint32_t msr_id, int cpu)
{
  char path[100];
  sprintf(path, "/dev/cpu/%d/msr", cpu);

  uint64_t msr_value;
  int retval = 0;

  int fd = open(path, O_RDONLY);
  if (fd >= 0)
    retval = pread(fd, &msr_value, sizeof(msr_value), msr_id);
  else printf("++ 00 Can not read MSR id=%d\n",msr_id);
  close(fd);

  return retval == sizeof(msr_value) ? msr_value : 0;
}

void getpowerunit_(double *val)
{
                uint32_t value;
                uint64_t msr_output = rdmsr(energy_unit_msr, 0);
                value = (msr_output & ENERGY_UNIT_MASK) >> ENERGY_UNIT_OFFSET;
                unsigned int energy_unit_divisor = 1 << value;
		*val = (double) energy_unit_divisor;
//	printf("============ energy_unit_divisor=%d\n",energy_unit_divisor);
}

void mperf_(double *msr_value1)
{

  char path[100];
  sprintf(path, "/dev/cpu/%d/msr", 0);

  uint64_t msr_value=-1;
  int retval = 0;

  int fd = open(path, O_RDONLY);
  if (fd >= 0)
    retval = pread(fd, &msr_value, sizeof(msr_value), MSR_IA32_MPERF);
  else printf("++ Can not read MSR id=%d\n",core_msr);
  close(fd);

 *msr_value1=(double) msr_value;
}



void perfctl_(int64_t *msr_value1,int msr_add)
{

  char path[100];
  sprintf(path, "/dev/cpu/%d/msr", 0);

  uint64_t msr_value=-1;
  int retval = 0;

  int fd = open(path, O_RDONLY);
  if (fd >= 0)
    retval = pread(fd, &msr_value, sizeof(msr_value), msr_add);
  else printf("++ Can not read MSR id=%d\n",core_msr);
  close(fd);

 *msr_value1=(int64_t) msr_value;
}

void write_perfctl(int64_t new_value,int msr_add,int cpu)
{
  char path[100];
  sprintf(path, "/dev/cpu/%d/msr", cpu);

  uint64_t msr_value=-1;
  int retval = 0;

  int fd = open(path, O_RDWR);
  if (fd >= 0)
    retval = pwrite(fd, &new_value, sizeof(new_value), msr_add);
  else printf("++ 11 Can not write MSR id=%d\n",core_msr);
  close(fd);
}

void aperf_(double *msr_value1)
{

  char path[100];
  sprintf(path, "/dev/cpu/%d/msr", 0);

  uint64_t msr_value=-1;
  int retval = 0;

  //int fd = open(path, O_RDONLY);
	int fd = open( path, O_RDWR );
	FILE *nf = fopen("newfile","w");
  if (fd >= 0)
	{
    retval = pread(fd, &msr_value, sizeof(msr_value), MSR_IA32_APERF);
		fprintf(nf,"%f\n",(double) msr_value);
		fflush(nf);
	}
  else printf("++ 22 Can not read MSR id=%d err:%d\n",core_msr,fd);
  close(fd);

 *msr_value1=(double) msr_value;
}


void corepower_(double *msr_value1)
{

  char path[100];
  sprintf(path, "/dev/cpu/%d/msr", 0);

  uint64_t msr_value=-1;
  int retval = 0;

  int fd = open(path, O_RDONLY);
  if (fd >= 0)
    retval = pread(fd, &msr_value, sizeof(msr_value), core_msr);
  else printf("++ 33 Can not read MSR id=%d\n",core_msr);
  close(fd);

 *msr_value1=(double) msr_value;
}

void cpupower_(double *msr_value1)
{

  char path[100];
  sprintf(path, "/dev/cpu/%d/msr", 0);

  uint64_t msr_value=-1;
  int retval = 0;

  int fd = open(path, O_RDONLY);
  if (fd >= 0)
    retval = pread(fd, &msr_value, sizeof(msr_value), cpu_msr);
  else printf("++ 44 Can not read MSR id=%d\n",cpu_msr);
  close(fd);

 *msr_value1=(double) msr_value;
}

void mempower_(double *msr_value1)
{

  char path[100];
  sprintf(path, "/dev/cpu/%d/msr", 0);

  uint64_t msr_value=-1;
  int retval = 0;

  int fd = open(path, O_RDONLY);
  if (fd >= 0)
    retval = pread(fd, &msr_value, sizeof(msr_value), mem_msr);
  else printf("++ 55 Can not read MSR id=%d\n",mem_msr);
  close(fd);

 *msr_value1=(double) msr_value;
}

void uncorepower_(double *msr_value1)
{

  char path[100];
  sprintf(path, "/dev/cpu/%d/msr", 0);

  uint64_t msr_value=-1;
  int retval = 0;

  int fd = open(path, O_RDONLY);
  if (fd >= 0)
    retval = pread(fd, &msr_value, sizeof(msr_value), uncore_msr);
  else printf("++ 66 Can not read MSR id=%d\n",uncore_msr);
  close(fd);

 *msr_value1=(double) msr_value;
}


int main()
{
//	getpower_(); 
    double stcpu,stcore,oldcpu,oldcore,divisor,stmem,oldmem,stuncore,olduncore,stmperf,oldmperf;
double staperf,oldaperf;
    double oldtime,STARTT,sttime;
    getpowerunit_(&divisor);
    cpupower_(&stcpu);
    corepower_(&stcore);
		mempower_(&stmem);
		uncorepower_(&stuncore);
    mytime_(&sttime);
mperf_(&stmperf);
aperf_(&staperf);
		STARTT = sttime;

FILE *f;
f = fopen("sandypower.dat","w");
long ints=0;
   while(ints<15000)
   {
		ints++;
	oldcpu = stcpu;
	oldcore = stcore;
	oldtime = sttime;
	oldmem = stmem;
	olduncore = stuncore;
	oldmperf = stmperf;
	oldaperf = staperf;
        corepower_(&stcore);
        //corepower_(0);
	cpupower_(&stcpu);
	mempower_(&stmem);
	uncorepower_(&stuncore);
	mperf_(&stmperf);
	aperf_(&staperf);
	mytime_(&sttime);
	int64_t myperfctl,misc_enable;
//	perfctl_(&myperfctl,IA32_PERF_CTL);
//	perfctl_(&misc_enable,IA32_MISC_ENABLE);

	int64_t myperfstatus;
//	perfctl_(&myperfstatus,IA32_PERF_STATUS);
	double telap= sttime-oldtime;
	//double cpu_p = (stcpu/divisor-oldcpu/divisor)/telap;
	//if(ints%10==0) printf("Time:%f MemPow:%f\t Core Power: %f\tCPU Power: %f mperf:%f perfctl:%lx perfstatus:%lx\n",
			 //     sttime-STARTT,(stmem-oldmem)/(telap*divisor)
			   //   ,(stcore-oldcore)/(telap*divisor),(stcpu-oldcpu)/(telap*divisor)
			      //,2.0*(staperf-oldaperf)/(stmperf-oldmperf),myperfctl,myperfstatus);
	//fprintf(f,"%f %f %f %f\n",sttime-STARTT,(stcore-oldcore)/(telap*divisor),(stcpu-oldcpu)/(telap*divisor)
	//	,	(stuncore-olduncore)/(telap*divisor));
	//fflush(f);
	int64_t newperfctl = myperfctl | (1L << 32);
	int64_t newmisc_enable = misc_enable & ~(1L << 16);
	int i;
/*
	for(i=0;i<12;i++)
	{
		write_perfctl(0x100000c00,IA32_PERF_CTL,i);
		write_perfctl(newmisc_enable,IA32_MISC_ENABLE,i);
	}
	perfctl_(&myperfctl,IA32_PERF_CTL);
*/
	if(ints%1==0) printf("Time:%f MemPow:%f\t Core Power: %f\tCPU Power: %f mperf:%f perfctl:%lx\n",
			      sttime-STARTT,(stmem-oldmem)/(telap*divisor)
			      ,(stcore-oldcore)/(telap*divisor),(stcpu-oldcpu)/(telap*divisor)
			      //,(stuncore-olduncore)/(telap*divisor));
			      ,2.0*(staperf-oldaperf)/(stmperf-oldmperf),myperfctl);
	fprintf(f,"%f %f %f %f %f %f\n",sttime-STARTT,(stcore-oldcore)/(telap*divisor),(stcpu-oldcpu)/(telap*divisor)
		,	(stuncore-olduncore)/(telap*divisor),(stmem-oldmem)/(telap*divisor),2.0*(staperf-oldaperf)/(stmperf-oldmperf));
	fflush(f);

	usleep(1000000);
   }
	fclose(f);
  return 0;
}

