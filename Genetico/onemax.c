#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAXGENES 10
const int numInd = 10;
const int iteraciones = 10; //muchas iteraciones tarda mucho en ejecutar (logicamente)
const double ProbCruce = 0.5;
const double ProbMutacion = 0.02;

struct individuo {
  int genes[MAXGENES];
  int puntuacion;
};
struct individuo *inicializar();
void imprimirPoblacion(struct individuo *p);
void evaluarIndividuo(struct individuo *p);
struct individuo seleccionar(struct individuo *p);
int comparaind(int *a1, int *a2);
void cruce(struct individuo padre1, struct individuo padre2,
           struct individuo *hijo1, struct individuo *hijo2);
void mutacion(struct individuo *hijo);
void ordenar(struct individuo *poblacion);
struct individuo *reemplazar(struct individuo hijo1, struct individuo hijo2,
                             struct individuo *poblacion);

void main(int argc, char *argv) {
  srand(time(NULL));
  struct individuo *poblacion, aux;
  struct individuo padre1, padre2, hijo1, hijo2;
  poblacion = inicializar();
  for (int i = 0; i < numInd; i++) {
    evaluarIndividuo(&poblacion[i]);
  }
  printf("Población inicial: \n");
  imprimirPoblacion(poblacion);
  ordenar(poblacion);
  printf("Población ordenada: \n");
  imprimirPoblacion(poblacion);
  for (int i = 0; i < iteraciones; i++) {
    padre1 = seleccionar(poblacion);
    padre2 = seleccionar(poblacion);

    cruce(padre1, padre2, &hijo1, &hijo2);
    mutacion(&hijo1);
    mutacion(&hijo2);
    evaluarIndividuo(&hijo1);
    evaluarIndividuo(&hijo2);
    reemplazar(hijo1, hijo2, poblacion);
    ordenar(poblacion);
  }
  printf("Población después de %d iteraciones: \n", iteraciones);
  imprimirPoblacion(poblacion);
}

struct individuo *inicializar() {
  struct individuo *p;
  p = (struct individuo *)malloc(numInd * sizeof(struct individuo));
  for (int i = 0; i < numInd; i++) {
    struct individuo aux;
    aux.puntuacion = 0;
    for (int j = 0; j < MAXGENES; j++) {
      aux.genes[j] = rand() % 2;
    }
    p[i] = aux;
  }
  return p;
}

void imprimirPoblacion(struct individuo *p) {
  for (int i = 0; i < numInd; i++) {
    printf("Individuo %d: ", i);
    for (int j = 0; j < MAXGENES; j++) {
      printf("%d ", p[i].genes[j]);
    }
    printf("Puntuación: %d\n", p[i].puntuacion);
  }
  printf("\n");
}

void evaluarIndividuo(struct individuo *p) {
  p->puntuacion = 0;
  for (int j = 0; j < MAXGENES; j++) {
    if (p->genes[j] == 1) { // priorizamos numero de 1s
      p->puntuacion++;
    }
  }
}

struct individuo seleccionar(struct individuo *p) {
  int i = rand() % 10;
  int j = rand() % 10;
  struct individuo aux;

  while (comparaind(p[i].genes, p[j].genes)) {
    j = rand() % 10;
  }
  if (p[i].puntuacion > p[j].puntuacion) {
    aux = p[i];
  } else
    aux = p[j];
  return aux;
}

int comparaind(int *a1, int *a2) {
  int res = 1;
  for (int j = 0; j < MAXGENES; j++) {
    if (a1[j] != a2[j]) {
      res = 0; // 0 si son distintos
    }
  }
  return res;
}

void mutacion(struct individuo *hijo) {
  int prob = 0;
  for (int i = 0; i < MAXGENES; i++) {
    prob = rand() % 10;
    if (prob < ProbMutacion * 10) {
      if ((*hijo).genes[i] == 0) {
        (*hijo).genes[i] = 1;
      } else {
        (*hijo).genes[i] = 0;
      }
    }
  }
}

void cruce(struct individuo padre1, struct individuo padre2,
           struct individuo *hijo1, struct individuo *hijo2) {
  int prob = rand() % 10;
  hijo1->puntuacion = 0;
  hijo2->puntuacion = 0;
  if (prob <= ProbCruce * 10) {
    for (int i = 0; i < MAXGENES / 2; i++) {
      (*hijo1).genes[i] = padre1.genes[i];
      (*hijo2).genes[i] = padre2.genes[i];
    }
    for (int i = MAXGENES / 2; i < MAXGENES; i++) {
      (*hijo1).genes[i] = padre2.genes[i];
      (*hijo2).genes[i] = padre1.genes[i];
    }
  } else {
    *hijo1 = padre1;
    *hijo2 = padre2;
  }
}
void ordenar(struct individuo *poblacion) {
  struct individuo *aux1, *aux2, aux3;
  int p1, p2;
  for (int i = 0; i < numInd; i++) {
    aux1 = &poblacion[i];
    for (int j = 0; j < numInd; j++) {
      aux2 = &poblacion[j];
      if ((*aux2).puntuacion > (*aux1).puntuacion) {
        aux3 = *aux1;
        *aux1 = *aux2;
        *aux2 = aux3;
      }
    }
  }
}
struct individuo *
reemplazar(struct individuo hijo1, struct individuo hijo2,
           struct individuo *poblacion) { // reemplazo los dos de menor
                                          // puntuacion por los dos nuevos
  poblacion[0] = hijo1;
  poblacion[1] = hijo2;
}
