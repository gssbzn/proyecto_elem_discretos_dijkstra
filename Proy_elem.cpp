/*
  Name: Proyecto de Elementos
  Copyright: 2005
  Author: Gustavo Bazan, Armando Bracho y Andrea Colmenares
  Date: 10/06/05 
  Description: Programa que verifica rutas optimas para una linea de taxis
*/

//Librerias
#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h> 

//Declaracion de constantes
#define N 200

//Declaracion de prototipos

//VARIOS
int compararcadenas(char *, char *);/*Compara si 2 cadenas son iguales*/
void tecla();         /*Pausa*/
char menu();
void ini(int Gp[][N], int Gn[][N], int Gb[][N]);/*Inicializa las Matrices en cero(0)*/

//CARGAR Y GUARDAR
void carga();/*Carga el archivo de entrada*/
void mostrar_e();/*Muestra el archivo de entrada*/
void mostrar_s();/*Muestra el archivo de salida*/

//VERIFICACION DE HORAS
bool verificarhorapico(int , int , char ap[]);/*Verifica si es hora pico*/
bool verificarhoranormal(int , int , char ap[]);/*Verifica si es hora normal*/
bool verificarhorabaja(int , int , char ap[]);/*Verifiva si es hora  de bajo flujo*/

//DIJKSTRA
void alg_Dijkstra(int G[][N],int cos[N], int cam[N], int, int);/*Algoritmo de Dijkstra*/
int camino(int cam[N], int, int, FILE *pf);/*Busca el camino entre un par de vertices*/

// Cuerpo del programa    
int main(){
       // declaracion de variables locales
       char opc;
       do{
         opc=menu();
         switch(opc){
          
          case '1':
             system ("cls");
             carga();
             tecla();
          break;
          
          case '2':
             system ("cls");
             printf("\t                  ARCHIVO DE ENTRADA                 \n\n");
             mostrar_e();
             tecla();
          break;
          
          case '3':
             system ("cls"); 
             printf("\t                  ARCHIVO DE SALIDA                 \n\n");
             mostrar_s();
             tecla();
          break;
 
          case '0':
               system ("cls");
               printf("\n\tлллллллллллллллллллллллллллллллллллллллллллллллллллл");
               printf("\n\tлл                                                лл");
               printf("\n\tлл                      ADIOS                     лл");  
               printf("\n\tлл                                                лл");
               printf("\n\tлллллллллллллллллллллллллллллллллллллллллллллллллллл\n\n");
          break;
          
          default: printf("\nOpcion Incorrecta\n\a"); break;
      }    
    
    }while(opc!='0');
    
    tecla();
    exit(0);
}

/******************************************************************************/

int compararcadenas(char *cad1, char *cad2){
    /*compara cadenas caracter a caracter con tolower*/   
    int i;   
    int resultado;   
    resultado=0;   
    
    if (strlen(cad1)!=strlen(cad2)) resultado=-1;   
    
    for (i=0;(unsigned)i<(unsigned)strlen(cad1) && resultado==0;i++) {      
        if (tolower(cad1[i])!=tolower(cad2[i])) resultado=-1;   
    }   
    return resultado;/*devuelve 0 si son iguales, -1 si son distintos*/
}

/******************************************************************************/

void tecla(){   
     printf("\n\n\tPresione cualquier tecla para continuar el programa...\n\a");   
     getch();   
     system("cls");   
     return;
}

/******************************************************************************/

char menu()
{
     system ("cls");
     system("color 0f");     
     printf("\n\n\n\n");      
     printf("\n\tлллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл");      
     printf("\n\tлл                                                          лл");      
     printf("\n\tлл                     Taxis ORBIS C.A.                     лл");      
     printf("\n\tлл                                                          лл");      
     printf("\n\tлллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл");      
     printf("\n\tлл                                                          лл");      
     printf("\n\tлл                1.-  Carga y Consulta                     лл");      
     printf("\n\tлл                2.-  Mostrar archivo de Entrada           лл");      
     printf("\n\tлл                3.-  Mostrar archivo de Salida            лл");              
     printf("\n\tлл                                                          лл");      
     printf("\n\tлл                0.-  Salir del programa                   лл");      
     printf("\n\tлл                                                          лл");      
     printf("\n\tлллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл\n\n");
           
      printf("\nЈQue opcion elije\? \n");
      return getch();
}

/******************************************************************************/

