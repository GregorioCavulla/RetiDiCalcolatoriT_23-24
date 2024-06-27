# <span style="color:#aa66ff">Implementazione RPC di SUN</span>

## <span style="color:#60c1fb">Introduzione</span>

Nell'implementazione di SUN viene utilizzata un'infrastruttura di supporto che ha il compito di:

- Scambiare messaggi per l'identificazione di chiamate e risposte e per l'identificazione unica della procedura in remoto.
- gestire l'eterogeneità dei dati scambiati, esegue il marshalling, unmarshalling e la serializzazione dei dati.
- gestisce alcuni errori dovuti alla distribuzione

Il client può controllare o meno il servizio con una semantica at least once. il server è sequenziale e le chiamate del cliente sono sincrone e bloccanti.
C'è quindi possibilità di deadlock nel caso in cui un server chieda un servizio al programma chiamate.

Limplementazione RPC di SUN si basa sul modello ONC (Open Network Computing) che include

- XDR (eXternal Data Representation) per la serializzazione dei dati
- RPCGEN (RPC Generator) per la generazione di codice degli stub
- Portmapper per la risoluzione dell'indirizzo del server
- NFS (Network File System) per la condivisione di file

## <span style="color:#60c1fb">Definizione del Programma RPC</span>

Le RPC sono basate su un contratto esplicito sulle imformazioni scambiate in due parti descrittive in linguaggio IDL:

1. Definizione di programma RPC: specifiche del protocollo RPC per i servizi offerti (identificazione dei servizi e il tipo dei parametri)
2. Definizione dei tipi dei parametri, solo se non è noto in RPC

Queste due parti sono raggruppate in un file .x (in formato XDR) che descrive solo il contratto di interazione.

Durante la definizione del servizio le RPC di SUN hanno i seguenti vincoli:

- ogni definizione di procedura ha un solo parametro d'ingresso e un solo paramentro d'uscita.
- gli identificatori usano lettere maiuscole
- ogni procedura ha un identificatore univoco

Il programmatore implementa:

- il programma client: main(), logica per reperimento dei parametri, chiamata alla procedura remota
- il programma server: procedura remota, logica per l'elaborazione dei parametri, invio della risposta

Non realizza il main() nel server e gli stub del client e server (vengono generati da RPCGEN)

## <span style="color:#60c1fb">Confronto tra Sviluppo Locale e Sviluppo Remoto</span>

Il codice di un servizio è quasi identico alla versione locale con alcune differenze:

- sia argomento di ingresso che uscita sono passati per riferimento
- il risultato punta a una variabile statica (allocazione globale) per evitare la perdita di memoria
- il nome della procedura cambia aggiungendo `_x_svc` alla fine del nome (x è il numero di versione)

Il cliente viene inovocato con il nome dell'host e il parametro per il servizio. Nel main() si crea il gestore del trasporto `CLIENT*` che gestisce la comunicazione con il server e può usare UDP (default) o TCP.

La variabile `CLIENT*` è richiesta al portmapper tramite `CLIENT* clnt_create(char* host, u_long n_prog, u_long n_vers, char* protocol)`

I passi per sviluppare un programma RPC sono:

- definire i servizi e i tipi di dati, se necessario
- generare in modo automatico gli stub e le funzioni di conversione di XDR con RPCGEN
- pubblicare i servizi dal lato server (portmapper, e registrare i servizi presso portmapper)
- reperire l'endpoint del server tramite portmapper e creare il gestore di trasporto per l'interazione con il server

## <span style="color:#60c1fb">Identificazione di RPC</span>

Un messaggio RPC deve contenere il numero di programma, di versione e di procedura. SUN usa di default l'intervallo 0 - 0x1FFFFFFF per i programmi RPC di interesse comune. L'utente può definire numeri di programma in 0x20000000 - 0x3FFFFFFF. Mentre l'intervallo 0x40000000 - 0x5FFFFFFF è riservato per i programmi che generano automaticamente i numeri di programma.

## <span style="color:#60c1fb">Livelli di RPC</span>

L'implementazione RPC di SUn ha tre livelli:

