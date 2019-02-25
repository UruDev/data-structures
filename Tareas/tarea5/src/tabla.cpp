/* 4838059 - 5008417 */
#include <stddef.h>
#include <stdio.h>
#include <string.h> 

#include "../include/tabla.h"

struct rep_tabla{
  cadena_t *chain; 
  nat len;
  nat count;
};

tabla_t crear_tabla(nat tamanio){
  tabla_t table = new rep_tabla;
  table->len = tamanio; 
  table->count = 0;
  table->chain = new cadena_t[tamanio];
  for(nat i= 0; i < tamanio; i++)
    table->chain[i] = crear_cadena();
  return table;
}

void asociar_en_tabla(int clave, char *valor, tabla_t &t){
  info_t data = crear_info(clave, valor);
  cadena_t chain = t->chain[clave % t->len];
  localizador_t first = inicio_cadena(chain);
  localizador_t loc = siguiente_clave(clave, first, chain);
  if(es_localizador(loc))
    remover_de_cadena(loc, chain);
  else
    ++t->count;
  insertar_al_final(data, chain);
}

void eliminar_de_tabla(int clave, tabla_t &t){
  cadena_t chain = t->chain[clave % t->len];
  localizador_t loc = siguiente_clave(clave, inicio_cadena(chain), chain);
  remover_de_cadena(loc, chain);
  --t->count;
}

void liberar_tabla(tabla_t &t){
  for(nat i= 0; i < t->len; i++)
    liberar_cadena(t->chain[i]);
  delete[] t->chain;
  delete t;
}

bool existe_asociacion(int clave, tabla_t t){
  cadena_t chain = t->chain[clave % t->len];
  localizador_t first = inicio_cadena(chain);
  return es_localizador(siguiente_clave(clave, first, chain));
}

char *valor_en_tabla(int clave, tabla_t t){
  cadena_t chain = t->chain[clave % t->len];
  localizador_t first = inicio_cadena(chain);
  return frase_info(info_cadena(siguiente_clave(clave, first, chain), chain));
}

bool esta_llena_tabla(tabla_t t){
  return t->count == t->len;
}
