/* 4838059 - 5008417 */
#include "../include/cadena.h"

#include <stddef.h>
#include <stdio.h>
#include <assert.h>
#include <string.h> 

struct nodo{
  info_t dato;
  nodo *anterior, *siguiente;
};

struct rep_cadena {
  nodo *inicio, *final;
};

cadena_t crear_cadena() {
  cadena_t res = new rep_cadena;
  res->inicio = res->final = NULL;
  return res;
}

void insertar_al_final(info_t i, cadena_t &cad){
  nodo *nuevo = new nodo;
  nuevo->dato = i;
  nuevo->siguiente = NULL;
  nuevo->anterior = cad->final;
  if(cad->final == NULL){
    assert(cad->inicio == NULL);
    cad->inicio = nuevo;
  }else{
    assert(cad->inicio !=NULL);
    cad->final->siguiente = nuevo;
  }
  cad->final = nuevo;
}

void insertar_antes(info_t i, localizador_t loc, cadena_t &cad){
  nodo *nuevo = new nodo;
  nuevo->dato = i; 
  nuevo->siguiente = loc;
  nuevo->anterior = loc->anterior;
  if (loc->anterior == NULL)  
    cad->inicio = nuevo;
  else{
    loc->anterior->siguiente = nuevo;
  }
  loc->anterior = nuevo;
}

void insertar_segmento_despues(cadena_t &sgm, localizador_t loc, cadena_t &cad){
  assert(es_vacia_cadena(cad) || localizador_en_cadena(loc, cad));
  if(es_vacia_cadena(cad)){
    cad->inicio = sgm->inicio;
    cad->final = sgm->final;  
  } else {
    if(!es_vacia_cadena(sgm)){
      sgm->inicio->anterior = loc;
      sgm->final->siguiente = loc->siguiente;
      if (es_final_cadena(loc, cad))
        cad->final = sgm->final;
      else
        loc->siguiente->anterior = sgm->final;
      loc->siguiente = sgm->inicio;
    }  
  }
  sgm->inicio = sgm->final = NULL;
}

cadena_t segmento_cadena(localizador_t desde, localizador_t hasta, cadena_t cad){
  cadena_t res = crear_cadena();
  if(!es_vacia_cadena(cad)){
    localizador_t loc = desde;
    while (loc != siguiente(hasta, cad)) {
      info_t info = copia_info(loc->dato);
      insertar_al_final(info, res);
      loc = siguiente(loc, cad);
    }
  }
  return res;
}

cadena_t separar_segmento(localizador_t desde, localizador_t hasta, cadena_t &cad){
  cadena_t chain = crear_cadena();
  if(!es_vacia_cadena(cad)){
    chain->inicio = desde;
    chain->final = hasta;
    if(desde != cad->inicio)
      desde->anterior->siguiente = hasta->siguiente;
    else
      cad->inicio = hasta->siguiente;
    if(hasta != cad->final)
      hasta->siguiente->anterior = desde->anterior;
    else
      cad->final = desde->anterior;
    desde->anterior = hasta->siguiente = NULL;
  }
  return chain;
}

cadena_t mezcla(cadena_t c1, cadena_t c2){
  cadena_t chain;
  if(es_vacia_cadena(c1) || es_vacia_cadena(c2)){
    if(es_vacia_cadena(c1))
      chain = segmento_cadena(c2->inicio, c2->final, c2);
    else
      chain = segmento_cadena(c1->inicio, c1->final, c1);
    return chain;
  }
  chain = crear_cadena();
  localizador_t loc1 = c1->inicio;
  localizador_t loc2 = c2->inicio;
  while(es_localizador(loc1) && es_localizador(loc2)){
    if(numero_info(loc1->dato) <= numero_info(loc2->dato)){
      insertar_al_final(copia_info(loc1->dato), chain);
      loc1 = loc1->siguiente;
    }else{
      insertar_al_final(copia_info(loc2->dato), chain);
      loc2 = loc2->siguiente;
    }
  }
  if(es_localizador(loc1) || es_localizador(loc2)){
    cadena_t sgm; 
    if(es_localizador(loc1))
      sgm = segmento_cadena(loc1, c1->final, c1);
    else
      sgm = segmento_cadena(loc2, c2->final, c2);
    insertar_segmento_despues(sgm, chain->final, chain);
    liberar_cadena(sgm);
  }
  return chain;
}

void remover_de_cadena(localizador_t &loc, cadena_t &cad){
  if(es_inicio_cadena(loc, cad) && es_final_cadena(loc, cad))
    cad->inicio = cad->final = NULL;
  else if(es_inicio_cadena(loc, cad)){
    cad->inicio = loc->siguiente;
    loc->siguiente->anterior = NULL;
  }else if(es_final_cadena(loc, cad)){
    cad->final = loc->anterior;
    loc->anterior->siguiente = NULL;
  }else{
    loc->anterior->siguiente = loc->siguiente;
    loc->siguiente->anterior = loc->anterior;
  }
  liberar_info(loc->dato); 
  delete loc;
}

