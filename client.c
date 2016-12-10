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
    // return su.val;
}

void writeline(char * line){
    int fid = open("story", O_APPEND|O_WRONLY);
    int shmkey = ftok("makefile",23);
    int shmid = shmget(shmkey, 1, 0);
    printf("shmid: %d\n",shmid);
    int * v = (int*)(malloc(sizeof(int)));
    shmat(shmid, v, 0);
    *v = strlen(line);
    printf("value: %d\n", *v);
    // shmdt(v);
    // int v = strlen(line);
    // shmctl(shmid, IPC_SET, &v);
    write(fid, line, strlen(line));
    close(fid);
}

char * readline() {
    char * chrStr;
    char c;
    char newLine[1000];
    int fid = open("story", O_RDONLY);
    int bytesread = -1;
    lseek(fid, -1, SEEK_END);
    read(fid, chrStr, 1);
    c = chrStr[0];
    //keeps track of the number of bytesread
    int bread = 1;
    //check if last char is last line
    if(c == '\n') {
        //pointer goes back 2, reads pushes pointer forwards 1; should allow backwards traversal of pointer over chars
        lseek(fid, -2, SEEK_END);
        read(fid, chrStr, 1);
        c = chrStr[0];
        bread++;
        //read until end of previous line is found
        while(c != '\n') {
            lseek(fid, -2, SEEK_END);
            read(fid, chrStr, 1);
            c = chrStr[0];
            bread++;
        }
        //read into newLine all the chars read; should be reading from end of second to last line to end of file aka should read only the last line
        read(fid, newLine, bread);
    }
    //close file as we don't need to read anymore
    close(fid);
    //return a string of the last line
    return newLine;
}

int main(){
    /*
    char * line;
    printf("semval: %d\n", tryaccess());
    writeline("\nDoes this work?\n");
    int shmkey = ftok("makefile",23);
    int shmid = shmget(shmkey, 1, 0);
    int * v = (int*)(malloc(sizeof(int)));
    shmat(shmid, v, 0);
    printf("value of shm: %d\n", *v);
    */

    //Reading from the file is still in defvelopment
    printf("Last Line in Story: %s\n", readline());


    //Writing to file works
    /*
    printf("Please enter the next line of the story: ");
    //may want to give input variable size, but i don't believe that is possible.
    char input[4096];
    fgets(input, sizeof(input), stdin);
    writeline(input);
    */
}
