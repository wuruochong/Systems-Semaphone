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

/*
union semun{
    int val;
    struct semid_ds *buff;
    unsigned long *buffer;
    struct seminfo *_buf;
};
*/

int main(int argc, char * argv[]){
    int semid;
    int key = ftok("makefile" , 22);
    int shmkey = ftok("makefile", 23);
    int sc;
    int id;
    int shmid;
    int mc;
    if (strncmp(argv[1], "-c", strlen(argv[1])) == 0){
      semid = semget(key, 1, IPC_CREAT| IPC_EXCL | 0644);
      shmid = shmget(shmkey, sizeof(int), IPC_CREAT | IPC_EXCL |0644);
      printf("shmid:%d\n", shmid);
      printf("semaphore created: %d\n", semid);
      union semun su;
      su.val = 1;
      sc = semctl(semid, 0, SETVAL, su);
      printf("value set: %d\n", sc);
      id = open("story", O_CREAT|O_TRUNC,0644);
      printf("file created: %d\n", id);
      int cv = close(id);
    }
    else if (strncmp(argv[1], "-v", strlen(argv[1])) == 0){
      semid = semget(key, 1, 0);
      sc = semctl(semid, 0, GETVAL);
      printf("semaphore value: %d\n",sc);
      shmid = shmget(shmkey, sizeof(int), 0);
      printf("shmid:%d\n",shmid);
      int * v = (int *)malloc(sizeof(int));
      shmat(shmid, v, SHM_RDONLY);
      printf("shared memory: %d\n", *v);
      int fd = open("story", O_RDONLY);
      struct stat st;
      stat("story", &st);
      int size = st.st_size;
      printf("size: %d\n",size);
      void * buf = malloc(size);
      read(fd, buf, size);
      printf("Story:%s\n", buf);
    }
    else if(strncmp(argv[1], "-r", strlen(argv[1])) == 0){
      semid = semget(key, 1, 0);
      sc = semctl(semid, 0, IPC_RMID);
      shmid = shmget(shmkey, sizeof(int), 0);
      struct shmid_ds d;
      printf("shmid: %d\n", shmid);
      mc = shmctl(shmid, IPC_RMID,NULL);
      printf("shared memory removed: %d\n",mc);
      printf("semaphore removed: %d\n", sc);
    }
    return 0;
}
