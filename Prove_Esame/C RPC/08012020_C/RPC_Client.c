
#include "RPC_xFile.h"
#include <rpc/rpc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  OutputFileList *out;
  void *voidValue;

  static FileName nome_file;
  static Input input;

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
  printf(" Operazioni: 1 per Elimina caratteri numerici in un file, 2 per "
         "Lista file carattere \n");
  while (gets(buffer) != 0) {
    if (strcmp(buffer, "1") == 0) { // Elimina caratteri numerici in un file
      printf("inserisci il nome di un file: \n");
      if (gets(nome_file.name) != 0) {
        ris = elimina_occorrenze_1(&nome_file, cl);
        if (ris == NULL) {
          clnt_perror(cl, host);
          exit(1);
        } else if (*ris == -1) {
          printf("Il direttorio richiesto non esiste sul server!\n");
        } else {
          printf("Ho eliminato %d numeri:\n", *ris);
        }
      } else {
        printf("[ERROR] Errore di input\n");
      }

    } else if (strcmp(buffer, "2") ==
               0) { // Richiesta conteggio caratteri nel file

      printf("inserisci il nome del direttorio: \n");
      if (gets(input.direttorio) != 0) {
        printf("inserisci il caratter: \n");
        if (gets(input.carattere) != 0) {
          printf("inserisci il numero di occorrenze: \n");
          // controllo intero
          while (scanf("%d", &input.occorrenze) != 1) {
            do {
              c = getchar();
              printf("%c", c);
            } while (c != '\n');
            printf("Inserire un intero\n");
            continue;
          } // fine controllo intero
          out = lista_file_carattere_1(&input, cl);
          if (out == NULL) {
            clnt_perror(cl, host);
            exit(1);
          } else if (out->numeroFiles == -1) {
            printf("Il direttorio richiesto non esiste sul server!\n");
          } else {
            printf("Ho trovato %d file:\n", out->numeroFiles);
            for (i = 0; i < out->numeroFiles; i++) {
              printf("%s\n", out->files[i].name);
            }
          }

        } else {
          printf("[ERROR] Errore di input\n");
        }
      } else {
        printf("[ERROR] Errore di input\n");
      }
    }      // if 2
    else { // Operazione richiesta non disponibile
      printf("[ERROR]Operazione richiesta non disponibile!!\n");
    }
    printf("Inserire:\n1\t inserimentoEvento \n2\t Biglietti \n^D\tper "
           "terminare: ");

  } // while

  // Libero le risorse, distruggendo il gestore di trasporto
  clnt_destroy(cl);
  printf("TERMINO ");
  exit(0);
} // main
