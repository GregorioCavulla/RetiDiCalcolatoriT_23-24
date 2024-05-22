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
    int numeroDirettori;
    Name files[10];
};

/*
*   Definizione del programma FILEPROG
*/
program FILEPROG {
    version FILEVERS {
        int ELIMINA_OCCORRENZE_NUMERICI(Name) = 1;
        Output LISTA_SOTTODIRETTORI(Name) = 2;
    } = 1;
} = 0x20000020;