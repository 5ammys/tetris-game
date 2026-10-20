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

void Vector_destroy(Vector* v) {
    if (!v) return;

    for (size_t i = 0; i < v->ce; i++) {
        if (v->vec[i].dato) {
            free(v->vec[i].dato);
            v->vec[i].dato = NULL;
        }
    }

    free(v->vec);
    v->vec = NULL;

    v->ce = 0;
    v->tam = 0;
}


int Vector_append(Vector *v, void* elemento, size_t tamDato) {
    if (!v || !elemento) return 0;

    if (v->ce == v->tam) {
        if (_resize(v, v->tam + 2) == -1) {
            return 0; // Falla de memoria en redimensión
        }
    }

    tNodo* nuevoNodo = v->vec + v->ce;

    nuevoNodo->dato = malloc(tamDato);
    if (!nuevoNodo->dato) return 0;

    memcpy(nuevoNodo->dato, elemento, tamDato);
    nuevoNodo->tam = tamDato;

    v->ce++;

    return 1;
}

int Vector_insertInOrderNoRepeat(Vector* v, void* elemento, size_t tamDato, Cmp cmp)
{
    void* search = Vector_bsearch(v, elemento, cmp);

    if (search) {
        memcpy(search, elemento, tamDato);
        return 2;
    }

    tNodo nodo;
    nodo.dato = malloc(tamDato);

    if (!nodo.dato)
        return -1;

    memcpy(nodo.dato, elemento, tamDato);
    nodo.tam = tamDato;

    if (v->ce == v->tam) {
        if (_resize(v, v->tam + 2) == -1) {
            free(nodo.dato);
            return -1;
        }
    }

    size_t i = v->ce;

    while (i > 0 &&
           cmp(nodo.dato, v->vec[i - 1].dato) < 0) {
        v->vec[i] = v->vec[i - 1];
        i--;
    }

    v->vec[i] = nodo;
    v->ce++;

    return 1;
}
// Funci�n para insertar en orden
int Vector_insertInOrder(Vector* v, void* elemento, size_t tamDato, Cmp cmp) {
    if(v->ce == 0){
        Vector_append(v, elemento, tamDato);
        return 1;
    }
    // Verificar si hay espacio para el nuevo elemento
    if (v->ce == v->tam) {
        if (_resize(v, v->tam + 2) == -1) {
            return -1;
        }
    }

    tNodo nodo;
    nodo.dato = malloc(tamDato);  // Reservamos memoria para el nuevo dato
    if (!nodo.dato) return -1;

    memcpy(nodo.dato, elemento, tamDato);
    nodo.tam = tamDato;

    // Puntero al �ltimo nodo v�lido
    tNodo* actual = v->vec + v->ce - 1;

    // Puntero a la posici�n donde vamos a insertar
    tNodo* destino = v->vec + v->ce;

    // Desplazamos los elementos mayores
    while (actual >= v->vec && cmp(nodo.dato, actual->dato) < 0) {
        *destino = *actual;
        destino--;
        actual--;
    }

    // Insertamos el nuevo nodo
    *destino = nodo;
    v->ce++;

    return 1;
}

// Funci�n de b�squeda binaria
void* Vector_bsearch(Vector* v, void* valor, Cmp cmp) {
    tNodo* ini = v->vec;
    tNodo* fin = v->vec + v->ce - 1;

    while (ini <= fin) {
        tNodo* medio = ini + (fin - ini) / 2;
        int comp = cmp(valor, medio->dato);

        if (comp == 0) {   // Se encontr� el valor
            return medio->dato;  // Se retorna la direccion
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
    return NULL;  // No se encontr� el valor
}

int Vector_getByPos(Vector* v, int pos, void * valor, size_t tamValor) {
    // 1. Validar que el vector exista, no esté vacío y la posición esté dentro del rango [0, ce - 1]
    if (!v || v->ce == 0 || pos < 0 || pos >= v->ce) {
        return -1; // Posición incorrecta o vector no válido
    }

    // 2. Obtener el nodo en la posición deseada
    tNodo * nodo = (v->vec) + pos;

    // 3. Validar que el puntero a los datos no sea NULL
    if (!nodo->dato || !valor) {
        return -1;
    }

    // 4. Copiar exactamente tamValor bytes al buffer de destino
    memcpy(valor, nodo->dato, tamValor);

    return 1; // Éxito
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

//Funci�n de comparaci�n entre dos Nodos
int compararInt(const void* a, const void* b) {
    int valorA = *(int*)a;
    int valorB = *(int*)b;
    return (valorA - valorB);
}
