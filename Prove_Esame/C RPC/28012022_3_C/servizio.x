/* servizio.x
 *
 */

struct Filename{
    char name[256];
};

struct Input1{
    char nome[256];
    char linea[256];
};

struct Input2{
    char direttorio[256];
    char prefix[8];
};

struct Output{
    int n_files;
    Filename files[6]; 
};

program SERVIZIOPROG{
    version SERVIZIOVERS{
        int CONTA_OCCORRENZE_LINEA(Input1) = 1;
        Output LISTA_FILE_PREFISSO(Input2) = 2;
    } = 1;
} = 0x20000015;