#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//#include <mpi.h>

#define NUMGENES 10
const int NumInd = 10;
const int NumIter = 10;
const double ProbCruce = 0.8;
const double ProbMutacion = 0.3;

struct individuo {
    int tgenes[NUMGENES];
    int fitness;
};

struct individuo* iniciarPoblacion(int tam);
void evaluarPoblacion(struct individuo* pob, int tam);
void mostrarPoblacion(struct individuo* pob, int tam);
void quicksort(struct individuo* pob, int n);
struct individuo torneo(struct individuo* pobOriginal, int tam, int* indice);
struct individuo* seleccionar(struct individuo* pobOriginal, int tam);
struct individuo* reemplazo(struct individuo* seleccion, struct individuo* poblacion, int tam);

int main(int argc, char* argv[])
{
    int tam = NumInd;
    struct individuo* poblacion;
    struct individuo* seleccion;
    struct individuo* aux;
    int contEvaluaciones = 0;
    int find = false;
    int i = 0;
   
    srand(time(NULL));

    poblacion = iniciarPoblacion(tam);
    evaluarPoblacion(poblacion, tam);
    quicksort(poblacion, tam); // Ordenación por conveniencia para mostrarPoblacion

    printf("Población inicial \n\n");
    mostrarPoblacion(poblacion, tam);

    while (!find && i < NumIter) {
        seleccion = seleccionar(poblacion, tam); //En selección se cruzan y mutan los individuos según la probabilidad
        evaluarPoblacion(seleccion, tam);
        quicksort(seleccion, tam);
        poblacion = reemplazo(seleccion, poblacion, tam);
        i++;
    }

    printf ("Población procesada \n\n");
    mostrarPoblacion(poblacion, tam);

    return 0;
}

struct individuo* iniciarPoblacion(int tam)
{
    struct individuo* pob;
    pob = (struct individuo*)malloc(tam * sizeof(struct individuo));
    if (pob == NULL)
        printf("Error al asignar array de individuos \n");
    for (int i = 0; i < tam; i++) {
        struct individuo nuevoInd;
        for (int j = 0; j < NUMGENES; j++) {
            nuevoInd.tgenes[j] = rand() % 2;
        }
        pob[i] = nuevoInd;
    }

    return pob;
}

void evaluarPoblacion(struct individuo* pob, int tam)
{
    for (int i = 0; i < tam; i++) {
        pob[i].fitness = 0;
        for (int j = 0; j < NUMGENES; j++) {
            if (pob[i].tgenes[j] == 0)
                pob[i].fitness++;
        }
    }
}

void qs(struct individuo* pob, int limite_izq, int limite_der) //quicksort recursivo (posible mejora)
{
    int izq, der, pivote;
    struct individuo temporal;
    izq = limite_izq;
    der = limite_der;
    pivote = pob[(izq + der) / 2].fitness;

    do {
        while (pob[izq].fitness < pivote && izq < limite_der)
            izq++;
        while (pivote < pob[der].fitness && der > limite_izq)
            der--;
        if (izq <= der) {
            temporal = pob[izq];
            pob[izq] = pob[der];
            pob[der] = temporal;
            izq++;
            der--;
        }

    } while (izq <= der);

    if (limite_izq < der) {
        qs(pob, limite_izq, der);
    }

    if (limite_der > izq) {
        qs(pob, izq, limite_der);
    }
}

void quicksort(struct individuo* pob, int n)
{
    qs(pob, 0, n - 1); //quicksort recursivo
}

void mostrarPoblacion(struct individuo* pob, int tam)
{
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < NUMGENES; j++) {
            printf("%i ", pob[i].tgenes[j]);
        }
        printf("   fitness: %i \n", pob[i].fitness);
    }
    printf("\n");
}

struct individuo torneo(struct individuo* pobOriginal, int tam, int* indice)
{
    *indice = 0;
    int indice2 = 0;
    struct individuo candidato1, candidato2;

    *indice = rand() % tam;
    candidato1 = pobOriginal[*indice]; //eligo primer candidato para el Torneo
    indice2 = rand() % tam;
    while (*indice == indice2) { //compruebo que no son indices iguales
        indice2 = rand() % tam;
    }
    candidato2 = pobOriginal[indice2]; // segundo candidato del torneo.
    if (candidato1.fitness > candidato2.fitness) {
        return candidato1;
    } else {
        *indice = indice2; // se cambian los índices para comprobar siempre el mismo indice y saber qué indice ha salido
        return candidato2;
    }
}

void cruzar(struct individuo padre1, struct individuo padre2, struct individuo* hijo1, struct individuo* hijo2)
{
    int prob = rand() % 10;
    if (prob <= ProbCruce * 10) {
        for (int i = 0; i < NUMGENES / 2; i++) {
            (*hijo1).tgenes[i] = padre1.tgenes[i];
            (*hijo2).tgenes[i] = padre2.tgenes[i];
        }
        for (int i = NUMGENES / 2; i < NUMGENES; i++) {
            (*hijo1).tgenes[i] = padre2.tgenes[i];
            (*hijo2).tgenes[i] = padre1.tgenes[i];
        }
    } else {
        *hijo1 = padre1;
        *hijo2 = padre2;
    }
}

void mutacion(struct individuo* hijo)
{
    int prob = 0;
    for (int i = 0; i < NUMGENES; i++) {
        prob = rand() % 10;
        if (prob < ProbMutacion * 10) {
            if ((*hijo).tgenes[i] == 0) {
                (*hijo).tgenes[i] = 1;
            } else {
                (*hijo).tgenes[i] = 0;
            }
        }
    }
}

struct individuo* seleccionar(struct individuo* pobOriginal, int tam)
{
    int indice, aux;
    struct individuo padre1, padre2, hijo1, hijo2;
    struct individuo* pob;
    pob = (struct individuo*)malloc(tam * sizeof(struct individuo));
    if (pob == NULL)
        printf("Error al asignar array de individuos \n");
    for (int i = 0; i < tam; i++) {
        padre1 = torneo(pobOriginal, tam, &indice);
        aux = indice;
        padre2 = torneo(pobOriginal, tam, &indice);
        while (indice == aux) {
            padre2 = torneo(pobOriginal, tam, &indice);
        } // Obtengo padres para el cruce
        cruzar(padre1, padre2, &hijo1, &hijo2); //si no se cruzan los hijos serán los padres.
        mutacion(&hijo1); //muto los hijos
        mutacion(&hijo2);
        pob[i] = hijo1;
        i++;
        pob[i] = hijo2;
    }
    return pob;
}

struct individuo* reemplazo(struct individuo* seleccion, struct individuo* poblacion, int tam)
{
    int cmp = 0;
    for (int i = 0; i < tam / 10; i++) {
        cmp = seleccion[i].fitness;
        int j = 0;
        while ((j < tam) && (cmp >= poblacion[j].fitness)) {
            j++;
        }
        if (j < tam) {
            poblacion[j] = seleccion[i];
        }
    }
    return poblacion;
}
