#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
int *solucione; //la matriz solucion



//Código realizado por Álvaro Sánchez Pinedo asp161
// Universidad de Alicante
//Grado Ing.Multimedia



void alarma(){

	printf("acabo de recibir un sigalarm\n");
}

void mostrar(){
	sleep(1);
	printf("LA MATRIZ RESULTANTE FINAL ES:  ");
	for(int p=0; p<9; p++){
		printf("%d ", *(solucione+p));
	}
	printf("\n");

}


int main(int argc, char*argv[]) 
{ 
	// Extraemos la matriz del archivo y la almacenamos en un vector de enteros
    int datoExtraido[18]; char textoExtraido [50];  //Variables de apoyo para las 3 matrizes, 
    int *matrizaux;	 							//las que obtenemos del fichero y la solución de la multiplicación

    FILE* fichero;
    fichero = fopen(argv[1], "rt"); //Abrir el fichero y leerlo con el bucle for, a su vez guardarlo en el datoExtraido
    								//DatoExtraido va a ser nuestro vector que guarde las dos matrices y matrizaux es donde
    								//guardaremos la matriz en memoria compartida
    for(int i=0; i<18; i++){

    	if(i==9){
    		printf("\n");
    	}
    fscanf (fichero, "%d", &datoExtraido[i]);
    printf("%d ",datoExtraido[i]);
    }
    fgets (textoExtraido, 50, fichero);
    fclose(fichero);


	char caracter = (char)argv[2][0];
    int comoEntero = caracter - '0';
   	printf("El carácter '%c' es %d en entero", caracter, comoEntero);




	pid_t* mispids;

// se crea la memoria compartida 
	int shmid = shmget(IPC_PRIVATE,sizeof(pid_t),IPC_CREAT|0666);
	int shmid2 = shmget(IPC_PRIVATE,sizeof(int*),IPC_CREAT|0666);
	int shmid3 = shmget(IPC_PRIVATE,sizeof(int*),IPC_CREAT|0666);
	int shmid4 = shmget(IPC_PRIVATE,sizeof(int*),IPC_CREAT|0666);




	//Variables de apoyo para la enumeración de la matriz resultante
	int *n=0;
	int cont;


// se vincula la memoria a las variables correspondientes
	mispids = (int *)shmat(shmid,0,0); 
	n=shmat(shmid2,0,0);
	pid_t padre = getpid(); 
	matrizaux=(int *)shmat(shmid3,0,0);
	solucione=(int *)shmat(shmid4,0,0);

		//Paso los valores de mi vector auxiliar a memoria compartida con otro vector llamado matrizaux,
		//La solucion mas adelante la guardare en un vector de memoria compartida llamado solucione
		for(int o=0; o<=17; o++){

		*(matrizaux + o) = datoExtraido[o];
	}





	//Aqui muestro por pantalla el pid de mis memorias compartidas
	*(mispids) = getpid(); 
		printf("\nSoy el proceso principal, mi pid es %d, y he guardado en memoria %d y el shmid es %d, ",getpid(),(int)(*(mispids)),shmid); 
		printf("Variable auxiliar %d, ",shmid2);
		printf("matriz donde guardo lo recibido del archivo txt %d, ",shmid3);
		printf("matriz donde guardo la solucion de la multiplicacion %d \n",shmid4);


//señales 
		signal(SIGUSR1, mostrar); //vencimiento de alarma
// generamos los hijos en horizontal que escriben en memoria  
		//Los he generado de 3 en 3 tal como el árbol de estructura del ejercicio. salida-----3*[salida----salida------salida]

	
	for(int j=0;j<3;j++){ 

			for(int f=0; f<3;f++){

			signal(SIGALRM,alarma);
			alarm(comoEntero);
			pause();

		}
				sleep(1);
		pid_t pidy = fork(); 
		if(pidy==0)

			/*signal(SIGALRM,alarma);
			alarm(comoEntero);
			pause();*/


		  //  Creo aqui 3 hijos dependiendo lo que vale la j


		{
			n=shmat(shmid2,0,0);
			*n=*n+1;

			mispids = (int *)shmat(shmid,0,0); // se vincula la memoria en los procesos hijo 
			*(mispids+j+1) = getpid();  // se escribe en memoria 



			if(j==0){
				*(solucione+0)=(*(matrizaux+0) * *(matrizaux+9) + *(matrizaux+1) * *(matrizaux+12) + *(matrizaux+2) * *(matrizaux+15));
				printf(" Soy el hijo con pid %d, la j es %d y he guardado en memoria %d, Mi resultado para la posicion %d es: %d\n",getpid(),j,(int)(*(mispids+j+1)),*n,*(solucione+*n-1)); 


			}else if(j==1){
				*(solucione+3)=(*(matrizaux+3) * *(matrizaux+9) + *(matrizaux+4) * *(matrizaux+12) + *(matrizaux+5) * *(matrizaux+15));
				printf(" Soy el hijo con pid %d, la j es %d y he guardado en memoria %d, Mi resultado para la posicion %d es: %d\n",getpid(),j,(int)(*(mispids+j+1)),*n,*(solucione+*n-1));



			}else{
				*(solucione+6)=(*(matrizaux+6) * *(matrizaux+9) + *(matrizaux+7) * *(matrizaux+12) + *(matrizaux+8) * *(matrizaux+15));
				printf(" Soy el hijo con pid %d, la j es %d y he guardado en memoria %d, Mi resultado para la posicion %d es: %d\n",getpid(),j,(int)(*(mispids+j+1)),*n,*(solucione+*n-1)); 



			}
	

		pid_t pidy = fork(); 
		if(pidy==0)  


			//  Aqui igual 3 hijos con lo que vale la j


		{ 
			n=shmat(shmid2,0,0);
			*n=*n+1;
			mispids = (int *)shmat(shmid,0,0); // se vincula la memoria en los procesos hijo 
			*(mispids+j+1) = getpid();  // se escribe en memoria

			

			if(j==0){
				*(solucione+1)=(*(matrizaux+0) * *(matrizaux+10) + *(matrizaux+1) * *(matrizaux+13) + *(matrizaux+2) * *(matrizaux+16));
				printf(" Soy el hijo con pid %d, la j es %d y he guardado en memoria %d, Mi resultado para la posicion %d es: %d\n",getpid(),j,(int)(*(mispids+j+1)),*n,*(solucione+*n-1)); 



			}else if(j==1){
				*(solucione+4)=(*(matrizaux+3) * *(matrizaux+10) + *(matrizaux+4) * *(matrizaux+13) + *(matrizaux+5) * *(matrizaux+16));
				printf(" Soy el hijo con pid %d, la j es %d y he guardado en memoria %d, Mi resultado para la posicion %d es: %d\n",getpid(),j,(int)(*(mispids+j+1)),*n,*(solucione+*n-1)); 


			} else{
				*(solucione+7)=(*(matrizaux+6) * *(matrizaux+10) + *(matrizaux+7) * *(matrizaux+13) + *(matrizaux+8) * *(matrizaux+16));
				printf(" Soy el hijo con pid %d, la j es %d y he guardado en memoria %d, Mi resultado para la posicion %d es: %d\n",getpid(),j,(int)(*(mispids+j+1)),*n,*(solucione+*n-1)); 


			}


		pid_t pidy = fork(); 
		if(pidy==0)


		  //  Igual otros 3 procesos hijos


		{ 

			n=shmat(shmid2,0,0);
			*n=*n+1;
			mispids = (int *)shmat(shmid,0,0); // se vincula la memoria en los procesos hijo 
			*(mispids+j+1) = getpid();  // se escribe en memoria


			if(j==0){
				*(solucione+2)=(*(matrizaux+0) * *(matrizaux+11) + *(matrizaux+1) * *(matrizaux+14) + *(matrizaux+2)* *(matrizaux+17));
				printf(" Soy el hijo con pid %d, la j es %d y he guardado en memoria %d, Mi resultado para la posicion %d es: %d\n",getpid(),j,(int)(*(mispids+j+1)),*n,*(solucione+*n-1)); 


			}else if(j==1){
				*(solucione+5)=(*(matrizaux+3) * *(matrizaux+11) + *(matrizaux+4) * *(matrizaux+14) + *(matrizaux+5) * *(matrizaux+17));
				printf(" Soy el hijo con pid %d, la j es %d y he guardado en memoria %d, Mi resultado para la posicion %d es: %d\n",getpid(),j,(int)(*(mispids+j+1)),*n,*(solucione+*n-1)); 



			}else{
				*(solucione+8)=(*(matrizaux+6) * *(matrizaux+11) + *(matrizaux+7) * *(matrizaux+14) + *(matrizaux+8) * *(matrizaux+17));
				printf(" Soy el hijo con pid %d, la j es %d y he guardado en memoria %d, Mi resultado para la posicion %d es: %d\n",getpid(),j,(int)(*(mispids+j+1)),*n,*(solucione+*n-1));

				kill(padre, SIGUSR1); // el último hijo mata al padre


			}





		break; 
		
	}

		break; 
		}
		break; 
		}   
} 

//debería mostrar por pantalla los valores de la matriz 1 a 1 de cada posición, entendiendo por posición de 1-9 dentro de una matriz 3x3

// Tenemos el arbol, ponemos un sleep(15) para probar el pstree 


		//sleep(7); 



// proceso de lectura de memoria desde el proceso principal 



		if (getpid() != padre) { 

// se desvincula la memoria desde los procesos hijo, en el if se indica. Desvinculamos todo lo que hemos creado

		//pause();
		shmdt((pid_t *)mispids);
		shmdt((int *)n);
		shmdt((int *)matrizaux);
		shmdt((int *)solucione);


		printf("se desvincula la memoria desde los procesos hijo\n");



		} 
		else  
		{ 

		//sleep(2); 



		pause();

// se desvincula la memoria y se borra del padre, ya que en el if indicamos que estamos en el padre.
	//Al ser el padre utilizamos shmctl para borrar completamente las memorias


			
		shmdt((pid_t *)mispids);
		shmdt((int *)n);
		shmdt((int *)matrizaux);
		shmdt((int *)solucione);
		
		printf("se desvincula la memoria y se borra\n");

		shmctl(shmid,IPC_RMID,0);
		shmctl(shmid2,IPC_RMID,0);
		shmctl(shmid3,IPC_RMID,0);
		shmctl(shmid4,IPC_RMID,0);
		




		}


	return  0;  
	}

