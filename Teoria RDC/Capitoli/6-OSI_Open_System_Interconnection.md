# <span style="color:#aa66ff">OSI - Open System Interconnection</span>

## <span style="color:#60c1fb">Introduzione</span>

OSI è uno standard di comunicazione tra sistemi aperti che permette a sistemi diversi di interoperare, consentendo loro di comunicare e operare tra loro in modo aperto e standardizzato. Si occupa del controllo, coordinamento e monitoraggio di sistemi interconnessi e nasce con l'obiettivo di creare un modello di riferimento per la comunicazione tra sistemi aperti, in modo che i sistemi possano comunicare tra loro indipendentemente dal produttore.

OSI è:

- Organizzato a livelli, ciascuno con un compito specifico
- Interamente ad oggetti
- Astratto e senza legami con le realizzazioni concrete
- Scenario ampio e di riferimento per le soluzioni

## <span style="color:#60c1fb">Architettura</span>

OSI propone un'architettura di soluzione per descrivere una comunicazione complessa, basandosi sul principio di astrazione ovvero nascondere i dettagli e mostrare solo le entità utili per l'utente finale. OSI prevede 7 livelli di astrazione:

- Livello Fisico
- Livello Data Link
- Livello Network
- Livello Trasporto
- Livello Sessione
- Livello Presentazione
- Livello Applicazione

Ogni livello ha l'obiettivo di comunicare con il pari tramite un protocollo, nascondendo i dettagli al livello superiore e usando gli strumenti esposti da quello inferiore per il suo obiettivo. I livelli OSI si devono attraversare in modo ordinato secondo la gerarchia introdotta. Ogni livello prevede un protocollo da realizzare e un servizio da presentare/richiedere attraverso un'interfaccia.

## <span style="color:#60c1fb">Nomi ed Entità</span>

Durante l'invio di informazioni si hanno:

- **Mittente**: entità che inizia la comunicazione
- **Destinatario**: entità che riceve la comunicazione
- **Intermediario**: entità che trasmette la comunicazione

Ogni azione comporta una comunicazione che passa attraverso i livelli del mittente e del ricevente, e almeno fino al livello di rete per gli intermediari. Ogni elemento attivo in un livello viene detto entità. Si definisce un sistema di nomi per le entità utilizzando la lettera iniziale maiuscola del livello a cui appartengono (es. S-layer, S-protocol per il livello di Sessione). L'interfaccia logica tra una n-1 entità e una n entità è detta **SAP** (Service Access Point), che identifica il punto di accesso che un servizio OSI offre al suo livello superiore.

## <span style="color:#60c1fb">Protocolli e Implementazioni</span>

OSI definisce solamente le specifiche di comunicazione senza dettare nessuna specifica a livello locale né suggerire alcuna tecnologia di soluzione. Per ogni livello sono possibili e riconosciute implementazioni a procedure e processi.

## <span style="color:#60c1fb">Formato dei Messaggi</span>

I messaggi scambiati tra i diversi livelli sono standardizzati. Le entità che descrivono questi messaggi sono:

- **SDU** (Service Data Unit): dati scambiati tra entità dello stesso livello
- **IDU** (Interface Data Unit): dati scambiati tra entità di livelli adiacenti
- **PDU** (Protocol Data Unit): dati scambiati tra entità di livelli diversi

Le operazioni verso il basso sono incapsulate e devono passare informazioni e comandi al protocollo. L'IDU contiene i dati e i comandi che il protocollo deve passare al livello inferiore, detta **ICI** (Interface Control Information), definendo poi un **PCI** (Protocol Control Information) che contiene i dati e i comandi che il protocollo deve passare al livello superiore.

## <span style="color:#60c1fb">Modalità di Connessione</span>

Due modalità di connessione:

- **Connectionless**: non c'è una connessione stabilita tra mittente e destinatario, ogni pacchetto è indipendente dagli altri, nessuna qualità di servizio garantita.
- **Connection-oriented**: c'è una connessione stabilita tra mittente e destinatario, i pacchetti sono inviati in sequenza e garantiti, qualità di servizio garantita.

## <span style="color:#60c1fb">Primitive</span>

Le entità pari cooperano tramite primitive per implementare le funzionalità del livello a cui appartengono. Le primitive sono:

- **data**: trasferimento di dati
- **connect**: per stabilire una connessione
- **disconnect**: per chiudere una connessione

Ciascuna primitiva ha 4 forme:

- **request**: richiesta di servizio
- **indication**: notifica di un evento
- **response**: risposta a una richiesta
- **confirm**: conferma di una risposta

## <span style="color:#60c1fb">Livelli</span>

### <span style="color:#66cc99">Livello Network</span>

