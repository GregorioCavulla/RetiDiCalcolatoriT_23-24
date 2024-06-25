# <span style="color:#aa66ff">Java RMI - Remote Method Invocation</span>

## <span style="color:#60c1fb">Introduzione</span>

RMI introfuce la possibilità di richiedere l'esecuzione di metodi remoti in Java integrando il tutto in un paradigma orientato agli oggetti. L'obiettivo è quello di permettere la comunicazione tra oggetti distribuiti in modo trasparente e semplice.

RMI è un insieme di strumenti, politiche e protocolli che permettono di invocare metodi su oggetti remoti. L'invocazione di metodi remoti è simile all'invocazione di metodi locali, ma con la differenza che i metodi remoti possono essere invocati da un'altra JVM.

Ogni interazione in RMI è sempre sincrona e bloccante, e il client attende sempre la risposta dal server.

## <span style="color:#60c1fb">Architettura RMI</span>

In Java non sono direttamente possibili i riferimenti remoti, RMI permette di costruirli tramite due proxy: lo **stub** e lo **skeleton**.

Usando il pattern proxy si nasconde al livello applicativo la complessità della comunicazione remota.

RMI utilizza una variabile interfaccia che dinamicamente può contenere un riferimento a una istanza di una classe qualunque che implementa l'interfaccia stessa.

I livelli presenti nell'architettura RMI sono:

- **stub**: rappresenta l'oggetto remoto sul client
- **skeleton**: riceve le invocazioni fatte sullo stub e le realizza chiamando l'oggetto sul server
- **RRL**: Remote Reference Layer, permette di creare riferimenti remoti
- **Transport Layer**: si occupa della comunicazione tra client.
- **registry**: servizio di nomi che consente al server di pubblicare i propri oggetti remoti e al client di ottenere i riferimenti a tali oggetti.

I due livelli RRL e TL sono parte della JVM. Un oggetto remoto consiste di:

- Un oggetto i cui metodi sono invocabili da un'altra JVM, potenzialmente in esecuzione su un'altra macchina
- Un oggetto descritto tramite una o più interfacce remote che dichiarano i metodi invocabili.

Il client invoca un metodo remoto come se fosse un metodo locale, ma in realtà il metodo viene invocato su un oggetto remoto, non è sicuro il successo della chiamata, anche per la semantica at-most-once di TCP.

## <span style="color:#60c1fb">Interfacce e Implementazione</span>

In RMI si fa distinzione tra:

- **definizione** del comportamento: tramite interfacce
  - ogni interfaccia deve estendere `java.rmi.Remote`
  - ogni metodo deve propagare `java.rmi.RemoteException`
- **implementazione** del comportamento: tramite classi
  - ogni classe deve implementare l'interfaccia
  - ogni classe deve estendere `java.rmi.server.UnicastRemoteObject`

I componenti remoti sono riferiti tramite variabili interfaccia, possono contenere solo istanze di classi che implementano l'interfaccia.

Per utilizzare RMI è necessario:

1. definire l'interfaccia remota e implementarla in una classe
2. compilare intefaccia e classe
3. generare lo stub e lo skeleton con `rmic -vcompat nomeClasse`
4. attivare il registry con `rmiregistry` e registrare il servizio
5. avviare il server
6. avviare il client con il nome del servizio ricevuto dal registry

Ciascun metodo ha un solo risutato di uscita, uno o più parametri di ingresso e può lanciare eccezioni.

Il registry, processo in esecuzione sull'host del server, è un servizio di nomi che consente al server di pubblicare i propri oggetti remoti e al client di ottenere i riferimenti a tali oggetti.

## <span style="color:#60c1fb">RMI Registry</span>

Il registry è un servizio di nomi che consente al server di pubblicare i propri oggetti remoti e al client di ottenere i riferimenti a tali oggetti.

Contiene un insieme di coppie {nome, riferimento} del tipo "//nomehost:porta/servizio - ip" e permette di registrare e ottenere riferimenti a oggetti remoti.

Il registry è un processo in esecuzione sull'host del server, che si avvia con il comando `rmiregistry` e si può specificare la porta con `rmiregistry porta`, di default è 1099.

