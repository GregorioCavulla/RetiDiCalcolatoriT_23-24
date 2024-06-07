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
#define N 10 // numero massimo di noleggi

#define max(a, b) ((a) > (b) ? (a) : (b))

void gestore(int signo)
{
    int stato;
    printf("esecuzione gestore di SIGCHLD \n");
    wait(&stato);
}

static int inizializzato = 0;

typedef struct /*da modificare in base alle necessita*/ // struttura per la ricezione di una richiesta UDP
{
    char targa[8];
    char patente[6];
    char tipoVeicolo[7];
    char img_folder[14];

} Prenotazione;

static Prenotazione table[N]; // tabella dei noleggi

void inizializza()
{
    if (inizializzato == 1)
    {
        return;
    }

    inizializzato = 1;
    for (int i = 0; i < N; i++)
    {
        strcpy(table[i].targa, "L");
        strcpy(table[i].patente, "0");
        strcpy(table[i].tipoVeicolo, "L");
        strcpy(table[i].img_folder, "L");
    }

    strcpy(table[0].targa, "AN123NL");
    strcpy(table[0].patente, "00001");
    strcpy(table[0].tipoVeicolo, "auto");
    strcpy(table[0].img_folder, "AN123NL_img/");

    strcpy(table[1].targa, "ED123DE");
    strcpy(table[1].patente, "00002");
    strcpy(table[1].tipoVeicolo, "camper");
    strcpy(table[1].img_folder, "ED123DE_img/");

    strcpy(table[2].targa, "EN321NE");
    strcpy(table[2].patente, "00003");
    strcpy(table[2].tipoVeicolo, "auto");
    strcpy(table[2].img_folder, "EN321NE");

    printf("inizializzato\n");

    for (int i = 0; i < N; i++)
    {
        printf("| %s | %s | %s | %s |\n", table[i].targa, table[i].patente, table[i].tipoVeicolo, table[i].img_folder);
    }
}

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

    inizializza(); // inizializzazione dati

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

        // if (FD_ISSET(socket_udp, &rset))
        // {
        //     serv_len = sizeof(struct sockaddr_in);
        //     printf("[DEBUG] ricevuta una richiesta di UDP \n");

        //     /* DA MODIFICARE COSA VIENE RICEVUTO */
        //     if (recvfrom(socket_udp, & /*questo*/, sizeof(/*questo*/), 0, (struct sockaddr *)&cliAddr, &serv_len))
        //     {
        //         printf("recvfrom");
        //         continue;
        //     }

        //     printf("[DEBUG] operazione datagram\n");

        //     hostUDP = gethostbyaddr((char *)&cliAddr.sin_addr, sizeof(cliAddr.sin_addr), AF_INET);
        //     if (hostUDP == NULL)
        //     {
        //         printf("no info client host \n");
        //     }
        //     else
        //     {
        //         printf("Operazione richiesta da: %s %i\n", hostUDP->h_name, (unsigned)ntohs(cliAddr.sin_port));
        //     }

        //     /**
        //      * CODICE DEL SERVER
        //      */

        //     rispostaUdp = htonl(rispostaUdp);

        //     if (sendto(socket_udp, rispostaUdp, sizeof(rispostaUdp), 0, (struct sockaddr *)&cliAddr, serv_len))
        //     {
        //         printf("sendto");
        //         continue;
        //     }
        //     printf("[DEBUG] ho mandato rispostaUdp (%d) al client\n ", rispostaUdp);
        // }

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

                char targaRichiesta[8] = {0};
                int fd, fileLength, numberOfFiles;
                char dirName[WORD_LENGTH], fileName[WORD_LENGTH], path[WORD_LENGTH * 2], buffer[LINE_LENGTH];
                DIR *dir;
                struct dirent *file;

                /* DA MODIFICARE COSA VIENE RICEVUTO */
                if ((read(socket_conn, &targaRichiesta, sizeof(targaRichiesta))) < 0)
                {
                    perror("read");
                    rispostaTcp = -1;
                    rispostaTcp = htonl(rispostaTcp);

                    if (write(socket_conn, &rispostaTcp, sizeof(rispostaTcp)) < 0)
                    {
                        perror("write");
                        continue;
                    }
                }

                rispostaTcp = 0;

                for (int i = 0; i < N; i++)
                {
                    if (strcmp(targaRichiesta, table[i].targa) == 0)
                    {
                        rispostaTcp++;
                    }
                }

                printf("[DEBUG] Server TCP (figlio): trovate %d prenotazioni per la targa %s\n", rispostaTcp, targaRichiesta);
                rispostaTcp = htonl(rispostaTcp);

                if (write(socket_conn, &rispostaTcp, sizeof(rispostaTcp)) < 0)
                {
                    perror("write");
                    continue;
                }

                for (int i = 0; i < N; i++)
                {
                    if (strcmp(targaRichiesta, table[i].targa) == 0)
                    {
                        sprintf(path, "%s_img", table[i].targa);
                    }

                    dir = opendir(path);
                    if (dir)
                    {
                        numberOfFiles = 0;
                        while ((file = readdir(dir)) != NULL)
                        {
                            if (strcmp(file->d_name, ".") != 0 || strcmp(file->d_name, "..") != 0)
                            {
                                numberOfFiles++;
                            }
                        }
                        rewinddir(dir);

                        numberOfFiles = htonl(numberOfFiles);
                        if (write(socket_conn, &numberOfFiles, sizeof(int)) < 0)
                        {
                            perror("write");
                            continue;
                        }
                        printf("[DEBUG] Server TCP (figlio): inviato numero di file %d\n", numberOfFiles);

                        while ((file = readdir(dir)) != NULL)
                        {
                            if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0)
                            {
                                continue;
                            }

                            strcpy(fileName, file->d_name);
                            write(socket_conn, fileName, sizeof(fileName));

                            printf("[DEBUG] Server TCP (figlio): inviato nome file %s\n", fileName);

                            sprintf(path, "%s/%s", table[i].targa, fileName);

                            fd = open(path, O_RDONLY, 0777);
                            if (fd < 0)
                            {
                                perror("open");
                                continue;
                            }

                            fileLength = lseek(fd, 0, SEEK_END);
                            lseek(fd, 0, SEEK_SET);
                            long netFileLength = htonl(fileLength);
                            write(socket_conn, &netFileLength, sizeof(netFileLength));
                            printf("[DEBUG] Server TCP (figlio): inviata lunghezza file %ld\n", fileLength);

                            while ((nread = read(fd, buffer, sizeof(buffer))) > 0)
                            {
                                if (write(socket_conn, buffer, nread) != nread)
                                {
                                    perror("write");
                                    break;
                                }
                            }
                            close(fd);
                        }
                        closedir(dir);
                    }
                    else
                    {
                        char response = 'N';
                        write(socket_conn, &response, sizeof(response));
                    }

                    // // da qui in avanti è tutta merda da sistemare e da riscrivere, non funziona un cazzo, non so cosa cazzo stia facendo

                    // for (int i = 0; i < N; i++)
                    // {
                    //     char path[LINE_LENGTH];
                    //     strcpy(path, table[i].targa);
                    //     strcat(path, "_img/");

                    //     if ((dir = opendir(path)) != NULL)
                    //     {
                    //         printf("[DEBUG] Server TCP (figlio): dir aperto %s\n", path);

                    //         while ((file = readdir(dir)) != NULL)
                    //         {
                    //             if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0)
                    //             {
                    //                 continue;
                    //             }

                    //             numberOfFiles++;
                    //         }
                    //         rewinddir(dir);

                    //         printf("[DEBUG] Server TCP (figlio): numero di file %d\n", numberOfFiles);

                    //         numberOfFiles = htonl(numberOfFiles); // converto il numero di file in network byte order

                    //         if (write(socket_conn, &numberOfFiles, sizeof(int)) < 0) // mando il numero di file
                    //         {
                    //             perror("write");
                    //             exit(1);
                    //         }

                    //         // invio i file
                    //         while ((file = readdir(dir)) != NULL)
                    //         {
                    //             if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0) // ignoro le directory . e ..
                    //             {
                    //                 continue;
                    //             }

                    //             strcpy(fileName, file->d_name);
                    //             write(socket_conn, fileName, sizeof(fileName)); // mando il nome del file
                    //             sprintf(path, "%s/%s", dirName, fileName);      // creo il path del file

                    //             fd = open(path, O_RDONLY, 0777);
                    //             if (fd < 0)
                    //             {
                    //                 perror("open");
                    //                 continue;
                    //             }

                    //             fileLength = lseek(fd, 0, SEEK_END); // calcolo la lunghezza del file
                    //             lseek(fd, 0, SEEK_SET);
                    //             long netFileLength = htonl(fileLength);                    // converto la lunghezza del file in network byte order
                    //             write(socket_conn, &netFileLength, sizeof(netFileLength)); // mando la lunghezza del file

                    //             while ((nread = read(fd, buffer, sizeof(buffer))) > 0) // leggo il file
                    //             {
                    //                 if (write(socket_conn, buffer, nread) != nread) // mando il file al client
                    //                 {
                    //                     perror("write");
                    //                     break;
                    //                 }
                    //             }

                    //             // aggiungo un carattere di terminazione
                    //             char terminator = '\1';
                    //             write(socket_conn, &terminator, sizeof(char)); // mando il carattere di terminazione

                    //             close(fd);
                    //         }
                    //         closedir(dir);
                    //     }
                    //     else
                    //     {
                    //         char response = 'N';
                    //         write(socket_conn, &response, sizeof(char));
                    //     }
                }
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