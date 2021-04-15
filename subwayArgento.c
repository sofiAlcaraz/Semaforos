#include <stdio.h>      // libreria estandar
#include <stdlib.h>     // para usar exit y funciones de la libreria standard
#include <string.h>
#include <pthread.h>    // para usar threads
#include <semaphore.h>  // para usar semaforos
#include <unistd.h>

#define LIMITE 50
//creo semaforos "compartidos"
sem_t sem_usarSalero;
sem_t sem_usarSarten;
sem_t sem_usarHorno;

//creo estructura de semaforos 
struct semaforos {
	sem_t sem_mezclar;
	sem_t sem_ponerSal;
	sem_t sem_agregarCarne;
	sem_t sem_empanar;
	sem_t sem_usarSarten;
	sem_t sem_listo1;
	sem_t sem_listo2;
	sem_t sem_listo3;
	sem_t sem_listo4;
	sem_t sem_entregar;
	//poner demas semaforos aqui
};

//creo los pasos con los ingredientes
struct paso {
   char accion [LIMITE];
   char ingredientes[4][LIMITE];

};

//creo los parametros de los hilos 
struct parametro {
 int equipo_param;
 struct semaforos semaforos_param;
 struct paso pasos_param[12];
};

//funcion para imprimir las acciones y los ingredientes de la accion

void* imprimirAccion(void *data, char *accionIn) {
	FILE *archivo;
	archivo=fopen("salidaReceta","a");
	struct parametro *mydata = data;
	//calculo la longitud del array de pasos 
	int sizeArray = (int)( sizeof(mydata->pasos_param) / sizeof(mydata->pasos_param[0]));
	//indice para recorrer array de pasos 
	int i;
	for(i = 0; i < sizeArray; i ++){
		//pregunto si la accion del array es igual a la pasada por parametro (si es igual la funcion strcmp devuelve cero)
		if(strcmp(mydata->pasos_param[i].accion, accionIn) == 0){
		fprintf(archivo,"\tEquipo %d - accion %s \n " , mydata->equipo_param, mydata->pasos_param[i].accion);
		//calculo la longitud del array de ingredientes
		int sizeArrayIngredientes = (int)( sizeof(mydata->pasos_param[i].ingredientes) / sizeof(mydata->pasos_param[i].ingredientes[0]) );
		//indice para recorrer array de ingredientes
		int h;
		fprintf(archivo,"\tEquipo %d -----------ingredientes : ----------\n",mydata->equipo_param); 
			for(h = 0; h < sizeArrayIngredientes; h++) {
				//consulto si la posicion tiene valor porque no se cuantos ingredientes tengo por accion 
				if(strlen(mydata->pasos_param[i].ingredientes[h]) != 0) {
							fprintf(archivo,"\tEquipo %d ingrediente  %d : %s \n",mydata->equipo_param,h,mydata->pasos_param[i].ingredientes[h]);
				}
			}
		}
	}
}

//funcion para tomar de ejemplo
void* cortar1(void *data) {
	//creo el nombre de la accion de la funcion 
	char *accion = "cortar1";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 30000 );
	//doy la señal a la siguiente accion (cortar me habilita mezclar)
    sem_post(&mydata->semaforos_param.sem_mezclar);

    pthread_exit(NULL);
}
void* mezclar(void *data){
	char *accion="mezclar";
	struct parametro *mydata = data;
	sem_wait(&mydata->semaforos_param.sem_mezclar);
	imprimirAccion(mydata,accion);
	usleep(3000);
	sem_post(&mydata->semaforos_param.sem_ponerSal);

}
void* ponerSal(void *data){

	char *accion="ponerSal";
	struct parametro *mydata = data;
	sem_wait(&mydata->semaforos_param.sem_ponerSal);
	sem_wait(&sem_usarSalero);
	imprimirAccion(mydata,accion);
	usleep(20000);
	sem_post(&sem_usarSalero);
	sem_post(&mydata->semaforos_param.sem_agregarCarne);
}
void* agregarCarne(void *data){
	char *accion="agregarCarne";
	struct parametro *mydata=data;
	sem_wait(&mydata->semaforos_param.sem_agregarCarne);
	imprimirAccion(mydata,accion);
	usleep(20000);
	sem_post(&mydata->semaforos_param.sem_empanar);

}
void* empanar(void *data){
	char *accion="empanar";
	struct parametro *mydata=data;
	sem_wait(&mydata->semaforos_param.sem_empanar);
	imprimirAccion(mydata,accion);
	usleep(40000);
	sem_post(&mydata->semaforos_param.sem_usarSarten);

}
void* usarSarten(void *data){
	char *accion="usarSarten";
	struct parametro *mydata=data;
	sem_wait(&mydata->semaforos_param.sem_usarSarten);
	sem_wait(&sem_usarSarten);
	imprimirAccion(mydata ,accion);
	usleep(5);
	sem_post(&sem_usarSarten);
	sem_post(&mydata->semaforos_param.sem_listo1);
}


