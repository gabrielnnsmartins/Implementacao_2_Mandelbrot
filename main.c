#include<stdlib.h>
#include<stdio.h>
#include<threads.h>

int serial_mandlebot(int coluna, int linha, int max_inter){
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
        if ((zn*zn)+(zy*zy)>4){
            break;
        }

        double zx_novo = ((zx*zx)-(zy*zy))+x;
        double zy_novo = (2*zx*zy)+y;

        zx = zx_novo;
        zy = zy_novo;
        inter+=1;
    }
}
}
}

int main(int argc, char *argv[]){
    return 0;
}
