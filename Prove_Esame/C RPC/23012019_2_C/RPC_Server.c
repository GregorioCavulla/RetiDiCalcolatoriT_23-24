/* RPC_Server.c 
 * visualizzare tutte le prenotazioni di un certo tipo di veicolo
 * agiornare il numero di patente
*/

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
static int inizializzato=0;

void inizializza(){
    if (inizializzato == 1){
        return;
    }

    inizializzato = 1;
    for(int i=0;i<N;i++){        
        strcpy(table[i].targa,"L");
        strcpy(table[i].patente,"L");
        strcpy(table[i].tipo,"L");
        strcpy(table[i].immagini,"L");
    }

    strcpy(table[0].targa,"DG547BV");
    strcpy(table[0].patente,"12345");
    strcpy(table[0].tipo,"auto");
    strcpy(table[0].immagini,"DG547BV_img/");

    strcpy(table[1].targa,"EY211LA");
    strcpy(table[1].patente,"54321");
    strcpy(table[1].tipo,"auto");
    strcpy(table[1].immagini,"EY211LA_img/");

    strcpy(table[2].targa,"EE123DE");
    strcpy(table[2].patente,"15486");
    strcpy(table[2].tipo,"camper");
    strcpy(table[2].immagini,"EE123DE_img/");
    
}

/********************************************************/
Output *visualizza_prenotazioni_1_svc(Tipo *input, struct svc_req *rp) {
    static Output result;
    int j=0;

    if(inizializzato==0){
        inizializza();
    }

    printf("Richieste prenotazioni di tipo:%s \n", input->tipo);
    
    for(int i=0;i<N;i++){
        if(table[i].targa[0] > 'E' || (table[i].targa[0] == 'E' && table[i].targa[0] > 'D')){
            if(strcmp(table[i].tipo,input->tipo)==0){
                result.ris[j++] = table[i];
            }

            if(j >= 6){
                return(&result);
            }
        }
    }

    return (&result);
    
}

int *aggiorna_licenza_1_svc(Input *input, struct svc_req *rp) {
    static int result=-1;
    int found=0;

    if(inizializzato==0){
        inizializza();
    }

    printf("Richiesto aggiornamento patente(%s) della targa:%s\n", input->patente,input->targa);

    for(int i=0;i<N;i++){
        if(found==0){
            if(strcmp(table[i].targa,input->targa)==0){
                strcpy(table[i].patente,input->patente);
                found=1;
                result=0;
            }
        }
    }

    return(&result);
}
