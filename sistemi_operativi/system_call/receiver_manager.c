/// @file receiver_manager.c
/// @brief Contiene l'implementazione del receiver_manager.

#include "err_exit.h"
#include "defines.h"
#include "shared_memory.h"
#include "semaphore.h"
#include "fifo.h"
#include "pipe.h"


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/shm.h>
#include <string.h>
#include <math.h>

struct pid
{
  int r1;
  int r2;
  int r3;
};

struct pid piid;

void createAFile(char path[]){
  int file = open(path, O_CREAT, S_IRWXU);

  //printf("fin qui tutto bene\n" );

  if(file == -1){
    ErrExit("crate");
  }
  close(file);
}

void writeInFile(char fileName[], char string[]){
  int file = open(fileName, O_WRONLY);


  if(file == -1){
    ErrExit("open");
  }



  ssize_t wrt = write(file, string, strlen(string));


  if (wrt != sizeof(*string)){
    ErrExit("write");
  }
  close(file);

}


void r1(){
  pid_t pid = fork();

  if(pid == 0){
    createAFile("OutputFiles/F4.csv");
    char intestazione[100]  = "Id;Message;Id Sender;Id Receiver;Time arrival;Time departure";
    writeInFile("OutputFiles/F4.csv", intestazione);


    sleep(1);
    exit(1);

  }
  else{
    piid.r1 = pid;
  }
}
void r2(){
  pid_t pid = fork();

  if(pid == 0){
    createAFile("OutputFiles/F5.csv");
    char intestazione[100]  = "Id;Message;Id Sender;Id Receiver;Time arrival;Time departure";
    writeInFile("OutputFiles/F5.csv", intestazione);


    sleep(2);
    exit(2);


  }

  else{
    piid.r2 = pid;
  }
}
void r3(){
  pid_t pid = fork();

  if(pid == 0){
    createAFile("OutputFiles/F6.csv");
    char intestazione[100]  = "Id;Message;Id Sender;Id Receiver;Time arrival;Time departure";
    writeInFile("OutputFiles/F6.csv", intestazione);



    sleep(3);
    exit(3);


  }

  else{
    piid.r3 = pid;
  }
}

int main(int argc, char * argv[]) {
  r1();
  r2();
  r3();

  char id[70];
  sprintf(id, "Id;PID\nR1;%d\nR2;%d;\nR3;%d", piid.r1, piid.r2, piid.r3);

  char s[50] = "OutputFiles/F9.csv";

  createAFile(s);
  writeInFile(s,id);


  return 0;
}
