#include <rpc/rpc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "xFile.h"
#define MAX_DATI 10
#define MAX_STRING 128

/*STATO INTERNO PRIVATO DEL SERVER*/
typedef struct
{
	char desc[MAX_STRING];
	char tipo[MAX_STRING];
	char data[MAX_STRING];
	char luogo[MAX_STRING];
	char disp[MAX_STRING];
	char prez[MAX_STRING];

} Dato;

//variabili globali statiche
static Dato dati[MAX_DATI];
static int inizializzato = 0;

/*Stato interno parzialmente implementato*/
void inizializza()
{
	int i;

	if (inizializzato == 1)
		return;

	for (i = 0; i < MAX_DATI; i++)
	{
		strcpy(dati[i].desc, "L");
		strcpy(dati[i].tipo, "L");
		strcpy(dati[i].data, "L");
		strcpy(dati[i].luogo, "L");
		strcpy(dati[i].disp, "L");
		strcpy(dati[i].prez, "L");
	}
	//alcuni dati da inizializzare
	strcpy(dati[1].desc, "String");
	strcpy(dati[1].tipo, "Verona");
	strcpy(dati[1].data, "11/01/2014");
	strcpy(dati[1].luogo, "Verona");
	strcpy(dati[1].disp, "40");
	strcpy(dati[1].prez, "40");

	strcpy(dati[2].desc, "Junentus-Inger");
	strcpy(dati[2].tipo, "Calcio");
	strcpy(dati[2].data, "03/05/2014");
	strcpy(dati[2].luogo, "Torino");
	strcpy(dati[2].disp, "21");
	strcpy(dati[2].prez, "150");

	strcpy(dati[8].desc, "GP Bologna");
	strcpy(dati[8].tipo, "Formula1");
	strcpy(dati[8].data, "07/09/2014");
	strcpy(dati[8].luogo, "Bologna");
	strcpy(dati[8].disp, "10");
	strcpy(dati[8].prez, "200");
	//ecc

	inizializzato = 1;
	printf("[DEBUG] Terminata inizializzazione struttura dati!\n");
}
void stampa()//metodo di stampa usato per il DEBUG interno
{
	for (int i = 0; i < MAX_DATI; i++)
	{

		printf("%s \t", dati[i].desc);
		printf("%s \t", dati[i].tipo);
		printf("%s \t", dati[i].data);
		printf("%s \t", dati[i].luogo);
		printf("%s \t", dati[i].disp);
		printf("%s \t\n", dati[i].prez);
	}
}

//PROCEDURA 1
int *inserimento_evento_1_svc(Evento *evento, struct svc_req *cl)
{
	inizializza();
	printf("[DEBUG] Inserimento evento!\n");

	static int esito;
	int i;
	esito = -1;
	if (evento == NULL)
	{
		return &esito;
	}



	//
	for (i = 0; i < MAX_DATI; i++)
	{
		if (strcmp(dati[i].desc, "L") == 0)
		{
			printf("[DEBUG] trovato evento libero!\n");

			strcpy(dati[i].desc, evento->desc);
			strcpy(dati[i].tipo, evento->tipo);
			strcpy(dati[i].data, evento->data);
			strcpy(dati[i].luogo, evento->luogo);
			strcpy(dati[i].disp, evento->disp);
			strcpy(dati[i].prez, evento->prez);

			//ecc
			esito = 0;
			break;
		}
	}
	printf("[DEBUG] fine inserimento evento!\n");
	stampa();

	return &esito;
}


//PROCEDURA 2
int *acquista_biglietti_1_svc(Biglietti *biglietto, struct svc_req *cl)
{
	inizializza();
	static int esito;

	printf("[DEBUG] acquista!\n");
	int i;
	esito = -1;

	if (biglietto == NULL)
	{
		return &esito;
	}
	
	for (i = 0; i < MAX_DATI; i++)
	{
		if (strcmp(dati[i].desc, biglietto->desc) == 0 && (atoi(dati[i].disp) - biglietto->nBiglietti) >= 0)
		{
			printf("[DEBUG] trovato evento acquistatibile!\n");

			//
			int fin = atoi(dati[i].disp) - biglietto->nBiglietti;
			strcpy(dati[i].disp, fin);
			esito = 0;
			break;
		}
	}
	printf("[DEBUG] fine acquista!\n");
	stampa();

	return &esito;
}