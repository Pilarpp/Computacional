#include <stdio.h>
#include <math.h>
#include "complex.h"
#include "gsl_rng.h"

gsl_rng *tau;


#define lambda 10 //parámetro, no está asociado a ninguna longitud de onda sino a la altura del pozo
#define N  500//Tamaño de la red
#define nciclos 111 //Debe tomar un valor entre 1 y N/4
#define nD 200 //Número de pasos 
#define iter 1000//Número de veces que se llevan a cabo los pasos

void CalculoBeta(fcomplex beta[N], fcomplex gamma[N], fcomplex phi[N+1], double s);
void CalculoChi(fcomplex alpha[N], fcomplex beta[N], fcomplex chi[N+1]);
void CalculoPhi(fcomplex chi[N+1], fcomplex phi[N+1]);



int main(void)
{
    double k,s, mod, fase, norma, modcuadrado, transmision, Valor, Ini;
    double x, y; //Num aleatorios
    double PD, PI, knorm, knorm1; //Probabilidades en los detectores + cte de normalización
    double V[N+1];
    int i,j, n, m, mT, t, alg;
    fcomplex phi[N+1], alpha[N], beta[N], chi[N+1], uno, gamma[N], A, InvGamma;
    FILE *Norma, *DatosSchrod, *nciclo, *nDoptimo, *DependN;
    
    extern gsl_rng *tau; // Puntero al estado del número aleatorio
    int semilla=657432;  //13514; //Semilla del generador de números aleatorios

    printf("Seleccione el algoritmo:\n");
    scanf("%i", &alg);
    

    tau=gsl_rng_alloc(gsl_rng_taus);//Inicializamos el puntero
    gsl_rng_set(tau,semilla);//Inicializamos la semilla

    Norma= fopen("4.Norma.txt", "w");
    DatosSchrod= fopen("4.DatosSchrod.txt", "w");
    nciclo= fopen("4.nciclooptimo.txt", "w");
    nDoptimo= fopen("4.nDoptimo.txt", "w");
    DependN= fopen("4.DependN.txt", "w");
     
    //Genero parámetros
    k=(2.0*M_PI*nciclos)/N;
    s=1/(4.0*k*k);
    mT=0; m=0;

    //Escribo el potencial
    for(j=0; j<N+1; j++)
    {
        if(j>=((2.0*N)/5.0) && j<=((3.0*N)/5.0))
        {
            V[j]=lambda*k*k;
        }
        else
        {
            V[j]=0;
        } 
    }

    //Escribo la función de onda inicial
    phi[0]=Complex(0,0);
    phi[N]=Complex(0,0);
    for(j=1; j<N; j++)
    {
        mod=exp((-8.0*pow((4*j-N),2))/(N*N));
        fase=k*j;
        phi[j]=Cgauss(fase, mod); //OJO, primero se pone la fase y luego el módulo
    }

    //Norma inicial
    norma=0;
    for (j=0;j<N+1;j++)
    {
        norma+= Cabs(phi[j])*Cabs(phi[j]);
    }
    //Normalizo
    for(j=0;j<=N;j++)
    {
        phi[j]=RCmul(1.0/sqrt(norma),phi[j]);
    }
    //Compruebo que la norma sale 1
    norma=0;
    for (j=0;j<N+1;j++)
    {
        norma+= Cabs(phi[j])*Cabs(phi[j]);
    }
    //fprintf(Norma,"%lf\n", norma);

    //Calculo de forma recursiva los alpha (no dependen del tiempo)
    alpha[N-1]=Complex(0,0);
    for(j=N-2; j>=0; j--)
    {
        uno=Complex(1,0);
        A=Complex(-2.0-V[j+1], 2.0/s); //Aj0
        InvGamma=Cadd(alpha[j+1],A); //Sumo alpha j con Aj0
        gamma[j]=Cdiv(uno,InvGamma); //Hago la inversa
        alpha[j]=RCmul(-1,gamma[j]); //Le cambio el signo
    }

    if(alg==1)
    {
    //PRIMER ALGORITMO
    while(m<iter)
    {
        //Evoluciono el sistema nD pasos
        for(n=0; n<nD; n++)
        {
           CalculoBeta(beta,gamma,phi,s);
           CalculoChi(alpha,beta, chi);
           CalculoPhi(chi,phi);   
        }

        //Cálculo de PD(nD)
        PD=0;
        for (j=(4.0*N/5.0);j<N+1;j++)
           {
             PD+=Cabs(phi[j])*Cabs(phi[j]);
           }
        x=gsl_rng_uniform(tau); //número aleatorio real entre [0,1]

        if(x<PD)
        {
            mT=mT+1; //Incremento el contador de detectados
            m=m+1; //Incremento el numero de exp
            //printf("x=%lf PD=%lf\n", x, PD);
            
            //Genero la función de onda inicial de nuevo
            phi[0]=Complex(0,0);
            phi[N]=Complex(0,0);
            for(j=1; j<N; j++)
            {
              mod=exp((-8.0*pow((4*j-N),2))/(N*N));
              fase=k*j;
              phi[j]=Cgauss(fase, mod); //OJO, primero se pone la fase y luego el módulo
            }
            norma=0;
            for (j=0;j<N+1;j++)
            {
                norma+= Cabs(phi[j])*Cabs(phi[j]);
            }
            //Normalizo
            for(j=0;j<=N;j++)
            {
                phi[j]=RCmul(1.0/sqrt(norma),phi[j]);
            }
        }
        else
        {
            //Calculo la función de onda tras la medida en el detector derecho
            for(j=(4.0*N/5.0); j<N+1; j++)
            {
                phi[j]=Complex(0,0);
            }
            knorm=0;
            for(j=0; j<N+1; j++) //Calculo k para normalizar
            {
                knorm+=Cabs(phi[j])*Cabs(phi[j]);
            }
            for(j=0; j<N+1; j++)
            {
                phi[j]=RCmul(1/sqrt(knorm), phi[j]);
            }

            //Cálculo de PI(nD)
            PI=0;
            for(j=0; j<(N/5+1); j++)
            {
                PI+=Cabs(phi[j])*Cabs(phi[j]);
            }
            y=gsl_rng_uniform(tau); //número aleatorio real entre [0,1]
            //printf("y=%lf, PI=%lf\n", y, PI);
            if(y<PI)
            {
                m=m+1; //Ha sido detectada a la izquierda luego paso al siguiente exp
                //Genero la función de onda inicial de nuevo
                phi[0]=Complex(0,0);
                phi[N]=Complex(0,0);
                for(j=1; j<N; j++)
                {
                  mod=exp((-8.0*pow((4*j-N),2))/(N*N));
                  fase=k*j;
                  phi[j]=Cgauss(fase, mod); //OJO, primero se pone la fase y luego el módulo
                }
                norma=0;
                for (j=0;j<N+1;j++)
                {
                    norma+= Cabs(phi[j])*Cabs(phi[j]);
                }
                //Normalizo
                for(j=0;j<=N;j++)
                {
                    phi[j]=RCmul(1.0/sqrt(norma),phi[j]);
                }
            }
            else
            {
                for(j=0; j<=N/5; j++)
                {
                    phi[j]=Complex(0,0);
                }
                knorm1=0;
                for(j=0; j<N+1; j++) //Calculo k para normalizar
                {
                    knorm1+=Cabs(phi[j])*Cabs(phi[j]);
                }
                for(j=0; j<N+1; j++)
                {
                    phi[j]=RCmul(1/sqrt(knorm1), phi[j]);
                }
            }
        }
    } 

    transmision=mT*1.0/m*1.0;
    printf("%lf\n", transmision);
}
    if(alg==2)
    {
    //SEGUNDO ALGORITMO
    for(t=95; t<iter; t+=10)
    {
        //Escribo la función de onda inicial
        phi[0]=Complex(0,0);
        phi[N]=Complex(0,0);
        for(j=1; j<N; j++)
        {
           mod=exp((-8.0*pow((4*j-N),2))/(N*N));
           fase=k*j;
           phi[j]=Cgauss(fase, mod); //OJO, primero se pone la fase y luego el módulo
        }
        norma=0;
        for (j=0;j<N+1;j++)
        {
          norma+= Cabs(phi[j])*Cabs(phi[j]);
        }
        //Normalizo
        for(j=0;j<=N;j++)
        {
           phi[j]=RCmul(1.0/sqrt(norma),phi[j]);
        }

        //Evoluciono el sistema nD pasos
        for(n=0; n<t; n++)
        {
           CalculoBeta(beta,gamma,phi,s);
           CalculoChi(alpha,beta, chi);
           CalculoPhi(chi,phi);
  
           norma=0;
           for (j=0;j<N+1;j++)
           {
              norma+= Cabs(phi[j])*Cabs(phi[j]);
           }
           //Normalizo
           for(j=0;j<=N;j++)
           {
               phi[j]=RCmul(1.0/sqrt(norma),phi[j]);
           }  
        }

        //Cálculo de PD(nD)
        PD=0;
        for (j=(4.0*N/5.0);j<N+1;j++)
           {
             PD+=Cabs(phi[j])*Cabs(phi[j]);
           }
        Valor=PD;
        printf("nD=%i,PD=%lf\n", t, PD);
        if(Valor<Ini) Ini=Ini;
        else Ini=Valor;
    } 
    printf("Coef transmision= %lf\n", Ini);
    }


    fclose(Norma);
    fclose(DatosSchrod);

    return 0;
}

void CalculoBeta(fcomplex beta[N],fcomplex gamma[N], fcomplex phi[N+1],double s)
{
    int j;
    fcomplex unidadimag, b, num1, num2;

    unidadimag=Complex(0,1);

    beta[N-1]=Complex(0,0);
    for(j=N-2; j>=0; j--)
    {
        num1=RCmul(4/s,unidadimag);
        b=Cmul(num1, phi[j+1]);
        num2=Csub(b, beta[j+1]);
        beta[j]=Cmul(gamma[j+1],num2);
    }
    return; 
}

void CalculoChi(fcomplex alpha[N], fcomplex beta[N], fcomplex chi[N+1])
{
    int j;
    fcomplex num1;

    chi[0]=Complex(0,0);
    for(j=0; j<N; j++)
    {
        num1=Cmul(alpha[j],chi[j]);
        chi[j+1]=Cadd(num1, beta[j]);
    }
    return;
}

//Calculo de la funcion de onda en el instante n+1
void CalculoPhi(fcomplex chi[N+1], fcomplex phi[N+1])
{
    int j;

    for(j=0; j<N+1; j++)
    {
        phi[j]=Csub(chi[j],phi[j]);
    }

    return;
}