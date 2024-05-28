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

static Prenotazione table[N];
static int inizializzato = 0;

void inizializza()
{
  if (inizializzato == 1)
  {
    return;
  }

  inizializzato = 1;
  for (int i = 0; i < N; i++)
  {
    strcpy(table[i].targa, "L");
    strcpy(table[i].patente, "L");
    strcpy(table[i].tipo, "L");
    strcpy(table[i].folder_img, "L");
  }

  strcpy(table[0].targa, "DG547BV");
  strcpy(table[0].patente, "12345");
  strcpy(table[0].tipo, "auto");
  strcpy(table[0].folder_img, "DG547BV_img/");

  strcpy(table[1].targa, "EY211LA");
  strcpy(table[1].patente, "54321");
  strcpy(table[1].tipo, "auto");
  strcpy(table[1].folder_img, "EY211LA_img/");

  strcpy(table[2].targa, "EE123DE");
  strcpy(table[2].patente, "15486");
  strcpy(table[2].tipo, "camper");
  strcpy(table[2].folder_img, "EE123DE_img/");

  printf("inizializzato\n");

  for (int i = 0; i < N; i++)
  {
    printf("%s|%s|%s|%s\n", table[i].targa, table[i].patente, table[i].tipo, table[i].folder_img);
  }
}

// DEFINIZIONE PRIMA OPERAZIONE

Output *visualizza_prenotazioni_1_svc(Tipo *input, struct svc_req *rp)
{
  static Output result;

  result.check = 0;

  if (inizializzato == 0)
  {
    inizializza();
  }

  printf("richiesto tipo: %s\n", input->tipo);

  // codice operazione
  for (int i = 0; i < N; i++)
  {
    if (table[i].targa[0] > 'E' || (table[i].targa[0] == 'E' && table[i].targa[1] >= 'D') || result.check >= 6)
    {
      if (strcmp(table[i].tipo, input->tipo) == 0)
      {
        result.prenotazioni[result.check] = table[i];
        result.check++;
      }
    }
  }

  return (&result);
}

// DEFINIZIONE SECONDA OPERAZIONE

int *aggiorna_licenza_1_svc(Input *input, struct svc_req *rp)
{
  static int result = -1;
  int found = 0;

  if (inizializzato == 0)
  {
    inizializza();
  }

  // codice operazione

  for (int i = 0; i < N; i++)
  {
    if (found == 0)
    {
      if (strcmp(table[i].targa, input->targa) == 0)
      {
        found = 1;
        strcpy(table[i].patente, input->nuova_patente);
        result = 0;
      }
    }
  }

  return (&result);
}