void* usarHorno(void *data){

	sem_wait(&sem_usarHorno);
	char *accion="usarHorno";
	struct parametro *mydata=data;
	imprimirAccion(mydata,accion);
	usleep(50000);
	sem_post(&sem_usarHorno);
	sem_post(&mydata->semaforos_param.sem_listo2);

}
void* cortar2(void *data){
	char *accion ="cortar2";
	struct parametro *mydata=data;
	imprimirAccion(mydata,accion);
	usleep(3);
	sem_post(&mydata->semaforos_param.sem_listo3);
}
void* cortar3(void *data){
	char *accion="cortar3";
	struct parametro *mydata=data;
	imprimirAccion(mydata,accion);
	usleep(3);
	sem_post(&mydata->semaforos_param.sem_listo4);
}
void* armar(void *data){
	char *accion="armar";
	struct parametro *mydata=data;
	sem_wait(&mydata->semaforos_param.sem_listo1);
	sem_wait(&mydata->semaforos_param.sem_listo2);
	sem_wait(&mydata->semaforos_param.sem_listo3);
	sem_wait(&mydata->semaforos_param.sem_listo4);
	imprimirAccion(mydata,accion);
	usleep(3);
	sem_post(&mydata->semaforos_param.sem_entregar);

}
void*  entregar(void *data){
	char *accion="entregar";
	struct parametro *mydata=data;
	sem_wait(&mydata->semaforos_param.sem_entregar);
	imprimirAccion(mydata,accion);
	usleep(3);
}

void* ejecutarReceta(void *i) {

	FILE *archivo;
	archivo=fopen("receta.txt","r");

	//variables semaforos
	sem_t sem_mezclar;
	sem_t sem_ponerSal;
	sem_t sem_agregarCarne;
	sem_t sem_empanar;
	sem_t sem_usarSarten;
	sem_t sem_listo1;
	sem_t sem_listo2;
	sem_t sem_listo3;
	sem_t sem_listo4;
	sem_t sem_entregar;
	//variables hilos
	pthread_t p1;
	pthread_t p2;
	pthread_t p3;
	pthread_t p4;
	pthread_t p5;
	pthread_t p6;
	pthread_t p7;
	pthread_t p8;
	pthread_t p9;
	pthread_t p10;
	pthread_t p11;
	//numero del equipo (casteo el puntero a un int)
	int p = *((int *) i);
	
	printf("Ejecutando equipo %d \n", p);

	//reservo memoria para el struct
	struct parametro *pthread_data = malloc(sizeof(struct parametro));

	//seteo los valores al struct

	//seteo numero de grupo
	pthread_data->equipo_param = p;

	//seteo semaforos
	//setear demas semaforos al struct aqui
	pthread_data->semaforos_param.sem_mezclar = sem_mezclar;
	pthread_data->semaforos_param.sem_ponerSal = sem_ponerSal;
	pthread_data->semaforos_param.sem_agregarCarne = sem_agregarCarne;
	pthread_data->semaforos_param.sem_empanar = sem_empanar;
	pthread_data->semaforos_param.sem_usarSarten = sem_usarSarten;
	pthread_data->semaforos_param.sem_listo1 = sem_listo1;
	pthread_data->semaforos_param.sem_listo2 = sem_listo2;
	pthread_data->semaforos_param.sem_listo3 = sem_listo3;
	pthread_data->semaforos_param.sem_listo4 = sem_listo4;
	pthread_data->semaforos_param.sem_entregar = sem_entregar;

	//seteo las acciones y los ingredientes (Faltan acciones e ingredientes) ¿Se ve hardcodeado no? ¿Les parece bien?
     	/*strcpy(pthread_data->pasos_param[0].accion, "cortar1");
	strcpy(pthread_data->pasos_param[0].ingredientes[0], "ajo");
        strcpy(pthread_data->pasos_param[0].ingredientes[1], "perejil");
	    strcpy(pthread_data->pasos_param[1].accion, "mezclar");
	strcpy(pthread_data->pasos_param[1].ingredientes[0], "ajo");
      strcpy(pthread_data->pasos_param[1].ingredientes[1], "perejil");
 	 strcpy(pthread_data->pasos_param[1].ingredientes[2], "huevo");
	strcpy(pthread_data->pasos_param[1].ingredientes[3], "carne");
	strcpy(pthread_data->pasos_param[2].accion,"ponerSal");
	strcpy(pthread_data->pasos_param[2].ingredientes[0],"mezcla");
	strcpy(pthread_data->pasos_param[3].accion,"agregarCarne");
	strcpy(pthread_data->pasos_param[3].ingredientes[0], "mezcla");
	strcpy(pthread_data->pasos_param[4].accion,"empanar");
	strcpy(pthread_data->pasos_param[5].accion,"usarSarten");
	strcpy(pthread_data->pasos_param[6].accion,"usarHorno");
	strcpy(pthread_data->pasos_param[7].accion,"cortar2");
	strcpy(pthread_data->pasos_param[8].accion,"cortar3");
	strcpy(pthread_data->pasos_param[9].accion,"armar");
	strcpy(pthread_data->pasos_param[10].accion,"entregar");
	*/
	char cad[70];
	int k=0;
	int j=0;
	while(fgets(cad,50,archivo)!=NULL){//lee por lineas,hasta ultim linea
		//fgets(cad,30,archivo);
		//printf("%s",cad);
		char *token=strtok(cad,";");
		//printf("%s\n",token);
		strcpy(pthread_data->pasos_param[k].accion,token);
		while(token!=NULL){
			strcpy(pthread_data->pasos_param[k].ingredientes[j],token);
			token=strtok(NULL,";");
			j++;
		}
		k++;
	}

	//inicializo los semaforos
	//inicializar demas semaforos aqui
	sem_init(&(pthread_data->semaforos_param.sem_mezclar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_ponerSal),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_agregarCarne),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_empanar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_usarSarten),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_listo1),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_listo2),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_listo3),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_listo4),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_entregar),0,0);

	//creo los hilos a todos les paso el struct creado (el mismo a todos los hilos) ya que todos comparten los semaforos 
	int rc;

    rc = pthread_create(&p1,                           //identificador unico
                            NULL,                          //atributos del thread
                                cortar1,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
	//crear demas hilos aqui
	rc=pthread_create(&p2,NULL,mezclar,pthread_data);
	rc=pthread_create(&p3,NULL,ponerSal,pthread_data);
	rc=pthread_create(&p4,NULL,agregarCarne,pthread_data);
	rc=pthread_create(&p5,NULL,empanar,pthread_data);
	rc=pthread_create(&p6,NULL,usarSarten,pthread_data);
	rc=pthread_create(&p7,NULL,usarHorno,pthread_data);
	rc=pthread_create(&p8,NULL,cortar2,pthread_data);
	rc=pthread_create(&p9,NULL,cortar3,pthread_data);
	rc=pthread_create(&p10,NULL,armar,pthread_data);
	rc=pthread_create(&p11,NULL,entregar,pthread_data);
	//join de todos los hilos
	pthread_join (p1,NULL);
	//crear join de demas hilos
	pthread_join (p2,NULL);
	pthread_join (p3,NULL);
	pthread_join (p4,NULL);
	pthread_join (p5,NULL);
	pthread_join (p6,NULL);
	pthread_join (p7,NULL);
	pthread_join (p7,NULL);
	pthread_join (p8,NULL);
	pthread_join (p9,NULL);
	pthread_join (p10,NULL);
	pthread_join (p11,NULL);
	//valido que el hilo se alla creado bien 
    if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1);
     }
	//destruccion de los semaforos 
	sem_destroy(&sem_mezclar);
	//destruir demas semaforos 
	sem_destroy(&sem_ponerSal);
	sem_destroy(&sem_agregarCarne);
	sem_destroy(&sem_empanar);
	sem_destroy(&sem_usarSarten);
	sem_destroy(&sem_listo1);
	sem_destroy(&sem_listo2);
	sem_destroy(&sem_listo3);
	sem_destroy(&sem_listo4);
	sem_destroy(&sem_entregar);
	//cierro archivo de texto
	fclose(archivo);
	//salida del hilo
	 pthread_exit(NULL);
}


