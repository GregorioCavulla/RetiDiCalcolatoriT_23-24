/* RPC_xFile.x 
 *
 */

struct Input{
    char direttorio[256];
    char carattere;
    int occorrenze;
};

struct FileName{
    char name[256];
};

struct OutputFileList{
    int nFiles;
    FileName files[6];
};

program FILEPROG {
    version FILEVERS{
        int ELIMINA_OCCORRENZE(FileName) = 1;
        OutputFileList LISTA_FILE_CARATTERE(Input) = 2;
    } = 1;
} = 0x20000015;



