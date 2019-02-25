/* 4838059 - 5008417 */
#include "../include/info.h"
#include "../include/cadena.h"
#include "../include/uso_cadena.h"
#include "../include/binario.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

struct rep_binario{
  info_t dato;
  rep_binario *izq;
  rep_binario *der;
};

binario_t crear_binario() { return NULL; }

bool insertar_en_binario(info_t i, binario_t &b){
  bool ret = false;
  if(i != NULL){
    binario_t insert = b;
    if(b == NULL){
      insert = new rep_binario;
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

binario_t remove_node(binario_t &root, const char *t, bool &removed){
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


static int recursive_AVL(binario_t b, bool av){
  if(b != NULL || !av){
    int altL, altR = 0;
    altL = recursive_AVL(b->izq, av);
    altR = recursive_AVL(b->der, av);
    if(altR - altL > 1 || altL - altR > 1){ 
      av = false;
      return -1;
    }
    if(altL > altR) return altL + 1;
    else return altR + 1;
  } else return 0;
}

bool es_AVL(binario_t b){
  int alt;
  alt = recursive_AVL(b, true);
  if (alt == -1) return false;  
  return true;
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