int main ()
{
	//inicializo semaforos
	sem_init(&sem_usarSalero,0,1);
	sem_init(&sem_usarSarten,0,1);
	sem_init(&sem_usarHorno,0,2);
	//creo los nombres de los equipos 
	int rc;
	int *equipoNombre1 =malloc(sizeof(*equipoNombre1));
	int *equipoNombre2 =malloc(sizeof(*equipoNombre2));
	int *equipoNombre3 =malloc(sizeof(*equipoNombre3));
//faltan equipos
	int *equipoNombre4 =malloc(sizeof(*equipoNombre4));

	*equipoNombre1 = 1;
	*equipoNombre2 = 2;
	*equipoNombre3 = 3;
	*equipoNombre4 = 4;
	//creo las variables los hilos de los equipos
	pthread_t equipo1;
	pthread_t equipo2;
	pthread_t equipo3;
	pthread_t equipo4;

	//inicializo los hilos de los equipos
    rc = pthread_create(&equipo1,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre1);

    rc = pthread_create(&equipo2,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre2);
  //faltn inicializaciones
	rc=pthread_create(&equipo3,NULL,ejecutarReceta,equipoNombre3);
	rc=pthread_create(&equipo4,NULL,ejecutarReceta,equipoNombre4);

   if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1);
     } 

	//join de todos los hilos
	pthread_join (equipo1,NULL);
	pthread_join (equipo2,NULL);
	pthread_join (equipo3,NULL);
	pthread_join (equipo4,NULL);
	//
	sem_destroy(&sem_usarSalero);
	sem_destroy(&sem_usarSarten);
	sem_destroy(&sem_usarHorno);

    pthread_exit(NULL);
	return -1;
}


//Para compilar:   gcc subwayArgento.c -o ejecutable -lpthread
//Para ejecutar:   ./ejecutable
