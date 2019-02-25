/* 4838059 - 5008417 */
#include "../include/uso_cadena.h"
#include "../include/cadena.h"
#include "../include/info.h"

#include <stddef.h>
#include <string.h> 
#include <stdio.h>
#include <assert.h>

bool pertenece(int i, cadena_t cad){
  return (siguiente_clave(i, inicio_cadena(cad), cad) != NULL);
}

nat longitud(cadena_t cad){
  nat cant= 0;
  localizador_t loc = inicio_cadena(cad);
  while(es_localizador(loc)){
    ++cant;
    loc = siguiente(loc, cad);
  }
  return cant;
}

bool son_iguales(cadena_t c1, cadena_t c2){
  localizador_t loc1 = inicio_cadena(c1);
  localizador_t loc2 = inicio_cadena(c2);
  while(es_localizador(loc1) && es_localizador(loc2) && son_iguales(info_cadena(loc1, c1), info_cadena(loc2, c2))){
    loc1 = siguiente(loc1, c1);
    loc2 = siguiente(loc2, c2);
  }
  return(!es_localizador(loc1) && !es_localizador(loc2));
}

cadena_t concatenar(cadena_t c1, cadena_t c2){
  cadena_t sgm1 = segmento_cadena(inicio_cadena(c1), final_cadena(c1), c1);
  cadena_t sgm2 = segmento_cadena(inicio_cadena(c2), final_cadena(c2), c2);
  insertar_segmento_despues(sgm2, final_cadena(sgm1), sgm1);
  liberar_cadena(sgm2);
  return sgm1;
}

void ordenar(cadena_t &cad){
  if(!es_vacia_cadena(cad) && !esta_ordenada(cad)){
    localizador_t loc1 = inicio_cadena(cad);
    localizador_t loc2 = siguiente(loc1, cad);
    while(es_localizador(loc1) && es_localizador(loc2)){
      if(numero_info(info_cadena(loc1, cad)) > numero_info(info_cadena(loc2, cad))){
        intercambiar(loc1, loc2, cad);
        ordenar(cad);
      }
      loc1 = loc2;
      loc2 = siguiente(loc2, cad);
    }
  }
}

char *copy_txt(localizador_t loc, cadena_t cad){
  char *aux = frase_info(info_cadena(loc, cad));
  char *text = new char[strlen(aux) + 1];
  strcpy(text, aux);
  return text;
}

void cambiar_todos(int original, int nuevo, cadena_t &cad){
  if(!es_vacia_cadena(cad)){
    localizador_t loc = inicio_cadena(cad);
    while(es_localizador(loc)){
      if(numero_info(info_cadena(loc, cad)) == original){
        insertar_antes(crear_info(nuevo, copy_txt(loc, cad)), loc, cad);
        localizador_t aux = siguiente(loc, cad);
        remover_de_cadena(loc, cad);
        loc = aux;
      }else
        loc = siguiente(loc, cad);
    }
  }
}

cadena_t subcadena(int menor, int mayor, cadena_t cad){
  cadena_t chain = crear_cadena();
  localizador_t loc = inicio_cadena(cad);
  while(es_localizador(loc)){
    if((menor <= numero_info(info_cadena(loc, cad))) && (numero_info(info_cadena(loc, cad)) <= mayor))
      insertar_al_final(copia_info(info_cadena(loc, cad)), chain);
    loc = siguiente(loc, cad);
  }
  return chain;
}
