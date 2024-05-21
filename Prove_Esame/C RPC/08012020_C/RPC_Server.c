/* RPC_Server.c */

#include "RPC_xFile.h"
#include <dirent.h>
#include <fcntl.h>
#include <rpc/rpc.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <unistd.h>

// DEFINIZIONE PRIMA OPERAZIONE

int *elimina_occorrenze_1_svc(FileName *input, struct svc_req *rp)
{
  static int result = -1;

  int fd_file, nread, fd_temp;
  char fileNameTemp[NAME_MAX];

  printf("richiesto file %s \n", input->fileName);

  fd_file = open(input->fileName, O_RDONLY);
  if (fd_file < 0)
  {
    printf("File inesistente\n");
    return (&result); //-1 file non esistente
  }
  else
  {
    fileNameTemp[0] = '\0';
    strcat(fileNameTemp, input->fileName);
    if ((fd_temp = open(fileNameTemp, O_CREAT | O_RDONLY, 0777)) < 0)
    {
      perror("errore apertura file temp");
      return (&result);
    }
    else
    {
      int nread;
      char car;
      result = 0;
      while (nread = read(fd_file, &car, 1) > 0)
      {
        if (isdigit(car))
        {
          write(fd_temp, &car, 1);
        }
        else
        {
          result++;
        }
      }
    }

    if (result >= 0)
    {
      printf("nel file %s sono stati eliminati %d caratteri numerici\n", input->fileName, result);
    }

    close(fd_file);
    close(fd_temp);

    rename(fileNameTemp, input->fileName);

    return (&result);
  }
}

// DEFINIZIONE SECONDA OPERAZIONE

OutputFileList *lista_file_carattere_1_svc(Input *input, struct svc_req *rp)
{
  static OutputFileList result;

  DIR *dir;
  struct dirent *dd;
  int occorrenze, fileNameL;
  char fileName[256];

  printf("richiesto direttorio %s\n", input->direttorio);
  if ((dir = opendir(input->direttorio)) == NULL)
  {
    printf("direttorio inesistente");
    result.numeroFiles = -1;
    return (&result);
  }

  result.numeroFiles = 0;
  while ((dd = readdir(dir)) != NULL && result.numeroFiles < 6)
  {
    strcpy(fileName, dd->d_name);

    fileNameL = strlen(fileName);

    if (fileNameL > 4 &&
        fileName[fileNameL - 1] == 't' &&
        fileName[fileNameL - 2] == 'x' &&
        fileName[fileNameL - 3] == 't' &&
        fileName[fileNameL - 4] == '.')
    {

      occorrenze = 0;

      for (int i = 0; i < fileNameL; i++)
      {
        if (fileName[i] == input->carattere)
        {
          occorrenze++;
          if (occorrenze == input->occorrenze)
          {
            strcpy(result.files[result.numeroFiles++].fileName, fileName);
            break;
          }
        }
      }
    }
  }

  closedir(dir);
  return (&result);
}