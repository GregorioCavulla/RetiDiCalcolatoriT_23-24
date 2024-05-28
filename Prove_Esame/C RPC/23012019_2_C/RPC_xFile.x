/*
*   RPC_xFile
*/

const N = 10;
const MAX_RIS = 6;

struct Prenotazione{
    char targa[8];
    char patente[6];
    char tipo[7];
    char folder_img[13];
};

struct Tipo{
    char tipo[7];
};

/*
*   Struttura di input 
*/
struct Input{
    char targa[8];
    char nuova_patente[6];
};

/*
*   Struttura di output
*/
struct Output{
    int check;
    Prenotazione prenotazioni[MAX_RIS];
};

/*
*   Definizione del programma FILEPROG
*/
program FILEPROG {
    version FILEVERS {
        Output VISUALIZZA_PRENOTAZIONI(Tipo) = 1;
        int AGGIORNA_LICENZA(Input) = 2;
    } = 1;
} = 0x20000020;