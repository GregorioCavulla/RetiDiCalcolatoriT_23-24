#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX_STRING_LENGTH 256

// consumatore.c e' un filtro
int main(int argc, char *argv[]) {

    char *file_in, *regex_char, read_char, buf[MAX_STRING_LENGTH];
    int   nread, fd;

    // controllo numero argomenti
    if (argc != 3) {
        perror(" numero di argomenti sbagliato, usage: ./consumatore <file di input> <caratteri da rimuovere>");
        exit(1);
    }

    file_in = argv[1]; // nome del file
    regex_char = argv[2]; // caratteri da rimuovere

    fd = open(file_in, O_RDONLY); // apertura file in lettura
    if (fd < 0) {
        perror("P0: Impossibile aprire il file.");
        exit(2);
    }

    // lettura del file
    while (nread = read(fd, &read_char, sizeof(char))) /* Fino ad EOF*/ {
        if (nread > 0){ // se ho letto qualcosa
            //se il carattere letto è nel set di caratteri da rimuovere, non lo stampo
            if (strchr(regex_char, read_char) == NULL){
            putchar(read_char);
            }
        }
        else {
            printf("(PID %d) impossibile leggere dal file %s", getpid(), file_in);
            perror("Errore in lettura");
            close(fd);
            exit(3);
        }
    }
    close(fd);
}

// done
