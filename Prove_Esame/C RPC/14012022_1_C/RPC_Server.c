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

int *elimina_occorrenze_numeriche_1_svc(Name *fileName, struct svc_req *rp)
{
  static int result = -1; // default in caso di errore
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

Output *lista_file_direttorio_1_svc(Name *dirName, struct svc_req *rp)
{
  static Output result;

  DIR *dir;
  struct dirent *entry;
  char path[256];

  // codice operazione

  if ((dir = opendir(dirName)) == NULL)
  {
    //  printf("opendir non ha funzionato");
    result.numeroFiles = -1;
    return &result;
  }

  result.numeroFiles = 0;

  while ((entry = readdir(dir)) != NULL && result.numeroFiles < 6)
  {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
    {
      continue;
    }

    // Costruisco il percorso del file
    char path[256];
    snprintf(path, sizeof(path), "%s/%s", dirName, entry->d_name);

    // Controllo se il file ha estensione .txt
    if (strstr(entry->d_name, ".txt") != NULL)
    {
      // aggiungo il file al result
      strncpy(result.files[result.numeroFiles].name, path, sizeof(result.files[result.numeroFiles]) - 1);
      result.numeroFiles++;
    }
  }

  closedir(dir);

  return (&result);
}