/*RPC_Client.c*/

#include "RPC_xFile.h"
#include <rpc/rpc.h>
#include <stdio.h>
#include <stdlib.h>

// Declaration for gets function
char *gets(char *s);

int main(int argc, char *argv[])
{
  CLIENT *cl;   // gestore del trasporto
  char *server; // nome host

  int *ris;            // Output of the RPC if integer
  OutputFileList *out; // Output of the RPC if struct

  char car, buffer[128]; // buffer per leggere stringhe

  // dichiarazioni statiche delle variabili per RPC che non possono essere
  // salvate sullo stack locale, fanno riferimento alle strutture dichiarate in
  // RPC_xFile.x
  static FileName nomeFile;
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
  printf("Operazioni: 1= elimina occorrenze, 2=lista file; ^D per terminare\n");

  while (gets(buffer))
  {
    // CONTROLLO OPERAZIONE INSERITA
    if (strcmp(buffer, "1") != 0 && strcmp(buffer, "2") != 0)
    {
      printf("Operazione non disponibile, inserire i valori indicati\n");
      printf(
          "Operazioni: 1= elimina occorrenze, 2=lista file; ^D per terminare\n");
      continue;
    }
    else if (strcmp(buffer, "1") == 0)
    { // OPERAZIONE 1, elimina occorrenze

      printf("Richiesta operazione: %s\n", buffer);
      // esempio operazione con input fileName e output intero
      printf("inserisci il nome del file: \n");
      gets(nomeFile.fileName);

      ris = elimina_occorrenze_1(&nomeFile, cl);

      if (ris < 0)
      {
        clnt_perror(cl, "E' avvenuto un errore lato server");
      }
      else if (ris == -1)
      {
        printf("file o directory non presente sul server \n");
      }
      else
      {
        printf("Successo: Ho eliminato %d numeri\n", ris);
      }
    }
    else if (strcmp(buffer, "2") == 0)
    { // OPERAZIONE 2 conteggio caratteri file remoto
      printf("Richiesta operazione: %s\n", buffer);
      // esempio operazione con input fileName e output struct
      printf("inserisci il nome del direttorio: \n");

      gets(input.direttorio);
      printf("inserisci il carattere: \n");
      gets(input.carattere);
      printf("inserisci il numero di occorrenze \n");

      // controllo che numero di occorrenze sia accettabile, fino a newline (\n)
      while (scanf("%d", &input.occorrenze) != 1)
      {
        do
        {
          car = getchar();
          printf("%c", car);
        } while (car != "\n");
        printf("inserisci un intero");
        continue;
      }
      gets(buffer);

      out = lista_file_carattere_1(&input, cl);

      if (out == NULL)
      {
        clnt_perror(cl, "E' avvenuto un errore lato server");
      }
      else if (out->numeroFiles == -1)
      { // parametro impostato dal server per
        //  segnalare errore, e.g. file o direttorio non presenti
        printf("Errore: il direttorio non esiste sul server");
      }
      else
      {
        printf("Successo: trovato %d file di testo con più di %d occorrenze nel nome del carattere %c", out->files, input.occorrenze, input.carattere);
        for (int i = 0; i < out->numeroFiles; i++)
        {
          printf("%s\n", out->files[i].fileName);
        }
      }
    } // endif
  }

  // DISTRUZIONE GESTORE DI TRASPORTO
  clnt_destroy(cl);
  printf("Client: termino...\n");
  exit(0);
}