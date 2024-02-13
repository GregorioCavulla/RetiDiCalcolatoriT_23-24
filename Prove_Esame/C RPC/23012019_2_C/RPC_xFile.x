/* RPC_xFile.x 
 *
 */

const N = 10;
const MAX_RIS = 6;

struct Prenotazione{
    char targa[8];
    char patente[6];
    char tipo[7];
    char immagini[13];
};

struct Output{
    Prenotazione ris[MAX_RIS];
};

struct Input{
    char targa[8];
    char patente[6];
};

struct Tipo{
    char tipo[7];
};

program OPERATIONPROG {
    version OPERATIONVERS {
        Output VISUALIZZA_PRENOTAZIONI(Tipo) = 1;
        int AGGIORNA_LICENZA(Input) = 2;
    } = 1;
} = 0x20000013;