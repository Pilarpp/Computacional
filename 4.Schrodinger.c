#include <stdio.h>
#include <math.h>
#include "complex.h"

#define lambda 3 //parámetro, no está asociado a ninguna longitud de onda sino a la altura del pozo
#define N 1000 //Tamaño de la red
#define nciclos 50 //Debe tomar un valor entre 1 y N/4
#define iter 500 //Número de iteracciones

void CalculoBeta(fcomplex beta[N], fcomplex gamma[N], fcomplex phi[N+1], double s);
void CalculoChi(fcomplex alpha[N], fcomplex beta[N], fcomplex chi[N+1]);
void CalculoPhi(fcomplex chi[N+1], fcomplex phi[N+1]);



int main(void)
{
    double k,s, mod, fase, norma, modcuadrado;
    double V[N+1];
    int i,j, n;
    fcomplex phi[N+1], alpha[N], beta[N], chi[N+1], uno, gamma[N], A, InvGamma;
    FILE *Norma, *DatosSchrod;

    Norma= fopen("4.Norma.txt", "w");
    DatosSchrod= fopen("4.DatosSchrod.txt", "w");
    
    //Genero parámetros
    k=(2.0*M_PI*nciclos)/N;
    s=1/(4.0*k*k);

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
    fprintf(Norma,"%lf\n", norma);

    for (j=0;j<N+1;j++)
        {
           modcuadrado=Cabs(phi[j])*Cabs(phi[j]);
           fprintf(DatosSchrod,"%i, %lf, %lf\n",j, modcuadrado, V[j]);
        }
        fprintf(DatosSchrod, "\n");

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


    //Bucle tiempo
    for(n=0; n<iter; n++)
    {
        CalculoBeta(beta,gamma,phi,s);
        CalculoChi(alpha,beta, chi);
        CalculoPhi(chi,phi);

        norma=0;
        for (j=0;j<N+1;j++)
        {
           norma+= Cabs(phi[j])*Cabs(phi[j]);
        }
        fprintf(Norma,"%lf\n",norma);

        for (j=0;j<N+1;j++)
        {
           modcuadrado=Cabs(phi[j])*Cabs(phi[j]);
           fprintf(DatosSchrod,"%i, %lf, %lf\n",j, modcuadrado, V[j]);
        }
        fprintf(DatosSchrod, "\n");
        


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