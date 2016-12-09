#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

union semun{
  int val;
  struct semid_ds *buff;
  unsigned long *buffer;
  struct seminfo *_buf;
};

int tryaccess(){
  int key = ftok("makefile",22);
  int semid = semget(key,0,0);
  union semun su;
  return semctl(semid, 0, GETVAL, su);
  // return su.val;
}

void writeline(char * line){
  int fid = open("story", O_APPEND|O_WRONLY);
  int shmkey = ftok("makefile",23);
  int shmid = shmget(shmkey, 1, 0);
  int * v = (int*)malloc(sizeof(int));
  shmat(shmid, v, 0);
  *v = strlen(line);
  shmdt(v);
  write(fid, line, strlen(line));
  close(fid);
}

int main(){
  char * line;
  printf("semval: %d\n", tryaccess());
  writeline("\nDoes this work?\n");
}
