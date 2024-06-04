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

#define WORD_LENGTH 64  // lunghezza massima di una parola
#define MAX_INPUT 128   // lunghezza massima di un input
#define LINE_LENGTH 256 // lunghezza massima di una linea
#define QUEUE_TCP 10240 // lunghezza massima della coda di connessioni pendenti

#define max(a, b) ((a) > (b) ? (a) : (b)) // massimo tra due numeri

void gestore(int signo)
{
    int stato;
    printf("esecuzione gestore di SIGCHLD \n");
    wait(&stato);
}

typedef struct /*da modificare in base alle necessita*/ // struttura per la ricezione di una richiesta UDP
{
    char dato1[WORD_LENGTH];
    char dato2[WORD_LENGTH];
} ReqUDP;

int main(int argc, char **argv)
{
    int socket_udp, socket_tcp, socket_conn, port, nfds, nread, serv_len; // variabili per la gestione dei socket, della porta, del numero di file descriptor, del numero di caratteri letti e della lunghezza dell'indirizzo
    struct hostent *hostUDP, *hostTCP;                                    // struttura per la gestione degli host, contiene informazioni come l'indirizzo IP dell'host e il suo nome
    struct sockaddr_in cliAddr, servAddr;                                 // struttura per la gestione degli indirizzi, contiene informazioni come l'indirizzo IP e la porta, sia del client che del server
    char buf[LINE_LENGTH]; /*da modificare in base alle necessita*/       // buffer per la ricezione e l'invio di dati
    ReqUDP req; /*da modificare in base alle necessita */                 // struttura per la ricezione di una richiesta UDP
    const int on = 1;                                                     // variabile per la gestione delle opzioni del socket
    fd_set rset;                                                          // maschera per la gestione dei file descriptor

    /*inizializzare i dati necessari ai servizio*/

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

    /*
     * creazione socket TCP di ascolto
     *
     *  // q: perchè lo crei due volte?
     *  // a: perchè il primo è sbagliato, non è stato controllato il valore di ritorno
     *  // q: ma non si può includere anche il secondo e il terzo controllo nel primo if?
     *  // a: si, ma è più chiaro così
     *  // q: ok perfetto, grazie
     *  // a: prego
     *
     */

    socket_tcp = socket(AF_INET, SOCK_STREAM, 0); // creazione socket
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

    /*
     * creazione socket UDP di ascolto
     */
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

    // il server si mette in attesa di richieste
    printf("[DEBUG] Server: mi metto in attesa\n");

    /*
     * ciclo di ricezione richieste
     */
    for (;;)
    {
        FD_SET(socket_udp, &rset);
        FD_SET(socket_tcp, &rset);
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

        /*
         *   GESTIONE RICHIESTE UDP
         */
        if (FD_ISSET(socket_udp, &rset))
        {
            printf("[DEBUG] ricevuta una richiesta di UDP \n");
            serv_len = sizeof(struct sockaddr_in);
            /*da modificare in base alle necessita */
            if (recvfrom(socket_udp, &req, sizeof(req), 0, (struct sockaddr *)&cliAddr, &serv_len))
            {
                printf("recvfrom");
                continue;
            }

            printf("[DEBUG] operazione "); //

            hostUDP = gethostbyaddr((char *)&cliAddr.sin_addr, sizeof(cliAddr.sin_addr), AF_INET);
            if (hostUDP == NULL)
            {
                printf("no info client host \n");
            }
            else
            {
                printf("Operazione richiesta da: %s %i\n", hostUDP->h_name, (unsigned)ntohs(cliAddr.sin_port));
            }

            /* CODICE DEL SERVER PER RICHIESTE UDP*/
            int esito = 0;

            // eisto=htonl(num); // conversione in network byte order se necessario.
            /*
             *   // Q: Quando è necessario?
             *   // A: Quando si inviano dati a un client che non è sulla stessa macchina
             *   //    e che potrebbe avere una diversa architettura hardware
             *   //    (es. big endian vs little endian)
             *
             *   // Q: quando invece client e server sono sulla stessa macchina?
             *   // A: in quel caso non è necessario convertire i dati in network byte order
             */

            // mando in dietro il esito
            if (sendto(socket_udp, esito, sizeof(esito), 0, (struct sockaddr *)&cliAddr, serv_len))
            {
                printf("sendto");
                continue;
            }
            printf("[DEBUG] ho mandato l'esito al client\n ");
        }

        /*
         *  GESTIONE RICHIESTE TCP
         */
        if (FD_ISSET(socket_tcp, &rset))
        {
            serv_len = sizeof(struct sockaddr_in);
            printf("[DEBUG] ho ricevuto una richiesta di TCP\n");
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

            if (hostTCP == NULL)
            {
                printf("no info client host\n");
                close(socket_conn);
                exit(6);
            }
            else
                printf("[DEBUG ]Server (figlio): host client e' %s \n", hostTCP->h_name);
            // processo figlio che gestisce la richiesta
            if (fork() == 0)
            {
                close(socket_tcp);
                printf("[DEBUG]Server (figlio): eseguo pid=%i\n", getpid());
                /* server code */

                // dichiarazione variabili

                int esito;
                int dato;

                while ((nread = read(socket_conn, &dato, sizeof(dato))) > 0) // finchè ci sono dati da leggere
                {
                    // implementazione del server TCP
                }
                write(socket_conn, &esito, sizeof(esito));
                // write(socket_conn, &zero, 1); invio di un zero binario
                /*
                 *  // q: perchè invii un zero binario?
                 *  // a: per segnalare al client che la trasmissione è terminata
                 *  // q: perchè in questo codice è commentato?
                 *  // a: perchè non è necessario, il client sa che la trasmissione è terminata quando il server chiude la connessione
                 *  // q: ok, grazie
                 *  // a: prego
                 */

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