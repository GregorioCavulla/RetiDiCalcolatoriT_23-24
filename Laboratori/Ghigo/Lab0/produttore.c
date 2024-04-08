#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX_STRING_LENGTH 256

// proposta, rendere il produttore un filtro
int main(int argc, char *argv[]) {
    int   fd, readValues, bytes_to_write, written;   // variabili per la gestione del file
    char *file_out; // nome del file
    char  riga[MAX_STRING_LENGTH], buf[MAX_STRING_LENGTH]; // buffer per la riga
    char  read_char; // variabile per la lettura di un carattere
    int   nread;    // numero di byte letti

    // controllo numero argomenti
    if (argc != 2) {
        perror(" numero di argomenti sbagliato\n 1: nome del file\n");
        exit(1);
    }

    file_out = argv[1]; // nome del file
    

    fd = open(file_out, O_WRONLY | O_CREAT | O_TRUNC, 00640);
    if (fd < 0) {
        perror("P0: Impossibile creare/aprire il file");
        exit(2);
    }

    printf("Inserisci la nuova riga, ^D per terminare\n");

     while (gets(riga)!=NULL){ // Fino ad EOF
        printf("Inserisci la nuova riga, ^D per terminare\n");
        /* la gets legge tutta la riga, separatori inclusi, e trasforma il fine
           linea in fine stringa */
        riga[strlen(riga) + 1] = '\0'; // aggiungo 0 binario per permettere una corretta strlen()
        riga[strlen(riga)] = '\n'; // aggiungo il fine linea
        written = write(fd, riga, strlen(riga)); // uso della primitiva
        if (written < 0) {
            perror("P0: errore nella scrittura sul file");
            exit(3);
        }
    }
    close(fd);
}

// done