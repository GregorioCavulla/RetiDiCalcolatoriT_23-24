const DATI = 10;
const MAX_STRING =128;

struct Evento {
		string desc<MAX_STRING>;
		string tipo<MAX_STRING>;
		string data<MAX_STRING>;
		string luogo<MAX_STRING>;
		string disp<MAX_STRING>;
		string prez<MAX_STRING>;

	};


struct Biglietti{
    string desc<MAX_STRING>;
	int nBiglietti;
};

program OPERATION {
	version OPERATIONVERS {  
		int INSERIMENTO_EVENTO(Evento) = 1;
        int ACQUISTA_BIGLIETTI(Biglietti)=2;
	} = 1;
} = 0x20000020;