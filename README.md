= Systemas Operativos: Trabajo Practico de Semaforos
 Sofia Melina_Alcaraz <sofiialcaraz16@gmail.com>
 v1, {docdate}.Docentes Mariano Vargas, Noelia Sosa e Ignacio Tula (COM-01)
 :title-page:
 :numbered:
 :source-highlighter: coderay
 :tabsize: 4

== Introducción

Ejercicio Subway Argento con Semaforos:
En la cocina de Subway Argento compiten cuatro equipos de cocineros para ganarse el puesto de trabajo en el local. ¿Quién será el primero en entregar 1 sándwich de milanesa terminado, de 15 cm, al dueño del local? La dificultad de la competencia es que la cocina no cuenta con el equipamiento individual  para cada equipo, sino que algunos de ellos son compartidos. Se tienen que ordenar para cocinar las milanesas ya que solo tienen un sartén para todos los equipos y este cocina una milanesa a la vez, existe solo un horno que cocina dos panes a la vez  y un solo salero.
En el siguiente informe se explicara como se resolvió el trabajo utilizando semaforos e hilos.
 
== Descripción

Primero realize el pseudocodigo que se mostrara en la seccion implementacion, realize una prueba
de escritorio y pasé al codigo.
Una vez que vi el video proporcioando por los profesores sobre el funcionamiento del codigo
dado, comence a completar el codigo.
Una vez terminado, probé el programa y funcionaba correctamente.
Luego trate de leer una archivo de texto pero no lo logré, asi que pase a escribir la salida en un txt.
Para que no se pise lo ya escrito use en fopen el valor "a ", que crea un txt si no existe, agrega lineas y no sobreeescribe.
Tambien cambie los printf por fprinf y le añadi  el archivo a escribir.
Para hacer la parte de escribir se me ocurrió utilizar dos while uno que lee por linea hasta el final del archivo
y otro que lee por tokens,separados por ";" hasta el final de la linea. Ademas dos iteradores para pasarle
al comparador csrpy.

== Implementación
Los semáforos mutex Individuales: sem_mezclar, sem_ponerSal, sem_agregarCarne, sem_empanar, sem_usarSarten,sem_listo1, sem_listo2, sem_listo3,sem_listo4,sem_entregar.
Semaforos compartidos mutex : sem_usarSalero, sem_usarSarten.
Semáforo contador compartido : sem_usarHorno==2.

Cada hilo haría una acción y funcionaria de la siguiente forma:
 sem_mezclar        0
 sem_ponerSal       0
 sem_agregarCarne   0
 sem_empanar        0
 sem_usarSarten     0
 sem_listo1         0
 sem_listo2         0
 sem_listo3         0
 sem_listo4         0
 sem_entregar       0

sem_usarSalero      1     
sem_usarSarten      1

sem_usarHorno       2

cortar1(){

		corta()
		v(sem_mezclar)

}
mezclar(){
	
		p(sem_mezclar)
		mezcla()
		v(sem_ponerSal)

}
ponerSal(){
	
		p(sem_ponerSal)
    		p(sem_usarSalero)
		usarSal()
    		v(sem_usarSalero)
		v(sem_agregarCarne)

}
agregarCarne{
	
		p(sem_agregarCarne)
		agregarLaCarne()
		v(empanar)

}
empanar(){
	
		p(sem_empanar)
		empanar()
		v(sem_usarSarten)

}
usarSarten(){
	
		p(sem_usarSarten)
     		p(sem_usarSarten);//compartido
		usarSarten()
     		v(sem_usarSarten)//compartido
    		v(sem_listo1)
 
}
usarHorno(){
	
		p(sem_usarHorno)//compartido
		uasrHorno()
    		p(sem_usarHorno)
		v(sem_listo2)

}
cortar2(){

		cortar2()
		v(sem_listo3)

}
cortar3(){

		cortar3()
		v(sem_listo4)

}
armar(){
	
		p(sem_listo1)
   		p(sem_listo2)
    		p(sem_listo3)
		p(sem_listo4)
		armar()
		v(sem_entregar)

}
entregar(){
	
		p(sem_entregar)
		entregar()

}

Cada hilo encendería al próximo semáforo para que se ejecute de forma correcta.
 Luego vi el video de noelia y prepare todo para llevar mi idea a codigo.
El tema de hilos fue sencillo, el problema luego de eso era leer de un archivo
 y que la salida salga en otro.
 
 
Para leer el archivo de texto pense los siguente:
i=0
j=0
while(llegue al final del fichero){
	cad=extraigo una linea del txt
	palabra=accion
  (paso la accion ,i)
	while(llego al final de la linea){
		otraPalabra=ingrediente 
	  (paso la ccion con el i y los ingredientes con la j)
    j++
  }
  i++
}
Para que esto funcione separe las palabras con “;” y use la siguiente estructura en el archivo 
receta.txt:
accion;ingrediente;ingrediente2;

== Conclusion

 EL codigo era bastante claro asi que no me costo mucho entenderlo, la parte de semaforos no me costo tanto ya que realize las practicas.
 El trabajo me ayudo mucho para refrescar temas y aprender cosas nuevas.
