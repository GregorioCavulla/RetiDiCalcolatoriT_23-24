/*
*   RPC_xFile
*/

/*
*   Struttura di input 
*/
struct Input{
    /*
    *   Eventuali campi per l'input, e.g.
    */
    Name fileName;
    int occorrenze;
    char carattere;
};


/*
*   Struttura nome per file o direttori
*/
struct Name{
    char name[256];
};

/*
*   Struttura di output
*/
struct Output{
    /*
    *   Eventuali campi di output, e.g.
    */
    int numeroDirettori;
    Name files[6];
};

/*
*   Definizione del programma FILEPROG
*/
program FILEPROG {
    version FILEVERS {
        <T> OPERAZIONE_1(<parametro>) = 1;
        <T> OPERAZIONE_2(<parametro>) = 2;
    } = 1;
} = 0x20000020;