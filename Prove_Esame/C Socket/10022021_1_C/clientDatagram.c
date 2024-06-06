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

typedef struct /*da modificare in base alle necessita*/ // struttura per la ricezione di una richiesta UDP
{
    char Identificatore[5];
    char dataString[WORD_LENGTH];
    int giorni;
    char modello[WORD_LENGTH];
    float costo_giornaliero;
    char nome_file_foto[WORD_LENGTH];

} Noleggio;

int main(int argc, char const *argv[])
{
    struct hostent *host;
    struct sockaddr_in clientAddr, servAddr;
    int port, sd, len, result, nread;
    Noleggio risposta;

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

    // CODICE DEL CLIENTE
    printf("Inserire  identificatore noleggio, Ctrl+D(Linux) o Ctrl+Z(Windows) per terminare: ");

    // CICLO INTERAZIONE
    char identificatore[5];
    int ris;
    while (gets(identificatore))
    {
        len = sizeof(servAddr);
        // invio identificatore al server
        if (sendto(sd, &identificatore, sizeof(identificatore), 0, (struct sockaddr *)&servAddr, len) < 0)
        { // 1
            perror("sendto");
            printf("Inserire  identificatore noleggio, Ctrl+D(Linux) o Ctrl+Z(Windows) per terminare: ");
            continue;
        }

        /* ricezione del risultato */
        if (recvfrom(sd, &ris, sizeof(ris), 0, (struct sockaddr *)&servAddr, &len) < 0)
        { // 2
            perror("recvfrom");
            // se questo invio fallisce il client torna all'inzio del ciclo
            printf("Inserire  identificatore noleggio, Ctrl+D(Linux) o Ctrl+Z(Windows) per terminare: ");
            continue;
        }

        ris = ntohl(ris);

        if (ris < 0)
        {
            printf("Cliente: Errore\n");
            continue;
        }

        if (ris == 0)
        {
            printf("Cliente: Noleggio non trovato\n");
            continue;
        }

        float costoTotale;

        if (ris >= 1)
        {
            printf("Cliente: Noleggio trovato\n");
            // ricezione della struttura
            if (recvfrom(sd, &costoTotale, sizeof(costoTotale), 0, (struct sockaddr *)&servAddr, &len) < 0)
            { // 3
                perror("recvfrom");
                printf("Inserire  identificatore noleggio, Ctrl+D(Linux) o Ctrl+Z(Windows)  per terminare: ");
                continue;
            }
            costoTotale = ntohl(costoTotale);
            printf("Il costo totale del noleggio %s e' %f\n", identificatore, costoTotale);
        }

        printf("Inserire identificatore noleggio , Ctrl+D(Linux) o Ctrl+Z(Windows)  per terminare: ");
    }

    // CICLO INTERAZIONE
    close(sd);
    printf("\n %s: termino...\n", argv[0]);
    exit(0);
}
