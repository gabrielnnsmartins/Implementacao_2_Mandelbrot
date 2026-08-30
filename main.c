#include<stdlib.h>
#include<stdio.h>
#include<time.h>

int serial_mandlebot(int coluna, int linha, int max_inter){
    int resultado_matriz[coluna][linha];
    double dx = 3.0 / coluna;
    double dy = 3.0 / linha;
    
    for(int col=0;col<coluna;col++){
        for(int lin=0;lin<linha;lin++){
    
    int inter = 0;
    double x=0.0, y=0.0;
    double zx=0.0, zy=0.0;
    
    x = -2.0 + (col*dx);
    y = 1.5 - (lin*dy);

    while(inter< max_inter){
        if ((zx*zx)+(zy*zy)>4){
            break;
        }

        double zx_novo = ((zx*zx)-(zy*zy))+x;
        double zy_novo = (2*zx*zy)+y;

        zx = zx_novo;
        zy = zy_novo;
        inter+=1;
    }
    resultado_matriz[col][lin] = (inter*255)/max_inter;
}
}
FILE *arquivo = fopen("mandelbrot_gnm_serial.pgm", "w");

if (arquivo!= NULL){
    for(int i=0; i<coluna;i++){
        for(int y=0; y<linha;y++){
            fprintf(arquivo,"%d ", resultado_matriz[i][y]);
        }
        frpintf(arquivo,"\n");
    }
    fclose(arquivo);
}
for(int i=0;i<linha;i++){
    free(resultado_matriz[i]);
}
free(resultado_matriz);
}



int main(int argc, char *argv[]){
    return 0;
}
