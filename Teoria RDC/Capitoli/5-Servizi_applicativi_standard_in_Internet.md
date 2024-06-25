# <span style="color:#aa66ff">Servizi Applicativi Standard in Internet</span>

## <span style="color:#60c1fb">Introduzione</span>

Applicazioni costruite sui protocolli con interfacce locali diverse. Si possono distinguere due macro categorie di servizi:

- **Applicazioni Client-Server**: applicazioni punto a punto che coinvolgono due entità alla volta. Esempi di queste sono stampa remota, terminali remoti, trasferimento di file.
- **Aplicazioni di infrastruttura distribuita**: applicazioni che prevedono un'infrastruttura sempre presente per fornire un servizio, come ad esempio il DNS, il Web, la posta elettronica.

Alcune applicazioni sono disponibili solo per UNIX come **rsh**, **rwho** e **rlogin**. Altre sono indipendenti dal sistema operativo come:

- **FTP**: File Transfer Protocol
- **SMTP**: Simple Mail Transfer Protocol
- **HTTP**: HyperText Transfer Protocol
- **DNS**: Domain Name System
- **Telnet**: Telnet Protocol
- **TFTP**: Trivial File Transfer Protocol
- **LPD**: Line Printer Daemon

## <span style="color:#60c1fb">Telnet - Virtual Terminal Protocol</span>

Le caratteristiche di Telnet sono:

- Comunicazione simmetrica con funzioni distinte, complementari e differenziate
- La gestione dell'eterogeneità avviene tramite l'interfaccia di terminale virtuale
- La negoziazione delle opzioni del collegamento tra il client e il server (ASCII 7 bit, 8 bit, ecc.)

Nel modello Client-Server, il client:

- Stabilisce una connessione TCP con il server, accetta i caratteri dall'utente e li invia al server
- Accetta i caratteri dal server e li visualizza sul terminale locale

Il server invece:

- Accetta le richieste di connessione dai client
- Esegue le azioni e manda i risultati al client generando processi figli per ogni connessione
- Continua ad accettare richieste di connessione

Per collegarsi a un server Telnet occorre il nome logico dell'host (o il suo IP) e il numero della porta (di default 23).

Telnet nasce per risolvere l'eterogeneità dei terminali:

- Set di caratteri
- Diversa codifica dei caratteri
- Lunghezza della linea e della pagina
- Shortcut differenti (escape, backspace, ecc.)

Sulla rete si considera un unico terminale standard, in ogni macchina si effettua la conversione da terminale locale a terminale standard e viceversa.

Telnet è half-duplex, ovvero il client e il server possono inviare dati solo in una direzione alla volta, e presenta diversi problemi:

- Se viene usato in modalità one char at a time, ci sono problemi di overhead
- Se viene usato in modalità one line at a time, sorgono problemi di buffering

Nel caso in cui lo stream dei dati sia pieno:

- **Flush**: il client invia un comando di flush al server
- **No client flow control**: il server invia un comando di no client flow control al client, che non può più inviare dati al server
- **Client flow control sliding window**: cambiamento della dimensione e controllo del client

## <span style="color:#60c1fb">NVT - Network Virtual Terminal</span>

Modello standard per la comunicazione dei terminali, i dati viaggiano in formato standard e vengono convertiti una volta ricevuti dal server (e viceversa al ritorno).

## <span style="color:#60c1fb">rlogin - Remote Login</span>

Servizio di login remoto su un'altra macchina UNIX. Permette di accedere a un'altra macchina senza dover inserire la password, se si è già autenticati.

Rlogin è molto più leggero di Telnet. Vengono utilizzate due connessioni, quattro processi ed è presente il controllo del flusso: il cliente tratta localmente i caratteri di controllo del terminale.
Sono presenti un cliente rlogin e un server remoto rlogind:

- Il client crea una connessione TCP con il server poi divide le funzioni in input e output:
  - Il genitore gestisce i caratteri che vanno dal cliente al server remoto
  - Il figlio gestisce i caratteri che vengono dal server remoto
- Il server rlogind accetta le connessioni dai client e crea un processo figlio per ogni connessione

## <span style="color:#60c1fb">FTP - File Transfer Protocol</span>

FTP è il protocollo utilizzato per trasferire file. Usa TCP che è affidabile e connection-oriented (TFTP utilizza UDP).
L'esecuzione di FTP è a livello applicativo o con accesso interattivo utente.

I comandi per trasferire file sono:

- **get remote-file [local-file]**: trasferisce un file dal server al client
- **put local-file [remote-file]**: trasferisce un file dal client al server
- **mget remote-files**: trasferisce più file dal server al client
- **mput local-files**: trasferisce più file dal client al server

In FTP tutte le informazioni viaggiano in chiaro e si usa una codifica a 3 cifre numeriche: la prima cifra codifica le interazioni, la seconda le risposte e la terza i parametri.

L'accesso da parte di più client è concorrente e sono presenti almeno due connessioni per ogni client:

- **Control connection**: per i comandi e le risposte
- **Data connection**: per il trasferimento dei dati

FTP offre anche la possibilità di avere uno stato che, in caso di interruzione della connessione, permette di riprendere il trasferimento dal punto in cui si era interrotto.

All'avvio della connessione, chi esegue la connect è detto attivo e chi esegue la accept è detto passivo:

- Nel caso in cui il client sia attivo, il server deve avere già fatto la listen ed essere in attesa di richieste. In questo caso il client deve garantire che le azioni siano nell'ordine giusto e coordinarsi con il server.

- Nel caso in cui il server sia attivo, il client esegue la listen e fa una accept sulla porta poi utilizza la get o la put.

## <span style="color:#60c1fb">Posta Elettronica</span>

Permette lo scambio asincrono di messaggi tra utenti.

Il servizio di mail usa un sistema di comunicazione punto a punto attraverso una rete di **UA** (User Agent) e **MTA** (Mail Transfer Agent).

Gli MTA trasferiscono le mail dall'UA del mittente all'UA del destinatario. Il protocollo usato è SMTP (Simple Mail Transfer Protocol) che usa TCP.

In questo sistema gli UA sono gli utenti finali e gli MTA sono i server di posta che si occupano del trasporto.

Un processo UA in background diventa il client del MTA che:

- Mappa il nome della macchina del destinatario in un indirizzo IP
- Tenta la connessione TCP con il mail server successivo o con il mail server del destinatario
- Se la connessione è riuscita copia il messaggio nel server del destinatario

Per il routing tra MTA si usa il DNS.

L'accesso ai singoli messaggi è regolato da diversi strumenti quali **POP3** (Post Office Protocol 3) e **IMAP** (Internet Mail Access Protocol).

## <span style="color:#60c1fb">SMTP - Simple Mail Transfer Protocol</span>

È il protocollo per il trasferimento tra MTA. I ruoli di sender e receiver possono essere invertiti per il trasferimento di mail.

## <span style="color:#60c1fb">Usenet news</span>

È un servizio di news distribuito su Internet. Le news sono organizzate in gruppi e i messaggi sono scritti in ASCII.

L'architettura è formata da tanti server in cui sono mantenute le news e da molti lettori di news (i client).

[<< Torna all'indice](../0-Indice.md)

[Prossimo argomento >>](./6-OSI_Open_System_Interconnection.md)
