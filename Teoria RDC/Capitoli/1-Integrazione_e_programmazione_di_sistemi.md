# <span style="color:#aa66ff">Integrazione e Programmazione di Sistemi</span>

## <span style="color:#60c1fb">Programmazione di Sistema</span>

**Componenti di Sistema**: programmi che consentono di preparare l'ambiente e forniscono i servizi. Eseguiti molte volte e spesso, quindi devono essere ottimizzati.

## <span style="color:#60c1fb">Programmazione a Microservizi</span>

**Microservizi**: programmi piccoli ed efficienti, capaci di interagire in modo dinamico, e che si possano comporre al bisogno, con costi limitati.

Si programma a microservizi per avere componenti che si coordinino tra loro in modo dinamico, veloce e ottimizzato.

## <span style="color:#60c1fb">Architettura delle Soluzioni</span>

Nell'espressione dei programmi ci sono due paradigmi principali:

- **Programmazione Imperativa**: linguaggi tradizionali e a oggetti, le istruzioni specificano interamente l'algoritmo da risolvere. La soluzione trovata è quella a cui giunge l'algoritmo.
- **Programmazione Dichiarativa**: le soluzioni vengono espresse secondo certi requisiti e vincoli specificati dall'utilizzatore, senza arrivare a una specifica soluzione.

Per lo sviluppo di un programma, dopo il design della soluzione, si passa alla codifica per arrivare all'esecuzione.

Per fare ciò si devono stabilire le risorse hardware su cui poter eseguire e attuare una corrispondenza tra parte logica e concreta di architettura.

La scelta dell'architettura è la scelta di **deployment**, in cui si decide come allocare le risorse logiche alle risorse fisiche disponibili.

La divisione è quindi tra:

- **Fase Statica**: analisi, progetto e codifica.
- **Fase Dinamica**: deployment, esecuzione e analisi delle performance.

Nel caso di programmi di sistema che devono eseguire molte volte, il progetto deve mirare a:

1. Semplicità delle strutture dati e degli algoritmi.
2. Minimizzazione del costo della configurazione.
3. Minimizzazione dell'impegno sulle risorse.
4. Minimizzazione dell'overhead.
5. Capacità di interazione con l'utente.
6. Prevenzione errori.
7. Capacità di controllo dell'esecuzione.

## <span style="color:#60c1fb">Semplicità degli Algoritmi e del Codice</span>

Gli algoritmi devono essere semplici:

1. Eliminare controlli ridondanti per ridurre i costi in eccesso.
2. Poco innestamento di procedure e funzioni per ridurre l'uso dello stack.
3. Eliminare le ripetizioni, e.g. utilizzare variabili per mantenere lo stato di operazioni.

Nel caso di programmi che interagiscono con l'utente:

1. Controllare i parametri di invocazione.
2. Verificare gli input dell'utente.
3. Consumare gli stream di input e le risorse in modo corretto.
4. Interagire usando un limitato numero di risorse di sistema.
5. Progettare bene i processi.

[<< Torna all'indice](../0-Indice.md)

[Prossimo argomento >>](./2-Generalità_obiettivi_e_modelli_di_base.md)