void carga()
{
     FILE *f, *pf;
     int Gp[N][N], Gn[N][N], Gb[N][N]; //Matrices de carga
     int vertices; //Guarda el numero de vertices por plano
     int i, j; //Indices de las matrices
     int k, l, m; //Contadores
     int pico, normal, baja; //Guarda los tempos para cada tipo de hora(pico, normal y baja)
     int partida, llegada; //Vertices de partida y llegada
     int hh, mm; // Hora y minutos
     int cos[N], cam[N]; // arreglo de costo y caminos generado por dijkstra
     char ap[2];// formato am/pm de la hora
     

     if (!(f=fopen("plano.in","r"))) /* controlamos si se produce un error */ 
     { 
	    printf("Error al abrir el fichero");	    
	    return; /* abandonamos la carga */ 
     } 
     if (!(pf=fopen("plano.out","w"))) /* controlamos si se produce un error */ 
     { 
	    printf("Error al abrir el fichero"); 
	    return; /* abandonamos la carga */
     }
     else 
     { 
        k=0;
        do{
           fscanf(f,"%d", &vertices);
           if (vertices >= N) //controlamos la capacidad de la matriz
           { 
              printf("\nEl plano excede la capacidad memoria\n\n");
              return;
           }
           if (vertices<=0) break; //cuando ya no hay mas planos
	       /*inicializar las matrices con 0 porque no tienen datos*/
           ini(Gp, Gn, Gb);
           
	       k++; // contamos el numero de planos
		   fprintf(pf,"Plano del Sector: %d\n",k); //escribimos al archivo
		   printf("\nPlano del Sector: %d\n",k);
           do{
		   	  fscanf(f,"%d", &i); //lee del archivo
			  if (i<=0) break; //cuando ya no hay caminos
              fscanf(f,"%d %d %d %d \n", &j, &pico, &normal, &baja); //lee del archivo
			  if (i<=vertices && j<=vertices || j<=0)//si los vertices i, j pertenecen al plano
              {
                 Gp[i-1][j-1]=pico; //matriz de horas pico
			     Gn[i-1][j-1]=normal; //mariz de horas normales
			     Gb[i-1][j-1]=baja; //matriz de horas de bajo flujo
              }
		   }while(i!=0);
		   l=0;
		   do{ 
			  fscanf(f,"%d",&partida);
			  if (partida<=0) break;
			  fscanf(f,"%2d:%2d %2s",&hh, &mm, ap);
			  do{
			    fscanf(f,"%d",&llegada);
				if(llegada<=0) break;//si no hay mas destinos
				//si los vertices de partida y llegada pertenecen al plano
				if (llegada<=vertices && partida<=vertices){
                   for(i=0;i<vertices && j!=-1;i++){
                      if(Gp[partida][i]!=0) j=-1;
                   }
                   if (j==-1){
                      if(verificarhorapico(hh,mm,ap)==true)
                      {
                        alg_Dijkstra(Gp, cos, cam, partida-1, vertices);
                      }
                      else if(verificarhoranormal(hh,mm,ap)==true)
                      {
                         alg_Dijkstra(Gn, cos, cam, partida-1, vertices);
                      }
                      else if (verificarhorabaja(hh,mm,ap)==true)
                      {
                         alg_Dijkstra(Gb, cos, cam, partida-1, vertices);
                      }
                      l++;
                      if (mm<10)fprintf(pf,"C%d: %d - %d - %d:0%1d %c%c\n",l, partida, llegada, hh, mm, ap[0],ap[1]);
                      else fprintf(pf,"C%d: %d - %d - %d:%2d %c%c\n",l, partida, llegada, hh, mm, ap[0],ap[1]);   
				      if (cos[llegada-1]<10000000)
                      {
					     printf("\nRuta mas rapida al vertice %d - %d: %d min",partida,llegada,cos[llegada-1]);
					     printf("\nCamino mas rapido al vertice %d - %d: \n",partida,llegada);
					     m=0;
                         m=camino(cam, llegada-1, partida-1, pf);
					     printf(" A: %d\n", llegada);
					     if (m>=10)fprintf(pf,"\n");
                         fprintf(pf,"%d\n", llegada);
					     fprintf(pf,"%d min\n\n",cos[llegada-1]);
				      }
				      else{ 
                         fprintf(pf,"\"El taxi no puede prestar el servicio.\"\n\n");
                         printf("\nEl taxi no puede prestar el servicio.\n");
                      }
                   }
                   else{ 
                      fprintf(pf,"\"El taxi no puede prestar el servicio.\"\n\n");
                      printf("\nEl taxi no puede prestar el servicio.");
                   }
                }
                else{ 
                      fprintf(pf,"\"El taxi no puede prestar el servicio.\"\n\n");
                      printf("\nEl taxi no puede prestar el servicio.");
                }
              }while(llegada!=0);
		   }while(!feof(f));
	   }while(vertices!= 0);
    }
	fclose(f);
	fclose(pf);
    return;       
}

/********************************************************************/

void alg_Dijkstra(int G[][N],int cos[N], int cam[N], int n, int v)
{
	//variable
	int S[N], V[N], i, aux, k, j;	// En S se van agregando los vщrtices ya tratados 
                                       // i es contador de vщrtices 

	// Aquэ se inicializan todos los caminos 
	for(i=0;i<v;i++) {
       cos[i]= 0;
       cam[i]=n;
       V[i]=0;
	   S[i]=-1;
    }
    S[n]= 0;
	V[n]= -1;
    cos[n]=0;
    
    for(i=0;i<v;i++) {
	  if (i!=n && G[n][i]>0){   
			cos[i]=G[n][i];}
	  else if (i==n)cos[i]=0;
      else{
			cos[i]=10000000;}
    }
	
    for(i=0;i<v-2;i++) {
		aux = 1000;
        for(j=0;j<v;j++) {
           if (V[j] != -1){
			   if(cos[j] < aux){
				   aux=cos[j];
                   k=j;
                   }
           }
        }
        V[k]=-1;
		S[k]=0;
	    for(j=0;j<v;j++) {
			if (V[j]!=-1){
				if (G[k][j]>0){
					if (cos[k] + G[k][j] < cos[j]){
						cos[j] = cos[k] + G[k][j];   
						cam[j] = k;
                    }
			      } 
			   }   
			}
		}
	

}// fin procedimiento alg_Dijkstra //

