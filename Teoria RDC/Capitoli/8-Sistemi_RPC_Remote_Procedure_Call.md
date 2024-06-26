# <span style="color:#aa66ff">Sistemi RPC - Remote Procedure Call</span>

## <span style="color:#60c1fb">Semantiche di Comunicazione</span>

Le principali semantiche sono:

- **at least once**: prevede ritrasmissiioni, il messaggio può arrivare più volte. È una semantica adatta ad azioni idempotenti (che possono essere ripetute senza effetti collaterali). In caso le cose vadano bene il messaggio arriva una o più volte, altrimenti non arriva mai il client non sa se il server ha ricevuto e il server non sa se il client ha richiesto.
- **at most once**: i pari lavorano per ottenere reliability: il messaggio, se arriva, arriva una volta sola. Un esempio di progetto at most once è TCP. Il client fa ritrasmissioni, mentre il server mantiene uno stato per riconoscere i messaggi duplicati. In caso di successo il messaggio arriva e viene trattato una sola volta, in caso di insuccesso ne il server ne il client sanno se l'altro ha ricevuto o trasmesso i lmessaggio.
- **exactly once**: al termine dell'operazione sia il client che il server sanno lo stato del pari, quindi c'è un accordo completo sull'interazione. In caso le cose vadano bene il messaggio arriva e viene trattato una sola volta, in caso le cose vadano male il client e il server sanno se il messaggio è arrivato o no.
- **may be** (best effort): il messaggio può arrivare o no, non c'è garanzia di consegna. È una semantica adatta a servizi non critici, come la trasmissione di video o audio in streaming, esempio UDP.

## <span style="color:#60c1fb">Introduzione ai Sistemi RPC</span>

I sistemi RPC mettono a disposizione delle procedure invocabili da remoto. In una chiamata a procedura remota sia i parametri che i risultati vengono trasmessi attraverso la rete. Il client invia la richiesta ed attende in modo sincrono fino alla risposta fornita dal server.

Rispetto alla chiamata a procedura locale:

- sono coinvolti processi distinti su nodi diversi
- essendo processi distinti, client e server hanno vita separata e non condividono lo spazio di indirizzamento
- sono possibili malfunzionamenti sui nodi o nell'infrastruttura

Le proprietà che contraddistinguono RPC sono:

- controllo dinamico del tipo dei parametri e del risultato
- trattamento dei parametri di ingresso e uscita sia su client che su server (marshalling e unmarshalling, o serializzazione e deserializzazione)
- è trasparente, ovvero il client non sa se la procedura è locale o remota
- controllo dei tipi e parametrizzazione
- binding distribuito (come si fa a sapere dove si trova la procedura remota?)
- possibile trattamento degli orfani

Le RPC non hanno troppi standard e ogni sistema usa primitive diverse. Dalla parte di client avvengono delle chiamate di servizio, mentre dalla parte del serve ci sono due tipi di politica di accettazione e risposta:

- modalità esplicita e sequenziale: il servizio è svolto da un unico processo e decide quando e se eseguire i metod
- modalità implicita e parallela: ogni servizio è eseguito da un processo indipendente, generato in modo automatico per ogni richiesta.

## <span style="color:#60c1fb">Tolleranza ai Guasti</span>

L'obiettivo applicativo è di mascherare i malfunzionamenti quali:

- la perdita di un messaggio di richiesta o di risposta
- il crash del nodo del client
- il crash del nodo del server

Il client può tentare diverse politiche e diversi comportamenti:

- aspettare per sempre
- usare un timeout e ritrasmettere
- usare un timeout e riportare un eccezione

Spesso si assume che le azioni siano idempotenti. Le RCP hanno semantiche tipiche e relative strategie:

- **at least once**: il client ritrasmette finché non riceve una risposta
- **at most once**: il client ritrasmette finché non riceve una risposta, il server mantiene uno stato per riconoscere i messaggi duplicati
- **exactly once**: il client e il server mantengono uno stato per riconoscere i messaggi duplicati
- **may be**: il client non fa ritrasmissioni

Nel caso in cui il crash sia avvenuto lato client si devono trattare i procesi orfani sul sever che attendono di consegnare il risultato. Le politiche tipiche sono:

- **sterminio**: ogni organo risultato di un crash viene distrutto
- **terminazione a tempo**: ogni organo risultato di un crash viene distrutto dopo un certo tempo
- **reincarnazione** (ad epoche): il tempo è divisio in epoche e tutto cio che è relativo all'epoca precedente è obsoleto e viene distrutto

## <span style="color:#60c1fb">NCA - Network Computing Architecture</span>

Nell'architettura delleRPC sono introdotti degli stub ai due endpoint della comunicazione. In questo modo le chiamate appaiono del tutto locali al client e si garantisce la massima trasparenza.
L'utente finale deve progettare e implementare unicazmente le parti applicative.

Il modello con gli stub è assimetrico infatti:

- il client invoca lo stub che si incarica di recupero del server, trattamento dei parametri, richiesta al supporto runtime, trasporto della richiesta
- il server riceve la richiesta dallo stub relativo che si incarica del trattamento dei parametri, al completamento del servizio, il server passa il risultato allo stub che si occupa del trasporto della risposta

## <span style="color:#60c1fb">Passaggio dei Parametri e Trattamento delle Eccezioni</span>

