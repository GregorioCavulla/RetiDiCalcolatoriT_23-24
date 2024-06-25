# <span style="color:#aa66ff">Programmazione di Rete e Modello C/S con Socket in Java</span>

## <span style="color:#60c1fb">Introduzione</span>

Grazie alle **socket**: end-point locale di un canale di comunicazione bidirezionale, possiamo realizzare comunicazioni fra macchine distinte. In Java, le socket sono implementate tramite la classe `Socket` e `ServerSocket` del package `java.net`.

La comunicazione può avvenire in due modi:

- **TCP** (Transmission Control Protocol): garantisce l'affidabilità della comunicazione, ma è più lento, attraverso le **socket stream**.
- **UDP** (User Datagram Protocol): più veloce, ma non garantisce l'affidabilità della comunicazione, attraverso le **socket datagram**.

Per identificare univocamente un processo si usa la coppia **{IP, porta}** con IP di 32 bit e porta di 16 bit. Sono le socket a legare il processo a un nome globale per ricevere o inviare dati, in modo da non dovere conoscere il PID del processo.

I server possono essere sia sequenziali con o senza connessione, sia paralleli con connessione.

## <span style="color:#60c1fb">Socket Datagram</span>

Permettono a due processi di scambiarsi messaggi senza stabilire una connessione. Esiste un solo tipo di socket datagram sia per client che per server: `public class DatagramSocket` con costruttore `DatagramSocket(InetAddress, int localPort)` in cui si crea una socket UDP e si fa un binding locale a una specifica porta e a uno specifico IP.

I metodi che permettono di inviare e ricevere sono `send(DatagramPacket p)` e `receive(DatagramPacket p)`. Il primo permette di inviare un pacchetto UDP, il secondo di riceverlo.

## <span style="color:#60c1fb">Socket Multicast</span>

Permettono a un server di inviare lo stesso messaggio a più client contemporaneamente se registrati su un indirizzo di gruppo. Si usa la classe `MulticastSocket` con costruttore `MulticastSocket(int port)`. Si può entrare e uscire dal gruppo con i metodi `joinGroup(InetAddress groupAddr)` e `leaveGroup(InetAddress groupAddr)`.

## <span style="color:#60c1fb">Socket Stream</span>

Permettono di avere un canale di comunicazione virtuale creato prima della comunicazione. Questa è bidirezionale, affidabile e con semantica **at-most-once** (il messaggio può essere perso, ma non duplicato). La connessione è definita da una quadrupla {IP, porta locale, IP remoto, porta remota} e dal protocollo TCP.

In Java sono presenti due classi per la creazione di socket stream: `Socket` e `ServerSocket`. La prima è per il client, la seconda per il server.

### <span style="color:#60c1fb">Socket Stream: Client</span>

Per il client il costruttore di `java.net.Socket` crea la socket, la lega a una porta locale e la connette a una porta remota.

```java
Socket s = new Socket(InetAddress.getByName("indirizzo"), porta);
```

### <span style="color:#60c1fb">Socket Stream: Server</span>

Per il server il costruttore di `java.net.ServerSocket` crea la socket, la lega a una porta locale e la mette in ascolto con il metodo bloccante `accept()`.

```java
ServerSocket ss = new ServerSocket(porta);
Socket s = ss.accept();
```

La chiamata di `accept()` blocca il server finché non arriva una richiesta di connessione.

---

Per leggere e scrivere dati si usano i metodi `getInputStream()` e `getOutputStream()` della classe `Socket`, che restituiscono rispettivamente un `InputStream` e un `OutputStream` che possono spedire e ricevere byte.

Si possono usare invece `DataInputStream` e `DataOutputStream` per spedire e ricevere dati di tipo primitivo come `int`, `double`, `boolean`, ecc.

```java
DataInputStream dis = new DataInputStream(s.getInputStream());
DataOutputStream dos = new DataOutputStream(s.getOutputStream());

int i = dis.readInt();
dos.writeInt(i);
```

## <span style="color:#60c1fb">Chiusura e Opzioni per le Socket</span>

Per chiudere una socket si usano i metodi `close()` delle classi `Socket` e `ServerSocket`, che permettono di liberare le risorse e di chiudere la connessione.

Per non chiudere completamente la socket si possono usare i metodi `shutdownInput()` e `shutdownOutput()` per chiudere rispettivamente l'input e l'output.

Per quanto riguarda le socket datagram, esistono i metodi `setSoTimeout(int timeout)` per impostare un timeout dopo il quale l'operazione di lettura o scrittura fallisce, `setReceiveBufferSize(int size)` per impostare la dimensione del buffer di ricezione e `setSendBufferSize(int size)` per impostare la dimensione del buffer di invio. Esistono anche `setSoLinger(boolean on, int linger)` per impostare il comportamento della socket in caso di chiusura e `setReuseAddress(boolean on)` per permettere il riutilizzo dell'indirizzo locale.

[<< Torna all'indice](../0-Indice.md)

[Prossimo argomento >>](./4-Programmazione_di_rete_e_modello_C\S_con_socket_in_C.md)
