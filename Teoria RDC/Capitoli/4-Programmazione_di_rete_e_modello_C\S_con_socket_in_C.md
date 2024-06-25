# <span style="color:#aa66ff">Programmazione di rete e modello C/S con socket in C</span>

## <span style="color:#60c1fb">Introduzione</span>

Nel caso locale, per la comunicazione e la sincronizzazione, si usano segnali e file. Nel caso specifico di processi coresidenti sullo stesso nodo ci sono 3 possibilità:

- **Pipe**: comunicazione tra processi con un parente in comune.
- **Pipe con nome**: comunicazione tra processi non imparentati.
- **Memoria condivisa**: utilizzabile su una stessa macchina.

Per la comunicazione tra processi su macchine diverse si usano le **socket**. In UNIX ogni processo mantiene una tabella di file aperti, in cui ogni sessione aperta sui file viene mantenuta con uno specifico file descriptor. Il paradigma d'uso è open, read/write, close.

Le socket sono conformi a questo paradigma.

Proprietà delle socket:

- **Eterogeneità**: possono essere usate per comunicare tra processi su macchine diverse.
- **Trasparenza**: la comunicazione fra processi è indipendente dalla localizzazione fisica.
- **Efficienza**: le socket sono implementate a livello di kernel, quindi sono molto efficienti.
- **Compatibilità**: i filtri possono lavorare in ambiti distribuiti senza subire alcuna modifica.
- **Completezza**: le socket supportano tutti i tipi di comunicazione.

## <span style="color:#60c1fb">Domini di Comunicazione, Nomi e Primitive Preliminari</span>

Esistono più domini di comunicazione, la prima scelta che si fa è il protocollo di comunicazione **PF** (Protocol Family), che può essere:

- PF_UNIX: comunicazione tra processi sullo stesso nodo.
- PF_INET: comunicazione tra processi su nodi diversi.

Successivamente occorre specificare l'**AF** (Address Family), che può essere:

- AF_UNIX: indirizzi per la comunicazione locale.
- AF_INET: indirizzi per la comunicazione su rete.

Le socket per potere essere utilizzate richiedono un binding tra il loro nome logico (che è un nome locale) e un indirizzo fisico, ovvero la porta su cui sono accessibili (che è un indirizzo di rete).

Per rappresentare i nomi delle socket, ovvero gli indirizzi, si usano due tipi di strutture:

- `sockaddr`: struttura generica per rappresentare un indirizzo.
- `sockaddr_in`: struttura specifica per rappresentare un indirizzo di rete.

Le primitive preliminari per la gestione delle socket sono:

- `socket()`: crea una socket, restituendo il file descriptor.
- `bind()`: associa un indirizzo ad una socket, restituendo un valore positivo in caso di successo.

Per ottenere il nome fisico a partire da un nome logico si usa la primitiva `gethostbyname()` che restituisce un puntatore ad una struttura `hostent`.

Per consentire ad un utente di usare dei nomi logici senza ricordare la porta: `getservbyname()` che restituisce il numero di porta relativo al servizio.

## <span style="color:#60c1fb">Presentazione dei dati</span>

Gli interi sono composti da più byte, e la rappresentazione dei dati in memoria può essere little-endian o big-endian. Per evitare problemi di compatibilità si usano le funzioni `htonl()` e `ntohl()`, rispettivamente per passare da **Host Byte Order** (HBO) a **Network Byte Order** (NBO) e viceversa.

## <span style="color:#60c1fb">Socket Datagram</span>

Le socket datagram permettono di formare half-association relative a un solo processo ma utilizzabili per comunicare con chiunque nel dominio.

Per scambiare i datagrammi occorre:

- Da parte del **client**:
  - Dichiarare le variabili di riferimento a una socket.
  - Conoscere l'indirizzo del server.
  - Conoscere la porta del servizio.
- Per il **server**:
  - Dichiarare le variabili di riferimento a una socket.
  - Conoscere la porta del servizio locale.
  - Poter ricevere su qualunque indirizzo IP locale.

