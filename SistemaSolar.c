#include<math.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//funciones a utilizar
int DatosInicial(char* condIni, double** x, double** y, double** vx, double** vy, double** m);

int main(void)
{
    double *x,*y,*vx,*vy,*m;
    int ncuerpos;
    FILE *fcondIni;

    char condIni[]="DatosIniciales";
    
    ncuerpos= DatosInicial(condIni,&x,&y,&vx,&vy,&m);
    printf("%i\n",ncuerpos);
    return 0;
}

int DatosInicial(char* condIni, double** x, double** y, double** vx, double** vy, double** m)
{
    int ncuerpos;
    int i;
    FILE* fcondIni;

    fcondIni=fopen(condIni, "r"); 
    fscanf(fcondIni, "%i\n",&ncuerpos);

    *x=(double*) malloc(ncuerpos*sizeof(double));
    *y=(double*) malloc(ncuerpos*sizeof(double));
    *vx=(double*) malloc(ncuerpos*sizeof(double));
    *vy=(double*) malloc(ncuerpos*sizeof(double));
    *m=(double*) malloc(ncuerpos*sizeof(double));

    for (i=0; i<ncuerpos; i++)
    {
        fscanf(fcondIni, "%lf %lf %lf %lf %lf\n",&(*x)[i], &(*y)[i], &(*vx)[i], &(*vy)[i], &(*m)[i]);
    }

    fclose(fcondIni);


    return ncuerpos;
}