#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>
#include <omp.h>
#include "mpi.h"

#define CHARS 100000

// caesar decode paralelizado
int main(int argc, char * argv[]) {

    // variables para MPI
    int myid, numprocs;
    char hostname[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    // variables 
    int data = CHARS/numprocs;
    char local[data];
    double start, stop;
    int maxdata = data;
    // aqui es donde cambiamos la llave para encriptar 
    int llave = 3;

    char *path_crypt;
    FILE *crypt_file;
    char mensaje_enc[CHARS], temp1;
    int x;
    if (myid == 0) {
        // path para el archivo a leer
        path_crypt = "texto_encriptado.txt";
        crypt_file = fopen(path_crypt, "r");

        //mensaje por si no encuentra le archivo
        if (crypt_file == NULL) {   
            printf("ERROR, no se pudo abrir el archivo txt\n"); 
            exit(-1);
        } 

        fread(mensaje_enc, CHARS, 1, crypt_file);
    }
    //dividimos el mensaje
    MPI_Scatter(mensaje_enc, data, MPI_CHAR, local, data, MPI_CHAR, 0, MPI_COMM_WORLD);

start = omp_get_wtime();

// paralelizamos el algoritmo para desencriptar el mensaje
#pragma omp parallel for
    for(x=0; x < maxdata; x++) {
        temp1 = local[x];
        if(temp1 >= 'a' && temp1 <= 'z'){
            temp1 = temp1 - llave;
            if(temp1 > 'z'){
                temp1 = temp1 + 'a' - 'z' - 1;
            }
            if(temp1 < 'a'){
                temp1 = temp1 + 'z' - 'a' + 1;
            }
            local[x] = temp1;
        } else if (temp1 >= 'A' && temp1 <= 'Z'){
            temp1 = temp1 - llave;
            if(temp1 > 'Z'){
                temp1 = temp1 + 'A' - 'Z' - 1;
            }
            if(temp1 < 'A'){
                temp1 = temp1 + 'Z' - 'A' + 1;
            }
            local[x] = temp1;
        }
    }
    // recolectamos el mensaje
    MPI_Gather(local, data, MPI_CHAR, mensaje_enc, data, MPI_CHAR, 0, MPI_COMM_WORLD);

    if (myid == 0){
        printf("Mensaje desencriptado: %s\n", mensaje_enc);
        fclose(crypt_file);
        stop = omp_get_wtime();
        printf("Tiempo de ejecución de la sección paralela = %f \n", stop-start);
    }
    
    MPI_Finalize();
    return 0;
}