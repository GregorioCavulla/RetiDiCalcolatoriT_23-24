/**
 * server.c
 *  Il server discrimina due servizi con la select:
 *    + valutazione costo del noleggio (UDP)
 *    + download lista degli sci noleggiabili
 **/

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <regex.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_PREN 10
#define ID_LENGTH 5
#define DATA_LENGTH 8
#define MODELLO_LENGTH 20
#define FOTO_LENGTH 30
#define BUFF_LENGTH 255
#define max(a, b)   ((a) > (b) ? (a) : (b))

/********************************************************/
void gestore(int signo)
{
    int stato;
    printf("esecuzione gestore di SIGCHLD\n");
    wait(&stato);
}
/********************************************************/

typedef struct
{
    char id[ID_LENGTH];
    char data[DATA_LENGTH];
    int giorni;
    char modello[MODELLO_LENGTH];
    int costo;
    char foto[FOTO_LENGTH];
} Prenotazione;


int main(int argc, char **argv)
{
    struct sockaddr_in cliaddr, servaddr;
    struct hostent *hostTcp, *hostUdp;
    int port, listen_sd, conn_sd, udp_sd, nread, maxfdp1, len;
    const int on = 1;
    fd_set rset;

    // TCP variabili
    char modello[MODELLO_LENGTH],buff[BUFF_LENGTH];
    int file_count,file_name_length,image_fd,file_length;

    // UDP variabili
    char reqId[ID_LENGTH];
    int ris;

    Prenotazione prenotazioni[MAX_PREN];
    for (int i = 0; i < MAX_PREN; i++)
    {
        Prenotazione p;
        strcpy(p.id,"L");
        strcpy(p.data,"-1/-1/-1");
        p.giorni = -1;
        strcpy(p.modello,"-1");
        p.costo = -1;
        strcpy(p.foto,"L");
        prenotazioni[i] = p;
    }

    strcpy(prenotazioni[0].id,"X123");
    strcpy(prenotazioni[0].data,"12/12/2022");
    prenotazioni[0].giorni=15;
    strcpy(prenotazioni[0].modello,"GoodSnow");
    prenotazioni[0].costo=10;
    strcpy(prenotazioni[0].foto,"GoodSnow.jpg");

    strcpy(prenotazioni[4].id,"P123");
    strcpy(prenotazioni[4].data,"15/12/2022");
    prenotazioni[4].giorni=15;
    strcpy(prenotazioni[4].modello,"GoodSnow");
    prenotazioni[4].costo=10;
    strcpy(prenotazioni[4].foto,"GoodSnow copy.jpg");

    strcpy(prenotazioni[1].id,"A123");
    strcpy(prenotazioni[1].data,"01/12/2022");
    prenotazioni[1].giorni=5;
    strcpy(prenotazioni[1].modello,"BadSnow");
    prenotazioni[1].costo=2;
    strcpy(prenotazioni[1].foto,"BadSnow.jpg");

    strcpy(prenotazioni[2].id,"C123");
    strcpy(prenotazioni[2].data,"20/12/2022");
    prenotazioni[2].giorni=2;
    strcpy(prenotazioni[2].modello,"Medium");
    prenotazioni[2].costo=8;
    strcpy(prenotazioni[2].foto,"Medium.jpg");

    strcpy(prenotazioni[3].id,"E123");
    strcpy(prenotazioni[3].data,"-1/-1/-1");
    prenotazioni[3].giorni=-1;
    strcpy(prenotazioni[3].modello,"Bob");
    prenotazioni[3].costo=7;
    strcpy(prenotazioni[3].foto,"Bob.jpg");

    /*printf("ID|DATA|GIORNI|MODELLO|COSTO|FOTO\n");


    for(int i=0;i<MAX_PREN;i++){
        printf("%s||%s||%d||%s||%d||%s\n",prenotazioni[i].id,prenotazioni[i].data,prenotazioni[i].giorni,prenotazioni[i].modello,prenotazioni[i].costo,prenotazioni[i].foto);
    }

    printf("%s\n",prenotazioni[0].id);
    printf("%s\n",prenotazioni[0].data);
    printf("%d\n",prenotazioni[0].giorni);
    printf("%s\n",prenotazioni[0].modello);
    printf("%d\n",prenotazioni[0].costo);
    printf("%s\n",prenotazioni[0].foto);*/

    /* CONTROLLO ARGOMENTI ---------------------------------- */
    if (argc != 2)
    {
        printf("Error: %s port \n", argv[0]);
        exit(1);
    }
    else
    {
        nread = 0;
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

    /* INIZIALIZZAZIONE INDIRIZZO SERVER ----------------------------------------- */
    memset((char *)&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);

    /* CREAZIONE E SETTAGGI SOCKET TCP --------------------------------------- */
    listen_sd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sd < 0)
    {
        perror("creazione socket ");
        exit(3);
    }
    printf("Server: creata la socket d'ascolto per le richieste di ordinamento, fd=%d\n",
           listen_sd);

    if (setsockopt(listen_sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    {
        perror("set opzioni socket d'ascolto");
        exit(3);
    }
    printf("Server: set opzioni socket d'ascolto ok\n");

    if (bind(listen_sd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("bind socket d'ascolto");
        exit(3);
    }
    printf("Server: bind socket d'ascolto ok\n");

    if (listen(listen_sd, 5) < 0)
    {
        perror("listen");
        exit(3);
    }
    printf("Server: listen ok\n");

    /* CREAZIONE E SETTAGGI SOCKET UDP --------------------------------------- */
    udp_sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_sd < 0)
    {
        perror("apertura socket UDP");
        exit(4);
    }
    printf("Creata la socket UDP, fd=%d\n", udp_sd);

    if (setsockopt(udp_sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    {
        perror("set opzioni socket UDP");
        exit(4);
    }
    printf("Set opzioni socket UDP ok\n");

    if (bind(udp_sd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("bind socket UDP");
        exit(4);
    }
    printf("Bind socket UDP ok\n");

    signal(SIGCHLD, gestore);

    /* PULIZIA E SETTAGGIO MASCHERA DEI FILE DESCRIPTOR ------------------------- */
    FD_ZERO(&rset);
    maxfdp1 = max(listen_sd, udp_sd) + 1;

    /* CICLO DI RICEZIONE RICHIESTE --------------------------------------------- */
    for (;;)
    {
        FD_SET(listen_sd, &rset);
        FD_SET(udp_sd, &rset);

        if ((nread = select(maxfdp1, &rset, NULL, NULL, NULL)) < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            else
            {
                perror("select");
                exit(5);
            }
        }
        /* GESTIONE RICHIESTE UDP  ----------------------------- */
        if (FD_ISSET(udp_sd, &rset))
        {
            printf("Ricevuta richiesta di UDP: valutazione costo del noleggio\n");
            len = sizeof(struct sockaddr_in);

            if (recvfrom(udp_sd, &reqId, sizeof(reqId), 0, (struct sockaddr *)&cliaddr, &len) < 0)
            {
                perror("recvfrom ");
                continue;
            }

            printf("Valutazione costo del noleggio con id %s\n", reqId);

            hostUdp = gethostbyaddr((char *)&cliaddr.sin_addr, sizeof(cliaddr.sin_addr), AF_INET);
            if (hostUdp == NULL)
            {
                printf("client host information not found\n");
            }
            else
            {
                printf("Operazione richiesta da: %s %i\n", hostUdp->h_name, (unsigned)ntohs(cliaddr.sin_port));
            }

            ris = -1;
            for ( int i = 0; i < MAX_PREN; i++ ) {
                if ( strcmp(prenotazioni[i].id,reqId) == 0 ) {
                    if((prenotazioni[i].costo!=-1) && (prenotazioni[i].giorni!=-1)){
                        ris = prenotazioni[i].costo * prenotazioni[i].giorni;
                    }
                    break;
                } 
            }

            // Send result to the client
            if (sendto(udp_sd, &ris, sizeof(int), 0, (struct sockaddr *)&cliaddr, len) < 0)
            {
                perror("sendto ");
                continue;
            }
        }
        /* GESTIONE RICHIESTE TCP  ----------------------------- */
        if (FD_ISSET(listen_sd, &rset))
        {
            printf("Ricevuta richiesta TCP: richiesta di download dei file\n");
            len = sizeof(cliaddr);
            if ((conn_sd = accept(listen_sd, (struct sockaddr *)&cliaddr, &len)) < 0)
            {
                if (errno == EINTR)
                {
                    perror("Forzo la continuazione della accept");
                    continue;
                }
                else
                    exit(6);
            }
            if (fork() == 0)
            {
                close(listen_sd);
                hostTcp = gethostbyaddr((char *)&cliaddr.sin_addr, sizeof(cliaddr.sin_addr), AF_INET);
                if (hostTcp == NULL)
                {
                    printf("client host information not found\n");
                    close(conn_sd);
                    exit(6);
                }
                else
                {
                    printf("Server (figlio): host client e' %s \n", hostTcp->h_name);
                }

                // Leggo il modello
                if ( read(conn_sd, modello, MODELLO_LENGTH) < 0 ) {
                    perror("read");
                    continue;
                }

                printf("Server (figlio): modello richiesto: %s\n", modello);

                file_count=0;
                for(int i=0;i<MAX_PREN;i++){
                    if(strcmp(modello,prenotazioni[i].modello)==0){
                        file_count++;
                    }
                }

                printf("Invio il numero delle foto trovate per il modello %s\n",modello);
                if(write(conn_sd,&file_count,sizeof(file_count))<0){
                    perror("write");
                    continue;
                }

                for(int i=0;i<MAX_PREN;i++){
                    if(strcmp(modello,prenotazioni[i].modello)==0){
                        //invio lunghezza nome file
                        file_name_length = htonl(strlen(prenotazioni[i].foto)+1);
                        if (write(conn_sd, &file_name_length, sizeof(int) ) < 0)
                        {
                            perror("Errore nell'invio del nome file\n");
                            continue;
                        }

                        //invio nome file
                        if (write(conn_sd, prenotazioni[i].foto, (strlen(prenotazioni[i].foto) + 1)) < 0)
                        {
                            perror("Errore nell'invio del nome file\n");
                            continue;
                        }

                        printf("file: %s\n",prenotazioni[i].foto);
                        image_fd = open(prenotazioni[i].foto,O_RDONLY);
                        file_length = lseek(image_fd,0,SEEK_END);
                        lseek(image_fd,0,SEEK_SET);

                        printf("Invio lunghezza file: %i\n",file_length);
                        file_length=htonl(file_length);

                        //invio lunghezza file
                        if (write(conn_sd, &file_length, sizeof(file_length)) < 0)
                        {
                            perror("Errore nell'invio della lunghezza\n");
                            continue;
                        }

                        // invio file
                        while ((nread = read(image_fd, buff, sizeof(buff))) > 0)
                        {
                            printf("%s\nnread: %i\n", buff, nread);
                            write(conn_sd, buff, nread);
                        }
                        close(image_fd);
                    }
                }
                // Libero risorse
                printf("Figlio TCP terminato, libero risorse e chiudo. \n");
                close(conn_sd);
                exit(0);
            }
            close(conn_sd);
        }
    }
}