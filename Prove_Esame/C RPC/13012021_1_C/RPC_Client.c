
#include <rpc/rpc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "RPC_xFile.h"

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
  void *voidValue;
  Output *out;

  static Name fileName;
  static Name dirName;

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

        printf("Inserire:\n1\t Lista dei sottodirettori \n
			2\t Elimina caratteri numerici \n^D\tper terminare: ");

	while (gets(buffer) != 0)
	{
    // OPERAZIONE 1
    if (strcmp(buffer, "1") == 0) {

      printf("inserire il nome del direttorio: \n");
      gets(dirName.name);

      // Invocazione remota
      out = lista_sottodir_1(&dirName, cl);

      // Controllo del risultato se punta ad un indirizzo vuoto
      if ((out == (int *)NULL)) {
        // Errore di RPC
        clnt_perror(cl, host);
        exit(1);
      }

      printf("Operazione fatta \n");
      // il valore all indirizzo puntato
      if (*out < 0) {
        printf("Problema del server");
      } else {
        printf("operazione riuscita");
      }

    } // if 1

    // SE OPERAZIONE 2
    else if (strcmp(buffer, "2") == 0) {

      printf("inserire il nome del direttorio: \n");
      gets(fileName.name);

      // Invocazione remota
      ris = elimina_occorrenze_1(&fileName, cl);

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
        printf("Ho eliminato %d caratteri numerici nel file %s\n", *ris,
               fileName.name);
    } // if 2
    // ALTRIMENTI
    else
      printf("[ERROR]Operazione richiesta non disponibile!!\n");

    printf("Inserire:\n1\t Lista dei sottodirettori \n
			2\t Elimina caratteri numerici \n^D\tper terminare: ");

	} // while

	// Libero le risorse, distruggendo il gestore di trasporto
	clnt_destroy(cl);
	printf("TERMINO ");
	exit(0);
} // main
