/* 4838059 - 5008417 */
#include "uso_tads.h"

#include <stdio.h>
#include <string.h> 
#include <assert.h>

static void reverse_iter(nat a, nat b, iterador_t it, iterador_t &reversed){
  if(esta_definida_actual(it)){
    char *current = actual_iter(it);
    if(a < b){
      ++a;
      avanzar_iter(it);
      reverse_iter(a, b, it, reversed);
    }
    char *str = new char[strlen(current) + 1];
    strcpy(str, current);
    agregar_a_iterador(str, reversed);
  }
}

iterador_t inverso_de_iter(nat a, nat b, iterador_t it){
  iterador_t reversed = crear_iterador();
  reiniciar_iter(it);
  nat index = 1;
  while(index < a){
    ++index;
    avanzar_iter(it);
  }
  reverse_iter(a, b, it, reversed);
  reiniciar_iter(it);
  reiniciar_iter(reversed);
  return reversed;
}

static void recursive_iter(char *since, char *until, iterador_t it, iterador_t &reversed){
  if(esta_definida_actual(it)){
    char *str = new char[strlen(actual_iter(it)) + 1];
    strcpy(str, actual_iter(it));
    if(hay_siguiente_en_iter(it) && strcmp(str, until) < 0){
      avanzar_iter(it);
      recursive_iter(since, until, it, reversed);
    }
    if(strcmp(str, since) >= 0)
      agregar_a_iterador(str, reversed);
    else
      delete[] str;
  }
}

iterador_t rango_en_conjunto(char *primero, char *ultimo, conjunto_t c){
  iterador_t it = crear_iterador();
  if(!es_vacio_conjunto(c)){
    iterador_t aux = iterador_conjunto(c);
    reiniciar_iter(aux);
    recursive_iter(primero, ultimo, aux, it);
    liberar_iterador(aux);
  }
  return it;
}

bool esta_ordenada_por_frase(cadena_t cad){
  bool ret = true;
  if(!es_vacia_cadena(cad)){
    localizador_t loc = inicio_cadena(cad);
    while(ret && es_localizador(siguiente(loc, cad))){
      localizador_t next = siguiente(loc, cad);
      char *small = frase_info(info_cadena(loc, cad));
      char *big = frase_info(info_cadena(next, cad));
      if(strcmp(small, big) >= 0)
        ret = false;
      else
        loc = siguiente(loc, cad);
    }
  }
  return ret;
}

static void print_lvl(binario_t tree, int lvl){
  if(tree == NULL)
    return;
  if(lvl == 1){
    char *str = frase_info(raiz(tree));
    printf("%s ", str);
  }else if(lvl > 1){
    print_lvl(izquierdo(tree), lvl-1);
    print_lvl(derecho(tree), lvl-1);
  }
}

void imprimir_por_niveles(binario_t b){
  for(nat height = altura_binario(b); height > 0; height--){
    print_lvl(b, height);
    if(height > 1)
      printf("\n");
  }
}

bool pertenece(int i, cadena_t cad){
  bool ret = false;
  if(!es_vacia_cadena(cad)){
    localizador_t loc = inicio_cadena(cad);
    while(!ret && es_localizador(loc)){
      if(numero_info(info_cadena(loc, cad)) != i)
        loc = siguiente(loc, cad);
      else
        ret = true;
    }
  }
  return ret;
}

nat longitud(cadena_t cad){
  nat len = 0;
  if(!es_vacia_cadena(cad)){
    localizador_t loc = inicio_cadena(cad);
    while(es_localizador(loc)){
      ++len;
      loc = siguiente(loc, cad);
    }
  }
  return len;
}

bool son_iguales(cadena_t c1, cadena_t c2){
  bool ret = true;
  if(!es_vacia_cadena(c1) && !es_vacia_cadena(c2)){
    localizador_t loc1 = inicio_cadena(c1);
    localizador_t loc2 = inicio_cadena(c2);
    while(ret && es_localizador(loc1) && es_localizador(loc2)){
      ret = son_iguales(info_cadena(loc1, c1), info_cadena(loc2, c2));
      loc1 = siguiente(loc1, c1);
      loc2 = siguiente(loc2, c2);
    }
    if((!es_localizador(loc1) && es_localizador(loc2)) || 
        (es_localizador(loc1) && !es_localizador(loc2)))
      ret = false; 
  }else
    ret = (es_vacia_cadena(c1) && es_vacia_cadena(c2));
  return ret;
}

cadena_t concatenar(cadena_t c1, cadena_t c2){
  cadena_t chain = segmento_cadena(inicio_cadena(c1), final_cadena(c1), c1);
  cadena_t sgm = segmento_cadena(inicio_cadena(c2), final_cadena(c2), c2);
  insertar_segmento_despues(sgm, final_cadena(chain), chain);
  liberar_cadena(sgm);
  return chain;
}

static localizador_t mayor(cadena_t chain){
  localizador_t pmayor = inicio_cadena(chain);
  localizador_t iterador = siguiente(inicio_cadena(chain), chain);
  while(es_localizador(iterador) && es_localizador(siguiente(iterador,chain))){
    if(numero_info(info_cadena(iterador, chain)) > numero_info(info_cadena(pmayor, chain)))
      pmayor = iterador;
    iterador = siguiente(iterador, chain);
  }
  return pmayor;
}

void ordenar(cadena_t &cad){
  if(!es_vacia_cadena(cad)){
    localizador_t iterador = inicio_cadena(cad);
    while(es_localizador(iterador) && es_localizador(siguiente(iterador,cad))){
      localizador_t pos_mayor = mayor(cad);
      intercambiar(pos_mayor, iterador, cad);
      iterador = siguiente(iterador, cad);
    }
  }
}

static char *copy_txt(info_t info){
  char *text = new char[strlen(frase_info(info)) + 1];
  strcpy(text, frase_info(info));
  return text;
}

void cambiar_todos(int original, int nuevo, cadena_t &cad){
  localizador_t loc = inicio_cadena(cad);
  while(es_localizador(loc)){
    loc = siguiente_clave(original, loc, cad);
    if(es_localizador(loc)){
      info_t data = info_cadena(loc, cad);
      info_t change = crear_info(nuevo, copy_txt(data));
      cambiar_en_cadena(change, loc, cad);
      liberar_info(data);
      loc = siguiente(loc, cad);
    }
  }
}

cadena_t subcadena(int menor, int mayor, cadena_t cad){
  localizador_t since = siguiente_clave(menor, inicio_cadena(cad), cad);
  localizador_t until = siguiente_clave(mayor, since, cad);
  return segmento_cadena(since, until, cad);
}
