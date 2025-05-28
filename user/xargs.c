#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

int
main(int argc, char *argv[])
{
  if(argc < 3){
    fprintf(2,"usage: find <path> <filename>\n");
    exit(1);
  }

  char buf[100];
  int n = read(0,buf,99); 
  if(n<=0||n>100){
    fprintf(2,"input error\n");
    exit(1);
  }

  char *argvs[MAXARG];
  int i=0;
  for(;i+1<argc;++i)
  {
    argvs[i] = argv[i+1];
  }

  // printf("%s",buf);
  
  int start = 0;
  for(int j=0;j<n;++j)
  {
    if(buf[j]==' '||buf[j]=='\n')
    {
      argvs[i]=&buf[start];
      buf[j]='\0';
      start = j+1;
      ++i;
    }
  }
  buf[n]='\0';
  argvs[i]=&buf[start];

  // for(int k=0;k<=i;++k)
  // {
  //   printf("%s  ",argvs[k]);
  // }

  if(fork()==0)
  {
    exec(argvs[0],argvs);
  }
  else{
    wait(0);
  }

  exit(0);
}