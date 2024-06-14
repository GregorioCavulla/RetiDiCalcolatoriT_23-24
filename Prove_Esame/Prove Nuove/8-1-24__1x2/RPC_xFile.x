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
*   Struttura di input 
*/
struct Input{

    Name dirName;
    char carattere;
    int occorrenze;
};

/*
*   Struttura di output
*/
struct Output{

    Name files[7];
};

/*
*   Definizione del programma FILEPROG
*/
program FILEPROG {
    version FILEVERS {
        int ELIMINA_OCCORRENZE(Name) = 1;
        Output LISTA_FILE_CARATTERE(Input) = 2;
    } = 1;
} = 0x20000020;
