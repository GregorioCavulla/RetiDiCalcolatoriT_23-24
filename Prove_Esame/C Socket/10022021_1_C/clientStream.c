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

    struct hostent *host;        // struttura per la gestione degli host, contiene informazioni come l'indirizzo IP dell'host e il suo nome
    struct sockaddr_in servAddr; // struttura per la gestione degli indirizzi, contiene informazioni come l'indirizzo IP e la porta, sia del client che del server
    int port, sd, nread;         // variabili per la gestione della porta, del socket, della lunghezza dell'indirizzo, del risultato e del numero di caratteri letti

    // dichiarazione variabili per gestione richiesta

    char modello[WORD_LENGTH];
    Noleggio risposta; // dati in input e in output
    int nRisposte;

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

    int fileLength;
    char buff[255];
    int img_fd;

    printf("Inserire il modello dello sci (EOF per terminare): ");

    while (gets(modello)) // finche' c'e' input
    {
        /*
         * codice per la gestione dell'input da utente e output su socket, ricezione del risultato
         */

        // invio richiesta
        if (write(sd, modello, strlen(modello) + 1) < 0)
        {
            perror("write");
            printf("Nome del modello, EOF per terminare: ");
            continue;
        }

        // ricezione risultato

        if (read(sd, &nRisposte, sizeof(nRisposte)) < 0)
        {
            perror("read");
            printf("Nome del modello, EOF per terminare: ");
            continue;
        }

        printf("trovate %d prenotazioni per il modello: %s", nRisposte, modello);

        for (int i = 0; i <= nRisposte; i++)
        {
            if (read(sd, &risposta, sizeof(risposta)) < 0)
            {
                perror("read");
                printf("Nome del modello, EOF per terminare: ");
                continue;
            }

            img_fd = open(risposta.nome_file_foto, O_WRONLY | O_CREAT, 0777);

            if (read(sd, &fileLength, sizeof(fileLength)) < 0)
            {
                perror("read");
                printf("Nome del modello, EOF per terminare: ");
                continue;
            }

            // fileLength = ntohl(fileLength);

            while (fileLength > 0)
            {
                if (sizeof(buff) < fileLength)
                {
                    nread = read(sd, buff, sizeof(buff));
                }
                else
                {
                    nread = read(sd, buff, fileLength);
                }

                if (nread <= 0)
                {
                    break;
                }

                write(img_fd, buff, nread);

                fileLength -= nread;
            }

            close(img_fd);

            printf("%s|%s|%d|%s|%f|%s\n", risposta.Identificatore, risposta.dataString, risposta.giorni, risposta.modello, risposta.costo_giornaliero, risposta.nome_file_foto);
        }

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
