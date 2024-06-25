# <span style="color:#aa66ff">Generalità, Obiettivi e Modelli di Base</span>

## <span style="color:#60c1fb">Introduzione ai Sistemi Distribuiti</span>

**Sistemi Distribuiti**: sistemi residenti in località diverse che usano comunicazione e cooperazione per ottenere risultati coordinati.

Sono più complessi di quelli concentrati, ma permettono l'accesso a risorse remote e permettono la loro condivisione come se fossero locali.

Ciò che si desidera ottenere è la trasparenza dell'allocazione, dinamicità del sistema e qualità dei servizi.

Lo sviluppo di un'applicazione distribuita prevede l'analisi, lo sviluppo sulla base di un algoritmo, la codifica e l'esecuzione.

Le due fasi principali sono:

- **Mapping**: fase in cui si stabilisce la configurazione per l'architettura e si stabilisce come allocare le risorse logiche sulle risorse fisiche.
- **Binding**: fase in cui si stabilisce come ogni entità dell'applicazione si lega alle risorse del sistema, può essere:
  - **Statico**: si stabilisce a priori.
  - **Dinamico**: si allocano dinamicamente durante l'esecuzione e si utilizzano dei sistemi di nomi per poter riferire in modo efficiente i processi con cui si vuole comunicare.

Per un'applicazione distribuita sono necessari i **processi**: azioni specifiche da eseguire sui dati locali.

Rispetto ai sistemi concentrati si aggiungono i **protocolli**: regole e procedure a cui ci si deve attenere per una corretta esecuzione.

**UNIX** standardizza i processi e mette a disposizione **API** per:

1. Interagire con i file (open, read, write, close).
2. Progettare filtri con una corretta gestione delle risorse.
3. Poter avere la concorrenza dei processi (fork, exec).
4. Poter gestire la comunicazione fra processi.

## <span style="color:#60c1fb">Processi</span>

I **processi** sono un'aggregazione di componenti, hanno uno spazio di indirizzamento e uno di esecuzione.

Possono essere:

- **Pesanti**: richiedono molte più risorse e il loro cambio di contesto è un'operazione molto pesante con molto overhead.
- **Leggeri**: condividono tra di loro la visibilità di un ambiente contenitore e sono caratterizzati da uno stato e un overhead limitati. Il contenitore unico è un processo pesante che fornisce la visibilità comune ai thread.
  Java supporta i processi leggeri, i thread creati sono mappati all'interno della JVM che è il processo pesante contenitore.

## <span style="color:#60c1fb">Modello Client/Server</span>

**Client/Server** è un modello di coordinamento tra due entità che cooperano per una comunicazione e un servizio.
È sempre molti a uno: un server e tanti client.

Il modello può essere:

- **Sincrono**: se è prevista una risposta (**asincrono** se non è prevista).
- **Bloccante**: se si attende la risposta del pari (**non bloccante**).
- **Asimmetrico**: se il cliente conosce a priori il server, ma non viceversa (**simmetrico** se si conoscono entrambi).
- **Dinamico**: se il binding tra client e server è dinamico, ovvero il server che risponde alle richieste può cambiare.

Di default, il modello è sincrono, bloccante, asimmetrico e dinamico.

Il server deve essere sempre pronto a eventuali richieste, viene detto quindi **demone**: processo sempre attivo in un ciclo infinito.
Il server oltre alla comunicazione deve realizzare il servizio con azioni locali, considerando molteplici client, quindi integrità dei dati, accessi concorrenti, autenticazioni.

Il client lavora in modo sincrono e con interazione bloccante. Per ovviare ad attese troppo lunghe, si usa un time-out che fa scattare un'eccezione.

## <span style="color:#60c1fb">Modelli Asincroni, Pull e Push</span>

Nel modello **pull**: detto anche polling, il cliente chiede ripetutamente un servizio allo stesso server con un time-out breve, fino a quando non riceve una risposta entro un time-out. È utile nel caso in cui si abbiano server spesso congestionati.

Nel modello **push**: il cliente fa una richiesta una sola volta, si sblocca e può fare altro. È il server che, eseguito il servizio, ha la responsabilità della consegna del risultato.

## <span style="color:#60c1fb">Modello a Delegazione</span>

