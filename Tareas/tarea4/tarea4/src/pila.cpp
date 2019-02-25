/* 4838059 - 5008417 */

#include <assert.h>
#include <string.h>
#include "pila.h"
#include <stdio.h>

struct rep_pila{
  char **array;
  int tope;
  int cota;
};


pila_t crear_pila(int tamanio){
  pila_t p = new rep_pila;
  p->tope = 0;
  p->array = new char*[tamanio];
  p->cota = tamanio;
  return p;
}

void apilar(char *t, pila_t &p){
  if(!es_llena_pila(p)){
    p->array[p->tope] = t;
    p->tope ++;
  }
}

void desapilar(pila_t &p){
  if(!es_vacia_pila(p)){
    delete[] p->array[p->tope-1];
    p->tope--;
  }
}

void liberar_pila(pila_t &p){ 
  while(!es_vacia_pila(p))
    desapilar(p);
  delete[] p->array;
  delete p;
}

bool es_vacia_pila(pila_t p){
  return p->tope == 0;
}

bool es_llena_pila(pila_t p){
  return p->tope == p->cota;
}

char *cima(pila_t p){
  assert(es_vacia_pila(p));
  return p->array[p->tope-1];
}
