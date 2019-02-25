/* 4838059 - 5008417 */
#include "../include/cola_binarios.h"

#include <stdio.h>

struct Node{
  binario_t tree;
  Node *next;
};

struct rep_cola_binarios{
  Node *first, *last;
};

cola_binarios_t crear_cola_binarios(){
  rep_cola_binarios *tail = new rep_cola_binarios;
  tail->first = tail->last = NULL;
  return tail;
}

void encolar(binario_t b, cola_binarios_t &c){
  Node *elem = new Node;
  elem->tree = b;
  elem->next = NULL;
  if(c->first == NULL)
    c->first = elem;
  else 
    c->last->next = elem;
  c->last = elem;	
}

void desencolar(cola_binarios_t &c){
  if(!es_vacia_cola_binarios(c)){
    Node *aux = c->first;
    c->first = c->first->next;
    if(c->first == NULL) 
      c->last = NULL;
    delete aux;
  }  
}

void liberar_cola_binarios(cola_binarios_t &c){
  while(!es_vacia_cola_binarios(c))
    desencolar(c);
  delete c;
}

bool es_vacia_cola_binarios(cola_binarios_t c){
  return (c->first == NULL);
}

binario_t frente(cola_binarios_t c){
  return c->first->tree;
}
