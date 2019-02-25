/* 4838059 - 5008417 */
#include "../include/cola_prioridad.h"

#include <stdio.h>

struct Node{
  prio_t pr;
  info_t data;
};

struct priority{
  bool pr;
  prio_t pos;
};

struct rep_cola_prioridad{
  Node *elem;
  priority *prArr;
  prio_t max;
  prio_t limit;
  prio_t capacity;
};

cola_prioridad_t crear_cp(unsigned int tamanio, prio_t MAX_PRIORIDAD){
  cola_prioridad_t cp = new rep_cola_prioridad;
  cp->elem = new Node[tamanio+1];
  for(prio_t i=0; i <= tamanio; i++){
    cp->elem[i].pr = 0;
    cp->elem[i].data = NULL;
  }
  cp->prArr = new priority[MAX_PRIORIDAD+1];
  for(prio_t x=0; x <= MAX_PRIORIDAD; x++){
    cp->prArr[x].pr = 0;
    cp->prArr[x].pos = 0;
  }
  cp->max = MAX_PRIORIDAD;
  cp->limit = 0;
  cp->capacity = tamanio;
  return cp;
}

static prio_t ascend(prio_t position, prio_t pr, cola_prioridad_t &queue){
  while(position >= 1 && queue->elem[position/2].pr > pr){
    queue->elem[position] = queue->elem[position/2];
    queue->prArr[queue->elem[position].pr].pos = position;
    if(position > 1)
      position = position/2;
  }
  return position;
}

void insertar_en_cp(info_t i, prio_t p, cola_prioridad_t &cp){
  prio_t position = ascend(cp->limit+1, p, cp);
  cp->elem[position].data = i;
  cp->elem[position].pr = p;
  cp->prArr[p].pr = 1;
  cp->prArr[p].pos = position;
  cp->limit++;
}

void priorizar(prio_t p, cola_prioridad_t &cp){
  prio_t position = cp->prArr[p].pos;
  cp->elem[position].pr = cp->elem[position].pr/2;
  Node aux = cp->elem[position];
  position = ascend(position, aux.pr, cp);
  cp->elem[position] = aux;
  cp->prArr[p].pr = 0;
  cp->prArr[p/2].pr = 1;
  cp->prArr[p/2].pos = position;
}

static prio_t descend(prio_t position, prio_t pr, cola_prioridad_t &queue){
  prio_t i = position;
  while(i*2 <= queue->limit){
    if((i*2)+1 <= queue->limit){ 
      Node aux;
      if(queue->elem[i*2].pr < queue->elem[(i*2)+1].pr)
        aux = queue->elem[i*2];
      else
        aux = queue->elem[(i*2)+1];
      if(pr > aux.pr){
        queue->elem[i] = aux;
        prio_t pos = i;
        i = queue->prArr[aux.pr].pos;
        queue->prArr[aux.pr].pos = pos;
      }else
        return i;
    }else if(pr > queue->elem[i*2].pr){
      queue->elem[i] = queue->elem[i*2];
      queue->prArr[queue->elem[i].pr].pos = i;
      i = i*2;
    }else
      return i;
  }
  return i;
}

void despriorizar(prio_t p, cola_prioridad_t &cp){
  prio_t position = cp->prArr[p].pos;
  Node aux = cp->elem[position];
  cp->prArr[aux.pr].pr = 0;
  aux.pr = (aux.pr+cp->max)/2;
  position = descend(position, aux.pr, cp);
  cp->elem[position] = aux;
  cp->prArr[aux.pr].pr = 1;
  cp->prArr[aux.pr].pos = position;
}

void eliminar_prioritario(cola_prioridad_t &cp){
  Node aux = cp->elem[1];
  liberar_info(aux.data);
  cp->prArr[aux.pr].pr = 0;
  cp->elem[1] = cp->elem[cp->limit];
  cp->limit--;
  aux = cp->elem[1];
  if((cp->limit >= 2 && cp->elem[1].pr > cp->elem[2].pr) || (cp->limit >= 3 && cp->elem[1].pr > cp->elem[3].pr)){
    prio_t pos = descend(1, aux.pr, cp);
    cp->elem[pos] = aux;
    cp->prArr[aux.pr].pos = pos;
  }else
    cp->prArr[aux.pr].pos = 1;
}

void liberar_cp(cola_prioridad_t &cp){
  while(cp->limit > 0)
    eliminar_prioritario(cp);
  delete[] cp->elem;
  delete[] cp->prArr;
  delete cp;
}

bool es_vacia_cp(cola_prioridad_t cp){
  return cp->limit == 0;
}

bool es_llena_cp(cola_prioridad_t cp){
  return cp->limit == cp->capacity;
}

bool hay_prioridad(prio_t p, cola_prioridad_t cp){
  bool exist = 0;
  if(p <= cp->max)
    exist = cp->prArr[p].pr == 1;
  return exist;
}

info_t prioritario(cola_prioridad_t cp){
  return cp->elem[1].data;
}

prio_t prioridad_prioritario(cola_prioridad_t cp){
  return cp->elem[1].pr;
}

prio_t max_prioridad(cola_prioridad_t cp){
  return cp->max;
}
