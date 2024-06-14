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

// DEFINIZIONE PRIMA OPERAZIONE

int *elimina_occorrenze_1_svc(Name *fileName, struct svc_req *rp)
{
  static int result = -1; // default in caso di errore
  // codice operazione

  int fd;
  int temp_fd;

  if ((fd = open(fileName->name, O_RDONLY)) < 0)
  {
    perror("open file");
    return (&result);
  }

  if ((temp_fd = open("temp.txt", O_CREAT | O_WRONLY, 0777)) < 0)
  {
    perror("open temp");
    return (&result);
  }

  result = 0;

  int nread;
  char car;

  while ((nread = read(fd, &car, 1)) > 0)
  {
    if (isdigit(car))
    {
      write(temp_fd, &car, 1);
    }
    else
    {
      result++;
    }
  }

  if (result >= 0)
  {
    printf("[DEBUG]: eliminati %d caratteri alfabetici da %s", result, fileName->name);
  }

  close(fd);
  close(temp_fd);

  rename("temp.txt", fileName->name);

  return (&result);
}

// DEFINIZIONE SECONDA OPERAZIONE

Output *lista_file_carattere_1_svc(Input *input, struct svc_req *rp)
{
  static Output result;
  // codice operazione

  DIR *dir;
  struct dirent *entry;
  char carInput = input->carattere;
  int occInput = input->occorrenze;

  char car;
  int occ;

  char fileName[256];
  int fileNameL;

  int j = 0;

  if ((dir = opendir(input->dirName.name)) == NULL)
  {
    perror("opendir");
    strcpy(result.files[0].name, "erroreNULL.xyz");

    return (&result);
  }

  while ((entry = readdir(dir)) != NULL)
  {
    strcpy(fileName, entry->d_name);
    fileNameL = strlen(fileName);

    if (fileNameL > 4 && fileName[fileNameL - 1] == "t" && fileName[fileNameL - 2] == "x" && fileName[fileNameL - 3] == "t" && fileName[fileNameL - 4] == ".")
    {
      occ = 0;
      for (int i = 0; i < fileNameL; i++)
      {
        if (fileName[i] == carInput)
        {
          occ++;
          if (occ == occInput)
          {
            strcpy(result.files[j].name, fileName);
            j++;
          }
        }
      }
    }
  }

  return (&result);
}