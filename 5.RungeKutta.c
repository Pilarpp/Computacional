#include <stdio.h>
#include <math.h>
#include <string.h>

#define G 6.67E-11
#define MT 5.9736E24
#define ML 0.07349E24
#define dTL 3.844E8
#define w 2.6617E-6
#define RT 6.378160E6
#define RL 1.7374E6
#define m 4.5932E4

#define h 60 //Salto temporal de 1 minuto
#define iter 100

double Calculaf(double r, double phi, double pr, double pphi, double delta, double mu, int t, int n);
double CalculaH(double sol[4], int t);

int main(void)
{
    int i, j, t, n, c;
    double delta, mu, v, theta;
    double k1[4], k2[4], k3[4], k4[4], sol[4], dist[iter];
    double xC, yC, xL, yL, xT, yT;
    double H, min;
    FILE *fcoord, *fh;

    fcoord=fopen("5.Coordenadas.txt", "w");
    fh=fopen("5.H.txt", "w");

    //Condiciones iniciales 
    t=0;
    sol[0]=RT*1.0/dTL; //r
    sol[1]=M_PI/4.0;; //phi
    v=11100/dTL;
    theta=7*M_PI/30.0; //42grados
    sol[2]=v*cos(theta-sol[1]); //pr
    sol[3]=v*sol[0]*sin(theta-sol[1]); //pphi

    //Escribo a fichero las condiciones iniciales
    xC=sol[0]*cos(sol[1]);
    yC=sol[0]*sin(sol[1]);
    xT=0; yT=0;
    xL=cos(w*t);
    yL=sin(w*t);
    fprintf(fcoord,"%lf,%lf\n", xL, yL);
    fprintf(fcoord,"%lf,%lf\n", xC, yC);
    fprintf(fcoord,"%lf,%lf\n", xT, yT);
    fprintf(fcoord, "\n");

    delta=(G*MT*1.0)/(pow(dTL,3));
    mu=1.0*ML/MT; 

    for(j=0; j<iter; j++)
    {    
     for(i=0; i<60; i++)//Bucle escribo la posición cada hora
     {
         
        //Calculo K1 para todas las variables
        for(n=0; n<4; n++)
        {
            k1[n]=h*Calculaf(sol[0], sol[1], sol[2], sol[3], delta, mu, t, n);
        }

        //Calculo K2 para todas las variables
        for (n=0; n<4; n++)
        {
            k2[n]=h*Calculaf(sol[0]+k1[0]/2, sol[1]+k1[1]/2, sol[2]+k1[2]/2, sol[3]+k1[3]/2, delta, mu, t+h/2.0, n);
        }

        //Calculo K3 para todas las variables
        for (n=0; n<4; n++)
        {
            k3[n]=h*Calculaf(sol[0]+k2[0]/2, sol[1]+k2[1]/2, sol[2]+k2[2]/2, sol[3]+k2[3]/2, delta, mu, t+h/2.0, n);
        }

        //Calculo K4 para todas las variables
        for (n=0; n<4; n++)
        {
            k4[n]=h*Calculaf(sol[0]+k3[0], sol[1]+k3[1], sol[2]+k3[2], sol[3]+k3[3], delta, mu, t+h, n);
        }

        //Solucion para cada variables
        for(n=0; n<4; n++)
        {
            sol[n]=sol[n]+(k1[n]+2*k2[n]+2*k3[n]+k4[n])/6.0;
        }

        t+=h;
        
     }
       //Coordenadas de la Luna
        xL=cos(w*t);
        yL=sin(w*t);
        fprintf(fcoord, "%lf,%lf\n", xL, yL);

        //Coordenadas del cohete
        xC=sol[0]*cos(sol[1]);
        yC=sol[0]*sin(sol[1]);
        fprintf(fcoord, "%lf,%lf\n", xC, yC);

        //Coordenadas de la tierra
        xT=0;
        yT=0;
        fprintf(fcoord, "%lf,%lf\n", xT, yT);

        fprintf(fcoord,"\n");

        //H' es una constante del movimiento
        H=CalculaH(sol, t);
        fprintf(fh, "%e\n",H);

        //Distancia Luna-cohete (para ajustar el ángulo theta)
        dist[j]=sqrt(pow((xL-xC), 2)+pow((yL-yC), 2));  
    }
  //Muestro por pantalla la distancia minima que hay entre la luna y el cohete en toda la trayectoria
  min=dist[0];  
  for (c = 1; c < iter; c++)
  {
    if (dist[c] < min)
    {
       min = dist[c];
    }
  }
    printf("dist minima=%lf\n", min);

    fclose(fcoord);
    fclose(fh);
    return 0;
}

double Calculaf(double r, double phi, double pr, double pphi, double delta, double mu, int t, int n)
{
    double f, rprima;

    if(n==0)
    {
        f=pr;
    }
    else if(n==1)
    {
        f=pphi/(r*r);
    }
    else if(n==2)
    {
        rprima=sqrt(1+r*r-2*r*cos(phi-w*t));
        f=(pphi*pphi)/(pow(r,3))-delta*(1.0/(pow(r,2))+(mu/(pow(rprima, 3)))*(r-cos(phi-w*t)));
    }
    else
    {
        rprima=sqrt(1+r*r-2*r*cos(phi-w*t));
        f=(-delta*mu*r/(pow(rprima, 3)))*sin(phi-w*t);
    }
    
    return f;
}

double CalculaH(double sol[4], int t)
{
    double r, phi, pr, pphi, rll, H;

    r=(sol[0])*dTL;
    phi=sol[1];
    pr=(sol[2])*m*dTL;
    pphi=(sol[3])*m*dTL*dTL;

    rll=pow(r*r+dTL*dTL-2*r*dTL*cos(phi-w*t), 1/2);
    H=(pr*pr)/(2*m)+(pphi*pphi)/(2*m*r*r)-G*m*MT/r-G*m*ML/rll;

    return H;
}