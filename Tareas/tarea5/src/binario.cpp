/* 4838059 - 5008417 */
#include "../include/binario.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <limits.h> 

struct rep_binario{
  info_t dato;
  rep_binario *izq;
  rep_binario *der;
};

binario_t crear_binario() { return NULL; }

bool insertar_en_binario(info_t i, binario_t &b){
  bool ret = false;
  if(i != NULL){
    if(b == NULL){
      binario_t insert = new rep_binario;
      insert->dato = i;
      insert->izq = insert->der = NULL;
      b = insert;
      return true;
    }else{ 
      char *b_char = frase_info(b->dato);
      char *i_char = frase_info(i);
      if(strcmp(i_char, b_char) < 0)
        ret = insertar_en_binario(i, b->izq);
      else if(strcmp(i_char, b_char) > 0)
        ret = insertar_en_binario(i, b->der);
    }
  }
  return ret;
}

info_t remover_mayor(binario_t &b){
  info_t res;
  if(b->der == NULL){
    res = b->dato;
    binario_t izq = b->izq;
    delete(b);
    b = izq;
  }else
    res = remover_mayor(b->der);
  return res;
}

static binario_t remove_node(binario_t &root, const char *t, bool &removed){
  if(root == NULL)
    return root;
  char *rem = frase_info(root->dato);
  if(strcmp(rem, t) > 0)
    root->izq = remove_node(root->izq, t, removed);
  else if(strcmp(rem, t) < 0)
    root->der = remove_node(root->der, t, removed);
  else{
    if(root->izq == NULL || root->der == NULL){
      binario_t aux;
      if(root->izq == NULL)
        aux = root->der;
      else
        aux = root->izq;
      liberar_info(root->dato);
      delete(root);
      removed = true;
      root = aux;
      return root;
    }
    info_t aux = root->dato;
    root->dato = remover_mayor(root->izq);
    liberar_info(aux);
    removed = true;
  }
  return root;
}

bool remover_de_binario(const char *t, binario_t &b){
  bool removed = false;
  if(!es_vacio_binario(b)){
    binario_t insert = remove_node(b, t, removed);
    if(insert != NULL)
      insertar_en_binario(insert->dato, b);
  }
  return removed;
}

void liberar_binario(binario_t &b){
  if(!es_vacio_binario(b)){
    liberar_binario(b->izq);
    liberar_binario(b->der);
    liberar_info(b->dato);
    delete(b);
  }
}

bool es_vacio_binario(binario_t b){
  return b == NULL;
}

static bool recursive_AVL(binario_t tree, int *height){
  int lh, rh, left, right = 0;
  if(tree == NULL){
    *height = 0;
    return 1;
  }
  left = recursive_AVL(tree->izq, &lh);
  right = recursive_AVL(tree->der, &rh);
  *height = (lh > rh? lh: rh) + 1;
  if(fabs(lh - rh) > 1)
    return 0;
  else
    return left && right;
}

bool es_AVL(binario_t b){
  int height = 0;
  return recursive_AVL(b, &height);
}

info_t raiz(binario_t b){
  return b->dato;
}

binario_t izquierdo(binario_t b){
 return b->izq;
}

binario_t derecho(binario_t b){
  return b->der;
}

binario_t buscar_subarbol(const char *t, binario_t b){
  binario_t bin;
  if(es_vacio_binario(b)){
    bin = NULL;
  }
  else{
    char *b_char = frase_info(b->dato); 
    if(strcmp(t,b_char) == 0)
      bin = b;
    else{
      if(strcmp(t, b_char) < 0)
        bin = buscar_subarbol(t, b->izq);
      else
        bin = buscar_subarbol(t,b->der); 
    }
  }
  return bin;
}

nat altura_binario(binario_t b){
  int altLeft, altRight;
  if(b == NULL) return 0;
  else{
    altLeft = altura_binario(b->izq);
    altRight = altura_binario(b->der);
    if(altLeft > altRight) return altLeft + 1;
    else return altRight + 1;
  } 
}

nat cantidad_binario(binario_t b){
 if (b == NULL) return 0;  
 else
 return 1 + cantidad_binario(b->izq) + cantidad_binario(b->der);
}

static void re_k(nat &k, binario_t b, info_t &kesimo){
  if(kesimo == NULL){
    if(b->izq != NULL)
      re_k(k, b->izq, kesimo);
    if(k == 1)
      kesimo = b->dato;
    --k;
    if(b->der != NULL)
      re_k(k, b->der, kesimo);
  }
}

info_t kesimo_en_binario(nat k, binario_t b){
  if(b->izq != NULL || k > 1){
    info_t kesimo = NULL;
    re_k(k, b, kesimo);
    return kesimo;
  }
  return b->dato;
}

static void recursive_lin(binario_t b, cadena_t cad){
  if(b != NULL){
    if(b->izq != NULL)
      recursive_lin(b->izq, cad);
    insertar_al_final(copia_info(b->dato), cad);
    if(b->der != NULL)
      recursive_lin(b->der, cad);
  }
}

cadena_t linealizacion(binario_t b){
  cadena_t cad = crear_cadena();
  if (b!= NULL)
    recursive_lin(b, cad);
  return cad; 
}

binario_t filtrado(int clave, binario_t b){
  if(b == NULL)
    return NULL;
  else{
    binario_t left, right;
    left = filtrado(clave, b->izq);
    right = filtrado(clave, b->der);
    if(numero_info(b->dato) < clave){
      binario_t tree = crear_binario();
      insertar_en_binario(copia_info(b->dato), tree);
      tree->izq = left;
      tree->der = right;
      return tree;
    }else if(left == NULL)
      return right;
    else if(right == NULL)
      return left;
    else{
      binario_t tree = crear_binario();
      info_t bigest = remover_mayor(left);
      insertar_en_binario(bigest, tree);
      tree->izq = left;
      tree->der = right;
      return tree;
    }
  }
}

static void recursive_print(int x, binario_t b){
  if(b->der != NULL)
    recursive_print(x+1, b->der);
  char *txt = info_a_texto(b->dato);    
  for(int i = 0; i < x; i++)
    printf("-");
  printf("%s\n", txt);
  delete[] txt;
  if(b->izq != NULL)
    recursive_print(x+1, b->izq);
}

void imprimir_binario(binario_t b){
  printf("\n");
  if(!es_vacio_binario(b))
    recursive_print(0, b);
}

static nat chain_len(cadena_t chain){
  nat count = 0;
  localizador_t aux = inicio_cadena(chain);
  while(es_localizador(aux)){
    aux = siguiente(aux, chain);
    count++;
  }
  return count;
}

static binario_t balanced_tree(nat len, localizador_t &loc, cadena_t &chain){
  if(len <= 0)
    return NULL;
  binario_t left = balanced_tree(len/2, loc, chain);
  binario_t tree = new rep_binario;
  tree->dato = copia_info(info_cadena(loc, chain));
  tree->izq = left;
  loc = siguiente(loc, chain);
  tree->der = balanced_tree(len-(len/2)-1, loc, chain);
  return tree;
}

binario_t crear_balanceado(cadena_t cad){
  binario_t tree = crear_binario();
  if(!es_vacia_cadena(cad)){
    localizador_t loc = inicio_cadena(cad);
    tree = balanced_tree(chain_len(cad), loc, cad);
  }
  return tree;
}
