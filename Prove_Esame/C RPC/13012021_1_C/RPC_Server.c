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

#define TEXT_FILE_EXTENSION ".txt"
#define MIN_TEXT_FILES 5

// DEFINIZIONE PRIMA OPERAZIONE
int *elimina_occorrenze_numerici_1_svc(Name *fileName, struct svc_req *rp)
{
  static int result;
  // codice operazione

  int fd = open(fileName->name, O_RDONLY);
  int temp = open("temp.txt", O_WRONLY | O_CREAT, 0777);

  char read_char;
  int nread;

  if (!fd || !temp)
  {
    result = -1;
    return (&result);
  }

  result = 0;

  while ((nread = read(fd, &read_char, 1)))
  {
    if (nread < 0)
    {
      perror("Erroe lettura file!");
      result = -1;
      return (&result);
    }

    if (read_char <= '9' && read_char >= '0')
    {
      result++;
    }
    else
    {
      nread = write(temp, &read_char, 1);
      if (nread < 0)
      {
        perror("Erroe scritura file!");
        result = -1;
        return (&result);
      }
    }
  }

  close(fd);
  close(temp);

  rename("temp.txt", fileName->name);

  printf("Eliminazioni: %d\n", result);

  return (&result);
}

// DEFINIZIONE SECONDA OPERAZIONE

Output *lista_sottodirettori_1_svc(Name *dirName, struct svc_req *rp)
{
  static Output result;
  DIR *dir;
  struct dirent *entry;
  char path[256];

  printf("richiesti sottodir di %s con almeno 5 file di testo\n", dirName);

  if ((dir = opendir(dirName)) == NULL)
  {
    //  printf("opendir non ha funzionato");
    result.numeroDirettori = -1;
    return &result;
  }

  result.numeroDirettori = 0;

  while ((entry = readdir(dir)) != NULL && result.numeroDirettori < 10)
  {

    // Ignoro il direttorio corrente e quello padre
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
    {
      //  printf("ci siamo\n");
      continue;
    }

    // creo un parh per il direttorio corrente
    snprintf(path, sizeof(path), "%s/%s", dirName, entry->d_name);
    // printf("path direttorio: %s\n\n", path);

    DIR *subDir;
    struct dirent *entry1;
    int fileCounter = 0, fileNameLen;
    char fileName[255];

    if ((subDir = opendir(path)) == NULL)
    {
      //  printf("non riesco ad aprire %s", path);
      result.numeroDirettori = -1;
      return &result;
    }

    if (subDir != NULL)
    {
      int giro = 0;
      while ((entry1 = readdir(subDir)) != NULL)
      {
        giro++;
        //  printf("giro numero %d, ho trovato %s\n\n", giro, entry1->d_name);
        strcpy(fileName, entry1->d_name);
        fileNameLen = strlen(fileName);
        if (fileNameLen > 4 && fileName[fileNameLen - 4] == '.' && fileName[fileNameLen - 3] == 't' && fileName[fileNameLen - 2] == 'x' && fileName[fileNameLen - 1] == 't')
        {
          fileCounter++;
          //    printf("Trovato il file di testo %s nella cartella %s\n", fileName, entry->d_name);
        }
        if (fileCounter >= 5)
        {
          //    printf("aggiungo alla lista di result il direttorio: %s\n", entry->d_name);
          strcpy(result.files[result.numeroDirettori].name, entry->d_name);
          result.numeroDirettori++;
          //    for (int i = 0; i < result.numeroDirettori; i++)
          //    {
          //      printf("result: %s, %d\n", result.files[i].name, result.numeroDirettori);
          //    }
        }
      }
      // printf("chiudo il subdir, funzionerà??\n");
      closedir(subDir);
      // printf("chiuso il subdir, memset funziona??\n");
      memset(path, 0, 256);
      // printf("si, memset funziona\n\n");
    }
  }
  printf("finito il check dei sottodir, funziona?\n\n");

  printf("invio tutto, ciao");
  return (&result);
}