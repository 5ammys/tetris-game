#include "Vector.h"

int Vector_create(Vector*v, unsigned tamVector){

   v->vec = malloc(tamVector*(sizeof(tNodo)));
   if(!v->vec){
    return 0;
   }

   v->ce = 0;
   v->tam = tamVector;

   return 1;
}

void Vector_destroy(Vector*v){
    free(v->vec);
    v->ce = 0;
    v->tam = 0;
}

int Vector_append(Vector *v, void* elemento, size_t tamDato){

    if(v->ce == v->tam){
        if(_resize(v,v->tam+2) == -1){
            return 0;
        }
    }

    tNodo nodo;
    nodo.dato = malloc(tamDato);
    if(!nodo.dato) return 0;

    memcpy(nodo.dato,elemento,tamDato);
    nodo.tam = tamDato;

    (v->vec + v->ce)->dato = nodo.dato;
    (v->vec + v->ce)->tam = nodo.tam;
    v->ce++;

    return 1;
}

//fncion para insertar en orden y no repetir
int Vector_insertInOrderNoRepeat(Vector* v, void* elemento, size_t tamDato, Cmp cmp) {

    tNodo nodo;
    nodo.dato = malloc(tamDato);  //reservamos memoria para el nuevo dato
    if (!nodo.dato) return -1;

    memcpy(nodo.dato, elemento, tamDato);
    nodo.tam = tamDato;

    void* search = Vector_bsearch(v,nodo.dato,cmp); //busca el elemento por la condicion

    if(search){
        memcpy(search,nodo.dato,tamDato); //si lo encuentra lo reemplaza y retorna 2
        free(nodo.dato);
        return 2;
    }

    //verificar si hay espacio para el nuevo elemento
    if (v->ce == v->tam) {
        if (_resize(v, v->tam + 2) == -1) {
            free(nodo.dato);
            return -1;
        }
    }

    //puntero al ultimo nodo valido
    tNodo* actual = v->vec + v->ce - 1;

    //puntero a la posicion donde vamos a insertar
    tNodo* destino = v->vec + v->ce;

    //desplazamos los elementos mayores
    while (actual >= v->vec && cmp(nodo.dato, actual->dato) < 0) {
        *destino = *actual;
        destino--;
        actual--;
    }

    //insertamos el nuevo nodo
    *destino = nodo;
    v->ce++;
    return 1;
}

//funcion para insertar en orden
int Vector_insertInOrder(Vector* v, void* elemento, size_t tamDato, Cmp cmp) {

    //verificar si hay espacio para el nuevo elemento
    if (v->ce == v->tam) {
        if (_resize(v, v->tam + 2) == -1) {
            return -1;
        }
    }

    tNodo nodo;
    nodo.dato = malloc(tamDato);  //reservamos memoria para el nuevo dato
    if (!nodo.dato) return -1;

    memcpy(nodo.dato, elemento, tamDato);
    nodo.tam = tamDato;

    //puntero al ultimo nodo valido
    tNodo* actual = v->vec + v->ce - 1;

    //puntero a la posicion donde vamos a insertar
    tNodo* destino = v->vec + v->ce;

    //desplazamos los elementos mayores
    while (actual >= v->vec && cmp(nodo.dato, actual->dato) < 0) {
        *destino = *actual;
        destino--;
        actual--;
    }

    //insertamos el nuevo nodo
    *destino = nodo;
    v->ce++;

    return 1;
}

//funcion de busqueda binaria
void* Vector_bsearch(Vector* v, void* valor, Cmp cmp) {
    tNodo* ini = v->vec;
    tNodo* fin = v->vec + v->ce - 1;

    while (ini <= fin) {
        tNodo* medio = ini + (fin - ini) / 2;
        int comp = cmp(valor, medio->dato);

        if (comp == 0) {   //se encontro el valor
            return medio->dato;  //se retorna la direccion
        }
        else
        {
            if (comp > 0) {
                ini = medio + 1;
            } else {
                fin = medio - 1;
            }
        }
    }
    return NULL;  //no se encontro el valor
}

int Vector_getByPos(Vector* v, int pos, void * valor, size_t tamValor) {
    //validar que el vector exista, no este vacio y la posicion este dentro del rango [0, ce - 1]
    if (!v || v->ce == 0 || pos < 0 || pos >= v->ce) {
        return -1; // Posicion incorrecta o vector no valido
    }

    //obtener el nodo en la posicion deseada
    tNodo * nodo = (v->vec) + pos;

    //validar que el puntero a los datos no sea NULL
    if (!nodo->dato || !valor) {
        return -1;
    }

    //copiar exactamente tamValor bytes al buffer de destino
    memcpy(valor, nodo->dato, tamValor);

    return 1; 
}

void* Vector_getRefByPos(Vector* v, int pos){
    if (!v || !v->vec) {
        return NULL;
    }

    if(pos>=v->ce || pos<0){
        return NULL;
    }
    return (v->vec+pos)->dato;

}

size_t Vector_count(Vector* v){
    return v->ce;
}

int _resize(Vector* v,size_t nuevoTamanio){

  void * nv = realloc((void*)v->vec,nuevoTamanio*(sizeof(tNodo)));
  if(nv == NULL){
    return -1;
  }
  v->vec = (tNodo*)nv;
  v->tam = nuevoTamanio;
  return 1;
}

void Vector_map(Vector*v,Accion accion){
  int i;
  tNodo* pivot = v->vec;

  for(i=0;i<v->ce;i++){
    accion(pivot->dato);
    pivot++;
  }
}

//funcion de comparacion entre dos Nodos
int compararInt(const void* a, const void* b) {
    int valorA = *(int*)a;
    int valorB = *(int*)b;
    return (valorA - valorB);
}
