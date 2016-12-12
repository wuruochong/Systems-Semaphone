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

int tryaccess(){
    int key = ftok("makefile",22);
    int semid = semget(key,0,0);
    union semun su;
    return semctl(semid, 0, GETVAL, su);
}

int writeline(){
    printf("Please enter the next line of the story: ");
    char input[4096];
    fgets(input, sizeof(input), stdin);
    int lastupdatelength = strlen(input);
    int fid = open("story", O_WRONLY | O_APPEND);
    write(fid, input, strlen(input));
    close(fid);
    return lastupdatelength;
}


void readline(int *lastupdatelength) {
    printf("Last line in story: ");
    char line[4096];
    int fid = open("story", O_RDONLY);
    if (*lastupdatelength) {
        lseek(fid, -1 * (*lastupdatelength), SEEK_END);
        read(fid, line, *lastupdatelength);
    }
    else {
        printf("There are no previous entries in story\n");
    }
    printf("%s", line);
    close(fid);
}


int main(){
    printf("Trying to access\n");
    while (1){
      if(tryaccess()){
        int key = ftok("makefile",22);
        int shmkey = ftok("makefile", 23);
        int semid = semget(key, 0, 0);
        int shmid = shmget(shmkey, 1, 0);
        struct sembuf sb;
        sb.sem_op = 1;
        sb.sem_num = 0;
        sb.sem_flg = SEM_UNDO;
        printf("Access Granted!\n");
        int *lastupdatelength = shmat(shmid, 0, 0);

        semop(semid, &sb, -1);
        readline(lastupdatelength);
        semop(semid, &sb, 1);

        semop(semid, &sb, -1);
        *lastupdatelength = writeline();
        semop(semid,&sb,1);

        return 0;
      }
    }
}
