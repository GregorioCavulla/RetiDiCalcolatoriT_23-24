/* RPC_Client.c */

#include "RPC_xFile.h"
#include <rpc/rpc.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    CLIENT         *clnt;
    int            *ris;
    Output *out;
    char           *server;
    char            car, ok[128];

    // The variables used for RPC must be NOT stored on the local stack,
    // otherwise they are inaccessible from the stubs. So, we must either
    //  (a) allocate them using malloc (e.g., strings), or
    //  (b) declare them static
    static Filename fileName;
    static Filename dirName;

    if (argc != 2) {
        fprintf(stderr, "uso: %s nomehost\n", argv[0]);
        exit(1);
    }

    clnt = clnt_create(argv[1], XFILEPROG, XFILEVERS, "udp");
    if (clnt == NULL) {
        clnt_pcreateerror(argv[1]);
        exit(1);
    }

    printf("Operazioni:  EO=Elimina occorrenze, LF=Lista file di testo\n");
    while (gets(ok)) {
        if ((strcmp(ok, "EO") != 0) && (strcmp(ok, "LF") != 0)) {
            printf("scelta non disponibile\n");
            printf("Operazioni:  EO=Elimina occorrenze, LF=Lista file di testo\n");
            continue;
        }

        printf("Richiesto servizio: %s\n", ok);

        // richiesta eliminazione occorrenze
        if (strcmp(ok, "EO") == 0) {
            printf("inserisci il nome del file: \n");
            gets(fileName.name);

            ris = elimina_occorrenze_1(&fileName, clnt);

            if (ris < 0) {
                clnt_perror(clnt, "E' avvenuto un errore lato server");
            } else {
                printf("Ho eliminato %d caratteri numerici nel file %s\n", *ris, fileName.name);
            }

        } // EO

        
        // richiesta lista file di testo
        else if (strcmp(ok, "LF") == 0)
        {

            printf("Inserisci il nome del direttorio: \n");
            gets(dirName.name);

            out = lista_filetesto_1(&dirName, clnt);

            if (out == NULL) {
                clnt_perror(clnt, "E' avvenuto un errore lato server");
            } else if(out->n_files == -2){
                printf("Il direttorio richiesto non esiste sul server!\n");
            } else if (out->n_files == -1){
                printf("Errore in apertura di un file!\n");
            } else {
                printf("Ho trovato %d file di testo nel direttorio %s\n",out->n_files,dirName.name);
                for(int i=0;i<out->n_files;i++){
                    printf("%s\n",out->files[i].name);
                }
            }

        } // LF
        
        else
        {
            printf("Servizio non supportato!\n");
        }
        printf("Operazioni:  EO=Elimina occorrenze, LF=Lista file di testo\n");
    } // while

    clnt_destroy(clnt);
    printf("Esco dal client\n");
}
