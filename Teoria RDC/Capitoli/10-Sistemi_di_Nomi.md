# <span style="color:#aa66ff">Sistemi di Nomi</span>

## <span style="color:#60c1fb">Introduzione</span>

Spesso ci si trova con molti sistemi di nomi che hanno proprietà diverse. Alcune di queste sono:

- **generalità**: ampia varietà di nomi disponibili e trattabili
- **definizioni multiple della stessa entità**: si può esare tanti nomi per lo stesso oggetto e grazie a questo si possono avere più punti di vista
- **distribuibilità**: si possono usare direttori partizionati e/o replicati
- **user friendliness**: nomi facili per utenti

Un problema nel distribuito è ritrovare le altre entità nel sistema, entità diverse richiedono livelli diversi di nomi, sevono più sistemi di naming e più livelli di nomi nel sistema con contesti di visibilità: quindi più funzioni di trasformazione da nome a entità. I nomi possono essere **nomi esterni** (nomi di utente), **nomi interni** (nomi di sistema, in binario). Tre possibili livelli sono:

- **nome logico esterno**: specifica a quale oggetto si riferisce e denota l'entità
- **indirizzo fisico**: specifica dove si trova l'entità
- **route**: specifica come raggiungere l'entità vedi ([Directory X.500](./10-Sistemi_di_Nomi.md#directory-x.500))

Ci possono essere problemi nell-identificare nomi di gruppo in quanto un nome di gruppo identifica una lista di nomi di entità. Accade la stessa cosa quando si vuole fare un multicast e si vuole inviare un messaggio a più entità.

## <span style="color:#60c1fb">Sistemi di Nomi</span>

Gli spazi dei nomi più usati sono:

- **spazio dei nomi piatto**: unico livello di nomi, con nessuna struttura adatto per pochi utenti e entità
- **spazio dei nomi partizionato**: per gerarchie e contesti (vedi [DNS](./2-Generalità_obiettivi_e_modelli_di_base.md#dns-domain-name-system))
- **spazio dei nomi descrittivo**: con riferimento ad una struttura di oggetti caratterizzati da attributi per identificare l'entità corrispondente (vedi [Directory X.500](./10-Sistemi_di_Nomi.md#directory-x.500))

Ci possono essere probemi nell'identificare nomi di gruppo in quanto un nome di gruppo identifica una lista di nomi di entità. Accade la stessa cosa quando si vuole fare un multicast e si vuole inviare un messaggio a più entità.

## <span style="color:#60c1fb">Componenti di un Sistema di Nomi</span>

In un servizio di nomi i client sono:

- coloro che devono risolvere un nome per potere riferire una risorsa
- le entità risorse che devono essere identificate

Il supporto deve anche considerare:

- comunicazione dei client con il name server
- il name server
- gestione dei nomi
- gestione delle tabelle e il coordinamento

Le comunicazioni dei client con il name server possono essere ottimizzate per le operazioni più frequenti:

- i client propongono la maggior parte del traffico
- le risorse da registrare fanno operazioni più rare e producono traffico più limitato.

nella realizzazione con molteplici name server, il servizio prevede una comunicazione tra loro uasndo messaggi singoli, datagrammi e connessioni di alto livello come RPC.

Il traffico tra i name server deve essere supportato e il coordinamento dei server deve essere minimizzato in termini di tempo e risorse. Allo stesso tempo si vuole garantire:

- in uno spazio piatto: una partizione dello spazio dei nomi per limitare la coordinazione
- in uno spazio partizionato: nomi che sono usati dalla autorità preposta senza coordinamento.

La gestione delle tabelle e il coordinamento creano problemi di consistenza e affidabilità. Sono quindi fondamentali due decisioni:

- distribuzione dei nomi: mantenuti in oggetti che ne hanno la responsabilità con un partizionamento tra i server responsabili
- la risoluzione dei nomi: le richieste dal client fluiscono fino al server che può fare una risposta. Questo processo prevede alcune fasi:

  - trovare l'autorità corretta
  - verificare le autorizzazioni all'operazione
  - eseguire l'operazione

  Ogni nodo specifica i name server noti e tende a limitare le comunicazioni al server. Strategie usuali per limitare i costi sono:

  - caching
  - routing tra i server
  - creazione di contesti o vicinati tra i server
  - propagazione di conoscenza tra i vicini

Nei sistemi come DNS si distribuisce e si risolve il nome in contesto locale e si ricorre ad altri contesti solo se necessario.

## <span style="color:#60c1fb">Directory X.500</span>

È un servizio standard di directory e nomi con relazione partizionata. decentralizzata, disponibile 24/7. È definita come collezione di sistemi aperti che cooperano per un database loggico di infromazioni sugli oggetti del mondo reale. Gli utenti possono legere o modificare l'inforazione o parte di essa se hanno i privilegi necessari-

La base è l'insieme delle informazioni che caratterizzano la struttura di directoru organizzate in un albero logico detto **DIT** (Directory Information Tree) che forma il **DIB** (Directory Information Base). L'organizzazione è basata sui contenuti e sulle ricerce per singole entità o per attributi ritrovando gruppi di nodi.

Ogni entry si trova attraverso diverse notazioni:

- **DN** (Distinguished Name): identifica univocamente l'oggetto all'interno del DIT.
- **RDN** (relative Distinguished Name): definisce univocamente un oggetto all'interno di un contesto.

Le ricerche possono essere fatte in modo globale o contestuale per uno specifico DN ma anche per contenuto dei nomi. Usando i filtri si possono sfruttare condizioni logiche, espressioni eregolari e condizioni aritmetiche sugli attributi. La ricerca sulla directory X.500 avviene attraverso agenti:

- **DUA** (Directory User Agent): interfaccia utente per fare richieste
- **DSA** (Directory System Agent): mantiene le informazioni di contesto
- **DSP** (Directory System Protocol): protocollo di comunicazione tra DUA e DSA
- **DAP** (Directory Access Protocol): protocollo di accesso alla directory

Dopo la connessione si fanno operazioni di lettura, confronto, ricerca, lista delle entità con ricerche ricorsive e iterative.

È anche presente **LDAP** (Lightweight Directory Access Protocol) portocollo limitato, compatibile con internet.
A differenza di un database in una directory:

- si associano attributi diversi anche a singoli oggetti
- gli oggetti sono indipendenti tra di loro e possono essere diversi
- si considera la relazione di contenimento alla base dell'organizzazione
- si possono avere proprietà di accesso differenziate per singoli oggetti
- si ottimizza considerando un numero elevato di letture e poche scritture

## <span style="color:#60c1fb">Protocolli di Directory e Protocolli di Discovery</span>

Considerando che un'entità può avere attributi con lente variazioni o rapide variazioni, le directory sono soluzioni di nomi globali completi e complessi con costo elevato delle operazioni. Per questo esistono le discovery, soluzioni di nomi locali essenziali con funzioni limitate a costo limitato, adatte per variazioni rapide.

Un servizio di directory garantisce:

- replicazione
- sicurezza- gestione multiserver

Alcuni esempi sono **UPnP** (Universal Plug And Play)

I protocolli di discovery sono usati per computazione distrubuita e cooperativa in locale. Ad esempio una unità deve trovarne altre in modo veloce e economico.
Un esempio è JINI, protocollo Java per il discoveri di appliance

[<< Torna all'indice](../0-Indice.md)

[Prossimo argomento >>](./11-TCP\IP_Protocolli_e_Scenari_d'Uso.md)
