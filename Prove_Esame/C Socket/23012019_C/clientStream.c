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
#define N 10

typedef struct /*da modificare in base alle necessita*/ // struttura per la ricezione di una richiesta UDP
{
    char targa[8];
    char patente[6];
    char tipoVeicolo[7];
    char img_folder[14];

} Prenotazione;

int main(int argc, char const *argv[])
{
    /* inizializzazioni variabili per la comunicazione */
    struct hostent *host;
    struct sockaddr_in servAddr;
    int port, sd, nread;
    char targa[MAX_INPUT], datoOutput[MAX_INPUT];

    // inizializzazioni variabili
    int risposta, fd, numberOfFiles;
    char dirName[WORD_LENGTH], bufferChar, fileName[WORD_LENGTH], response;
    long fileLength;

    int numeroPrenotazioni;

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
        servAddr.sin_addr.s_addr = ((struct in_addr *)(host->h_addr))->s_addr;
        servAddr.sin_port = htons(port);
    }

    // CREAZIONE SOCKET
    sd = socket(AF_INET, SOCK_STREAM, 0);
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

    /**
     * CICLO INTERAZIONE
     */

    printf("Inserire  numero di targa , Ctrl+D(Linux) o Ctrl+Z(Windows)  per terminare: ");

    while (gets(targa))
    {

        // invio richiesta
        if (write(sd, targa, strlen(targa)) < 0)
        {
            perror("write");
            printf("Inserire  numero di targa , Ctrl+D(Linux) o Ctrl+Z(Windows)  per terminare: ");
        }

        // ricezione risultato
        if (read(sd, &risposta, sizeof(risposta)) < 0)
        {
            perror("read");
            printf("Inserire  numero di targa , Ctrl+D(Linux) o Ctrl+Z(Windows)  per terminare: ");
            continue;
        }

        risposta = ntohl(risposta);

        if (risposta == -1)
        {
            printf("Errore server\n");
            printf("Inserire  numero di targa , Ctrl+D(Linux) o Ctrl+Z(Windows)  per terminare: ");
            continue;
        }
        else
        {
            numeroPrenotazioni = risposta;
        }

        for (int i = 0; i < numeroPrenotazioni; i++)
        {

            if (read(sd, &numberOfFiles, sizeof(numberOfFiles)) < 0)
            {
                perror("read");
                printf("Inserire  numero di targa , Ctrl+D(Linux) o Ctrl+Z(Windows)  per terminare: ");
                continue;
            }

            numberOfFiles = ntohl(numberOfFiles);

            for (int j = 0; j < numberOfFiles; j++)
            {
                if (read(sd, &fileName, sizeof(fileName)) < 0)
                {
                    perror("read");
                    printf("Inserire  numero di targa , Ctrl+D(Linux) o Ctrl+Z(Windows)  per terminare: ");
                    continue;
                }

                if ((fd = open(fileName, O_WRONLY | O_CREAT, 0777)) < 0)
                {
                    perror("open");
                    printf("Inserire  numero di targa , Ctrl+D(Linux) o Ctrl+Z(Windows)  per terminare: ");
                    continue;
                }

                if (read(sd, &fileLength, sizeof(fileLength)) < 0)
                {
                    perror("read");
                    printf("Inserire  numero di targa , Ctrl+D(Linux) o Ctrl+Z(Windows)  per terminare: ");
                    continue;
                }

                fileLength = ntohl(fileLength);

                printf("[DEBUG] fileLength: %ld\n", fileLength);

                while (fileLength > 0)
                {
                    nread = read(sd, &bufferChar, sizeof(char));
                    if (nread <= 0)
                    {
                        perror("read");
                        break;
                    }

                    if (bufferChar == '\1') // se il carattere è il carattere di fine file
                    {
                        break;
                    }
                    else if (bufferChar == '\0') // se il carattere è il carattere di fine riga
                    {
                        printf("\n");
                    }
                    if ((write(fd, &bufferChar, sizeof(char))) < 0)
                    {
                        perror("write");
                        break;
                    }
                    fileLength -= nread;
                }

                printf("file ricevuto\n");
                if (close(fd) < 0)
                {
                    perror("close");
                }
            }
        }
        printf("Inserire  numero di targa , Ctrl+D(Linux) o Ctrl+Z(Windows)  per terminare: ");
    }

    // FINE CICLO INTERAZIONE

    // LIBERO LE RISORSE
    printf("[DEBUG] \nClient: termino...\n");
    shutdown(sd, SHUT_WR);
    shutdown(sd, SHUT_RD);
    close(sd);
    exit(0);
}
