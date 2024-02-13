/*
 * RPC_xFile.x
 */

struct Filename{
    char name[256];
};

struct Output{
    int n_files;
    Filename files[6];
};

program XFILEPROG{
    version XFILEVERS{
        int ELIMINA_OCCORRENZE(Filename) = 1;
        Output LISTA_FILETESTO(Filename) = 2;
    } = 1;
} = 0x20000015;