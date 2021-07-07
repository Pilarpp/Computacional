#include<stdio.h>
#include<math.h>
#include "gsl_rng.h"

gsl_rng *tau;

#define N 128 //Número de particulas
#define PMC 1000000 //Número de pasos montecarlo
//#define T 1.5 //Temperatura

//Defino las funciones a utilizar
double Calculap(int R[N][N],int n, int m, double T);
double CalculaE(int R[N][N],int n, int m);
void InicioRed(int R[N][N]);
double Calculaf(int R[N][N],int paso);

int main(void)
{
    int R[N][N];
    double f[N/20], Valor4[N/20];
    int i, j, r, k, n, m, iter;
    int t, paso, k1;
    double x, p, Valor1, Valor2, Valor3, E, Esum, resto, mN, T;
    FILE* fred, *fmN, *fEnergia, *fCalor, *fcorrelacion;
    extern gsl_rng *tau; // Puntero al estado del número aleatorio
    int semilla=1352514; //Semilla del generador de números aleatorios

    tau=gsl_rng_alloc(gsl_rng_taus);//Inicializamos el puntero
    gsl_rng_set(tau,semilla);//Inicializamos la semilla
    printf("T=\n");
    scanf("%lf", &T);

    //Abro el fichero sobre el que escribir la matriz
    fred= fopen("red.txt", "w");
    fmN= fopen("fmN.txt", "w");
    fEnergia= fopen("fEnergia.txt", "w");
    fCalor= fopen("fCalor.txt", "w");
    fcorrelacion= fopen("fCorrelacion.txt", "w");
    

    //Inicializo la red
    InicioRed(R);

    
    iter=N*N; //Número de iteracciones
    for(k1=0; k1<PMC; k1++)
    {
        for(r=0; r<iter; r++)
        {
        //Genero una posición aleatoria
        n=gsl_rng_uniform_int(tau,N); //fila entre [0,N-1]
        m=gsl_rng_uniform_int(tau,N); //columna entre [0,N-1]
        
        //Calculo p
        p=Calculap(R,n,m, T);

        //Genero numero aletorio real entre[0,1]        
        x=gsl_rng_uniform(tau); 

        //Cambio el signo si el numero es menor que p
        if(x<p) R[n][m]= -R[n][m];
        }

        resto=(k1+1)%100;
        if(resto==0)
        {    
           
           //Magnetización mN
            mN=0;
            for(i=0; i<N; i++)
            {
              for(j=0;j<N;j++)
              {
                mN+=(R[i][j]);
              }
            }
            mN=abs(mN);
            Valor1+=mN/(N*N);
            
           //Energía
           Esum=0;
           for(i=0; i<N; i++)
            {
              for(j=0;j<N;j++)
              {
                E=CalculaE(R, i, j);
                Esum+=E;
              }
            }
            Esum=-Esum/2.0;
            Valor2+=Esum;

            //Calor específico
            Valor3+=pow(Esum, 2);

            //Funcion de correlacion
            /*for(k=0;k<N;k++)
            {
                for(j=0;j<N;j++)
                {
                    for(i=0;i<=(N/2);i++)
                    {
                        if(i<=(N-1-j))
                        {
                           f[i]+=R[j][k]*R[j+i][k];
                        }
                        else 
                        {
                           f[i]+=R[j][k]*R[i-(N-1-j)-1][k];    
                        }
                    }
                }
                
            }*/
        }
    }
    printf("mN=%lf\n", Valor1/(PMC/100));
    printf("En= %lf\n", Valor2/(PMC*2*N/100));
    printf("Calor=%lf\n", (Valor3/(PMC/100)-(Valor2/(PMC/100))*(Valor2/(PMC/100)))/(N*N*T));
    /*for(i=0; i<=N/2; i++)
    {
        fprintf(fcorrelacion,"%i %lf\n", i, f[i]/(PMC*N*N/100));
    }*/
    
    fclose(fred);

    return 0;
}


double Calculap(int R[N][N],int n, int m, double T)
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

double CalculaE(int R[N][N],int n, int m)
{
    double Sum, E;

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
    E=R[n][m]*Sum;

    return E;
}

double Calculaf(int R[N][N],int paso)
{
    double pos, f;
    int n, m, k1;
    f=0;
    for(n=0; n<N; n++)
            {
              for(m=0;m<N;m++)
              {
                if(n==0)
                {
                  pos=R[0][m]*R[0+paso][m];
                }
                else if(n==N-1)
                {
                  pos=R[N-1][m]*R[paso-1][m];
                }
                else if(n!=N-1 && n!=0 && paso<=(N-1-n))
                {
                    pos=R[n][m]*R[n+paso][m];
                }
                else
                {
                    k1=(paso-(N-1-n));
                    pos=R[n][m]*R[k1-1][m];    
                }
                f+=pos;
              }
            }

    return f;
}

void InicioRed(int R[N][N])
{
    int i, j, d;

    //printf("Escriba:\n");
    //printf("1 Si desea inicializar la red ordenada\n");
    //printf("2 Si desea inicializar la red desordenada\n");
    //scanf("%i",&d);
    d=1;

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