/********************************************************************/

int camino(int P[N], int j, int n, FILE *pf)
{
    int i, k;
    if (k==10){
     fprintf(pf,"\n");
     k=0;
    }
    if (P[j]==n){
       printf("Desde: %d,", P[j]+1);
       fprintf(pf,"%d - ", P[j]+1);
       k=1;
    }
    else{
       i=P[j];
       k=camino(P, i, n, pf);
       if (k==10){
          fprintf(pf,"\n");
          k=0;
       }
       k++;
       fprintf(pf,"%d - ", P[j]+1);
       printf(" A: %d,", P[j]+1);
    }
	return(k);
}

/********************************************************************/

void ini(int Gp[][N], int Gn[][N], int Gb[][N])
{
    int i,j;
    for(i=0;i<N;i++){
       for (j=0;j<N;j++){
          Gp[i][j]=0;
          Gn[i][j]=0;
		  Gb[i][j]=0;
       }
    }
    return;
}

/********************************************************************/
/*Horas pico	Horario: 	7:00 am - 9:00 am,	11:30 am - 2:30 pm,	5:30 pm - 7:30 pm */
bool verificarhorapico(int hh, int mm, char ap[])
{
     bool res=false;
     if (hh>=7 && hh<9 && compararcadenas("am",ap)==0) res=true;
     else if (hh==9 && mm==0 && compararcadenas("am",ap)==0) res=true;
     else if (hh==11 && mm>=30 && compararcadenas("am",ap)==0) res=true;
     else if (hh==12 && compararcadenas("pm",ap)==0) res=true;
     else if (hh==1 && compararcadenas("pm",ap)==0) res=true;
     else if (hh==2 && mm<=30 && compararcadenas("pm",ap)==0) res=true;
     else if (hh==5 && mm>=30 && compararcadenas("pm",ap)==0) res=true;
     else if (hh==6 && compararcadenas("pm",ap)==0) res=true;
     else if (hh==7 && mm<=30 && compararcadenas("pm",ap)==0) res=true;
     return (res);             
}

/********************************************************************/
/*Horas normales	Horario:	9:01 am - 11:29 am,	2:31 pm - 5:29 pm, 	7:31 pm - 9:59 pm */
bool verificarhoranormal(int hh, int mm, char ap[])
{
     bool res=false;
     if (hh>=9 && hh<11 && compararcadenas("am",ap)==0) res=true;
     else if (hh==11 && mm<30 && compararcadenas("am",ap)==0) res=true;
     else if (hh==2 && mm>30 && compararcadenas("pm",ap)==0) res=true;
     else if (hh>=3 && hh<5 && compararcadenas("pm",ap)==0) res=true;
     else if (hh==5 && mm<30  && compararcadenas("pm",ap)==0) res=true;     
     else if (hh==7 && mm>30 && compararcadenas("pm",ap)==0) res=true;
     else if (hh>=8 && hh<10 && compararcadenas("pm",ap)==0) res=true;
     return (res);                 
}

/********************************************************************/
/*Horas bajo flujo	Horario:	10:00 pm - 6:59 am */
bool verificarhorabaja(int hh, int mm, char ap[])
{
     bool res=false;
     if ((hh>=10 && hh<12 && compararcadenas("pm",ap)==0)) res=true;
     else if (hh==12 && compararcadenas("am",ap)==0) res=true;
     else if (hh>=1 && hh<7  && compararcadenas("am",ap)==0) res=true;
     return (res);           
}	

/********************************************************************/
   
void mostrar_e()
{
     FILE *pf;
     char s[50];
     if (!(pf=fopen("plano.in","r"))) /* controlamos si se produce un error */ 
     { 
	    printf("Error al abrir el fichero"); 
	    return; /* abandonamos la carga */
     }
     else 
     { 
        do{
           fgets(s,50,pf); //lee del archivo
           printf("%s",s);         
        }while(!feof(pf));
     }
     fclose(pf);
     return;
}

/********************************************************************/

void mostrar_s()
{
     FILE *pf;
     char s[50];
     if (!(pf=fopen("plano.out","r"))) /* controlamos si se produce un error */ 
     { 
	    printf("Error al abrir el fichero"); 
	    return; /* abandonamos la carga */
     }
     else 
     { 
        do{
           fgets(s,50,pf); //lee del archivo
           printf("%s",s);         
        }while(!feof(pf));
     }
     fclose(pf);
     return;
}


