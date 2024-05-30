/*RPC_Client.c*/

#include "RPC_xFile.h"
#include <rpc/rpc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
  CLIENT *cl;   // gestore del trasporto
  char *server; // nome host

  int *ris;    // Output of the RPC if integer
  Output *out; // Output of the RPC if struct

  char car, buffer[128]; // buffer per leggere stringhe
  int ok = 0;

  // dichiarazioni statiche delle variabili per RPC che non possono essere
  // salvate sullo stack locale, fanno riferimento alle strutture dichiarate in
  // RPC_xFile.x
  static Prenotazione prenotazione;
  static Tipo tipo;
  static Input input;

  // CONTROLLO DEGLI ARGOMENTI
  if (argc != 2)
  {
    fprintf(stderr, "uso: %s nomehost\n", argv[0]);
    exit(1);
  }

  // CREAZIONE GESTORE DI TRASPORTO
  cl = clnt_create(argv[1], FILEPROG, FILEVERS, "udp");
  if (cl == NULL)
  {
    clnt_pcreateerror(argv[1]);
    exit(1);
  }

  // INTERAZIONE CON L'UTENTE
  printf("Operazioni: 1= VISUALIZZA_PRENOTAZIONI_1, 2=AGGIORNA_LIECNZA_2; ^D per terminare\n");

  while (gets(buffer))
  {
    // CONTROLLO OPERAZIONE INSERITA
    if (strcmp(buffer, "1") != 0 && strcmp(buffer, "2") != 0)
    {
      printf("Operazione non disponibile, inserire i valori indicati\n");
      printf(
          "Operazioni: 1= VISUALIZZA_PRENOTAZIONI_1, 2=AGGIORNA_LIECNZA_2; ^D per terminare\n");
      continue;
    }
    else if (strcmp(buffer, "1") == 0)
    { // VISUALIZZA_PRENOTAZIONI_1
      printf("Richiesta operazione: %s\n", buffer);
      // esempio operazione con input fileName e output intero
      printf("inserisci il tipo del veicolo (auto o camper): \n");
      gets(tipo.tipo);

      // input check su tipo veicolo
      while (ok == 0)
      {
        if (strcmp(tipo.tipo, "camper") == 0 || strcmp(tipo.tipo, "auto") == 0)
        {
          ok = 1;
        }
        else
        {
          printf("input non valido: %s\n\n", tipo.tipo);
          printf("inserisci il tipo del veicolo (auto o camper): \n");
          gets(tipo.tipo);
        }
      }

      out = visualizza_prenotazioni_1(&tipo, cl);

      if (out == NULL)
      {
        clnt_perror(cl, "E' avvenuto un errore lato server");
      }
      else if (out->check = 0)
      {
        printf("non è stata trovata nessuna prenotazione con tipo %s\n", tipo.tipo);
      }
      else
      {
        printf("Successo!\nLista prenotazioni con tipo %s:\n", tipo.tipo);
        for (int i = 0; i < N; i++)
        {
          if (strlen(out->prenotazioni[i].targa) > 0)
          {
            printf("%s|%s|%s|%s\n", out->prenotazioni[i].targa, out->prenotazioni[i].patente, out->prenotazioni[i].tipo, out->prenotazioni[i].folder_img);
          }
        }
      }
    }
    else if (strcmp(buffer, "2") == 0)
    { // AGGIORNA_LIECNZA_2
      printf("Richiesta operazione: %s\n", buffer);
      // esempio operazione con input fileName e output struct
      printf("inserisci la targa: \n");
      gets(input.targa);

      while (ok == 0)
      {
        if (strlen(input.targa) == 7)
        {
          ok = 1;
        }
        else
        {
          printf("input non valido: %s\n\n", input.targa);
          printf("inserisci una targa valida <XXxxxXX>: \n");
          gets(input.targa);
        }
      }

      printf("inserisci la nuova patente: \n");
      gets(input.nuova_patente);

      while (ok == 0)
      {
        if (strlen(input.nuova_patente) == 6)
        {
          ok = 1;
        }
        else
        {
          printf("input non valido: %s\n\n", input.nuova_patente);
          printf("inserisci una patente valida <xxxxx>: \n");
          gets(input.nuova_patente);
        }
      }

      ris = aggiorna_licenza_1(&input, cl);

      if (ris < 0)
      {
        clnt_perror(cl, "E' avvenuto un errore lato server");
      }
      else
      {
        printf("Successo!\n");
      }
    } // endif
    ok = 0;
  }
  // DISTRUZIONE GESTORE DI TRASPORTO
  clnt_destroy(cl);
  printf("Client: termino...\n");
  exit(0);
}