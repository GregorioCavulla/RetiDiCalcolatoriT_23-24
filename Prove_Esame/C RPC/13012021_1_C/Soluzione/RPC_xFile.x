/* 
 *  RPC_xFile.x
 */

struct Name{
    char name[256];
};

struct Output{
    int n_files;
    Name files[6];
};

program XFILEPROG{
    version XFILEVERS{
        int ELIMINA_OCCORRENZE(Name) = 1;
        Output LISTA_SOTTODIRETTORI(Name) = 2;
    } = 1;
} = 0x20000015;