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
    char udpFileName[LINE_LENGTH];
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
            if (recvfrom(socket_udp, &udpFileName, sizeof(udpFileName), 0, (struct sockaddr *)&cliAddr, &serv_len) < 0)
            {
                perror("recvfrom");
                continue;
            }

            printf("[DEBUG] operazione datagram "); //

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

            file_fd = open(udpFileName, O_RDONLY);
            strcpy(tempFile, udpFileName);
            strcat(tempFile, "_temp.txt");

            if ((tempFile_fd = open(tempFile, O_WRONLY | O_CREAT, 0777)) < 0)
            {
                perror("Errore apertura file temp");
                esito = -1;
            }

            printf("[DEBUG] file aperto %s\n", tempFile);

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

            while ((nread = read(file_fd, &charRead, sizeof(char))) != 0)
            {
                if (nread < 0)
                {
                    perror("read");
                    esito = -1;
                    close(file_fd);
                    close(tempFile_fd);
                    if (sendto(socket_udp, &esito, sizeof(int), 0, (const struct sockaddr *)&cliAddr, serv_len) < 0)
                    {
                        perror("sendto");
                        continue;
                    }
                    continue;
                }

                printf("\n sono qui \n");

                if (charRead != 'A' && charRead != 'E' && charRead != 'I' && charRead != 'O' && charRead != 'U' && charRead != 'a' && charRead != 'e' && charRead != 'i' && charRead != 'o' && charRead != 'u')
                {
                    if (write(tempFile_fd, &charRead, sizeof(char)) < 0)
                    {
                        perror("write");
                        esito = -1;
                        close(file_fd);
                        close(tempFile_fd);
                        if (sendto(socket_udp, &esito, sizeof(int), 0, (const struct sockaddr *)&cliAddr, serv_len) < 0)
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
            }

            close(file_fd);
            close(tempFile_fd);

            rename(tempFile, udpFileName);

            // eisto=htonl(num);

            // mando in dietro l esito
            if (sendto(socket_udp, &esito, sizeof(esito), 0, (struct sockaddr *)&cliAddr, serv_len))
            {
                perror("sendto");
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
            // Inside the child process handling the TCP request
            if (fork() == 0)
            {
                close(socket_tcp);
                printf("[DEBUG]Server (figlio): eseguo pid=%i\n", getpid());
                /* server code */
                char dirName[WORD_LENGTH], fileName[WORD_LENGTH], path[WORD_LENGTH * 2], buffer[LINE_LENGTH];
                DIR *dir;
                struct dirent *file;
                int nread, fd, numberOfFiles;
                long fileLength;

                while ((nread = read(socket_conn, dirName, sizeof(dirName))) > 0)
                {
                    printf("[DEBUG] Server TCP (figlio): ricevuto %s\n", dirName);
                    numberOfFiles = 0;

                    if ((dir = opendir(dirName)) != NULL)
                    {
                        printf("[DEBUG] Server TCP (figlio): dir aperto %s\n", dirName);
                        char response = 'S';
                        write(socket_conn, &response, sizeof(char));

                        while ((file = readdir(dir)) != NULL)
                        {
                            if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0)
                            {
                                continue;
                            }

                            numberOfFiles++;
                        }

                        rewinddir(dir);

                        printf("[DEBUG] Server TCP (figlio): numero di file %d\n", numberOfFiles);

                        numberOfFiles = htonl(numberOfFiles);

                        if (write(socket_conn, &numberOfFiles, sizeof(int)) < 0)
                        {
                            perror("write");
                            exit(1);
                        }

                        while ((file = readdir(dir)) != NULL)
                        {
                            if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0)
                            {
                                continue;
                            }

                            strcpy(fileName, file->d_name);
                            write(socket_conn, fileName, sizeof(fileName));
                            sprintf(path, "%s/%s", dirName, fileName);

                            fd = open(path, O_RDONLY);
                            if (fd < 0)
                            {
                                perror("open");
                                continue;
                            }

                            fileLength = lseek(fd, 0, SEEK_END);
                            lseek(fd, 0, SEEK_SET);
                            long netFileLength = htonl(fileLength);
                            write(socket_conn, &netFileLength, sizeof(netFileLength));

                            while ((nread = read(fd, buffer, sizeof(buffer))) > 0)
                            {
                                if (write(socket_conn, buffer, nread) != nread)
                                {
                                    perror("write");
                                    break;
                                }
                            }

                            // aggiungo un carattere di terminazione
                            char terminator = '\1';
                            write(socket_conn, &terminator, sizeof(char));

                            close(fd);
                        }
                        closedir(dir);
                    }
                    else
                    {
                        char response = 'N';
                        write(socket_conn, &response, sizeof(char));
                    }
                }
                close(socket_conn);
                exit(0);
            }
            close(socket_conn);
        }
    }
}
