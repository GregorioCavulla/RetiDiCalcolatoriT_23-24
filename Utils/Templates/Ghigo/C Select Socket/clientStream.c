#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define WORD_LENGTH 64  // lunghezza massima di una parola
#define MAX_INPUT 128   // lunghezza massima di un input
#define LINE_LENGTH 256 // lunghezza massima di una linea

int main(int argc, char const *argv[])
{

    struct hostent *host;                             // struttura per la gestione degli host, contiene informazioni come l'indirizzo IP dell'host e il suo nome
    struct sockaddr_in servAddr;                      // struttura per la gestione degli indirizzi, contiene informazioni come l'indirizzo IP e la porta, sia del client che del server
    int port, sd, nread;                              // variabili per la gestione della porta, del socket, della lunghezza dell'indirizzo, del risultato e del numero di caratteri letti
    char datoInput[MAX_INPUT], datoOutput[MAX_INPUT]; // dati in input e in output

    /* CONTROLLO ARGOMENTI ---------------------------------- */
    if (argc != 3)
    {
        printf("[ERR]:%s serverAddress serverPort\n", argv[0]);
        exit(1);
    }
    printf("[DEBUG] Client avviato\n");

    /* PREPARAZIONE INDIRIZZO SERVER ----------------------------- */
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
    port = atoi(argv[2]);
    // Verifico port
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
        servAddr.sin_addr.s_addr = ((struct in_addr *)(host->h_addr))->s_addr; // assegno l'indirizzo IP del server
        servAddr.sin_port = htons(port);                                       // assegno la porta del server
    }

    // CREAZIONE SOCKET
    sd = socket(AF_INET, SOCK_STREAM, 0); // creazione socket
    if (sd < 0)
    {
        perror("[ERR] apertura socket ");
        exit(3);
    }
    printf("[DEBUG] Creata la socket sd=%d\n", sd);
    // CONNECT e BIND IMPLICITA
    if (connect(sd, (struct sockaddr *)&servAddr, sizeof(struct sockaddr)) < 0)
    {
        perror("[ERR] Errore in connect");
        exit(4);
    }
    printf("[DEBUG] Connect ok\n");

    // CICLO INTERAZIONE
    int ris;

    printf("Inserire un dato (EOF per terminare): ");

    while (gets(datoInput)) // finche' c'e' input
    {
        /*
         * codice per la gestione dell'input da utente e output su socket, ricezione del risultato
         */

        // input check

        // invio richiesta
        write(sd, datoInput, strlen(datoInput));

        // ricezione risultato
        read(sd, &datoOutput, sizeof(datoOutput));

        // response check

        // stampa risultato

        printf("Inserire un dato (EOF per terminare): ");
    }

    // FINE CICLO INTERAZIONE

    // LIBERO LE RISORSE
    printf("[DEBUG] \nClient: termino...\n");
    shutdown(sd, SHUT_WR);
    shutdown(sd, SHUT_RD);
    close(sd);
    exit(0);
}
