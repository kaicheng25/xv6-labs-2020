#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define RD 0
#define WR 1

void seive(int pleft[2])
{
  int fir;
  if(read(pleft[RD],&fir,sizeof(int))==0){
    close(pleft[RD]);
    exit(0);
  }
  fprintf(1,"prime %d\n",fir);

  int pright[2];
  pipe(pright);

  if(fork()!=0)
  {
    int buf;
    close(pright[RD]);
    while(read(pleft[RD],&buf,sizeof(int))!=0)
    {
      if(buf%fir!=0)
      {
        write(pright[WR],&buf,sizeof(int));
      }
    }
    close(pleft[RD]);
    close(pright[WR]);
    wait(0);
    exit(0);
  }
  else{
    close(pleft[RD]);
    close(pright[WR]);
    seive(pright);
  }
  
}


int
main(int argc, char *argv[])
{
  if(argc != 1){
    fprintf(2, "Usage: primes\n");
    exit(1);
  }

  int stp[2];
  pipe(stp);

  if(fork()!=0)
  {
    close(stp[RD]);
    for(int i=2;i<36;++i)
    {
      write(stp[WR],&i,sizeof(i));
    }
    close(stp[WR]);
    wait(0);
  }
  else{
    close(stp[WR]);
    seive(stp);
  }

  exit(0);
}