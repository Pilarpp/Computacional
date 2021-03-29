#include <stdio.h>
#include <math.h>

//Funciones a utilizar
double Dominante(double a, double b,double d);


int main(void)
{
    //Definimos las variables a utilizar
    double a,b,d; //Elementos de la Matriz
    double dom; //valor propio dominante
    double v[2];
    int i; //Interacciones para el método de las potencias

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
       printf("El valor dominante de la matriz introducida es %lf\n",dom);
       printf("\n");
       printf("A continuación, se procede a estimar, mediante el MÉTODO DE LAS POTENCIAS, el mayor autovalor de la matriz introducida.\n");
       printf("-----------------------------------------------------------------------------------------------------------------------\n");
       printf("Para ello, introduzca el número de interacciones deseadas: Interacciones= "); scanf("%i",&i);
       printf("Introduzca una aproximación inicial como autovector de la matriz introducida: v0= ");scanf("%lf %lf",&v[0], &v[1]); 
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