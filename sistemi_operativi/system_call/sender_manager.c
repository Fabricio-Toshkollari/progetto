/// @file sender_manager.c
/// @brief Contiene l'implementazione del sender_manager.

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

#define MAX_READ 1000
#define MAX_STRING 100

/*
void errExit(char *msg){
  printf("%s\n", msg);
}
*/

struct Msg
{
  char id[20];
  char message[MAX_STRING];
  char idSender[20];
  char idReciver[20];
  char timeArrival[20];
  char timeDeparture[20];
};

struct pid
{
  int s1;
  int s2;
  int s3;
};

struct pid piid;

struct Msg f0[3];

//crea un file
void createAFile(char path[]){
  int file = open(path, O_CREAT, S_IRWXU);

  //printf("fin qui tutto bene\n" );

  if(file == -1){
    ErrExit("crate");
  }
  close(file);
}


//scrivi in un file
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

//legge un file
void readAFile(char fileName[], char * buffer){
  int file = open(fileName, O_RDONLY);

  if (file == -1){
    ErrExit("open");
  }

  //leggo il file
  ssize_t rd = read(file, buffer, MAX_READ);
  buffer[rd] = '\0';

  /*

  if (rd != -1){
    ErrExit("write");
  }
  */

  close(file);
}

//divide i file e li mette in un array
/*
void separatore(char file[], char array, int col, int rig) {
  int i, j = 0, z = 0;
  for(i = 0; i < MAX_READ; i++){
    if (file[i] == ',' || file[i] == '\n'){
      j++;
      z = 0;
    }
    else{
      array[j][z] = file[i];
      z++;
    }
  }
}
*/

void s1(){
  pid_t pid = fork();

  if(pid == 0){



    //leggo il file f0
    char file[MAX_READ];
    readAFile("InputFiles/F0.csv", file);


    //salvare file in un array
    //msg f0[3]
    //char parola[MAX_STRING];
    int i, j = 1, z = 0, controllo = 0;
    for(i = 0; i <= strlen(file); i++){


        if (file[i] == ';' || file[i] == '\n' ){
          j++;
          z = 0;
        }

        else{
          if(j % 8 == 1){
            f0[j/8].id[z] = file[i];

          }
          if(j % 8== 2){
            f0[j/8].message[z] = file[i];

          }
          if(j % 8== 3){
            f0[j/8].idSender[z] = file[i];

          }
          if(j % 8== 4){
            f0[j/8].idReciver[z] = file[i];

          }
          if(j % 8== 4){
            //f0[j/6].timeArrival[z] = '';

          }
          if(j % 8== 5){
            //f0[j/6].timeDeparture[z] = '';

          }
          z++;
        }
      }




    //scrivere nel file F1 il contenuto dell' array
    createAFile("OutputFiles/F1.csv");
    char result[MAX_READ] = "";

    strcat(result, "Id;Message;Id Sender;Id Receiver;Time arrival;Time departure\n");

    printf("%s\n", f0[3].id);

    for (i = 1; i <= 3; i++){

      strcat(result, f0[i].id);
      strcat(result, ";");
      strcat(result, f0[i].message);
      strcat(result, ";");
      strcat(result, f0[i].idSender);
      strcat(result, ";");
      strcat(result, f0[i].idReciver);
      strcat(result, "\n");
    }


    writeInFile("OutputFiles/F1.csv", result);

    //eliminazione dell'array

    //uccisione s1 dopo un secondo

    sleep(1);
    printf("chiudo s1\n");

    exit(1);


  }

  else{
    piid.s1 = pid;
  }
}

void s2(){
  pid_t pid = fork();

  if(pid == 0){
    //creazione file f2
    createAFile("OutputFiles/F2.csv");
    char intestazione[100]  = "Id;Message;Id Sender;Id Receiver;Time arrival;Time departure";
    writeInFile("OutputFiles/F2.csv", intestazione);

    //uccisione s2 dopo 2 secondi
    sleep(2);
    exit(2);

  }


  else{
    piid.s2 = pid;
  }
}

void s3(){
  pid_t pid = fork();

  if(pid == 0){
    createAFile("OutputFiles/F3.csv");
    char intestazione[100]  = "Id;Message;Id Sender;Id Receiver;Time arrival;Time departure";
    writeInFile("OutputFiles/F3.csv", intestazione);

    //uccisione s3 dopo 3 secondi
    sleep(3);
    exit(3);

  }

  else{
    piid.s3 = pid;
  }
}

int main(int argc, char const *argv[]) {

  char s[50] = "OutputFiles/F8.csv";
  //char file[MAX_READ];
  s1();
  s2();
  s3();

  char id[70];
  sprintf(id, "Id;PID\nS1;%d\nS2;%d;\nS3;%d", piid.s1, piid.s2, piid.s3);



  createAFile(s);
  writeInFile(s,id);



  //terminare processo dopo che tutti i processi hanno finito
  sleep(3);


  return 0;
}