Si può implementare un registry personalizzato estendendo `java.rmi.server.UnicastRemoteObject` e implementando l'interfaccia `java.rmi.registry.Registry`.

## <span style="color:#60c1fb">Stub e Skeleton</span>

Stub e Skeleton sono due proxy che permettono di nascondere la complessità della comunicazione remota.

La comunicazione avviene:

- il client invoca un metodo sullo stub
- lo stub serializza i parametri e li invia allo skeleton
- lo skeleton deserializza i parametri e invoca il metodo sull'oggetto remoto
- l'oggetto remoto esegue il metodo e restituisce il risultato allo skeleton
- lo skeleton serializza il risultato e lo invia allo stub
- lo stub deserializza il risultato e lo restituisce al client

## <span style="color:#60c1fb">Serializzazione e Passaggio dei Parametri</span>

In sistemi RPC i parametri di ingresso e uscita subiscono due trasformazioni:

- **marshalling**: trasformazione dei parametri in un formato trasmissibile
- **unmarshalling**: trasformazione dei parametri in un formato utilizzabile

In Java, quindi in RMI, si ha un BYTECODE uniforme quindi non c'è bisogn di marshalling e unmarshalling.

Si hanno:

- **serializzazione**: trasformazione di un oggetto in un flusso di byte
- **deserializzazione**: trasformazione di un flusso di byte in un oggetto

Stub e Skeleton si occupano di serializzare e deserializzare i parametri:

- i tipi primitivi sono trasferiti direttamente per valore sia nel caso locale che remoto
- gli oggetti sono trasferiti per riferimento nel caso locale e per valore (serializzando) nel caso remoto

## <span style="color:#60c1fb">Livello di Trasporto: Concorrenza e Comunicazione</span>

Per quanto riguarda la concorrenza si possono realizzare diverse implementazioni che devono prevedere un server parallelo thread-safe.
Java RMI utilizza i thread e ne crea uno per ogni richiesta.

Nel caso della comunicazione l'unico vincolo è il buon utilizzo delle risorse: se esiste già una connessione tra due JVM si cerca di riutilizzarla.

## <span style="color:#60c1fb">Distribuzione delle Classi (Deployment) e Class Loading</span>

In REM è necessario che siano disponibili a runtime gli opportuni file .class nelle località che lo richiedono

Il server deve poter accedere a:

- Interfacce che definiscono il servizio a compile-time
- implementazioni del servizio a compile-time
- stub e skeleton a runtime
- altre classi necessarie a runtime e compile-time

Il client deve poter accedere a:

- interfacce che definiscono il servizio a compile-time
- stub delle classi di implementazione del servizio a runtime
- classi necessarie a runtime e compile-time

Java definisce un ClassLoader per ogni JVM, che carica le classi in base a un albero di classpath.

Il ClassLoader di RMI, detto **RMI ClassLoader**, è un ClassLoader che esegue due operazioni fondamentali:

- estrae il campo codebase dal riferimento dell'oggetto remoto
- usa i codebase per caricare le classi necessarie dalla locazione remota.

## <span style="color:#60c1fb">Sicurezza in RMI</span>

In ogni JVM e per ogni ClassLoader esiste un SecurityManager che controlla le operazioni che possono essere eseguite.
Sia il client che il server devono essere lanciati specificando il file di policy che può specificare:

- il range di porte in cui client e server possono comunicare
- consentire di prelevare il codice da un server http
- consentire di accedere a file locali

Un possibile problema del registry è che accedento è possibile ridirigere il client a un server malevolo, per questo è possibile usare un registry sicuro dove i metodi bind e rebind sono invocabili solo dall'host su cui è in esecuzione il registry.

Per quanto riguarda il bootstrap (ottenere il riferimento all'oggetto remoto) Java mette a disposizione la classe Naming che realizza metodi statici per effettuare le operazioni di registrazione, deregistrazione e lookup.

[<< Torna all'indice](../0-Indice.md)

[Prossimo argomento >>](./8-Sistemi_RPC_Remote_Procedure_Call.md)
