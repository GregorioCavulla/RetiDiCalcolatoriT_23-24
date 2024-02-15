/* RPC_xFile
*/

struct Name{
	char name[256];
};

struct Output{
	int numeroDirettori;
	Name files[6];
};

program FILEPROG {
	version FILEVERS {  
		int ELIMINA_OCCORRENZE(Name) = 1;
        OutputDirList LISTA_SOTTODIRETTORI(Name)=2;
	} = 1;
} = 0x20000020;