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
    int ris;
    char dirName[WORD_LENGTH], bufferChar;

    printf("inserisci il nome del direttorio, ^D per terminare");
    while (gets(dirName))
    {
        write(sd, dirName, strlen(dirName));

        read(sd, &bufferChar, sizeof(char));

        if (bufferChar == 'S')
        {
            while ((nread = read(sd, &bufferChar, sizeof(char))) > 0)
            {
                if (bufferChar == '\1')
                {
                    break;
                }
                else if (bufferChar = '\0')
                {
                    printf("\n");
                }
                else
                {
                    write(1, &bufferChar, sizeof(char));
                }
            }
        }
        else
        {
            printf("dir non presente sul server");
        }

        printf("inserisci il nome del direttorio, ^D per terminare");
    }

    // FINE CICLO INTERAZIONE

    // LIBERO LE RISORSE
    printf("[DEBUG] \nClient: termino...\n");
    shutdown(sd, SHUT_WR);
    shutdown(sd, SHUT_RD);
    close(sd);
    exit(0);
}
