#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#define LINE_LENGTH 256
#define MAX_INPUT 128
#define WORD_LENGTH 64

int main(int argc, char const *argv[])
{
    /* inizializzazioni variabili per la comunicazione */

    struct hostent *host;
    struct sockaddr_in clientAddr, servAddr;
    int port, sd, len, result, nread, ris;

    char fileName[LINE_LENGTH];

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
        servAddr.sin_addr.s_addr = ((struct in_addr *)(host->h_addr))->s_addr;
        servAddr.sin_port = htons(port);
    }

    /* CREAZIONE SOCKET ---------------------------------- */
    sd = socket(AF_INET, SOCK_DGRAM, 0);
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

    /**
     * CORPO DEL CLIENT:
     */

    printf("Inserire il nome del file , Ctrl+D(Linux) o Ctrl+Z(Windows)  per terminare: ");

    while (gets(fileName))
    {
        // invio del nome del file
        if (sendto(sd, &fileName, sizeof(fileName), 0, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
        {
            perror("sendto");
            // se questo invio fallisce il client torna all'inzio del ciclo
            printf("Inserire il nome del file , Ctrl+D(Linux) o Ctrl+Z(Windows)  per terminare: ");
            continue;
        }

        // ricevo il risultato (numero di vocali eliminate)
        if (recvfrom(sd, &ris, sizeof(ris), 0, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
        {
            perror("recvfrom");
            // se questo invio fallisce il client torna all'inzio del ciclo
            printf("Dammi il nome di file, EOF per terminare: ");
            continue;
        }

        ris = ntohl(ris); // ricostruisco il numero di vocali eliminato (da network a host byte order)

        if (ris < 0) // stabilito nel codice del server
        {
            printf("Cliente: Errore\n");
        }
        else
        {
            printf("Cliente: eliminate %d vocali \n", ris);
        }
        printf("Inserire  il nome del file , Ctrl+D(Linux) o Ctrl+Z(Windows)  per terminare: ");
    }

    // CICLO INTERAZIONE
    close(sd);
    printf("\n %s: termino...\n", argv[0]);
    exit(0);
}
