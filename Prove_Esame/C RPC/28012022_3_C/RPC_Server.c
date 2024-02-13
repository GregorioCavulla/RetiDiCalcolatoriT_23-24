/* contaServer.c */

#include "servizio.h"
#include <dirent.h>
#include <fcntl.h>
#include <rpc/rpc.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/********************************************************/
int *conta_occorrenze_linea_1_svc(Input1 *input, struct svc_req *rp) {
    static int result = 0;
    char buff[1024],linea[255];
    int count,nread,fd,pos,occorrenze;
    
    printf("Richiesto file %s e linea %s\n", input->nome,input->linea);

    fd=open(input->nome,O_RDONLY);
    if(fd<0){
        result=-1;
    }else{
        count=0;
        occorrenze=0;
        while ((nread=read(fd,buff,sizeof(buff)))!=0){
            for(int i=0;i<nread;i++){
                if(buff[i]=='\n'){
                    linea[count] = '\0';
                    count=0;
                    if(strcmp(linea,input->linea)==0){
                        occorrenze++;
                    }
                }else{
                    linea[count] = buff[i];
                    count++;
                }
            }
        }
        linea[count]='\0';
        //controllo che anche l'ultima linea sia uguale alla linea richiesta
        if(strcmp(linea,input->linea)==0){
            occorrenze++;
        }
        result=occorrenze;
    }

    close(fd);
    return (&result);
}


Output *lista_file_prefisso_1_svc(Input2 *input, struct svc_req *rp) {
    DIR                 *dir;
    struct dirent       *dd;
    int                  i, fd_file, ret, cnt;
    char                 fullpath[256];
    static Output result;

    printf("Richiesti file nel direttorio %s con prefisso '%s' \n", input->direttorio,input->prefix);
    if ((dir = opendir(input->direttorio)) == NULL) {
        result.n_files = -1;
        return (&result);
    }

    // Max 6 files can be returned, so we insert also this control in the
    // while loop condition
    result.n_files = 0;
    while ((dd = readdir(dir)) != NULL && result.n_files < 6) {

        // Create a valid path from the current working directory
        snprintf(fullpath, sizeof(fullpath), "%s/%s", input->direttorio, dd->d_name);
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
        int prefLength = strlen(input->prefix);
        if(strncmp(dd->d_name,input->prefix,prefLength)==0){
            strcpy(result.files[result.n_files].name,dd->d_name);
            result.n_files++;
        }

        // Clean the path string
        memset(fullpath, 0, 256);
    }

    return (&result);
}