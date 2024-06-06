#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define LINE_LENGTH 256
#define MAX_INPUT 128
#define WORD_LENGTH 64

int main(int argc, char const *argv[])
{

    struct hostent *host;
    struct sockaddr_in servAddr;
    int port, sd, nread;
    char datoInput[MAX_INPUT], datoOutput[MAX_INPUT];

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

    // CICLO INTERAZIONE
    int ris, fd;
    char dirName[WORD_LENGTH], bufferChar, fileName[WORD_LENGTH];
    long fileLength;

    printf("inserisci il nome del direttorio, ^D per terminare: ");
    while (gets(dirName))
    {
        dirName[strcspn(dirName, "\n")] = 0; // Remove newline character
        write(sd, dirName, sizeof(dirName));

        char response;
        int numberOfFiles;
        read(sd, &response, sizeof(response));
        if (response == 'S')
        {
            printf("dir presente sul server\n");

            if (read(sd, &numberOfFiles, sizeof(numberOfFiles)) < 0)
            {
                perror("read");
                exit(5);
            }
            numberOfFiles = ntohl(numberOfFiles);
            printf("numberOfFiles: %d\n", numberOfFiles);

            while (read(sd, fileName, sizeof(fileName)) > 0 || numberOfFiles-- > 0)
            {
                printf("fileName: %s\n", fileName);

                fd = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, 0777);
                if (fd < 0)
                {
                    perror("open");
                    continue;
                }

                long netFileLength;
                read(sd, &netFileLength, sizeof(netFileLength));
                fileLength = ntohl(netFileLength);

                while (fileLength >= 0)
                {
                    nread = read(sd, &bufferChar, sizeof(char));
                    if (nread <= 0)
                    {
                        perror("read");
                        break;
                    }

                    if (bufferChar == '\1')
                    {
                        break;
                    }
                    else if (bufferChar == '\0')
                    {
                        printf("\n");
                    }
                    else
                    {
                        write(fd, &bufferChar, sizeof(char));
                    }

                    fileLength -= nread;
                }
                printf("file %s ricevuto\n", fileName);
                if (close(fd) < 0)
                {
                    perror("close");
                }
                printf("file %s chiuso\n", fileName);
                break;
            }
        }
        else
        {
            printf("dir non presente sul server\n");
        }

        printf("inserisci il nome del direttorio, ^D per terminare: ");
    }

    // FINE CICLO INTERAZIONE

    // LIBERO LE RISORSE
    printf("[DEBUG] \nClient: termino...\n");
    shutdown(sd, SHUT_WR);
    shutdown(sd, SHUT_RD);
    close(sd);
    exit(0);
}
