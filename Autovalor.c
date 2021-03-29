#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//Funciones a utilizar
double Dominante(double a, double b,double d);
void MultiMatriz(double A[2][2], double v[],double m[]);
double AproxAutovalor(double A[2][2], double v[], double m[]);


int main(void)
{
    //Definimos las variables a utilizar
    double A[2][2];
    double a,b,d; //Elementos de la Matriz A
    double dom; //valor propio dominante calculado analíticamente
    double domAprox; //Estimación del valor proprio dominante por el Método de las potencias
    double v[2], m[2]; //autovector y vector resultante al multiplicar una matriz por un vcetor
    int i, j; //Interacciones para el método de las potencias y contador 

    //Introducción al programa a utilizar 
    printf("--------------------------------------------------------------------------\n"); 
    printf("Método de las potencias para calcular el autovalor dominante de una matriz\n");
    printf("--------------------------------------------------------------------------\n");

    //Se introduce por la terminal la matriz con la que se va a trabajar
    printf("Introduzca los elementos de la matriz simétrica 2x2:\n");
    printf(" a     b\n");
    printf(" b     d\n");
    printf("{Recuerde que los decimales se escriben con punto, por ejemplo, 2.54}\n");
    printf("----------------------------------------------------\n");
    printf("a= "); scanf("%lf", &a);
    printf("b= "); scanf("%lf", &b);
    printf("d= "); scanf("%lf", &d);
    printf("----------------------------------------------------\n");

    //Cálculo analítido del autovalor dominante de la matriz
    dom= Dominante(a,b,d);

    //Mostrar por pantalla el autovalor dominante o un mensaje en caso de no tenerlo
    if (dom!=0)
    {
       printf("El valor dominante calculado de forma analítica de la matriz introducida es %lf\n",dom);
       printf("\n");

       //METODO DE LAS POTENCIAS
       printf("A continuación, se procede a estimar dicho autovalor dominante mediante el MÉTODO DE LAS POTENCIAS:\n");
       printf("---------------------------------------------------------------------------------------------------\n");
       printf("Para ello, introduzca el número de interacciones deseadas: Interacciones= "); scanf("%i",&i);
       A[0][0]=a; A[0][1]=b; A[1][0]=b; A[1][1]=d; //Introduzco los elementos en la matriz
       v[0]=1;v[1]=1; //Aproximación inicial de autovector de la matriz A

       //Aproximación del Autovector
       for (j=0; j<i; j++)
       {
          MultiMatriz(A,v,m);
          v[0]=m[0]; v[1]=m[1];
       }
       v[0]=v[0]/v[1]; v[1]=1; //Normalización del autovector dominante de A para que la segunda componente sea 1 sin pérdida de generalidad

       //Aproximación del Autovalor: COEFICIENTE DE RAYLEIGH
       domAprox= AproxAutovalor(A,v,m);
       printf("---> El autovalor dominante aproximado por el método de las potencias es %lf\n", domAprox);

    }
       else
       printf("La matriz introducida no tiene valor propio dominante\n");
    

    return 0;


}

double Dominante(double a, double b,double d)
{
    double Dom, alpha1, alpha2, t, f1,f2; //Las variables t,f1,f2 son definidas para optimizar el número de cálculos

    t=sqrt(pow((d-a),2)+4*pow(b,2)); //Habría que repetir dos veces este cálculo, para simplicar lo definimos en la variable t

    //Cálculo de los valores propios
    alpha1=((a-d)+t)/2+d;
    alpha2=((a-d)-t)/2+d;

    f1= fabs(alpha1); f2=fabs(alpha2); //Defino los valores absolutos de los valores propios

    if (f1>f2)
       Dom=alpha1;
       else if (f2>f1)
       Dom=alpha2;
       else //los casos restantes son aquellos en los que no exite valor propio dominante
       Dom=0; //Otorgo el valor 0 en caso de no tener valor propio dominante ya que un valor dominante nunca puede valer 0.
    return Dom;
}

void MultiMatriz(double A[2][2], double v[],double m[]) //Defino una función que multiplique una matriz 2x2 por un vector
{

   m[0]=A[0][0]*v[0]+A[0][1]*v[1];
   m[1]=A[1][0]*v[0]+A[1][1]*v[1];

  return;
}

double AproxAutovalor(double A[2][2], double v[], double m[])
{
   double domAprox, num, den; 

   MultiMatriz(A,v,m);
   num=m[0]*v[0]+m[1]*v[1];
   den=v[0]*v[0]+v[1]*v[1];

   domAprox=num/den;
   return domAprox;
}

