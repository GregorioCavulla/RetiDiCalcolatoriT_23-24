/* contaClient.c */

#include "servizio.h"
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
    static Input1 input1;
    static Input2 input2;

    if (argc != 2) {
        fprintf(stderr, "uso: %s nomehost\n", argv[0]);
        exit(1);
    }

    clnt = clnt_create(argv[1], SERVIZIOPROG, SERVIZIOVERS, "udp");
    if (clnt == NULL) {
        clnt_pcreateerror(argv[1]);
        exit(1);
    }

    printf("operazioni:  CO=Conta occorrenze, LF=Lista File\n");
    while (gets(ok)) {
        if ((strcmp(ok, "CO") != 0) && (strcmp(ok, "LF") != 0)) {
            printf("scelta non disponibile\n");
            printf("operazioni:  CO=Conta occorrenze, LF=Lista File\n");
            continue;
        }

        printf("Richiesto servizio: %s\n", ok);

        // richiesta conteggio occorrenze
        if (strcmp(ok, "CO") == 0) {
            printf("inserisci il nome del file: \n");
            gets(input1.nome);
            printf("inserisci la riga: \n");
            gets(input1.linea);

            ris = conta_occorrenze_linea_1(&input1, clnt);

            if (ris < 0) {
                clnt_perror(clnt, "E' avvenuto un errore lato server");
            } else {
                printf("Ho contato %d occorrenze della linea %s nel file %s\n", *ris, input1.linea,input1.nome);
            }

        } // CO
        // richiesta conteggio caratteri nel file remoto
        else if (strcmp(ok, "LF") == 0)
        {

            printf("inserisci il nome del direttorio: \n");
            gets(input2.direttorio);
            printf("Inserisci il prefisso dei file da cercare\n");
            gets(input2.prefix);

            out = lista_file_prefisso_1(&input2, clnt);

            if (out == NULL) {
                clnt_perror(clnt, "E' avvenuto un errore lato server");
            } else if (out->n_files == -1) {
                printf("Il direttorio richiesto non esiste sul server o errore in apertura di un file!\n");
            } else {
                printf("Ho trovato %d file con prefisso '%s' nel direttorio %s\n", out->n_files,input2.prefix,input2.direttorio);
                for(int i=0;i<out->n_files;i++){
                    printf("%s\n",out->files[i].name);
                }
            }

        } // CF
        else
        {
            printf("Servizio non supportato!\n");
        }
        printf("operazioni:  CO=Conta occorrenze, LF=Lista File\n");
    } // while

    clnt_destroy(clnt);
    printf("Esco dal client\n");
}