Il livello di network si occupa delle realizzazioni di routing tra reti diverse, oltre a definire il sistema di nomi delle entità.
L'obiettivo è il passaggio delle informazioni interferendo il meno possibile sul comportamento locale.

I compiti del livello di rete sono:

- **Indirizzamento**
- **Controllo di flusso**
- **controllo di congestione**
- **migliorare l'efficienza**
- **evitare deadlock**

Rispetta il pricipio di separazione: i nodi intermedi devono poter interagire solo per le funzionalità necessare e non essere toccati ai livelli applicativi.

### <span style="color:#66cc99">Livello Trasporto</span>

Il livello di trasporto si occupa di trasferire i dati tra due entità di livello superiore, garantendo la corretta consegna dei dati e la qualità del servizio. Si occupa di:

- **Controllo di flusso**
- **Controllo di errore**
- **Controllo di sequenza**
- **Controllo di congestione**

### <span style="color:#66cc99">Livello Sessione</span>

La sessione considera e determina i meccanismi per il dialogo tra entità diverse. Nella sessione il dialogo può:

- essere bidirezionale
- essere molteplice e strutturato in attività separate e diverse
- considerare le risorse impegnate
- avere garanzie di correttezza e affidabilità

Un dialogo può avere molte dimensioni possibili e anche molte specifiche diffeerenziate di qualità.

Il dialogo trae vantaggio da un supporto ad hoc di funzione che siano capaci di garantire i protocolli e i meccanismi per il miglior supporto. La sessione standardizza una serie di componenti per avere a disposizione tutte le funzionalità necessarie secono di requisiti per:

- gestione dell'interazione
- per la sincronizzazione
- gestione delle eccezioni

Nel dialogo tra pari c'è la possibilità di intervenire automaticamente:

- se la trasmissione di un file da due ore si blocca dopo un'ora, il sistema può riprendere la trasmissione dal punto in cui si è interrotta
- se si verificano errori nella comunicazione, si ha un rollback automatico
- se nel trasferimento di molti MB avviene un crash, ricominciando.

I punti di sincronizzazione sono definiti in modo che il sistema possa riprendere da dove si è interrotto.

Ci sono due categorie:

- **punti di sincronizzazione maggiori**: il sistema riprende da dove si è interrotto
- **punti di sincronizzazione minori**: il sistema riprende da dove si è interrotto, ma con un rollback

In caso di recovery si prevedono anche strategie diverse:

- **abbandono**: si da un reset e si riparte da zero
- **ripristino**: si riparte da dove si è interrotto
- **ripristino diretto dall'utente**

Strutturazioe e sincronizzazione avvengono attraverso oggetti astratti detti token intesi come oggetti di controllo che permettono di sincronizzare e controllare il dialogo.

Ci sono vari tipi di token:

- **data token**: token che trasporta dati
- **release token**: token che rilascia il controllo
- **synchronize minor token**: cread un punto di sincronizzazione minore
- **synchronize major token**: crea un punto di sincronizzazione maggiore

### <span style="color:#66cc99">Livello Presentazione</span>

La codifica delle informazioni non è univoca, e ogni pari può usare codifiche diverse. Il compito del livello di presentazione è quello di tradurre i dati in un formato comune, in modo che i dati possano essere interpretati correttamente dal destinatario. Il livello di presentazione si occupa di:

- **Codifica dei dati**
- **Compressione dei dati**
- **Crittografia dei dati**

In caso di problemi di eterogeneità ci sono due soluzioni:

- dotare ogni nodo di tutte le funzioni di conversione
- concordare un formato comune di rappresentazione

Nel caso in cui ci sia una uniformità dei dati (es. JAVA), non c'è bisogno di conversione.

Il livello di presentazione stabilisce come negoziare e definire una base comune per la comunicazione.

Il numero di fasi dipende dal numero di eventi che si sono verificati durante il protocollo. Il **bidding** è l'insieme delle fasi necessarie per raggiungere un accordo e comprende, normalmente, 5 fasi:

- sender fa un broadcast della sua esistenza
- receiver fanno un offerta (bid)
- sender sceglie tra le offerte
- receiver accoglie l'offerta (contract)
- accordo

### <span style="color:#66cc99">Livello Applicazione</span>

È il livello che si interfaccia con l'utente finale della comunicazione. L'obiettivo è l'astrazione: nascondere la complessità dei livelli sottostanti.
OSI adotta un approccio basato sul modello a oggetti per la specifica delle applicazioni:

- uso di template e package per definire gli oggetti
- pura ereditarietà statica tra astrazioni
- oggetti da manipolare come interfacce

[<< Torna all'indice](../0-Indice.md)

[Prossimo argomento >>](./7-Java_RMI_Remote_Method_Invocation.md)
