/* 4838059 - 5008417 */
#include "../include/conjunto.h"
#include "../include/binario.h"

#include "string.h"

struct rep_conjunto{
  binario_t ab;
};

conjunto_t crear_conjunto(){
  conjunto_t conj = new rep_conjunto();
  conj->ab = crear_binario();    
  return conj;
}

conjunto_t construir_conjunto(cadena_t cad){
  conjunto_t conj = crear_conjunto();
  conj->ab = crear_balanceado(cad);    
  return conj;
}

bool incluir(info_t i, conjunto_t &c){
  return insertar_en_binario(i, c->ab);
}

static void add_tree(binario_t add, binario_t &tree){
  if(!es_vacio_binario(add)){
    info_t data = copia_info(raiz(add));
    bool added = insertar_en_binario(data, tree);
    if(!added)
      liberar_info(data);
    add_tree(izquierdo(add), tree);
    add_tree(derecho(add), tree);
  }
}

conjunto_t union_conjunto(conjunto_t s1, conjunto_t s2){
  conjunto_t set = new rep_conjunto();
  add_tree(s1->ab, set->ab);
  add_tree(s2->ab, set->ab);
  return set;
}

static void abInter(binario_t bin1, binario_t bin2, binario_t &binint){
  if(!es_vacio_binario(bin1)){
    if(!es_vacio_binario(buscar_subarbol(frase_info(raiz(bin1)), bin2))){
      info_t data = copia_info(raiz(bin1));
      insertar_en_binario(data, binint);
    }
    abInter(izquierdo(bin1), bin2, binint);
    abInter(derecho(bin1), bin2, binint);
  }
}

conjunto_t interseccion(conjunto_t c1, conjunto_t c2){
  conjunto_t set = crear_conjunto();
  if(!es_vacio_conjunto(c1) && !es_vacio_conjunto(c2))
    abInter(c1->ab, c2->ab, set->ab); 
  return set;
}

static void abDif(binario_t bin1, binario_t bin2, binario_t &binint){
  if(!es_vacio_binario(bin1)){
    if(es_vacio_binario(buscar_subarbol(frase_info(raiz(bin1)), bin2))){
      info_t data = copia_info(raiz(bin1));
      insertar_en_binario(data, binint);
    }
    abDif(izquierdo(bin1), bin2, binint);
    abDif(derecho(bin1), bin2, binint);  
  }
}

conjunto_t diferencia(conjunto_t c1, conjunto_t c2){
  conjunto_t set = crear_conjunto();
  if(!es_vacio_conjunto(c1)){
    if(es_vacio_conjunto(c2))
      add_tree(c1->ab, set->ab);
    else
      abDif(c1->ab, c2->ab, set->ab);
  }
  return set;
}

void excluir(char *t, conjunto_t &c){
  remover_de_binario(t, c->ab);
}

void liberar_conjunto(conjunto_t &c){
  liberar_binario(c->ab);
  delete(c);  
}

bool pertenece_conjunto(char *t, conjunto_t c){
  return !es_vacio_binario(buscar_subarbol(t, c->ab));
}

bool es_vacio_conjunto(conjunto_t c){
  return es_vacio_binario(c->ab);
}

static char *copy_txt(info_t info){
  char *text = new char[strlen(frase_info(info)) + 1];
  strcpy(text, frase_info(info));
  return text;
}

static void recursive_it(binario_t tree, iterador_t it){
  if(!es_vacio_binario(tree)){
    if(!es_vacio_binario(izquierdo(tree)))
      recursive_it(izquierdo(tree), it);
    agregar_a_iterador(copy_txt(raiz(tree)), it);
    if(!es_vacio_binario(derecho(tree)))
      recursive_it(derecho(tree), it);
  }
}

iterador_t iterador_conjunto(conjunto_t c){
  iterador_t it = crear_iterador();
  recursive_it(c->ab, it);
  return it;
}
