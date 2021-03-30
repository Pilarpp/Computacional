#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//Funciones a utilizar
double Dominante(double a, double b,double d);
void MultiMatriz(double A[2][2], double v[],double m[]);
double AproxAutovalor(double A[2][2], double m[]);


int main(void)
{
    //Definimos las variables a utilizar
    double A[2][2]; 
    double a,b,d; //Elementos de la Matriz A
    double dom; //valor propio dominante calculado analíticamente
    double domAprox; //Estimación del valor propio dominante por el Método de las potencias
    double v[2], m[2]; //autovector y vector resultante al multiplicar una matriz por un vcetor
    int i; //Interacciones para el método de las potencias 
    double r; //Cota para el método de las potencias

    FILE *f1; //Puntero a fichero
    f1=fopen("EstimacionesDominante.txt", "w"); //Abro el fichero donde escribiré las estimaciones del valor propio dominante

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

    //Mostrar por pantalla el autovalor dominante + método de las potencias ó un mensaje en caso de no tenerlo
    if (dom!=0)
    {
       printf("El valor dominante calculado de forma analítica de la matriz introducida es %lf\n",dom);
       printf("\n");
       fprintf(f1,"%lf\n",dom); // El primer elemento del fichero es el valor analítico
       

       //METODO DE LAS POTENCIAS
       printf("A continuación, se procede a estimar dicho autovalor dominante mediante el MÉTODO DE LAS POTENCIAS:\n");
       printf(" (*)El programa toma como aproximación inicial de autovector v0=(1,1).\n");
       printf(" (*)Se ejecuta hasta que la diferencia entre el valor analítico y el estimado es menor a 0.0000001 o se han alcanzado 100 interacciones.\n");
       printf(" (*)Los resultados de las estimaciones se recogen en el fichero EstimacionesDominante.txt\n");
       printf("-------------------------------------------------------------------------------------------------------------------------------------\n");

       A[0][0]=a; A[0][1]=b; A[1][0]=b; A[1][1]=d; //Asigno los valores a los elementos de la matriz
       v[0]=1;v[1]=1; //Aproximación inicial de autovector de la matriz A

       
       //Escritura en Fichero de las estimaciones del valor dominante para cada interacción
       fprintf(f1,"Interacción      Valor dominante\n");
       fprintf(f1,"---------------------------------\n");

       r=0.0000001; //Cota para la diferencia entre el valor analítico y el estimado
       for(i=0;  fabs(domAprox-dom)>r; i++) //Ejecutamos el bucle hasta que se alcance la cota impuesta
       {
         if (i>=100) break; //Salimos del bucle si se han alcanzado las 100 interacciones sin cumplirse la condición de cota.

         MultiMatriz(A,v,m); //Cálculo del autovector aproximado
         domAprox= AproxAutovalor(A,m); //Cálculo del autovalor a partir del autovector aproximado
         fprintf(f1,"  %i                 %lf\n", i+1, domAprox); //Escritura de cada interacción en el fichero
         v[0]=m[0]; v[1]=m[1]; //Pasamos la aproximación al autovector para calcular la siguiente aproximación en la posterior interacción.
       }

       fclose(f1); //Cerramos el fichero

    }
       else
       printf("La matriz introducida no tiene valor propio dominante\n");
    
    return 0;
}

//Cálculo analítico del valor propio dominante
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

//Función que multiplica una matriz 2x2 por un vector
void MultiMatriz(double A[2][2], double v[],double m[]) //Le paso la matriz y el vector a multiplicar, así como el vector donde quiero que guarde la operación
{

   m[0]=A[0][0]*v[0]+A[0][1]*v[1];
   m[1]=A[1][0]*v[0]+A[1][1]*v[1];

  return;
}


//Función que aproxima el Autovalor Dominante a partir del autovector estimado
double AproxAutovalor(double A[2][2], double m[])
{
   double domAprox, num, den; 
   double v1[2]; //Vector auxiliar donde guardar la multiplicicación de la matriz A por el autovector m

   MultiMatriz(A,m,v1); //Multiplicación de la matriz A por el autovector m
   //COEFICIENTE DE RAYLEIGH
   num=v1[0]*m[0]+v1[1]*m[1];
   den=m[0]*m[0]+m[1]*m[1];

   domAprox=num/den;
   return domAprox;
}

