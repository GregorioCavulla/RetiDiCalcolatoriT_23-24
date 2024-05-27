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

    int numeroFiles;
    Name files[6];
};

/*
*   Definizione del programma FILEPROG
*/
program FILEPROG {
    version FILEVERS {
        int ELIMINA_OCCORRENZE_NUMERICHE(Name) = 1;
        Output LISTA_FILE_DIRETTORIO(Name) = 2;
    } = 1;
} = 0x20000020;