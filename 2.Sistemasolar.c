#include<math.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//funciones a utilizar
int DatosInicial(char* condIni, double** x, double** y, double** vx, double** vy, double** m);
void CalcFuerza(double x1, double x2, double y1, double y2, double m1, double m2, double* fuerzax, double* fuerzay);
void CalcAceleracion(double* x, double* y, double* ax, double* ay, double* m, int ncuerpos);
void Evolucion(double* x, double*y, double* vx, double* vy, double* ax, double* ay, double* wx, double* wy, double* m, int ncuerpos, double h, double hmedio, int PasosxMedida);
double CalcEnergia(double* x, double* y, double* vx, double* vy, double* m, int ncuerpos);
void Tierra(double* x, double* y, double* xt, double* yt, int ncuerpos);


int main(void)
{
    double *x,*y,*vx,*vy,*m;
    double *ax, *ay, *wx, *wy; 
    double *xini, *ypre, *vueltas, *xt, *yt;
    double h, hmedio, GuardaMedida;
    double E, t;
    int ncuerpos;
    int PasosxMedida, i, j;
    FILE *fposiciones, *fEnergia, *fposTierra;

    char condIni[]="datosGerman.txt";

    //Leo los datos del fichero y descubro cuántos cuerpos forman mi sistema
    ncuerpos= DatosInicial(condIni,&x,&y,&vx,&vy,&m);  

    //Una vez conocido el número de cuerpos, asigno memoria a la aceleración 
    xini=(double*)malloc(ncuerpos*sizeof(double));
    ypre=(double*)malloc(ncuerpos*sizeof(double));
    vueltas=(double*)malloc(ncuerpos*sizeof(double));
    ax=(double*)malloc(ncuerpos*sizeof(double));
    ay=(double*)malloc(ncuerpos*sizeof(double));
    wx=(double*)malloc(ncuerpos*sizeof(double));
    wy=(double*)malloc(ncuerpos*sizeof(double));
    xt=(double*)malloc(ncuerpos*sizeof(double));
    yt=(double*)malloc(ncuerpos*sizeof(double));

    //Defino el paso y cada cuánto quiero guardar la medida
    h=0.01;
    GuardaMedida=0.1;

    hmedio=0.5*h;
    PasosxMedida=round(GuardaMedida/h); //redondeamos la división al entero más próximo

    //Abro ficheros para escribir los resultados
    fposiciones= fopen("posiciones.txt", "w");
    fEnergia=fopen("energias.txt", "w");
    fposTierra=fopen("posTierra.txt", "w");

    //Calculo las aceleraciones en el instante inicial
    CalcAceleracion(x, y, ax, ay, m, ncuerpos);

    t=0;
    //Paso las coordenadas iniciales respecto a la tierra
        Tierra(x,y,xt,yt,ncuerpos);

        //Escribo en el fichero las posiciones respecto a la tierra de todos los planetas en la iteracción i
        for(j=0; j<ncuerpos; j++)
        {
            fprintf(fposTierra,"%lf, %lf\n", xt[j],yt[j]);
        }
        fprintf(fposTierra, "\n");

    for(i=0; i<1000; i++) //Iteracciones del programa
    {
        //Evolucion del sistema por iteracción
        Evolucion(x, y, vx, vy, ax, ay, wx, wy, m, ncuerpos, h, hmedio, PasosxMedida);
        t+=h*PasosxMedida;

        //Compruebo el periodo
        for (j=1; j<ncuerpos; j++)
        {
            if(ypre[j]<0 && y[j]>=0 && x[j]>0 && vueltas[j]<1)
            {
                printf("El periodo del planeta %i se encuentra entre [%lf,%lf] días\n",j, (t-h)*58.1, t*58.1);
                vueltas[j]=vueltas[j]+1;
            }
        }
        
        
        //Escribo en el fichero las posiciones de todos los planetas en la iteracción i
        for(j=0; j<ncuerpos; j++)
        {
            fprintf(fposiciones,"%lf, %lf\n", x[j],y[j]);
            ypre[j]=y[j];
        }
        fprintf(fposiciones, "\n");

        //Cálculo de la energía total del sistema en cada iteracción y escritura a fichero
        E=CalcEnergia(x, y, vx, vy, m, ncuerpos);
        fprintf(fEnergia, "%lf\n", E);    

        //Cambio de coordenadas respecto a la tierra
        Tierra(x,y,xt,yt,ncuerpos);

        //Escribo en el fichero las posiciones respecto a la tierra de todos los planetas en la iteracción i
        for(j=0; j<ncuerpos; j++)
        {
            fprintf(fposTierra,"%lf, %lf\n", xt[j],yt[j]);
        }
        fprintf(fposTierra, "\n");

    }

    fclose(fposiciones);
    fclose(fEnergia);
    fclose(fposTierra);

    //Liberar memoria dinámica
    free(x);
    free(y);
    free(vx);
    free(vy);
    free(m);
    free(vueltas);
    free(xini);
    free(ypre);
    free(xt);
    free(yt);
    
    return 0;
}