I parametri nelle RPC possono essere passati per valore (preferito) o riferimento.

Nel primo caso viene effettuato un passoggio con trasferimento e vista.
Nel caso di passaggio per riferimento il passaggio è senza trasferimento e l'oggetto viene reso remoto.
Vengono quindi usati oggetti che rimangono nel nodo di partenza e devono essere identificati in modo univoco nell'intero sistema. Se si vuole riferire una entità del client, si passa il riferimento alla stessa entità che i nodi remoti possono riferire attraverso RPC.

Le RPC prevedono il trattamento degli errori tramite una gestione delle eccezioni.

## <span style="color:#60c1fb">IDL - Interface Definition Language</span>

Gli **Interface Definition Languafe** sono linguaggi per la descrizione di operazioni remote, specifica del servizio e generazione degli stub.
Gli IDL consentono:

- identificazione unica del servizio tra quelli disponibili con un nome astratto e prevedono diverse versioni del servizio.
- la definizione astratta dei dati da trasmettere in input e output usando un linguaggio astratto di definizione dei dati.

Gli IDL hanno lo scopo di supporto allo sviluppo di una applicazione permettendo la generazione automatica degli stub dall'interfaccia specificata dall'utente.
Questo processo avviene grazie all **RPCGEN** (Remote Procedure Call Generator) che genera automaticamente gli stub e i file di supporto.

## <span style="color:#60c1fb">Sviluppo e Fasi di Supporto RPC</span>

Dopo la specifica del contratto in IDL, sono presenti:

- **compilazione di sorgenti e stub**: vengono prodotti gli stub e ci si assicura che client e server siano compatibili
- **binding delle entità**: ottenimento dell'aggancio tra i client e il server
- **trasporto dei dati**:è intrinseco allo strumento e si preferisce velocità ed efficienza
- **controllo della concorrenza**: consente di usare stessi strumenti per funzione diverse
- **supporto alla rappresentazione dei dati**: per superare l'eterogeneità si trasformano i dati

## <span style="color:#60c1fb">RPC Binding e Sistemi di Nomi</span>

Il binding del client al sever può essere effettuato secondo due possibili modalità:

- **pessimistica e statica**: la compilazione risolve ogni problema prima dell'esecuzione e forza un binding statico nel distribuito. Costo limitato ma poco flessibile.
- **ottimistica e dinamica**: il binding dinamico ritarda la decisione alla necessità. Con costi maggiori, ma consente di dirigere le richieste sul gestore più scarico.

Il binding dinamico viene ottenuto in due fasi:

- **fase statica di servizio**: prima dell'esecuzione il client specifica a chi vuole essere connesso con un nome unico. Si associano dei nomi unici di sistema alle operazione o alle interfacce, poi si effettua il binding.
- **fase dinamica di indirizzamento all'uso**: durante l'esecuzione il client deve essere collegato al server (addressing). In questa fase si cercano gli eventuali server pronti per il servizio.

La prima fase (statica) è risolta con un numero associato staticamente all'interfaccia del servizio. Questa deve avere costi limitati durante il servizio e può essere:

- **esplicita**: il client deve raggiungere un server mendiante un multicast o broadcast attendendo solo la prima risposta.
- **implicita**: si fa uso di un name server che registra tutti i server e agisce su opportune tabelle di binding prevedendo funzioni di ricerca di nomi, registrazione, aggiornamento ed eliminazione.

In caso di binding dinamico ogni chiamata richiede un collegamento dinamico. Spesso viene usato lo steso binding ottenuto come se fosse statico per questione di costo: il binding quindi avviene meno frequentemente. Generalmente si usa lo stesso binding per molte chiamate allo stesso server.

Le RPC hanno portato molti sistemi di nomi detti binder, boker, name server. Un bindier deve fornire operazioni come:

- **Lookup**
- **Register**
- **Unregister**

Il nome del server può dipendere o meno dal nodo di residenza, se dipendente ogni variazione deve essere comunicata al binder. Il binding è attuato coem servizio coordinato di più server: Vengono usati binder multipli per limitare l'overhead ai singoli client e inizialmente questi ultimi usano un broadcast per trovare il binder più conveniente.

## <span style="color:#60c1fb">RPC Asincrone</span>

Le RPC asincrone e non bloccanti permettono che il client non si blocchi in attesa di una risposta. Ne esistono due tipi:

- **a bassa latenza**: manda un messaggio di richiesta e trascura il risultato
- **a throughput elevato**: raggruppa le richieste in un unico messaggio

## <span style="color:#60c1fb">Proprietà delle RPC</span>

Proprità delle RPC visibili all'utente sono:

- ci sono entità che possono richiedere operazioni o metodi di oggetti
- la semantica di comunicazione (may be, at least once, at most once, exactly once)
- modi di comunicare (sincrono, asincrono, non bloccante)
- durata massima e eccezioni (ritrasmissione, timeout, eccezioni)

Proprietà delle RPC non visibili dall'utente sono:

- ricerca del servitore: uso di nomi con broker unico o multipli
- presentazione dei dati (linguaggio idl e generazione automatica degli stub)
- passaggio dei parametri
- eventuali legami con le risorse del server

[<< Torna all'indice](../0-Indice.md)

[Prossimo argomento >>](./9-Implementazione_RPC_di_SUN.md)
