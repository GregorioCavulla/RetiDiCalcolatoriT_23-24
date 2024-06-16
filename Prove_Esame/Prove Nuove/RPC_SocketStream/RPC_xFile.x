/*
*   RPC_xFile
*/


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
        int ELIMINA_OCCORRENZE(Name) = 1;
        Output LISTA_SOTTODIRETTORIO(Name) = 2;
    } = 1;
} = 0x20000020;
