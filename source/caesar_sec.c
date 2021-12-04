#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


// metodos
void encriptar(int llave, char mensaje[100000]);
void desencriptar(int llave);

// metodo para encriptar el mensaje
void encriptar(int llave, char mensaje[100000]){
    char temp;
    int i;
    FILE *out_file;

     for(i=0; i < 100000; i++) {
        temp = mensaje[i];
        if(temp >= 'a' && temp <= 'z'){
            temp = temp + llave;
            if(temp > 'z'){
                temp = temp - 'z' + 'a' - 1;
            }
            if(temp < 'a'){
                temp = temp - 'a' + 'z' + 1;
            }
            mensaje[i] = temp;
        } else if (temp >= 'A' && temp <= 'Z'){
            temp = temp + llave;
            if(temp > 'Z'){
                temp = temp - 'Z' + 'A' - 1;
            }
            if(temp < 'A'){
                temp = temp - 'A' + 'Z' + 1;
            }
            mensaje[i] = temp;
        }
    }
    
    printf("Mensaje encriptado: %s\n", mensaje);
    out_file = fopen("texto_encriptado.txt", "w");
    fprintf(out_file, "%s", mensaje);
    fclose(out_file);
}

void desencriptar(int llave){
    char *path;
    FILE *crypt_file;
    char mensaje_enc[100000], temp;
    int i;

     // path para el archivo a leer
    path = "texto_encriptado.txt";
    crypt_file = fopen(path, "r");

    //mensaje por si no encuentra le archivo
    if (crypt_file == NULL) {   
        printf("ERROR, no se pudo abrir el archivo txt\n"); 
        exit(-1);
    } 

    fread(mensaje_enc, 100000, 1, crypt_file);

    printf("Mensaje encriptado: %s\n", mensaje_enc);

    for(i=0; i < 100000; i++) {
        temp = mensaje_enc[i];
        if(temp >= 'a' && temp <= 'z'){
            temp = temp - llave;
            if(temp > 'z'){
                temp = temp + 'a' - 'z' - 1;
            }
            if(temp < 'a'){
                temp = temp + 'z' - 'a' + 1;
            }
            mensaje_enc[i] = temp;
        } else if (temp >= 'A' && temp <= 'Z'){
            temp = temp - llave;
            if(temp > 'Z'){
                temp = temp + 'A' - 'Z' - 1;
            }
            if(temp < 'A'){
                temp = temp + 'Z' - 'A' + 1;
            }
            mensaje_enc[i] = temp;
        }
    }
    printf("Mensaje desencriptado: %s\n", mensaje_enc);
}


// caesar encode secuencial
int main(int argc, const char * argv[]) {
    // variables 
    char *path;
    FILE *in_file;
    char mensaje[100000], temp;
    int llave = 3;
    int i;
    int llave2 = 3;

    // path para el archivo a leer
    path = "alphabet.txt";
    in_file = fopen(path, "r");

    //mensaje por si no encuentra le archivo
    if (in_file == NULL) {   
        printf("ERROR, no se pudo abrir el archivo txt\n"); 
        exit(-1);
    } 

    // variable para asignarle nuestro mensaje al file que adjuntamos
    fread(mensaje, 100000, 1, in_file);

    printf("Mensaje original: %s\n", mensaje);

    /*printf("Escriba la llave: ");
    scanf("%d", &llave);*/

    encriptar(llave, mensaje);

    /*printf("Escriba la llave para desencriptar: ");
    scanf("%d", &llave2);*/

    if(llave == llave2){
        desencriptar(llave2);
    } else{
        printf("La llave no coincide\n");
    }
    
    fclose(in_file);
    return 0;
}
