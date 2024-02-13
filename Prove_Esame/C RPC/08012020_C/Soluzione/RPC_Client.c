/* Client.c */

#include "RPC_xFile.h"
#include <rpc/rpc.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    CLIENT         *clnt;
    int            *ris;
    OutputFileList *out;
    char           *server;
    char            car, ok[128];

    // The variables used for RPC must be NOT stored on the local stack,
    // otherwise they are inaccessible from the stubs. So, we must either
    //  (a) allocate them using malloc (e.g., strings), or
    //  (b) declare them static
    static FileName nome_file;
    static Input    input;

    if (argc != 2) {
        fprintf(stderr, "uso: %s nomehost\n", argv[0]);
        exit(1);
    }

    clnt = clnt_create(argv[1], FILEPROG, FILEVERS, "udp");
    if (clnt == NULL) {
        clnt_pcreateerror(argv[1]);
        exit(1);
    }

    printf("Operazioni: EO:Elimina caratteri numerici in un file, LF:Lista file carattere \n");
    while (gets(ok)) {
        if ((strcmp(ok, "EO") != 0) && (strcmp(ok, "LF") != 0)) {
            printf("scelta non disponibile\n");
            printf("Operazioni: EO:Elimina caratteri numerici in un file, LF:Lista file carattere \n");
            continue;
        }

        printf("Richiesto servizio: %s\n", ok);

        // richiesta eliminazione caratteri numerici
        if (strcmp(ok, "EO") == 0) {
            printf("inserisci il nome di un file: \n");
            gets(nome_file.name);

            ris = elimina_occorrenze_1(&nome_file, clnt);

            if (ris == NULL) {
                clnt_perror(clnt, "E' avvenuto un errore lato server");
            } else if (ris == -1) {
                printf("Il direttorio richiesto non esiste sul server!\n");
            } else {
                printf("Ho eliminato %d numeri:\n", ris);
            }

            // clean input buffer
            memset(nome_file.name, 0, sizeof(nome_file));

        } // EO
        // richiesta conteggio caratteri nel file remoto
        else if (strcmp(ok, "LF") == 0)
        {

            printf("inserisci il nome del direttorio: \n");
            gets(input.direttorio);
            printf("inserisci il caratter: \n");
            gets(input.carattere);
            printf("inserisci il numero di occorrenze: \n");

            //controllo intero
            while (scanf("%d", &input.occorrenze) != 1){
                do{
                    car = getchar();
                    printf("%c",car);
                } while(car != '\n');
                printf("Inserire int");
                continue;
            }
            gets(ok);

            out = lista_file_carattere_1(&input, clnt);

            if (out == NULL) {
                clnt_perror(clnt, "E' avvenuto un errore lato server");
            } else if (out->nFiles == -1) {
                printf("Il file richiesto non esiste sul server!\n");
            } else {
                printf("Trovati %d file di testo con piu' di %d occorrenze nel nome del carattere %c\n",out->nFiles,input.occorrenze,input.carattere);
                for(int i=0;i<out->nFiles;i++){
                    printf("%s\n",out->files[i].name);
                }
            }



        } // CF
        else
        {
            printf("Servizio non supportato!\n");
        }
        printf("Operazioni: EO:Elimina caratteri numerici in un file, LF:Lista file carattere \n");
    } // while

    clnt_destroy(clnt);
    printf("Esco dal client\n");
}