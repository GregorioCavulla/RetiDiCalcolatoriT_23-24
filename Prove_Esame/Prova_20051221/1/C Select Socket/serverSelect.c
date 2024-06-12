#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/param.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define QUEUE_TCP 10240
#define LINE_LENGTH 256
#define WORD_LENGTH 64
#define MAX_FILES 255

#define max(a, b) ((a) > (b) ? (a) : (b))

void gestore(int signo)
{
    int stato;
    printf("esecuzione gestore di SIGCHLD \n");
    wait(&stato);
}

// typedef struct
// {
//     char dato1[WORD_LENGTH];
//     char dato2[WORD_LENGTH];
// } reqUDP;

int main(int argc, char **argv)
{
    /* inizializzazione dati per la comunicazione */

    int socket_udp, socket_tcp, socket_conn, port, nfds, nread, serv_len;
    struct hostent *hostUDP, *hostTCP;
    struct sockaddr_in cliAddr, servAddr;
    const int on = 1;
    fd_set rset;

    // UDP
    char udpFileName[WORD_LENGTH];   // ...
    int rispostaUdp = 0, udpFile_fd; // ...
    long udpFileLength;              // ...
    // TCP
    DIR *dir;
    struct dirent *file;
    char dirName[WORD_LENGTH];       // ...
    int rispostaTcp = 0, tcpFile_fd; // ...
    long tcpFileLength;              // ...

    /* CONTROLLO ARGOMENTI ---------------------------------- */
    if (argc != 2)
    {
        printf("Error: %s port \n", argv[0]);
        exit(1);
    }
    else
    {
        nread = 0;
        // controllo che il secondo argomento sia un intero
        while (argv[1][nread] != '\0')
        {
            if ((argv[1][nread] < '0') || (argv[1][nread] > '9'))
            {
                printf("Secondo argomento non intero\n");
                exit(2);
            }
            nread++;
        }
        port = atoi(argv[1]);
        if (port < 1024 || port > 65535)
        {
            printf("Porta scorretta...");
            exit(2);
        }
    }

    /* inizializzazione indirizzo server */
    memset((char *)&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = INADDR_ANY;
    servAddr.sin_port = htons(port);

    // creazione socket d ascolto TCP
    socket_tcp = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_tcp < 0)
    {
        printf("errore creazione socket TCP");
        exit(3);
    }
    printf("[DEBUG] creata socket di ascolto TCP, fd=%d \n", socket_tcp);
    socket_tcp = socket(AF_INET, SOCK_STREAM, 0);
    if (setsockopt(socket_tcp, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    {
        printf("errore di opzione socket di ascolto TCP");
        exit(3);
    }
    printf("[DEBUG] set opzioni socket di ascolto OK \n");
    if (bind(socket_tcp, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
        printf("errore di bind socket di ascolto TCP");
        exit(3);
    }
    printf("[DEBUG] bind socket di ascolto TCP OK \n");
    if (listen(socket_tcp, QUEUE_TCP) < 0)
    {
        printf("errore di listen socket di ascolto TCP");
        exit(3);
    }
    printf("[DEBUG] listen socket di ascolto TCP OK \n");

    // creazione e settaggi UDP
    socket_udp = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_udp < 0)
    {
        printf("errore creazione socket UDP");
        exit(3);
    }
    printf("[DEBUG] creata socket di ascolto UDP, fd=%d \n", socket_udp);
    if (setsockopt(socket_udp, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    {
        printf("errore di opzione socket di ascolto UDP");
        exit(3);
    }
    printf("[DEBUG] set opzioni socket di ascolto OK \n");
    if (bind(socket_udp, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
        printf("errore di bind socket di ascolto UDP");
        exit(3);
    }
    printf("[DEBUG] bind socket di ascolto UDP OK \n");

    // gestore signal
    signal(SIGCHLD, gestore);

    // pulizia e settaggio maschera dei file descriptor
    FD_ZERO(&rset);
    nfds = max(socket_tcp, socket_udp) + 1;

    // ciclo di ricezione richieste
    printf("[DEBUG] Server: mi metto in attesa\n");
    for (;;)
    {
        FD_SET(socket_udp, &rset); // setto il bit del file descriptor
        FD_SET(socket_tcp, &rset); // setto il bit del file descriptor
        if ((nread = select(nfds, &rset, NULL, NULL, NULL)) < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            else
            {
                printf("select");
                exit(5);
            }
        }

        /**
         * Gestione richieste UDP
         */

        if (FD_ISSET(socket_udp, &rset))
        {
            serv_len = sizeof(struct sockaddr_in);
            printf("[DEBUG] ricevuta una richiesta di UDP \n");

            /* DA MODIFICARE COSA VIENE RICEVUTO */
            if (recvfrom(socket_udp, & /*questo*/, sizeof(/*questo*/), 0, (struct sockaddr *)&cliAddr, &serv_len))
            {
                printf("recvfrom");
                continue;
            }

            printf("[DEBUG] operazione datagram\n");

            hostUDP = gethostbyaddr((char *)&cliAddr.sin_addr, sizeof(cliAddr.sin_addr), AF_INET);
            if (hostUDP == NULL)
            {
                printf("no info client host \n");
            }
            else
            {
                printf("Operazione richiesta da: %s %i\n", hostUDP->h_name, (unsigned)ntohs(cliAddr.sin_port));
            }

            /**
             * CODICE DEL SERVER
             */

            rispostaUdp = htonl(rispostaUdp);

            if (sendto(socket_udp, rispostaUdp, sizeof(rispostaUdp), 0, (struct sockaddr *)&cliAddr, serv_len))
            {
                printf("sendto");
                continue;
            }
            printf("[DEBUG] ho mandato rispostaUdp (%d) al client\n ", rispostaUdp);
        }

        /**
         *Gestione richieste TCP
         */

        if (FD_ISSET(socket_tcp, &rset))
        {
            serv_len = sizeof(struct sockaddr_in);
            printf("[DEBUG] ho ricevuto una richiesta di TCP\n");

            // accetto la connessione
            if ((socket_conn = accept(socket_tcp, (struct sockaddr *)&cliAddr, &serv_len)) < 0)
            {
                if (errno == EINTR)
                {
                    perror("Forzo la continuazione della accept");
                    continue;
                }
                else
                    exit(6);
            }
            hostTCP = gethostbyaddr((char *)&cliAddr.sin_addr, sizeof(cliAddr.sin_addr), AF_INET);

            printf("[DEBUG] operazione stream\n");

            if (hostTCP == NULL)
            {
                printf("no info client host\n");
                close(socket_conn);
                exit(6);
            }
            else
            {
                printf("[DEBUG ]Server (figlio): host client e' %s \n", hostTCP->h_name);
            }

            /**
             * Il server TCP crea un processo figlio per gestire la richiesta del client
             */

            if (fork() == 0)
            {
                close(socket_tcp);
                printf("[DEBUG]Server (figlio): eseguo pid=%i\n", getpid());

                /**
                 * CODICE DEL SERVER
                 */

                /* DA MODIFICARE COSA VIENE RICEVUTO */
                while ((nread = read(socket_conn, & /*questo*/, sizeof(/*questo*/))) > 0)
                {
                }

                // restituisco
                write(socket_conn, &rispostaTcp, sizeof(rispostaTcp));
                printf("[DEBUG] ho mandato rispostaTcp (%d) al client\n ", rispostaTcp);

                // chiusura di socket connessione
                shutdown(socket_conn, SHUT_RD);
                shutdown(socket_conn, SHUT_WR);
                printf("[DEBUG] Server TCP (figlio): chiudo, pid=%i\n", getpid());
                close(socket_conn);
                exit(1);
            } // figlio
            // padre chiude il socket di connessione
            close(socket_conn);
        }
    }
}