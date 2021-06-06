#include<stdio.h>
#include<math.h>
#include "gsl_rng.h"

gsl_rng *tau;

#define N 50 //Número de particulas
#define PMC 100 //Número de pasos montecarlo
#define T 1.0 //Temperatura

//Defino las funciones a utilizar
double Calculap(int R[N][N],int n, int m);
void InicioRed(int R[N][N]);

int main(void)
{
    int R[N][N];
    int i, j, r, k, n, m, iter;
    double x, p;
    FILE* fred;
    extern gsl_rng *tau; // Puntero al estado del número aleatorio
    int semilla=1352514; //Semilla del generador de números aleatorios

    tau=gsl_rng_alloc(gsl_rng_taus);//Inicializamos el puntero
    gsl_rng_set(tau,semilla);//Inicializamos la semilla

    //Abro el fichero sobre el que escribir la matriz
    fred= fopen("red.txt", "w"); 

    //Inicializo la red
    InicioRed(R);

    for(i=0; i<N; i++)
        {
            for(j=0; j<N; j++)
            {
                fprintf(fred,"%i", R[i][j]);
                if(j!=N-1) fprintf(fred,","); //Separo los datos con comas
            }            
        fprintf(fred,"\n");
        }
        fprintf(fred,"\n");
    
    iter=N*N; //Número de iteracciones
    for(k=0; k<PMC; k++)
    {
      for(r=0; r<iter; r++)
      {
        //Genero una posición aleatoria
        n=gsl_rng_uniform_int(tau,N); //fila entre [0,N-1]
        m=gsl_rng_uniform_int(tau,N); //columna entre [0,N-1]
        
        //Calculo p
        p=Calculap(R,n,m);

        //Genero numero aletorio real entre[0,1]        
        x=gsl_rng_uniform(tau); 

        //Cambio el signo si el numero es menor que p
        if(x<p) R[n][m]= -R[n][m];
      }
    //Escribo a fichero mi red
      for(i=0; i<N; i++)
        {
            for(j=0; j<N; j++)
            {
                fprintf(fred,"%i", R[i][j]);
                if(j!=N-1) fprintf(fred,","); //Separo los datos con comas
            }            
        fprintf(fred,"\n");
        }
        fprintf(fred,"\n");
    }
    fclose(fred);
    
    return 0;
}


double Calculap(int R[N][N],int n, int m)
{
    double p, deltaE, p1, Sum;

    //Condiciones de contorno
    if(n==0)
    {
        if(m==0)
        {
            Sum=R[n+1][m]+R[N-1][m]+R[n][m+1]+R[n][N-1];
        } 
        else if(m==N-1)
        {
            Sum=R[n+1][m]+R[N-1][m]+R[n][0]+R[n][m-1];
        } 
        else 
        {
            Sum=R[n+1][m]+R[N-1][m]+R[n][m+1]+R[n][m-1];
        }
    }
    else if(n==N-1)
    {
        if(m==0) Sum=R[0][m]+R[n-1][m]+R[n][m+1]+R[n][N-1];
        else if(m==N-1) Sum=R[0][m]+R[n-1][m]+R[n][0]+R[n][m-1];
        else Sum=R[0][m]+R[n-1][m]+R[n][m+1]+R[n][m-1];
    }
    else if(m==0 && n!=0 && n!=N-1) Sum=R[n+1][m]+R[n-1][m]+R[n][m+1]+R[n][N-1];
    else if (m==0 && n!=0 && n!=N-1) Sum=R[n+1][m]+R[n-1][m]+R[n][0]+R[n][m-1];
    else Sum=R[n+1][m]+R[n-1][m]+R[n][m+1]+R[n][m-1];

    //Calculo el valor de p
    deltaE=2*R[n][m]*Sum;
    p1=exp(-deltaE/T);

    if(p1<1) p=p1;
    else p=1;    
    
    return p;
}

void InicioRed(int R[N][N])
{
    int i, j, d;

    printf("Escriba:\n");
    printf("1 Si desea inicializar la red ordenada\n");
    printf("2 Si desea inicializar la red desordenada\n");
    scanf("%i",&d);

    if(d==2)
    {
        for(i=0; i<N; i++)
        {
           for(j=0;j<N;j++)
           {
              R[i][j]=gsl_rng_uniform_int(tau,2); //Número aleatorio 0 ó 1
              if(R[i][j]==0) R[i][j]=-1;
           }
        }
    }
    else if(d==1)
    {
        for(i=0; i<N; i++)
        {
            for(j=0;j<N;j++)
            {
                R[i][j]=1; //Primeramente utilizamos una red ordenada
            }
        }
    }
    else printf("El número introducido no es correcto\n");
}