int DatosInicial(char* condIni, double** x, double** y, double** vx, double** vy, double** m)
{
    int ncuerpos;
    int i;
    double Msol, c, CstG;
    FILE* fcondIni;

    fcondIni=fopen(condIni, "r"); 
    fscanf(fcondIni, "%i\n",&ncuerpos);

    *x=(double*) malloc(ncuerpos*sizeof(double));
    *y=(double*) malloc(ncuerpos*sizeof(double));
    *vx=(double*) malloc(ncuerpos*sizeof(double));
    *vy=(double*) malloc(ncuerpos*sizeof(double));
    *m=(double*) malloc(ncuerpos*sizeof(double));

    CstG=6.6764E-11;
    c=1.496E+11;
    Msol=1.989E+30;

    for (i=0; i<ncuerpos; i++)
    {
        //En caso de que los datos introducidos ya estén reescalados:
        //fscanf(fcondIni, "%lf %lf %lf %lf %lf\n",&(*m)[i],&(*x)[i], &(*y)[i], &(*vx)[i], &(*vy)[i]);
        //printf("%1.11lf %1.11lf %1.11lf\n",(*m)[i],(*x)[i], (*y)[i]); Para comprobar que me lee bien el fichero

        //En caso de que los datos introducidos necesiten ser reescalados
        fscanf(fcondIni, "%lf %lf %lf\n",&(*m)[i],&(*x)[i],&(*vy)[i]);
        (*m)[i]=(*m)[i]/Msol;
        (*x)[i]=(*x)[i]/c;
        (*y)[i]=0;
        (*vx)[i]=0;
        (*vy)[i]=(*vy)[i]*sqrt(c/(CstG*Msol));
    }


    fclose(fcondIni);

    return ncuerpos;
}

//Cálculo de la fuerza que ejerce el cuerpo 2 sobre el 1
void CalcFuerza(double x1, double x2, double y1, double y2, double m1, double m2, double* fuerzax, double* fuerzay)
{
    double dx, dy, daux;

    dx=x1-x2;
    dy=y1-y2;

    daux= m1*m2/pow(dx*dx+dy*dy,3./2.); //definimos esta variable para no repetir este cálculo 2 veces por cada ejecución de la función

    *fuerzax= -daux*dx;
    *fuerzay= -daux*dy;

    return;
}

//Cálculo de la aceleración de todos los cuerpos en el instante t
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
            CalcFuerza(x[i], x[j],y[i],y[j],m[i],m[j], &fuerzax, &fuerzay);

        //Contribución ejercida por el cuerpo j sobre la aceleración del cuerpo i 
        ax[i]+= fuerzax/m[i]; 
        ay[i]+= fuerzay/m[i];
        //Contribución ejercida por el cuerpo i sobre la aceleración del cuerpo j
        ax[j]+= -fuerzax/m[j];
        ay[j]+= -fuerzay/m[j];
        }
    }
    
    return; 
}

//Evolución del sistema
void Evolucion(double* x, double*y, double* vx, double* vy, double* ax, double* ay, double* wx, double* wy, double* m, int ncuerpos, double h, double hmedio, int PasosxMedida)
{
    int i, j;

    for(j=0; j<PasosxMedida; j++)
    {
        
        for(i=1; i<ncuerpos; i++)
        {
            wx[i]=vx[i]+hmedio*(ax[i]);
            wy[i]=vy[i]+hmedio*(ay[i]);
            
            //Cálculo de r(t+h)
            x[i]=x[i]+h*(wx[i]);
            y[i]=y[i]+h*(wy[i]);
        }

        //Calculo las aceleraciones a tiempo t+h a partir de las posiciones en t+h
        CalcAceleracion(x, y, ax, ay, m, ncuerpos);
        

        //Cálculo de v en t+h
        for(i=0; i<ncuerpos; i++)
        {
            vx[i]=wx[i]+hmedio*(ax[i]);
            vy[i]=wy[i]+hmedio*(ay[i]);
        }

    }
    return;
}

//Cálculo de la energía
double CalcEnergia(double* x, double* y, double* vx, double* vy, double* m, int ncuerpos)
{
    double E_cin, E_pot, dx, dy;
    int i,j;

    //Cálculo energía cinética
    for (i=0;i<ncuerpos; i++)
    {
        E_cin+=m[i]*(vx[i]*vx[i]+vy[i]*vy[i]);
    }
    E_cin=E_cin/2;

    //Cálculo energía potencial
    for(i=0; i<ncuerpos; i++)
    {
        for(j=i+1; j<ncuerpos; j++)
        {
            dx= x[i]-x[j];
            dy= y[i]-y[j];
            E_pot+= -m[i]*m[j]/sqrt(dx*dx+dy*dy);
        }
    }

    return E_cin+E_pot;
}

//Calcular órbitas respecto a la tierra
void Tierra(double* x, double* y, double* xt, double* yt, int ncuerpos)
{
    int j;

    for (j=0; j<ncuerpos; j++)
    {
        xt[j]=x[j]-x[3];
        yt[j]=y[j]-y[3];
    }
    return;
}