void liberar_cadena(cadena_t &cad){
  nodo *a_borrar;
  while(cad->inicio != NULL){
    a_borrar = cad->inicio;
    cad->inicio = cad->inicio->siguiente;
    liberar_info(a_borrar->dato);
    delete(a_borrar);
  }
  delete cad;
}

bool es_localizador(localizador_t loc){
  return loc != NULL;
}

bool es_vacia_cadena(cadena_t cad){
  return (cad->inicio == NULL) && (cad->final == NULL);
}

bool esta_ordenada(cadena_t cad){
  bool res = true;
  if(!es_vacia_cadena(cad)){
    localizador_t loc = inicio_cadena(cad);
    while(res && es_localizador(siguiente(loc, cad))){
      localizador_t sig_loc = siguiente(loc, cad);
      if(numero_info(info_cadena(loc, cad)) > numero_info(info_cadena(sig_loc, cad)))
        res = false;
      else
        loc = siguiente(loc, cad);
    }
  }
  return res;
}

bool es_final_cadena(localizador_t loc, cadena_t cad){
  return(!es_vacia_cadena(cad) && loc == cad->final);
}

bool es_inicio_cadena(localizador_t loc, cadena_t cad){
  return(!es_vacia_cadena(cad) && loc == cad->inicio);
}

bool localizador_en_cadena(localizador_t loc, cadena_t cad){
  if(es_vacia_cadena(cad) || !es_localizador(loc))
    return false;
  localizador_t cursor = cad->inicio;
  while(es_localizador(cursor) && cursor != loc)
    cursor = cursor->siguiente;
  return cursor == loc;
}

bool precede_en_cadena(localizador_t loc1, localizador_t loc2, cadena_t cad){
  localizador_t cursor = loc1;
  while(es_localizador(cursor) && (cursor != loc2))
    cursor = siguiente(cursor, cad);
  return ((cursor == loc2) && (localizador_en_cadena(loc1, cad)));
}

localizador_t inicio_cadena(cadena_t cad){
  if(es_vacia_cadena(cad))
    return NULL;
  return cad->inicio;
}

localizador_t final_cadena(cadena_t cad){
  if(es_vacia_cadena(cad))
    return NULL;
  return cad->final;
}

localizador_t kesimo(nat k, cadena_t cad){
  if(!es_vacia_cadena(cad)){
    localizador_t loc = cad->inicio;
    nat n = 1;
    while(k != 0 && es_localizador(loc) && n < k){
      ++n;
      loc = loc->siguiente;
    }
    if(es_localizador(loc) && n == k)
      return loc;
  }
  return NULL;
}

localizador_t siguiente(localizador_t loc, cadena_t cad){
  return loc->siguiente;
}

localizador_t anterior(localizador_t loc, cadena_t cad){
  return loc->anterior;
}

localizador_t menor_en_cadena(localizador_t loc, cadena_t cad){
  assert(localizador_en_cadena(loc, cad));
  localizador_t res = loc;
  while(es_localizador(siguiente(loc, cad))){
    loc = siguiente(loc, cad);
    if(numero_info(info_cadena(loc, cad)) < numero_info(info_cadena(res, cad)))
      res = loc;
  }
  return res;
}

localizador_t siguiente_clave(int clave, localizador_t loc, cadena_t cad){
  assert(es_vacia_cadena(cad) || localizador_en_cadena(loc, cad));
  localizador_t res = loc;
  if(es_vacia_cadena(cad))
    res = NULL;
  else{
    while(es_localizador(res) && numero_info(info_cadena(res, cad)) != clave)
      res = siguiente(res, cad);
  }
  return res;
}

localizador_t anterior_clave(int clave, localizador_t loc, cadena_t cad){
  assert(es_vacia_cadena(cad) || localizador_en_cadena(loc, cad));
  localizador_t res = loc;
  if(es_vacia_cadena(cad))
    res = NULL;
  else{
    while(es_localizador(res) && numero_info(info_cadena(res, cad)) != clave)
      res = anterior(res, cad);
  }
  return res;
}

info_t info_cadena(localizador_t loc, cadena_t cad){
  return loc->dato;
}

void cambiar_en_cadena(info_t i, localizador_t loc, cadena_t &cad){
  assert(localizador_en_cadena(loc, cad));
  loc->dato = i;
}

void intercambiar(localizador_t loc1, localizador_t loc2, cadena_t &cad){
  info_t aux = loc1->dato;  
  cambiar_en_cadena(loc2->dato, loc1, cad);
  cambiar_en_cadena(aux, loc2, cad);
}

void imprimir_cadena(cadena_t cad){
  if(!es_vacia_cadena(cad)){
    localizador_t loc = cad->inicio;
    do{
      char *txt = info_a_texto(loc->dato);
      printf("%s", txt);
      delete[] txt;
      loc = loc->siguiente;
    }while(loc != NULL);
  }
  printf("\n"); 
}
