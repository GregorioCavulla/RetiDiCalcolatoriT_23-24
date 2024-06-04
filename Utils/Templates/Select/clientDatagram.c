#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#define WORD_LENGTH 64  // lunghezza massima di una parola
#define MAX_INPUT 128   // lunghezza massima di un input
#define LINE_LENGTH 256 // lunghezza massima di una linea

typedef struct /*da modificare in base alle necessita */ // struttura per la richiesta UDP al server
{
    char fileName[WORD_LENGTH];
    char parola[WORD_LENGTH];
} ReqUDP;

int main(int argc, char const *argv[])
{
    struct hostent *host;                                 // struttura per la gestione degli host, contiene informazioni come l'indirizzo IP dell'host e il suo nome
    struct sockaddr_in clientAddr, servAddr;              // struttura per la gestione degli indirizzi, contiene informazioni come l'indirizzo IP e la porta, sia del client che del server
    int port, sd, len, result, nread;                     // variabili per la gestione della porta, del socket, della lunghezza dell'indirizzo, del risultato e del numero di caratteri letti
    ReqUDP req; /*da modificare in base alle necessita */ // struttura per la richiesta UDP al server

    /* CONTROLLO ARGOMENTI ---------------------------------- */
    if (argc != 3)
    {
        printf("Error: %s serverAddress serverPort\n", argv[0]);
        exit(1);
    }

    // INIZIALIZZAIONE INDIRIZZO CLIENT
    memset((char *)&clientAddr, 0, sizeof(struct sockaddr_in));
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr = INADDR_ANY;
    clientAddr.sin_port = 0;
    // INIZIALIZZAZIONE INDIRIZZO SERVER
    memset((char *)&servAddr, 0, sizeof(struct sockaddr_in));
    servAddr.sin_family = AF_INET;
    host = gethostbyname(argv[1]);

    // VERIFICA INTERO porta
    nread = 0;
    while (argv[2][nread] != '\0')
    {
        if ((argv[2][nread] < '0') || (argv[2][nread] > '9'))
        {
            printf("Secondo argomento non intero\n");
            printf("Error:%s serverAddress serverPort\n", argv[0]);
            exit(2);
        }
        nread++;
    }
    port = atoi(argv[2]); // converto la porta in intero

    // Verifico che la porta sia corretta, e nella giusta fascia
    if (port < 1024 || port > 65535)
    {
        printf("%s = porta scorretta...\n", argv[2]);
        exit(2);
    }
    // Verifico host
    if (host == NULL)
    {
        printf("%s not found in /etc/hosts\n", argv[1]);
        exit(2);
    }
    else
    {
        servAddr.sin_addr.s_addr = ((struct in_addr *)(host->h_addr))->s_addr; // assegno l'indirizzo IP del server all'indirizzo del server
        servAddr.sin_port = htons(port);                                       // assegno la porta del server alla porta del server
    }

    /* CREAZIONE SOCKET ---------------------------------- */
    sd = socket(AF_INET, SOCK_DGRAM, 0); // creo la socket
    if (sd < 0)
    {
        perror("[ERR] apertura socket");
        exit(1);
    }
    printf("[DEBUG] %s: creata la socket sd=%d port=%d \n", argv[0], sd, port);

    /* BIND SOCKET, a una porta scelta dal sistema --------------- */
    if (bind(sd, (struct sockaddr *)&clientAddr, sizeof(clientAddr)) < 0)
    {
        perror("[ERR] bind socket ");
        exit(1);
    }
    printf("[DEBUG] %s: bind socket ok, alla porta %i\n", argv[0], clientAddr.sin_port);

    // CODICE DEL CLIENTE
    printf("Inserire  ---------- , Ctrl+D(Linux) o Ctrl+Z(Windows)  per terminare: ");

    // CICLO INTERAZIONE
    int ris;
    while (gets(req.fileName)) // dato da input, usato per la richiesta al server
    {
        // invio richiesta una struttura di 2 dati
        len = sizeof(servAddr);

        if (sendto(sd, &req, sizeof(req), 0, (struct sockaddr *)&servAddr, len) < 0) // invio la richiesta al server
        {
            perror("sendto");
            // se questo invio fallisce il client torna all'inzio del ciclo
            printf("Dammi il nome di file, EOF per terminare: ");
            continue;
        }

        /* ricezione del risultato */
        if (recvfrom(sd, &ris, sizeof(ris), 0, (struct sockaddr *)&servAddr, &len) < 0) // ricevo il risultato dal server
        {
            perror("recvfrom");
            // se questo invio fallisce il client torna all'inzio del ciclo
            printf("Dammi il nome di file, EOF per terminare: ");
            continue;
        }
        if (ris < 0)
        {
            printf("Cliente: Errore\n");
        }
        else
        {
            printf("Cliente: risultato ottenuto = %d\n", ris);
        }
        printf("Inserire  ---------- , Ctrl+D(Linux) o Ctrl+Z(Windows)  per terminare: ");
    }

    // CICLO INTERAZIONE
    close(sd);
    printf("\n %s: termino...\n", argv[0]);
    exit(0);
}
