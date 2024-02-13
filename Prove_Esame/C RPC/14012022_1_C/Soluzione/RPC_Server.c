/* RPC_Server.c */

#include "RPC_xFile.h"
#include <dirent.h>
#include <fcntl.h>
#include <rpc/rpc.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/********************************************************/
int *elimina_occorrenze_1_svc(Filename *input, struct svc_req *rp) {
    static int result = 0;
    char car;
    int nread,fd,occorrenze,temp;
    
    printf("Richiesta eliminazione caratteri numerici da file %s\n", input->name);

    fd=open(input->name,O_RDONLY);
    temp = open("temp.txt",O_WRONLY|O_CREAT,0777);

    if(fd<0 || temp<0){
        perror("Errore apertura file");
        result=-1;
        return(&result);
    }else{
        occorrenze=0;
        while ((nread=read(fd,&car,1))!=0){
            if(nread<0){
                perror("Errore lettura file");
                result = -1;
                return(&result);
            }
            if(car <= '9' && car >= '0'){
                occorrenze++;
            }else{
                nread = write(temp,&car,1);
                if(nread<0){
                    perror("Errore nella scrittura su file");
                    result=-1;
                    return(&result);
                }
            }
        }
        result=occorrenze;
    }

    close(fd);
    close(temp);

    rename("temp.txt",input->name);

    printf("Effettuate %d eliminazioni\n",result);

    return(&result);
}


Output *lista_filetesto_1_svc(Filename *input, struct svc_req *rp) {
    DIR                 *dir;
    struct dirent       *dd;
    int                  i, fd_file, ret, cnt, fNameL;
    char                 fullpath[256],fName[256];
    static Output result;

    printf("Richiesti file di testo nel direttorio %s\n",input->name);
    if ((dir = opendir(input->name)) == NULL) {
        printf("Direttorio non esistente\n");
        result.n_files = -2;
        return (&result);
    }

    // Max 6 files can be returned, so we insert also this control in the
    // while loop condition
    result.n_files = 0;
    while ((dd = readdir(dir)) != NULL && result.n_files < 6) {

        // Create a valid path from the current working directory
        snprintf(fullpath, sizeof(fullpath), "%s/%s", input->name, dd->d_name);
        printf("file %s nella cartella\n",fullpath);

        // Open file
        fd_file = open(fullpath, O_RDONLY);
        if (fd_file < 0) {
            printf("FILE: %s\n", fullpath);
            perror("Open file:");
            result.n_files = -1;
            return (&result);
        }

        //Controllo che il file inizi con il prefisso dato
        strcpy(fName,dd->d_name);
        fNameL = strlen(fName);
        if(fNameL > 4 && fName[fNameL-1]=='t' && fName[fNameL-2]=='x' && fName[fNameL-3]=='t' && fName[fNameL-4]=='.'){
            strcpy(result.files[result.n_files].name,dd->d_name);
            result.n_files++;
        }

        // Clean the path string
        memset(fullpath, 0, 256);
    }

    return (&result);
}