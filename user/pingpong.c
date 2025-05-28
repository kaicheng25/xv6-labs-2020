#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define RD 0
#define WR 1


int
main(int argc, char *argv[])
{
  if(argc != 1){
    fprintf(2, "Usage: pingpong\n");
    exit(1);
  }
  
  int p1[2], p2[2];
  int buf;
  pipe(p1);
  pipe(p2);
  if(fork() != 0){
    close(p1[RD]);
    close(p2[WR]);
    if(write(p1[WR], "p", 1)!=1){
      fprintf(2, "parent write error\n");
      exit(1);
    }
    if(read(p2[0], &buf, 1)==1){
      fprintf(1, "%d: received pong\n", getpid());
    }

    close(p1[WR]);
    close(p2[RD]);
  }
  else{
    close(p1[1]);
    close(p2[0]);
    if(read(p1[0], &buf, 1)==1){
        fprintf(1, "%d: received ping\n", getpid());
        if(write(p2[1], "p", 1)!=1){
          fprintf(2, "child write error\n");
          exit(1);
        }
    }
    close(p1[0]);
    close(p2[1]);
  }

  exit(0);
}