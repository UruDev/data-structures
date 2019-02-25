/* 4838059 - 5008417 */
#include "iterador.h"

#include <stdio.h>
#include <string.h> 

struct Node{
  char *data;
  Node *next;
};

struct rep_iterador{
  Node *head, *iter, *end;
};

iterador_t crear_iterador(){
  iterador_t list = new rep_iterador;
  list->head = list->iter = list->end = NULL;
  return list;
}

void agregar_a_iterador(char *t, iterador_t &i){
  if(strlen(t) <= 256){
    char *str = t;
    Node *added = new Node;
    added->data = str;
    added->next = NULL;
    if(i->head == NULL)
      i->head = added;
    else
      i->end->next = added;
    i->end = added;
  }
}

void reiniciar_iter(iterador_t &i){
  i->iter = i->head;
}

void avanzar_iter(iterador_t &i){
  if(i->iter != NULL)
    i->iter = i->iter->next;
}

char *actual_iter(iterador_t &i){
  return i->iter->data;
}

bool hay_siguiente_en_iter(iterador_t i){
  return i->iter->next != NULL;
}

bool esta_definida_actual(iterador_t i){
  return i->iter != NULL;
}

cadena_t iterador_a_cadena(iterador_t i){
  i->iter = i->head;
  cadena_t chain = crear_cadena(); 
  while(i->iter != NULL){
    char *str = new char[strlen(i->iter->data) + 1];
    strcpy(str, i->iter->data);
    info_t info = crear_info(0, str);
    insertar_al_final(info, chain);
    i->iter = i->iter->next;
  }
  return chain;
}

void liberar_iterador(iterador_t &i){
  while(i->head != NULL){
    Node *aux = i->head;
    i->head = i->head->next;
    delete[] aux->data;
    delete aux;
  }
  delete i;
}
