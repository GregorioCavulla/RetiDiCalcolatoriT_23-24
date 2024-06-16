/* RPC_Server.c */

#include "RPC_xFile.h"
#include <dirent.h>
#include <fcntl.h>
#include <rpc/rpc.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define N 6

// DEFINIZIONE PRIMA OPERAZIONE

int *elimina_occorrenze_1_svc(Name *fileName, struct svc_req *rp)
{
  printf("elimina occorrenze");
  static int response = -1; // default in caso di errore
  // codice operazione
  int fd;
  int temp_fd;

  fd = open(fileName->name, O_RDONLY);
  if (fd < 0)
  {
    perror("open file: ");
    return (&response);
  }

  temp_fd = open("temp.txt", O_CREAT | O_WRONLY, 0777);
  if (temp_fd < 0)
  {
    perror("open temp: ");
    return (&response);
  }

  response = 0;

  int nread;
  int car;

  while ((nread = read(fd, &car, 1)) > 0)
  {
    if (!isdigit(car))
    {
      write(temp_fd, &car, 1);
    }
    else
    {
      response++;
    }
  }

  if (response >= 0)
  {
    printf("[DEBUG]: eliminati %d caratteri numerici da %s", response, fileName->name);
  }

  close(fd);
  close(temp_fd);

  rename("temp.txt", fileName->name);

  return (&response);
}

// DEFINIZIONE SECONDA OPERAZIONE

Output *lista_sottodirettorio_1_svc(Name *dirName, struct svc_req *rp)
{
  static Output result;
  result.numeroDirettori = 0; // Inizializza a 0 invece di -1

  DIR *dir;
  struct dirent *entry;

  dir = opendir(dirName->name);
  if (dir == NULL)
  {
    perror("open dir: ");
    return (&result);
  }

  char fileName[256];
  int fileCounter = 0;
  int fileNameL;
  int countMax = 0;

  while ((entry = readdir(dir)) != NULL && countMax < N)
  {
    if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
    {
      DIR *dir1;
      struct dirent *file;

      char subdirPath[512];
      strcpy(subdirPath, dirName->name);
      strcat(subdirPath, "/");
      strcat(subdirPath, entry->d_name);

      if ((dir1 = opendir(subdirPath)) != NULL)
      {
        fileCounter = 0; // Reset file counter for each directory
        while ((file = readdir(dir1)) != NULL)
        {
          if (strcmp(file->d_name, ".") != 0 && strcmp(file->d_name, "..") != 0)
          {
            strcpy(fileName, file->d_name);
            fileNameL = strlen(fileName);

            if (fileNameL > 4 &&
                fileName[fileNameL - 1] == 't' &&
                fileName[fileNameL - 2] == 'x' &&
                fileName[fileNameL - 3] == 't' &&
                fileName[fileNameL - 4] == '.')
            {
              fileCounter++;
            }
          }
        }
        closedir(dir1);

        if (fileCounter >= 5)
        {
          strcpy(result.files[countMax].name, entry->d_name);
          result.numeroDirettori++;
          countMax++;
        }
      }
    }
  }
  closedir(dir);

  return (&result);
}