- **alto**: è il livello utente, vengono forniti servizi RPC stardard ai client:
  - **rnusers()**: restituisce il numero di utenti connessi
  - **rusers()**: restituisce informazioni sugli utenti connessi
  - **rstat()**: ottiene informazioni sullo stato del server
  - **rwall()**: invia un messaggio a tutti gli utenti connessi
  - **getmaster()**: ottiene il nome del server master
  - **getrpcport()**: ottiene il numero di porta del server
- **intermedio**: per definire e utilizzare nuovi servizi RPC per procedure singole, due primitive:
  - **callrpc()**: invia una chiamata RPC e prova l'esecuzione di una procedura remota
  - **registerrpc()**: registra una procedura remota
- **basso**: la gestione avanzata del protocollo RPC, le chiamate `svc_register()` e `svc_run()` sono usate per registrare e gestire le procedure remote possono essere implementate con funzioni di più basso livello. Per il server:
  - **svcudp_create()** e **svctcp_create()**: creano un gestore di trasporto UDP o TCP
  - **pmap_unset()**: elimina un servizio dal portmapper
  - **svc_register()**: registra una procedura remota
  - **svc_run()**: esegue il server
- per il client:
  - **clientudp_create()** e **clienttcp_create()**: creano un gestore di trasporto UDP o TCP
  - **clnt_call()**: invia una chiamata RPC
  - **clnt_perror()**: per analizzare `clnt_call()` stampa un messaggio di errore
  - **clnt_destroy()**: distrugge un gestore di trasporto

## <span style="color:#60c1fb">Omogeneità dei Dati</span>

Per far comunicare due nodi eterogenei ci sono due soluzioni:

- dotare ogni nodo di tutte le funzioni di conversione possibili per ogni rappresentazioni dei dati: performance elevaate ma alto numero di funzioni di conversioni [ n(n-1) ]
- concordare un formato comune di rappresentazione dei dati: performance ridotte ma minor numero di funzioni di conversione [ 2n ]

In questo caso il formato comune è XDR che si occupa di marshalling dei dati. Si colloca al livello 6 di OSI ([livello di presentazione](./6-OSI_Open_System_Interconnection.md#livello-presentazione)).

XDR al suo interno ha procedure di conversione relative a tipi atomici predefiniti e tipi standard. Gestisce anche strutture dati, `callrpc()` accetta solo un argomento di ingresso e uno di uscita, infatti si devono usare strutture per raggruppare gli argomenti. Per ogni informazione da trasmettere si devono effetturare due trasformazioni. In XDR sono presenti due tipi di definizione (paragrafo
[Definizione del Programma RPC](9-Implementazione_RPC_di_SUN.md#definizione-del-programma-rpc)):

- **definizione di tipi di dati**
- **definizione delle specifiche di protocollo RPC**

## <span style="color:#60c1fb">Portmapper</span>

Il portmapper è il server (RPC) di nomi. Identifica il numero di poeta associato ad un qualsiasi programma ed esegue l'allocazione dinamica dei servizi sui nodi. Ha una sola porta dedicata, con un solo processo (111).

Registra i servizi sul nodo e offre le seguenti procedure:

- inserimento di un servizio
- eliminazione di un servizio
- corrispondenza tra associazione astratta e porta
- intera lista delle corrispondenze
- supporto a esecuzione remota
  Di default utilizza il protocollo UDP

## <span style="color:#60c1fb">Modalità Asincrona Batch</span>

Di default un client RPC è sincrono col server. Per renderlo asincrono è necessario:

- che il client utilizzi TCP
- specificare un timeout nuullo al client in modo da continuare subito l'esecuzione
- assicurarsi che il server non preveda una risposta

Nella `clnt_call()` e nella `clnt_control()` occorre specificare un timeout nullo per ofni chiamata.

Visto che il server non deve inviare risposte, nella procedura bisogna dichiarare `xdr-void` come funzione XDR o passarle 0 come argomento.

Server e client basano la loro fiducia sul trasporto affidabile, quindi è importante usare TCP per evitare di perdere messaggi. Le richieste del client vengono messe nel buffer TCP e gestite dal trasporto senza bloccare il processo che le genera.

[<< Torna all'indice](../0-Indice.md)

[Prossimo argomento >>](./10-Sistemi_di_Nomi.md)