In caso di interazione sincrona non bloccante, il cliente può delegare a un **proxy** la funzionalità di ricezione della risposta. Il proxy opererà in modo push o pull per fornire la risposta al cliente.

## <span style="color:#60c1fb">Modello ad Eventi</span>

È un modello molti a molti, sono presenti molti client e molti server, e un gestore che gestisce la comunicazione. È chiamato anche pub/sub. I server pubblicano i loro servizi (publisher), i client si iscrivono al gestore (subscribers).

## <span style="color:#60c1fb">Modello ad Agenti Multipli</span>

I servizi vengono forniti dal coordinamento di più server detti agenti, che forniscono un servizio globale unico.

## <span style="color:#60c1fb">Stato</span>

Lo **stato** mantiene traccia delle comunicazioni e delle azioni precedenti.

Il rapporto tra cliente e servitore può quindi essere:

- **Stateless**: che non tiene traccia dello stato.
- **Stateful**: che tiene lo stato dell'interazione.

Se viene mantenuto uno stato si può avere un accordo predefinito tra le entità interagenti e avere una gestione condivisa che faciliti la comunicazione.

Solitamente lo stato è memorizzato nel server:

- Un server stateless è più leggero e affidabile.
- Un server stateful garantisce dimensioni più contenute dei messaggi e una maggiore velocità di risposta.

## <span style="color:#60c1fb">Tipi di Server</span>

Un server può essere:

- **Sequenziale**: richieste processate una alla volta, con una coda di attesa. Per limitare l'overhead si può limitare la lunghezza della coda.
- **Concorrente**: gestisce più richieste contemporaneamente. Migliori prestazioni ma più complesso. Può essere:
  - **Mono processo**: dove un unico processo gestisce sia coda che servizio.
  - **Multi processo**: in cui il processo server gestisce la coda e genera processi figli per ogni richiesta.

## <span style="color:#60c1fb">Sistemi di Nomi</span>

Per accedere ai servizi è necessario identificarli e trovarli. Questa funzione è svolta dai **sistemi di nomi**: server capaci di fornire servizi di gestione e mantenimento dei nomi.

Un cliente che vuole richiedere un servizio deve poter riferire il server, questo è possibile attraverso un nome trasparente rispetto alla locazione del server.

Il binding dei nomi può essere statico o dinamico: nel caso statico i riferimenti sono risolti prima dell'esecuzione, nel caso dinamico i riferimenti sono risolti al momento del bisogno.

Nel caso distribuito le risorse e il binding sono dinamici.

Un sistema di nomi deve rendere possibile inserire nuove entità durante l'esecuzione, e consentire di ritrovare il nome delle entità.
Si distinguono in:

- **Gestori Partizionati**: ciascuno è responsabile di una sola parte (partizione) dei riferimenti di località. Sono presenti più gestori.
- **Gestori Replicati**: ciascuno è responsabile, insieme ad altri, di una parte dei riferimenti.

## <span style="color:#60c1fb">DNS, Domain Name System</span>

**DNS** è un sistema di nomi basato su un insieme di gestori coordinati per rispondere a query che richiedono un **indirizzo IP**: indirizzo fisico corrispondente a un nome di dominio.

I gestori si occupano di gestire tabelle di corrispondenza tra nomi e indirizzi IP {nome, indirizzo}.

DNS introduce i nomi logici in una gerarchia di domini. Una richiesta viene inoltrata a un **name resolver** proprio di ogni dominio.

Quando il client chiede un mapping al resolver, questo fornisce la risposta o perché la conosce già (è presente in cache) o perché la chiede a un altro resolver, seguendo la gerarchia ad albero è possibile raggiungere il resolver che conosce la risposta.

I client e i name resolver utilizzano il protocollo UDP sulla porta 53, TCP nel caso di messaggi di grandi dimensioni.

Il protocollo tra server DNS ha due tipi di query:

- **Iterativa**: si fornisce al richiedente la risposta o un riferimento a un altro resolver.
- **Ricorsiva**: il resolver chiede a un altro resolver la risposta e la fornisce al richiedente.

[<< Torna all'indice](../0-Indice.md)

[Prossimo argomento >>](./3-Programmazione_di_rete_e_modello_C\S_con_socket_in_Java.md)