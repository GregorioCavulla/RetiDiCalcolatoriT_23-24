/* Server.c 
 * eliminare tutti i numeri all'interno di un file
 * ricevere la lista dei file di testo di un direttorio i cui nomi contengono almeno uno
 * specificato numero di occorrenze di un carattere indicato
*/

#include "RPC_xFile.h"
#include <dirent.h>
#include <fcntl.h>
#include <rpc/rpc.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define NAME_LENGTH 64

/********************************************************/
int *elimina_occorrenze_1_svc(FileName *input, struct svc_req *rp) {
    static int result=-1;
    int fd_file, nread, fd_temp;
    char fileNameTemp[NAME_LENGTH];

    printf("Richiesto file %s \n", input->name);

    fd_file = open(input->name, O_RDONLY);
    if (fd_file < 0) {
        printf("File inesistente\n");
        return (&result);
    } else {
        fileNameTemp[0]='\0';
        strcat(fileNameTemp,input->name);
        strcat(fileNameTemp,"_temp");
        if((fd_temp = open(fileNameTemp, O_CREAT | O_WRONLY, 0777))<0){
            perror("Errore apertura file temp");ù
            return (&result);
        } else{
            int nread;
            char car;
            result=0;
            while (nread = read(fd_file, &car, 1) > 0) {
                if (isdigit(car)) {
                    write(fd_temp,&car,1);
                }else {
                    result++;
                }
            }
        }

        if(result>=0){
            printf("Nel file %s sono stati eliminati %d caratteri numerici\n",input->name,result);
        }   

        close(fd_file);
        close(fd_temp);

        rename(fileNameTemp,input->name);

        return (&result); 
    }
}

OutputFileList *lista_file_carattere_1_svc(Input *input, struct svc_req *rp) {
    DIR                 *dir;
    struct dirent       *dd;
    int                  occorrenze,fileNameL;
    char                 fileName[256];
    static OutputFileList result;

    printf("Richiesto direttorio %s\n", input->direttorio);
    if ((dir = opendir(input->direttorio)) == NULL) {
        result.nFiles = -1;
        return (&result);
    }

    // Max 6 files can be returned, so we insert also this control in the
    // while loop condition
    result.nFiles = 0;
    while ((dd = readdir(dir)) != NULL && result.nFiles < 6) {

        fileName = strcpy(fileName dd->d_name);

        fileNameL = strlen(fileName);

        if(fileNameL > 4 && fileName[fileNameL-1]=='t' && fileName[fileNameL-2]=='x' && fileName[fileNameL-3]=='t' && fileName[fileNameL-4]=='.'){
            
            occorrenze=0;

            for(int i=0;i<fileNameL;i++){
                if(fileName[i]==input->carattere){
                    occorrenze++;

                    if(occorrenze == input->occorrenze){
                        strcpy(result.files[result.nFiles++].name, fileName);
                        break;
                    }
                }
            }
        }
    }

    closedir(dir);
    return (&result);
}