#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<time.h>
#include<omp.h>

typedef struct{
    int id;
    int num_threads;
    int linha;
    int coluna;
    int max_interacoes;
    int **resultado_matriz;
}dados_thread;

void serial_mandlebot(int linha, int coluna, int max_inter){
    int **resultado_matriz = malloc(linha * sizeof(int *));
        for(int i=0;i<linha;i++){
           resultado_matriz=malloc(coluna*sizeof(int));
        }
        
    double dx = 3.0 / coluna;
    double dy = 3.0 / linha;
    
    for(int lin=0;lin<linha;lin++){
        for(int col=0;col<coluna;col++){
    
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
    resultado_matriz[lin][col] = (inter*255)/max_inter;
}
}
FILE *arquivo = fopen("mandelbrot_gnm_serial.pgm", "w");

if (arquivo!= NULL){
    for(int i=0; i<coluna;i++){
        for(int y=0; y<linha;y++){
            fprintf(arquivo,"%d ", resultado_matriz[i][y]);
        }
        fprintf(arquivo,"\n");
    }
    fclose(arquivo);
}
for(int i=0;i<linha;i++){
    free(resultado_matriz[i]);
}
free(resultado_matriz);
}

void mandlebot_dividido_em_pthreads(void *arg){
    dados_thread *dado = (dados_thread *)arg;

    int coluna = dado->coluna;
    int linha = dado->linha;
    int max_interacoes = dado->max_interacoes;
    int **resultado_matriz = dado->resultado_matriz;

    double dx = 3.0/coluna;
    double dy = 3.0/linha;

    for(int lin= dado->id;lin<linha;lin+=dado->num_threads){
        for(int col=0;col<coluna;col++){

        int inter = 0;
        double x=0.0, y=0.0;
        double zx=0.0, zy=0.0;
        x = -2.0 + (col*dx);
        y = 1.5 - (lin*dy);

        while(inter< max_interacoes){
        if ((zx*zx)+(zy*zy)>4){
                break;
        }

        double zx_novo = ((zx*zx)-(zy*zy))+x;
        double zy_novo = (2*zx*zy)+y;

        zx = zx_novo;
        zy = zy_novo;
        inter+=1;
    }
        resultado_matriz[lin][col] = (inter*255)/max_interacoes;
        }
    }
}

void pthreads_mandlebot(int coluna, int linha, int max_inter, int num_threads){
        int **resultado_matriz = malloc(linha * sizeof(int *));
        for(int i=0;i<linha;i++){
           resultado_matriz=malloc(coluna*sizeof(int));
        }

        pthread_t threads[num_threads];
        dados_thread dado_t[num_threads];

        for (int i=0; i<num_threads;i++){
            dado_t[i].id = i;
            dado_t[i].num_threads = num_threads;
            dado_t[i].linha = linha;
            dado_t[i].coluna = coluna;
            dado_t[i].max_interacoes = max_inter;
            dado_t[i].resultado_matriz = resultado_matriz;

            pthread_create(&threads[i], NULL,mandlebot_dividido_em_pthreads,&dado_t[i]);
        }

        for(int i=0; i<num_threads;i++){
            pthread_join(&threads[i], NULL);
        }
        
        FILE *arquivo = fopen("mandelbrot_gnm_pthreads.pgm", "w");

    if (arquivo!= NULL){
        for(int i=0; i<linha;i++){
            for(int y=0; y<coluna;y++){
                fprintf(arquivo,"%d ", resultado_matriz[i][y]);
        }
            fprintf(arquivo,"\n");
        }
    fclose(arquivo);
    }
    for(int i=0;i<linha;i++){
        free(resultado_matriz[i]);
    }
    free(resultado_matriz);
}

void openmp_mandlebot(int coluna, int linha, int max_inter, int num_threads){
     int **resultado_matriz = malloc(linha * sizeof(int *));
        for(int i=0;i<linha;i++){
           resultado_matriz=malloc(coluna*sizeof(int));
        }
    
    double dx = 3.0/coluna;
    double dy = 3.0/linha;

    #pragma omp parallel for num_threads(num_threads)

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

    resultado_matriz[lin][col] = (inter*255)/max_interacoes;
    }
    }

    FILE *arquivo = fopen("mandelbrot_gnm_openmp.pgm", "w");

    if (arquivo!= NULL){
        for(int i=0; i<linha;i++){
            for(int y=0; y<coluna;y++){
                fprintf(arquivo,"%d ", resultado_matriz[i][y]);
        }
            fprintf(arquivo,"\n");
        }
        fclose(arquivo);

        for (int i=0; i<linha;i++){
            free(resultado_matriz[i]);
        }
        free(resultado_matriz);
}
}




int main(int argc, char *argv[]){
    int altura = atoi(argv[0]);
    int largura = atoi(argv[1]);
    int max_interacoes = atoi(argv[2]);
    int num_threads = atoi(argv[3]);

    double *tempos = malloc(4 * sizeof(int));

    clock_t inicio_serial = clock();
    serial_mandlebot(altura,largura,max_interacoes);
    clock_t fim_serial = clock();

    double tempo_serial = (double) (fim_serial - inicio_serial) / CLOCKS_PER_SEC;
    tempos[0] = tempo_serial;

    clock_t inicio_pthreads1 = clock();
    pthreads_mandlebot(largura,altura,max_interacoes,num_threads);
    clock_t fim_pthreads1 = clock();

    double tempo_pthreads1 = (double) (fim_serial - inicio_serial)/CLOCKS_PER_SEC;

    clock_t inicio_openmp = clock();
    openmp_mandlebot(largura, altura, max_interacoes, num_threads);
    clock_t fim_openmp = clock();

    double tempo_openmp = (double) (fim_openmp - inicio_openmp)/CLOCKS_PER_SEC;



    return 0;
}
