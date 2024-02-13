

#include "RPC_xFile.h"
#include "Soluzione/RPC_xFile.h"
#include <dirent.h>
#include <fcntl.h>
#include <rpc/rpc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// eliminare tutte le occorrenze di caratteri numerici all'interno di un file di
// testo
int *elimina_occorrenze_1_svc(FileName *input, struct svc_req *cl) {
  static int result = -1;
  int fd_file, nread, fd_temp;
  char fileNameTemp[256];

  printf("Richiesto file %s\n", input->name);

  if ((fd_file = open(input->name, O_RDONLY)) < 0) {
    result = -1;
    printf("File inesistente\n");
    return (&result);
  } else {
    // creao il file temp che andrà a sostituire il file originale
    fileNameTemp[0] = '\0';
    strcat(fileNameTemp, input->name);
    strcat(fileNameTemp, "_Temp");

    if ((fd_temp = open(fileNameTemp, O_CREAT | O_WRONLY, 0777)) < 0) {
      perror("Errore apertura file temp");
      return (&result);
    } else {
      int nread;
      char car;
      result = 0;
      while ((nread = read(fd_file, &car, 1)) > 0) { // lettura a carattere
        if (!isdigit(car)) {
          write(fd_temp, &car, 1);
        } else {
          result++;
        }
      }
    }
  }

  if (result >= 0) {
    printf("Nel file %s sono stati eliminati %d caratteri numerici\n",
           input->name, result);
  }

  close(fd_file);
  close(fd_temp);

  rename(fileNameTemp, input->name);

  return (&result);
} // fine procedura 1

// lista dei file di testo di un direttorio i cui nomi i cui nomi contengono
// almeno uno specificato numero di occorrenze di un carattere indicato
// dall’utente

OutputFileList *lista_file_carattere_1_svc(Input *input, struct svc_req *rp) {
  DIR *dir;
  struct dirent *dd; // struttura per la lettura dei file nella directory
  int nOccorrenze,
      fileNameLen; // numero di occorrenze del carattere, lunghezza del fileName
  char fileName[256];
  static OutputFileList result;

  printf("Richiesto direttorio %s\n", input->direttorio);
  if ((dir = opendir(input->direttorio)) == NULL) {
    result.numeroFiles = -1;
    return (&result);
  }

  // Max 6 files can be returnetd, so we control this in the while

  result.numeroFiles = 0;

  while ((dd = readdir(dir)) != NULL && result.numeroFiles < 6) {
    strcpy(fileName, dd->d_name);

    if (fileNameLen > 4 && fileName[fileNameLen - 1] == 't' &&
        fileName[fileNameLen - 2] == 'x' && fileName[fileNameLen - 3] == 't' &&
        fileName[fileNameLen - 4] == '.') {
      nOccorrenze = 0;

      for (int i = 0; i < fileNameLen; i++) {
        if (fileName[i] == input->carattere) {
          nOccorrenze++;
        }

        if (nOccorrenze == input->carattere) {
          strcpy(result.files[result.numeroFiles++].name, fileName);
          break;
        }
      }
    }
  }

  closedir(dir);
  return (&result);
}