#include<math.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//funciones a utilizar
int DatosInicial(char* condIni, double** x, double** y, double** vx, double** vy, double** m);
void CalcAceleracion(double* x, double* y, double* ax, double* ay, double* m, int ncuerpos);
void CalculaFuerza(double x1, double x2, double y1, double y2, double m1, double m2, double* fuerzax, double* fuerzay);

int main(void)
{
    double *x,*y,*vx,*vy,*m;
    double *ax, *ay;
    int ncuerpos;
    double h, halfh, GuardaMedida;
    int PasosxMedida;

    char condIni[]="DatosIniciales";

    //Leo los datos del fichero y descubro cuántos cuerpos forman mi sistema
    ncuerpos= DatosInicial(condIni,&x,&y,&vx,&vy,&m);

    //Una vez conocido el número de cuerpos, asigno memoria a la aceleración 
    ax=(double*)malloc(ncuerpos*sizeof(double));
    ay=(double*)malloc(ncuerpos*sizeof(double));

    //Defino el paso y cada cuánto quiero guardar la medida
    h=0.001;
    GuardaMedida=0.001;

    halfh=h*0.5;
    PasosxMedida=round(GuardaMedida/h); //redondeamos la división al entero más próximo

    //Liberar memoria dinámica
    free(x);
    free(y);
    free(vx);
    free(vy);
    free(m);

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

//Cálculo de la fuerza que ejerce el cuerpo 2 sobre el 1
void CalculaFuerza(double x1, double x2, double y1, double y2, double m1, double m2, double* fuerzax, double* fuerzay)
{
    double dx, dy, daux;

    dx=x1-x2;
    dy=y1-y2;

    daux= m1*m2/pow(dx*dx+dy*dy,3./2.); //definimos esta variable para no repetir este cálculo 2 veces por cada ejecución de la función

    *fuerzax= -daux*dx;
    *fuerzay= -daux*dy;

    return;
}
void CalcAceleracion(double* x, double* y, double* ax, double* ay, double* m, int ncuerpos)
{
    int i, j;
    double fuerzax, fuerzay;
    
    //Inicializamos la aceleración a cero
    for (i=0; i<ncuerpos; i++)
    {
        ax[i]=0;
        ay[i]=0;
    }

    //Acción de interacción entre todos los cuerpos para calcular la aceleración
    for(i=0; i<ncuerpos; i++)
    {
        for(j=i+1; j<ncuerpos; j++)
        {
            //Calcula la fuerza que ejerce el cuerpo j sobre el i
            CalculaFuerza(x[i], x[j],y[i],y[j],m[i],m[j], &fuerzax, &fuerzay);
        }
        //Contribución ejercida por el cuerpo j sobre la aceleración del cuerpo i 
        ax[i]+= fuerzax/m[i]; 
        ay[i]+= fuerzay/m[i];
        //Contribución ejercida por el cuerpo i sobre la aceleración del cuerpo j
        ax[j]+= -fuerzax/m[j];
        ax[j]+= -fuerzax/m[j];
    }

    return; 
}