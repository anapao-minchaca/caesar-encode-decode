#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>
#include <omp.h>
#include "mpi.h"

#define CHARS 100000

// caesar encode paralelizado
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
 
    char *path;
    FILE *in_file;
    char mensaje[CHARS];
    char temp;
    int i;
    FILE *out_file;

    if (myid == 0){
        // path para el archivo a leer
        path = "texto_largo.txt";
        in_file = fopen(path, "r");

        //mensaje por si no encuentra le archivo
        if (in_file == NULL) {   
            printf("ERROR, no se pudo abrir el archivo txt\n"); 
            exit(-1);
        } 

        // variable para asignarle nuestro mensaje al file que adjuntamos
        fread(mensaje, CHARS, 1, in_file);
    }
    // dividimos el mensaje
    MPI_Scatter(mensaje, data, MPI_CHAR, local, data, MPI_CHAR, 0, MPI_COMM_WORLD);
    
    printf("Mensaje original: %s\n", mensaje);

start = omp_get_wtime();

// paralelizamos el algoritmo de encriptado
#pragma omp parallel for
     for(i=0; i < maxdata; i++) {
        temp = local[i];
        if(temp >= 'a' && temp <= 'z'){
            temp = temp + llave;
            if(temp > 'z'){
                temp = temp - 'z' + 'a' - 1;
            }
            if(temp < 'a'){
                temp = temp - 'a' + 'z' + 1;
            }
            local[i] = temp;
        } else if (temp >= 'A' && temp <= 'Z'){
            temp = temp + llave;
            if(temp > 'Z'){
                temp = temp - 'Z' + 'A' - 1;
            }
            if(temp < 'A'){
                temp = temp - 'A' + 'Z' + 1;
            }
            local[i] = temp;
        }
    }

    // recolectamos el mensaje
    MPI_Gather(local, data, MPI_CHAR, mensaje, data, MPI_CHAR, 0, MPI_COMM_WORLD);

    if (myid == 0) {
        printf("Mensaje encriptado: %s\n", mensaje);
        out_file = fopen("texto_encriptado.txt", "w");
        fprintf(out_file, "%s", mensaje);
        fclose(out_file);
        stop = omp_get_wtime();
        printf("Tiempo de ejecución de la sección paralela = %f \n", stop-start);
    }
    
    MPI_Finalize();
    return 0;
}