Per la comunicazione oltre a `read()` e `write()` (per mantenere omogeneità con i file di UNIX) si usano `sendto()` e `recvfrom()` che restituiscono il numero di byte scritti o letti.

## <span style="color:#60c1fb">Socket Stream</span>

Le socket stream prevedono una risorsa che rappresenta a connessione virtuale tra le entità interagenti.

È presente una entità attiva (client) che richiede il servizio e una passiva (server) che accetta e risponde.

Nella comunicazione ci sono due fasi, una prima fase assimetrica dove avviene l'accettazione del client, e una fase simmetrica dove avviene lo scambio di dati.

La connessione permane fino alla chiusura di una delle due entità.

I processi possono utilizzare le socket per lavorare in remoto leggendo e scrivendo da input e output.

La primitiva `connect()` è una primitiva di comunicazione sincrona e termina quando la richiesta è accodata o in caso di errore. La `connect()` restituisce il controllo quando ha depositato la richiesta nella coda del server.

Il server crea una coda attraverso la primitiva `listen()` e accetta le richieste con `accept()`.

Per la comunicazione si usano `send()` e `recv()`.

## <span style="color:#60c1fb">Chiusura delle socket</span>

Su ogni flusso viaggiano dati in stream fino alla fine del file del peer che segnala con una chiusura che non ci sono più dati da leggere e si aspetta che i dati siano consumati.

La chiusura causa una indicazione di fine file (EOF) all'altro.

Per chiusura si usano le primitive `close()` e `shutdown()`.

## <span style="color:#60c1fb">Opzioni per le primitive</span>

Le primitive `setsockopt()` e `getsockopt()` permettono di impostare e ottenere le opzioni di una socket.

Alcune opzioni possibili sono:

- SO_SNDTIMEO e SO_RCVTIMEO: timeout per la ricezione e l'invio.
- SO_SNDBUF e SO_RCVBUF: dimensione del buffer di invio e ricezione.
- SO_REUSEADDR: permette di riutilizzare un indirizzo locale.
- SO_KEEPALIVE: permette di inviare pacchetti di controllo per verificare la connessione.
- SO_LINGER: permette di specificare il tempo di attesa prima di chiudere la connessione.

## <span style="color:#60c1fb">Modalità asincrone e non bloccanti per le primitive</span>

Le primitive `fcntl()` e `ioctl()` permettono di impostare la modalità asincrona e non bloccante per le primitive di I/O. La modalità asincrona permette di eseguire altre operazioni mentre si aspetta la risposta, mentre la modalità non bloccante permette di non bloccare il processo in attesa di una risposta. L'utente viene avvisato quando la risposta è pronta da un segnale SIGIO.

Nel caso in cui si debba mandare il segnale a un gruppo di processi si utilizza la primitiva `ioctl()` con l'attributo SIOCSPGRP.

Per ottenere socket non bloccanti si usa la primitiva `ioctl()` con l'attributo FIONBIO che ha valore 1 per il non bloccante e 0 per il bloccante. In questo modo si modificano le primitive:

- `accept()` restituisce un errore di tipo EWOULDBLOCK.
- `connect()` restituisce un errore di tipo EINPROGRESS.
- `recv()` e `send()` restituiscono un errore di tipo EWOULDBLOCK.
- `read()` e `write()` restituiscono un errore di tipo EWOULDBLOCK.

## <span style="color:#60c1fb">La Primitiva Select</span>

La primitiva `select()` permette di gestire l'attesa multipla sincrona. Ha un timeout intrinseco.

La `select()` invocata sospende il processo fino al timeout o al primo evento:

- Lettura: dati disponibili per la lettura.
- Scrittura: spazio disponibile per la scrittura.
- Eccezione: condizione eccezionale.

La `select()` restituisce il numero di file descriptor pronti e modifica i set di file descriptor.

[<< Torna all'indice](../0-Indice.md)

[Prossimo argomento >>](./5-Servizi_applicativi_standard_in_Internet.md)
