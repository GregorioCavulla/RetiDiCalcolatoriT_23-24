# <span style="color:#aa66ff">TCP/IP: Protocolli e Scenari D'Uso</span>

## <span style="color:#60c1fb">Introduzione</span>

Nello standard TCP/IP, il livello 2 è formato dai protocolli:

- **ICMP**: per lo scambio di messaggi di controllo
- **IP**: per lo scambio di datagrammi
- **ARP e RARP**: che permettono l'interazione con il livello fisico e i nomi

Al livello 3:

- **TCP**: per scambiare informazioni tramite flusso di byte bidirezionale con canale virtuale, controllo di flusso, dati afidabili e non duplicati.
- **UDP**: per lo scambio di messaggi end to end senza garanzie o controlli

In **TCP/IP** non sono consentiti broadcast, il costo sarebbe troppo elevato. Sono permessi solo in locale. Un broadcast può essere:

- **limitato**: per tutti gli host della rete locale
- **diretto** : per gli host di una rete specifica

È possibile effettuare multicast sfruttando indirizzi IP di classe D: gli host che sono registrati possono ricevere e mandare messaggi al gruppo di multicast.

## <span style="color:#60c1fb">ARP - Address Resolution Protocol</span>

Una macchina ha sia IP che MAC. Quando questa deve comunicare con un altra, sa l'IP ma non il MAC.
Il MAC potrebbe essere ottenuto mediante un mappaggio estraendolo dall'indirizzo IP ma violerebbe l'indipendenza dei livelli.
È necessario un portocollo dinamico che permetta.
ARP, locale, semplice e efficente, basato su broadcast procede nel seguente modo:

- la macchina A ha l'indirizzi IP della B. A invia un broadcast in cui chiede il MAC di B
- il primo host che riconosce l'IP di B e sa il MAC, lo invia ad A

ARP fa anche uso di caching per non dovere ricorrere tutte le volte a un broadcast

## <span style="color:#60c1fb">RARP - Reverse Address Resolution Protocol</span>

Per risalire all'IP a partire dal MAC. La macchina (ad esempio all'accensione) chiede con un broadcast il suo indirizzo ip.
Modello deprecato e sostituito da DHCP.

## <span style="color:#60c1fb">DHCP - Dynamic Host Confiuration Protocol</span>

Protocollo per l'attribuzione dinamica di indirizzi IP, permette di risparmiare indirizzi assegnandoli al bisogno e in modo dinamico.

Nell'ordine:

1. un client fa un broadcast mandando una DHCPDISCOVER
2. i server gli inviano una DHCPOFFER con parametri di scelta
3. il client sceglie tramite DHCPREQUEST
4. viene confermata la scelta con DHCPACK
5. il client invia il rilascio dell'offerta con DHCPRELEASE o invia DHCPLEASE per mantenere l'IP

## <span style="color:#60c1fb">NAT - Network Address Translation</span>

NAT è un protocollo usato per traslare gli indirizzi IP di una rete privata in indirizzi IP globali in modo da evitare di attribuire un indirizzo IP pubblico a ciascun host in una rete privata. Ogni rete privata ha un solo IP pubblico che la identifica nella rete internet,

Grazie alla tabella NAT presente nel router è possibile traslare l'indirizzo IP privato del singolo host in un indirizzo pubblico.

## <span style="color:#60c1fb">ICMP - Internet Control Message Protocol</span>

È un protocollo di festione e controllo degli indirizzi IP che serve a migliorare la qualità best-effort.
Consente di inviare messaggi di controllo o di errore al nodo sorgente del messaggio.
Usato anche per coordinare le entità di livello IP, per rendere note condizioni anomale a chi ha mandato datagrammi usando IP. Soltanto il nodo sorgente può prevedere a correggere.

I messaggi ICMP contengono un header e i primi 64 bit del datagramma che ha causato il problema. L'header è formato da:

- type: specifica il formato del messaggio
- code: fornisce un'ulteriore qualificazione del messaggio
- checksum: per controllare la correttezza del messaggio

## <span style="color:#60c1fb">UDP - User Datagram Protocol</span>

Protocollo di trasporto best-effort e basso costo.
Per distinguere i processi in esecuzione sullo stesso nodo si utilizza un nome formato da IP e porta.

UDP è unrelaiable e connectionless: messaggi possono essere persi, duplicati, ritardati o consegnati fuori orfine.
I messaggi UDP sonon composti da header e area dati, con l'header che contiene prote, lunghezza e checksum. Il messaggio è interamente contenuto nell'area dati di IP.

In UDP e TCP alcuni numeri di porta sono noti detti well known port, altri sono assegnati dinamicamente.

## <span style="color:#60c1fb">ARQ - Automatic Repeat Request and Continuos Requests</span>

**ARQ** (Automatic Repeat Request) è una strategia di controllo di errore che rivela un errore senza correggerlo. I pacchetti corrotti vengono scartati e viene richiesta la loro ritrasmissione.

Per riconoscere i messaggi corrotti è necessario che vengano inizialmente codificati da un codificatore e fopo la trasmissione un decodificatore li legge. 3 modi per farlo:

- **Stop and Wait**: il mittente invia un pacchetto e aspetta l'ack del destinatario
- **Go Back N**: il mittente ha a disposizione un buffer dove memorizza i pacchetti da spedire, se non riceve l'ack del destinatario, ripete la trasmissione
- **Selective Repeat**: anche il destinatario ha un buffer, se i messaggi sono ricevuti correttamente vengono svuotati entrambi i buffer

