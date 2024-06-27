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
- **spazio dei nomi partizione**: con riferimento ad una struttura di oggetti caratterizzati da attributi per identificare l'entità corrispondente

## <span style="color:#60c1fb">Componenti di un Sistema di Nomi</span>

## <span style="color:#60c1fb">Directory X.500</span>

## <span style="color:#60c1fb">Protocolli di Directory e Protocolli di Discovery</span>

[<< Torna all'indice](../0-Indice.md)

[Prossimo argomento >>](./11-TCP\IP_Protocolli_e_Scenari_d'Uso.md)
