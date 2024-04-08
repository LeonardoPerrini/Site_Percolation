/*Leonardo Perrini 2065029
Questo programma genera un Lattice quadrato di lato L occupandolo con nodi funzionanti o guasti con probabilità
p. Realizza poi una clusterizzazione mediante una variante basata sull'algoritmo di Hoshen-Kopelman individuando tutti
i cluster connessi (solo dei nodi guasti), utile per realizzare un render con pyhton della clusterizzazione*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define L 2000  // dimensione del lattice
#define p 0.593  // probabilità di avere un nodo guasto

#define max(a,b) (a>b?a:b)  // definizione di funzioni max e min
#define min(a,b) (a>b?b:a)

typedef unsigned long int num;

num *labels;     // array delle labels, dove x è la label attuale che ha un dato cluster e labels[x] è la label che dovrebbe avere (ad esempio in seguito ad un' unione), labels[0] è il numero totale di labels diverse
num  n_labels;     // numero massimo di labels diverse (caso peggiore L*L, realistico L*L/2) 

num lattice[L][L];  // lattice

void initLattice(void){  // inizializzazione del reticolo e dei nodi
    double x;
    for(num i=0; i<L; i++){
        for(num j=0; j<L; j++){
            x = drand48();
            if(x<p){
                lattice[i][j] = 1;
            }
            else{
                lattice[i][j] = 0;
            }
        }
    }
}

num find(num x){  //funzione find dell'algoritmo di Union-Find
  num y = x;      //trova la label del cluster di un nodo
  while(labels[y] != y){
    y = labels[y];
  }
  while(labels[x] != x){  //questa parte della funzione è il cuore di tutto
    num temp = labels[x];  //perché cambia la label di tutti i nodi di un dato cluster
    labels[x] = y;
    x = temp;
  }
  return y;
}

num unione(num x, num y) {  //funzione union dell'algoritmo di Union-Find
  return labels[find(x)] = find(y);
}

num new_cluster(void){  //funzione che crea una nuova label (un nuovo cluster)
  labels[0]++;
  labels[labels[0]] = labels[0];
  return labels[0];
}

void initialize_labels(num max_labels){  //inizializza l'array delle labels
  n_labels = max_labels;
  labels = calloc(n_labels, sizeof(num));
  labels[0] = 0;  // labels[0] è il numero totale di labels diverse (cioè il numero di cluster)
}

void clean(void){  //libera l'array delle labels
  n_labels = 0;
  free(labels);
  labels = 0;
}

void print_lattice(FILE *fp){  //stampa il lattice su file
  for (num i=0; i<L; i++){
    for (num j=0; j<L; j++){
      fprintf(fp, "%lu ",lattice[i][j]);
    }
    fprintf(fp, "\n");
  }
}

num clusterize(void){  //funzione prinicipale che clusterizza il lattice
  initialize_labels(L*L);
  for(num i=0; i<L; i++){
    for(num j=0; j<L; j++){
      if(lattice[i][j] != 0){
        num up = (i==0 ? 0 : lattice[i-1][j]);    //  vicino in alto  
	      num left = (j==0 ? 0 : lattice[i][j-1]);  //  vicino a sinistra
        switch (!!up + !!left){  //
          case 0:
            lattice[i][j] = new_cluster();
            break;
          case 1:
            lattice[i][j] = max(up,left);
            break;
          case 2:
            lattice[i][j] = unione(up, left);
            break;
        }
      }
    }
  }
  num *new_labels = calloc(n_labels, sizeof(num));  //mappo le labels in nuove labels che partono da 1 in ordine sicuramente crescente
  for(num i=0; i<L; i++){
    for(num j=0; j<L ; j++){
      if(lattice[i][j] != 0){
        num x = find(lattice[i][j]);
        if(new_labels[x] == 0){
          new_labels[0]++;
          new_labels[x] = new_labels[0];
        }
        lattice[i][j] = new_labels[x];
      }
    }
  }

  num total_clusters = new_labels[0];  //numero totale di cluster
  free(new_labels);
  clean();

  return total_clusters;
}

num percolato(void){  //funzione che controlla se il lattice percola
  num controllo = 0;
  for(num i=0; i<L; i++){
    if(lattice[i][0]==lattice[i][L-1] && lattice[i][0]!=0){
      controllo = lattice[i][0];
      break;  
    }
    else if(lattice[0][i]==lattice[L-1][i] && lattice[0][i]!=0){
      controllo = lattice[0][i];
      break;
    }
  }
  return controllo;  //se il lattice percola ritorna la label del cluster percolante, altrimenti 0
}

int main(void){
  clock_t start, end;
  start = clock();
  srand48(time(NULL));
  FILE *fp;
  fp = fopen("percolation.dat", "w");

  initLattice();
    
  num clusters = clusterize();
    
  print_lattice(fp);
    
  printf("Sono stati trovati %lu clusters connessi\n", clusters);

  fclose(fp);
  end = clock();
  printf("Tempo di esecuzione: %f s\n", ((double) (end - start)) / CLOCKS_PER_SEC);
  
  return 0;
}