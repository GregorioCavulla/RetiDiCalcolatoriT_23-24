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

## <span style="color:#60c1fb">ICMP - Internet Control Message Protocol</span>

## <span style="color:#60c1fb">UDP - User Datagram Protocol</span>

## <span style="color:#60c1fb">ARQ - Automatic Repeat Request and Continuos Requests</span>

## <span style="color:#60c1fb">TCP - Transmission Control Protocol</span>

### <span style="color:#66cc99">TCP: Fase Iniziale</span>

### <span style="color:#66cc99">TCP: Fase di Comunicazione</span>

### <span style="color:#66cc99">TCP: Fase Finale</span>

[<< Torna all'indice](../0-Indice.md)
