#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX_STRING_LENGTH 256
// consumatore.c e' un filtro
int main(int argc, char *argv[]) {
  char *file_in, *prefix;
  char err[MAX_STRING_LENGTH], buffer[MAX_STRING_LENGTH];

  int ret, found, nread, i, fd;
  char read_char;
  // controllo numero argomenti
  // se invoco con 2 argomenti utilizzero stdin come file di input
  // se invoco con 3 argomenti utilizzero il file passato come argomento
  if (argc < 2 || argc > 3) {
    printf("Error:%s prefix < filename \n OR \n %s prefix filename\n", argv[0],
           argv[0]);
    return EXIT_FAILURE;
  } else if (argc == 2) {
    printf("Invocation with only prefix\n");
    prefix = argv[1];
    // 0 Standard input (stdin)
    fd = 0;
  } else if (argc == 3) {
    printf("Invocation with prefix and file name\n");
    prefix = argv[1];
    file_in = argv[2];

    fd = open(file_in, O_RDONLY);
    if (fd < 0) {
      perror("P0: Impossibile aprire il file");
      return EXIT_FAILURE;
    }
  }
  /*
      file_in = argv[1];
      fd = open(file_in, O_RDONLY);
      if (fd < 0) {
          perror("P0: Impossibile aprire il file.");
          exit(2);
      }
  */

  found = 0;

  while ((nread = read(fd, &read_char, sizeof(char)))) /* Fino ad EOF*/ {
    if (nread < 0) {
      printf("(PID %d) impossibile leggere dal file %s", getpid(), file_in);
      perror("Errore in lettura");
      close(fd);
      exit(3);
      return EXIT_FAILURE;
    }

    // leggo il carattere e controllo se è presente nel prefix
    for (int i = 0; i < strlen(prefix); i++) {
      if (read_char == prefix[i]) {
        found = 1;
      }
    }

    if (!found) {
      printf("%c", read_char);
    }

    found = 0;
  }
  close(fd);
  return EXIT_SUCCESS;
}
