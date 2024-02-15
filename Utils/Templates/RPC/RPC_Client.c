
#include <rpc/rpc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xFile.h"

#define MAX_STRING 128 //
int isNumber(char *text) {
  int j;
  j = strlen(text);
  while (j--) {
    if (text[j] >= '0' && text[j] <= '9')
      continue;

    return 0;
  }
  return 1;
}

int main(int argc, char *argv[]) {
  char *host;              // nome host
  CLIENT *cl;              // gestore del trasporto
  char buffer[MAX_STRING]; // buffer per leggere stringhe
  // dichiarazioni delle variabili
  int i, *ris;
  char c;
  Evento evento;
  void *voidValue;
  Biglietti biglietto;

  /****************************************************************************************************************/

  // CONTROLLO DEGLI ARGOMENTI
  if (argc != 2) {
    printf("[ERROR] usage: %s server_host\n", argv[0]);
    exit(1);
  }
  host = argv[1];

  // CREAZIONE GESTORE DI TRASPORTO
  cl = clnt_create(host, FILEPROG, FILEVERS, "udp");
  if (cl == NULL) {
    clnt_pcreateerror(host);
    exit(1);
  }

  // INTERAZIONE CON L UTENTE

        printf("Inserire:\n1\t inserimentoEvento \n
			2\t Biglietti \n^D\tper terminare: ");

	while (gets(buffer) != 0)
	{
    // OPERAZIONE 1
    if (strcmp(buffer, "1") == 0) {
      // lettura di una stringa non nulla
      do {
        printf("inserire desc\n");
        if (gets(buffer) != 0)
          break;
      } while (1);
      // lo si salva nella struttura
      evento.desc = (char *)malloc(strlen(buffer) + 1);
      strcpy(evento.desc, buffer);
      // lettura di una stringa valida tra scelte
      do {
        printf("inserire tipo\n");
        gets(buffer);
        if (strcmp(buffer, "Concerto") == 0 ||
            strcmp(buffer, "Formula1") == 0 || strcmp(buffer, "Calcio") == 0)
          break;
      } while (1);
      evento.tipo = (char *)malloc(strlen(buffer) + 1);
      strcpy(evento.tipo, buffer);
      // lettura di un numero int
      do {
        printf("inserire il prezz\n");
        gets(buffer);
        if (isNumber(buffer) == 1)
          break;
      } while (1);
      evento.prez = (char *)malloc(strlen(buffer) + 1);
      strcpy(evento.prez, buffer);

      // Invocazione remota
      ris = inserimento_evento_1(&evento, cl);

      // Controllo del risultato se punta ad un indirizzo vuoto
      if ((ris == (int *)NULL)) {
        // Errore di RPC
        clnt_perror(cl, host);
        exit(1);
      }

      printf("Operazione fatta \n");
      // il valore all indirizzo puntato
      if (*ris < 0) {
        printf("Problema del server");
      } else {
        printf("operazione riuscita");
      }

    } // if 1

    // SE OPERAZIONE 2
    else if (strcmp(buffer, "2") == 0) {
      do {
        printf("inserire desc\n");
        if (gets(buffer) != 0)
          break;
      } while (1);
      biglietto.desc = (char *)malloc(strlen(buffer) + 1);
      strcpy(biglietto.desc, buffer);

      // lettura di un numero int
      do {
        printf("inserire il numero di biglietti\n");
        gets(buffer);
        if (isNumber(buffer) == 1)
          break;
      } while (1);
      biglietto.nBiglietti = atoi(buffer);

      // Invocazione remota
      ris = acquista_biglietti_1(&biglietto, cl);

      // Controllo del risultato
      if (ris == NULL) {
        // Errore di RPC
        clnt_perror(cl, host);
        exit(1);
      }

      if (*ris < 0)
        // Eventuale errore di logica del programma
        printf("[ERROR] nel server");
      else if (*ris == 0)
        // Tutto ok
        printf("Success\n");
    } // if 2
    // ALTRIMENTI
    else
      printf("[ERROR]Operazione richiesta non disponibile!!\n");

    printf("Inserire:\n1\t inserimentoEvento \n2\t Biglietti \n^D\tper "
           "terminare: ");

	} // while

	// Libero le risorse, distruggendo il gestore di trasporto
	clnt_destroy(cl);
	printf("TERMINO ");
	exit(0);
} // main
