/* OpDatagram_client: richiede la valutazione di un'operazione tra due interi */

#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define LINE_LENGTH 256

/*Struttura di una richiesta*/
/********************************************************/
typedef struct
{
    int op1;
    int op2;
    char tipoOp;
} Request;
/********************************************************/

int main(int argc, char **argv)
{
    struct hostent *host;                    // definizione della struttura hostent per la risoluzione dei nomi
    struct sockaddr_in clientaddr, servaddr; // definizione delle strutture per gli indirizzi
    int port, sd, num_i, num2, len, ris, ok; // definizione delle variabili per la porta, il socket, i numeri, la lunghezza, il risultato e il controllo
    char okstr[LINE_LENGTH];                 // definizione della stringa per il controllo dell'input
    char c;                                  // definizione del carattere per il controllo dell'operazione
    Request req;                             // definizione della struttura per la richiesta

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
    num_i = 0; // inizializza il contatore

    // Il ciclo while verifica che il secondo argomento sia un intero, se non lo è stampa un messaggio di errore e termina il programma.
    while (argv[2][num_i] != '\0') // finchè non si raggiunge la fine della stringa
    {
        if ((argv[2][num_i] < '0') || (argv[2][num_i] > '9'))
        {
            printf("Secondo argomento non intero\n");
            printf("Error:%s serverAddress serverPort\n", argv[0]);
            exit(2);
        }
        num_i++;
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
    printf("Primo operando (intero), EOF per terminare: ");

    /* ATTENZIONE!!
     * Cosa accade se la riga e' piu' lunga di LINE_LENGTH-1?
     * Stesso dicasi per le altre gets...
     * Come si potrebbe risolvere il problema?
     */
    while ((ok = scanf("%i", &num_i)) != EOF)
    {
        if (ok != 1)
        {
            /* Problema nell'implementazione della scanf. Se l'input contiene PRIMA
             * dell'intero altri caratteri la testina di lettura si blocca sul primo carattere
             * (non intero) letto. Ad esempio: ab1292\n
             *				  ^     La testina si blocca qui
             * Bisogna quindi consumare tutto il buffer in modo da sbloccare la testina.
             */
            do
            {
                c = getchar();
                printf("%c ", c);
            } while (c != '\n');
            printf("Inserire il Primo operando (intero), EOF per terminare: ");
            continue;
        }

        // quando arrivo qui l'input e' stato letto correttamente
        req.op1 = num_i;
        // Consumo il new line, ed eventuali altri caratteri
        // immessi nella riga dopo l'intero letto
        gets(okstr);

        printf("Inserire secondo operando (intero): ");

        while (scanf("%i", &num2) != 1)
        {
            do
            {
                c = getchar();
                printf("%c ", c);
            } while (c != '\n');
            printf("Secondo operando (intero): ");
        }

        req.op2 = num2;
        gets(okstr); // consumo resto linea

        do
        {
            printf("Operazione (+ = addizione, - = sottrazione, * = moltiplicazione, / = "
                   "divisione): ");
            c = getchar();
        } while (c != '+' && c != '-' && c != '*' && c != '/');

        req.tipoOp = c;
        gets(okstr); // consumo resto linea
        printf("Stringa letta: %s\n", okstr);

        /* lettura completata */
        printf("Operazione richiesta: %d %c %d \n", req.op1, req.tipoOp, req.op2);

        /* richiesta operazione */
        len = sizeof(servaddr);
        if (sendto(sd, &req, sizeof(Request), 0, (struct sockaddr_in *)&servaddr, len) < 0)
        {
            perror("sendto");
            continue;
        }

        /* ricezione del risultato */
        printf("Attesa del risultato...\n");
        if (recvfrom(sd, &ris, sizeof(ris), 0, (struct sockaddr_in *)&servaddr, &len) < 0)
        {
            perror("recvfrom");
            continue;
        }

        printf("Esito dell'operazione: %i\n", ris);
        printf("Primo operando (intero), EOF per terminare: ");

    } // while gets

    // CLEAN OUT
    close(sd);
    printf("\nClient: termino...\n");
    exit(0);
}
