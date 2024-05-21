/*
*   RPC_xFile
*/

/*
*   Struttura di input 
*/
struct Input{
    char direttorio[256];
    char carattere;
    int occorrenze;
};

struct FileName{
    char fileName[256];
};

/*
*   Struttura di output
*/
struct OutputFileList{
    int numeroFiles;
    FileName files[6];
};

/*
*   Definizione del programma FILEPROG
*/
program FILEPROG {
    version FILEVERS {
        int ELIMINA_OCCORRENZE(FileName) = 1;
        OutputFileList LISTA_FILE_CARATTERE(Input) = 2;
    } = 1;
} = 0x20000020;