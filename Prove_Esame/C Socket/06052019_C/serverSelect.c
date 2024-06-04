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
// } ReqUDP;

int main(int argc, char **argv)
{
    int socket_udp, socket_tcp, socket_conn, port, nfds, nread, serv_len;
    struct hostent *hostUDP, *hostTCP;
    struct sockaddr_in cliAddr, servAddr;
    char buf[LINE_LENGTH];
    char udpReq[LINE_LENGTH];
    const int on = 1;
    fd_set rset;
    // TODO init data

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

        // gestione richieste UDP
        if (FD_ISSET(socket_udp, &rset))
        {
            printf("[DEBUG] ricevuta una richiesta di UDP \n");
            serv_len = sizeof(struct sockaddr_in);
            // da modificare
            if (recvfrom(socket_udp, &udpReq, sizeof(udpReq), 0, (struct sockaddr *)&cliAddr, &serv_len))
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
            /* CODICE DEL SERVER*/

            int esito = 0;

            int file_fd, tempFile_fd;
            char tempFile[WORD_LENGTH], charRead;

            file_fd = open(udpReq, O_RDONLY);
            strcpy(tempFile, udpReq);
            strcat(tempFile, "_temp");
            if ((tempFile_fd = open(tempFile, O_WRONLY | O_CREAT, 0777)) < 0)
            {
                perror("Errore apertura file temp");
                esito = -1;
            }

            if (file_fd < 0)
            {
                perror("open");
                esito = -1;
                if (sendto(socket_udp, &esito, sizeof(int), 0, (struct sockaddr *)&cliAddr, serv_len) < 0)
                {
                    perror("sendto");
                    continue;
                }
                continue;
            }

            esito = 0;

            while ((nread = read(file_fd, &charRead, sizeof(char)) < 0))
            {
                if (nread < 0)
                {
                    perror("read");
                    esito = -1;
                    close(file_fd);
                    close(tempFile_fd);
                    if (sendto(socket_udp, &esito, sizeof(int), 0, (struct socketaddr *)&cliAddr, serv_len) < 0)
                    {
                        perror("sendto");
                        continue;
                    }
                    continue;
                }

                if (charRead != 'A' && charRead != 'E' && charRead != 'I' && charRead != 'O' && charRead != 'U' && charRead != 'a' && charRead != 'e' && charRead != 'i' && charRead != 'o' && charRead != 'u')
                {
                    if (write(tempFile_fd, &charRead, sizeof(char)) < 0)
                    {
                        perror("write");
                        esito = -1;
                        close(file_fd);
                        close(tempFile_fd);
                        if (sendto(socket_udp, &esito, sizeof(int), 0, (struct socketaddr *)&cliAddr, serv_len) < 0)
                        {
                            perror("sendto");
                            continue;
                        }
                        continue;
                    }
                }
                else
                {
                    esito++;
                }

                close(file_fd);
                close(tempFile_fd);

                rename(tempFile, udpReq);
            }

            // eisto=htonl(num);
            // mando in dietro il esito
            if (sendto(socket_udp, esito, sizeof(esito), 0, (struct sockaddr *)&cliAddr, serv_len))
            {
                printf("sendto");
                continue;
            }
            printf("[DEBUG] ho mandato l'esito al client\n ");
        }

        // Gestione richieste TCP
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
                int esito;
                int dato;

                char dirName[WORD_LENGTH], files[MAX_FILES][WORD_LENGTH], fileName[WORD_LENGTH], path[WORD_LENGTH * 2], buff[WORD_LENGTH];
                char risp, c;
                DIR *dir;
                struct dirent *file;
                int dir_name_length, file_count, consonanti, vocali, ind, file_name_length, i, fd, file_length;

                while ((nread = read(socket_conn, &dirName, sizeof(dir))) > 0)
                {
                    if ((dir = opendir(dirName)) != NULL)
                    {
                        risp = "S";
                        write(socket_tcp, &risp, sizeof(char));
                        file_count = 0;
                        i = 0;
                        while ((file = readdir(dir)) != NULL)
                        {
                            strcat(file->d_name, "\0");
                            printf("%s\n", file->d_name);
                            ind = 0;
                            vocali = 0;
                            consonanti = 0;

                            while ((c = file->d_name[ind]) != '\0')
                            {
                                if (c != 'A' && c != 'a' && c != 'E' && c != 'e' && c != 'I' && c != 'i' && c != 'O' && c != 'o' && c != 'U' && c != 'u')
                                {
                                    consonanti++;
                                }
                                else
                                {
                                    vocali++;
                                }
                                ind++;

                                if (consonanti > 0 && vocali > 0)
                                {
                                    strcpy(files[i++], file->d_name);
                                    file_count++;
                                    break;
                                }
                            }

                            // Scrivo path
                            strcpy(path, dirName);
                            strcat(path, "/");
                            strcat(path, fileName);

                            fd = open(path, O_RDONLY);

                            if (fd < 0)
                            {
                                perror("open");
                                continue;
                            }

                            // invio il file

                            while ((nread = read(fd, buff, sizeof(buff))) > 0)
                            {
                                if (nread < 0)
                                {
                                    perror("read");
                                    close(fd);
                                    close(socket_tcp);
                                    exit(6);
                                }
                                if (write(socket_tcp, buff, nread) < 0)
                                {
                                    perror("write");
                                    close(fd);
                                    close(socket_tcp);
                                    exit(6);
                                }
                            }

                            close(fd);
                        }
                    }
                    else
                    {
                        risp = 'N';
                        write(socket_tcp, &risp, sizeof(char));
                    }
                }
                write(socket_conn, &esito, sizeof(esito));
                // write(socket_conn, &zero, 1); invio di un zero binario

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