Per evitare problemi di sincronia si utilizza il protocollo **Continuous Request**: si manda un certo numero di messaggi, pari alla dimensione del buffer, in modo ripetuto. Il mittente mantiene un bffer per i messaggi e si blocca quando è pieno, quando arriva un ACK il messaggio è tolto dal buffer.

In caso di errore ci sono 2 strategie:

- **Selective Retransmission**: si ritrasmette solo il pacchetto corrotto
- **Go Back N**: si ritrasmettono tutti i pacchetti a partire da quello corrotto

## <span style="color:#60c1fb">TCP - Transmission Control Protocol</span>

Protocollo per trasmissione di dati con connessione end to end tra processi di nodi distinti. Un end point è definito dalla coppia {IP, porta}, mentre una connessione TCP è definita dalla quadrupla {IP sorgente, porta sorgente, IP destinazione, porta destinazione}.
Possono esserci èiù connessioni che utilizzano la stessa porta a patto che cambi uno degli altri parametri.

Il servizio di trasmissione TCP è uno stream full duplex affidabile, orientato alla connessione, con controllo di flusso e controllo di congestione.

Nell'header TCP sono presenti i seguenti campi:

- porte sorgente e destinazione
- numero di sequenza
- numero di ack
- lunghezza dell'header, della finestra di ricezione e il code bit
- un puntatore urgente e il checksum

Con eventuali opzioni aggiuntive:

- **URG**: se il puntatore urgente è attivo
- **ACK**: se il numero di ack è valido
- **PUSH**: se il mittente deve inviare i dati al destinatario
- **RST**: resetta la connessione
- **SYN**: sincronizza i numeri di sequenza
- **FIN**: chiude la connessione

TCP tende a spezzare i messaggi in segmenti di dimensione variabile, per evitare grosso overhead (nel caso di messaggi troppo corti), ne una frammentizzazione a livello IP (nel caso di messaggi troppo lunghi).

Si usa Countinuos request per la trasmissione dei dati, con un buffer di invio e un buffer di ricezione. L'arrivo di un ACK implica che sono arrivati anche i messaggi precedenti.

Viene usato un buffer detto sliding window per il controllo di flusso, che permette di inviare un certo numero di messaggi senza attendere l'ACK.

### <span style="color:#66cc99">TCP: Fase Iniziale</span>

Per stabilire la connessione, il mittente attua il **Three Way Handshake** che avviene nel seguente modo:

1. A invia a B un pacchetto SYN richiedendo la connessione
2. B risponde con un pacchetto SYN+ACK
3. A risponde con un pacchetto ACK

Queste tre fasi sono fondamentali per ottenere la semantica at most once. Serve per stabilire se entrambi i nodi sono disponibili e pronti a comunicare.

Sempre in questa fase vengono scambiati i numeri di porta disponibili, il tempo di trasmissione, di risposta la dimensione del buffer di ricezione e la dimensione della finestra di trasmissione.

### <span style="color:#66cc99">TCP: Fase di Comunicazione</span>

A regime si fanno continui aggiornamenti dei valori in base alla situazione corrente rilevata.
Dopo il calcolo iniziale del **timeout** si aggiorna in base al tempo di trasmissione e di risposta. Il timeout principale è la base di molti parametri.

Il **controllo di flusso** è basato su sliding windows e dimensione preferenziale dei segmenti da inviare.
Per quanto riguarda la sliding window:

- la dimensione viene inviata per ogni segmento
- se è 0 il mittente si blocca
- ogni pari comunica all'altro la propria situazione con la finestra

Per quanto riguarda la dimensione dei segmenti:

- i dati non vengono inviati fin quando non si raggiunge la dimensione preferenziale
- l'algoritmo che evita l'invio di messaggi corti è il Nagle's Algorithm

Il **controllo di congestione** è basato su un algoritmo di controllo di flusso che si basa su un meccanismo di feedback. Il mittente invia un certo numero di messaggi e si blocca quando la finestra è piena. Quando arriva l'ACK il mittente riprende a inviare.

Per il recovery il mittente, in modo locale, dimezza la finestra di invio e raddoppia il timeout, algoritmo detto **Slow Start**. Utilizzato anche per evitare una congestione iniziale.

Oltre allo slow start esiste anche la politica di **congestion avoidance**: in caso di presunta congestione il mittente dimezza la finestra di invio e la incrementa di 1 per ogni ACK ricevuto.

Alcune trategie tipiche in TCP sono:

- **ricalcolo del timeout**: in base al tempo di trasmissione e di risposta
- **exponential backoff**: in caso di errore si raddoppia il timeout
- **silly window syndrome**: per evitare che il mittente invii messaggi troppo corti
- **limiti al time wait**: per limitare la durata delle risorse per la connessione
- **long fat pipes**: per mantener piene le pipe a banda elevata

### <span style="color:#66cc99">TCP: Fase Finale</span>

La chiusura di una connessione TCP avviene in 4 fasi, è monodirezionale nel verso di output, ovvero definita per un solo verso.

Se A chiude nel suo versio di uscita TCP la chiude solo da A a B (su questo canale possono comunque passare ACK).
Nella chiusura A:

- invia un pacchetto FIN
- B risponde con un ACK
- al termine del traffico da B a A, B invia un pacchetto FIN
- A risponde con un ACK e chiude la connessione

[<< Torna all'indice](../0-Indice.md)
