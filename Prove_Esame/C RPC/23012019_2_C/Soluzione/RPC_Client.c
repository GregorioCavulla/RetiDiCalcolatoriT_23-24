/* RPC_Client.c */

#include "RPC_xFile.h"
#include <rpc/rpc.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    CLIENT *clnt;
    Output *output;
    int *ris;
    char *server;
    char ok[128];

    // The variables used for RPC must be NOT stored on the local stack,
    // otherwise they are inaccessible from the stubs. So, we must either
    //  (a) allocate them using malloc (e.g., strings), or
    //  (b) declare them static
    static Tipo tipo;
    static Input input;

    if (argc != 2) {
        fprintf(stderr, "uso: %s nomehost\n", argv[0]);
        exit(1);
    }
    server = argv[1];

    clnt = clnt_create(server, OPERATIONPROG, OPERATIONVERS, "udp");
    if (clnt == NULL) {
        clnt_pcreateerror(server);
        exit(1);
    }

    printf("Operazioni:  VP=Visualizza prenotazioni, AL=Aggiorna licenza\n");
    while (gets(ok)) {
        if ((strcmp(ok, "VP") != 0) && (strcmp(ok, "AL") != 0)) {
            printf("Scelta non disponibile\n");
            printf("Operazioni:  VP=Visualizza prenotazioni, AL=Aggiorna licenza\n");
            continue;
        }

        printf("Richiesto servizio: %s\n", ok);

        // visualizza prenotazioni di un certo tipo
        if (strcmp(ok, "VP") == 0) {
            printf("Inserisci il tipo di veicolo: \n");
            gets(tipo.tipo);
            printf("inserisci la soglia: \n");

            output = visualizza_prenotazioni_1(&tipo, clnt);

            if (output == NULL) {
                clnt_perror(clnt, "E' avvenuto un errore lato server");
            } 

            printf("Veicoli trovati\n");
            for(int i=0;i<N;i++){
                printf("%s|%s|%s|%s\n",output->ris[i].targa,output->ris[i].patente,output->ris[i].tipo,output->ris[i].immagini);
            }

        } // VP

        // aggiorna numero di patente per una certa targa
        else if (strcmp(ok, "AL") == 0)
        {

            printf("Inserisci il numero di patente aggiornato: \n");
            gets(input.patente);
            printf("Inserisci il numero di targa: \n");
            gets(input.targa);

            ris = aggiorna_licenza_1(&input, clnt);

            if (ris == NULL) {
                clnt_perror(clnt, "E' avvenuto un errore lato server");
            } 

            if(*ris == -1){
                printf("Impossibile aggiornare il numero di patente per il veicolo selezionato\n");
            }

            if(*ris == 0){
                printf("Numero di patente aggiornato correttamente per il veicolo %s\n",input.targa);
            }

        } // AL
        else
        {
            printf("Servizio non supportato!\n");
        }
        printf("Operazioni:  VP=Visualizza prenotazioni, AL=Aggiorna licenza\n");
    } // while

    clnt_destroy(clnt);
    printf("Esco dal client\n");
}