/* lw_client.c
 * Il cliente invia il nome di un file remoto al server e
 * riceve la lunghezza della parola più lunga del file.
 */

#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define LINE_LENGTH 256

int main(int argc, char **argv)
{
    struct hostent *host;                    // definizione della struttura hostent per la risoluzione dei nomi
    struct sockaddr_in clientaddr, servaddr; // definizione delle strutture per gli indirizzi
    int port, sd, nread, len = 0, wordCount; // definizione delle variabili per la porta, il socket, la lunghezza, il conteggio delle parole
    char nomeFile[LINE_LENGTH];              // definizione della stringa per il controllo dell'input
    char c;                                  // definizione del carattere per il controllo dell'operazione

    /* CONTROLLO ARGOMENTI ---------------------------------- */
    if (argc != 3)
    {
        printf("Error:%s serverAddress serverPort\n", argv[0]);
        exit(1);
    }

    /* INIZIALIZZAZIONE INDIRIZZO CLIENT E SERVER --------------------- */
    memset((char *)&clientaddr, 0, sizeof(struct sockaddr_in)); // azzera la struttura dati clientaddr di tipo sockaddr_in di dimensione sizeof(struct sockaddr_in)
    clientaddr.sin_family = AF_INET;                            // assegna alla famiglia di indirizzi di clientaddr il valore AF_INET (IPv4)
    clientaddr.sin_addr.s_addr = INADDR_ANY;                    // assegna all'indirizzo IP di clientaddr il valore INADDR_ANY

    /* Passando 0 ci leghiamo ad un qualsiasi indirizzo libero,
     * ma cio' non funziona in tutti i sistemi.
     * Se nel nostro sistema cio' non funziona come si puo' fare?
     */

    // q: Passando 0 ci leghiamo ad un qualsiasi indirizzo libero, ma cio' non funziona in tutti i sistemi. Se nel nostro sistema cio' non funziona come si puo' fare?
    // a: Si può fare in modo che il client si leghi ad un indirizzo specifico, ad esempio clientaddr.sin_addr.s_addr = inet_addr("..."); dove al posto dei puntini si inserisce l'indirizzo IP del client.
    clientaddr.sin_port = 0;

    memset((char *)&servaddr, 0, sizeof(struct sockaddr_in)); // azzera la struttura dati servaddr di tipo sockaddr_in di dimensione sizeof(struct sockaddr_in)
    servaddr.sin_family = AF_INET;                            // assegna alla famiglia di indirizzi di servaddr il valore AF_INET (IPv4)
    host = gethostbyname(argv[1]);                            // restituisce un puntatore alla struttura hostent contenente informazioni sull'host specificato

    // CONTROLLO ARGOMENTI  ---------------------------------- */

    /* VERIFICA INTERO */
    nread = 0; // inizializza il contatore

    // Il ciclo while verifica che il secondo argomento sia un intero, se non lo è stampa un messaggio di errore e termina il programma.
    while (argv[2][nread] != '\0') // finchè non si raggiunge la fine della stringa
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

    /* VERIFICA PORT e HOST */
    if (port < 1024 || port > 65535)
    {
        printf("%s = porta scorretta...\n", argv[2]);
        exit(2);
    }
    if (host == NULL)
    {
        printf("%s not found in /etc/hosts\n", argv[1]);
        exit(2);
    }
    else
    {
        servaddr.sin_addr.s_addr = ((struct in_addr *)(host->h_addr))->s_addr;
        servaddr.sin_port = htons(port);
    }

    // FINITO CONTROLLO ARGOMENTI ---------------------------------- */

    /* CREAZIONE SOCKET ---------------------------------- */
    sd = socket(AF_INET, SOCK_DGRAM, 0); // crea un socket di tipo AF_INET (IPv4) e di tipo SOCK_DGRAM (datagram)
    if (sd < 0)
    {
        perror("apertura socket");
        exit(1);
    }
    printf("Client: creata la socket sd=%d\n", sd);

    /* BIND SOCKET, a una porta scelta dal sistema --------------- */
    if (bind(sd, (struct sockaddr_in *)&clientaddr, sizeof(clientaddr)) < 0)
    {
        perror("bind socket ");
        exit(1);
    }
    printf("Client: bind socket ok, alla porta %i\n", clientaddr.sin_port);

    /* CORPO DEL CLIENT: ciclo di accettazione di richieste da utente */
    printf("Inserisci nome del file, EOF per terminare: ");

    /* ATTENZIONE!!
     * Cosa accade se la riga e' piu' lunga di LINE_LENGTH-1?
     * Stesso dicasi per le altre gets...
     * Come si potrebbe risolvere il problema?
     */

    while (gets(nomeFile))
    {
        printf("Nome file: %s\n", nomeFile); // stampa il nome del file
        len = sizeof(servaddr);              // assegna alla lunghezza il valore della dimensione dell'indirizzo del server

        if (sendto(sd, &nomeFile, strlen(nomeFile) + 1, 0, (struct sockaddr *)&servaddr, len) < 0)
        {                                                           // invia il nome del file al server, se l'invio fallisce stampa un messaggio di errore
            perror("sendto");                                       // stampa un messaggio di errore
            printf("Inserisci nome del file, EOF per terminare: "); // stampa un messaggio
            continue;
        }

        printf("In attesa di risposta...\n");

        if (recvfrom(sd, &wordCount, sizeof(wordCount), 0, (struct sockaddr *)&servaddr, &len) < 0)
        {                       // riceve il conteggio delle parole dal server, se la ricezione fallisce stampa un messaggio di errore
            perror("recvfrom"); // stampa un messaggio di errore
            continue;
        }

        if (wordCount < 0)
        {
            printf("Errore nel conteggio delle parole\n");
        }
        else
        {
            printf("Il file %s contiene %d parole\n", nomeFile, wordCount);
        }
        printf("Inserisci nome del file, EOF per terminare: ");
    }

    // CLEAN OUT
    close(sd);
    printf("\nClient: termino...\n");
    exit(0);
}
