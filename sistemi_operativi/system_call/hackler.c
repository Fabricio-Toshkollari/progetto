/// @file client.c
/// @brief Contiene l'implementazione del client.

#include "defines.h"
#include "err_exit.h"

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

struct hack
{
  char id[20];
  char delay[20];
  char target[20];
  char action[20];

};
struct hack hac[4];



void createAFile(char path[]){
  int file = open(path, O_CREAT, S_IRWXU);



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





int main(int argc, char * argv[]) {
  char file[MAX_READ];
  readAFile("InputFiles/F7.csv", file);

  int i , j = 0, z = 0;
  for(i = 0; i < strlen(file);i++){
    if (file[i] == ';' || file[i] == '\n' ){
      j++;
      z = 0;
    }
    else{
      if(j % 4 == 0){
        hac[j/4].id[z] = file[i];
      }
      if(j % 4 == 1){
        hac[j/4].delay[z] = file[i];
      }
      if(j % 4 == 2){
        hac[j/4].target[z] = file[i];
      }
      if(j % 4 == 3){
        hac[j/4].action[z] = file[i];
      }
      z++;
    }

  }

  createAFile("OutputFiles/F7_out.csv");
  char reverse[90] = "";
  strcat(reverse, "Id;Delay;Target;Action\n");



  for (i = 1; i < j/4; i++ ){
     strcat(reverse, hac[j/4-i].id);
     strcat(reverse, ";");
     strcat(reverse, hac[j/4-i].delay);
     strcat(reverse, ";");
     strcat(reverse, hac[j/4-i].target);
     strcat(reverse, ";");
     strcat(reverse, hac[j/4-i].action);
     strcat(reverse, "\n");

  }

  writeInFile("OutputFiles/F7_out.csv", reverse);

  sleep(2);

  return 0;
}
