/* 5008417 */

#include "../include/info.h"
#include <string.h>
#include <stdio.h>
#include <limits.h>

struct rep_info{
  int num;
  char *frase;
};

info_t crear_info(int num, char *frase){
  info_t nueva = new rep_info;
  nueva->num = num;
  nueva->frase = frase;
  return nueva;
}

info_t copia_info(info_t info){
  info_t copia = new rep_info;
  copia->num = info->num;
  copia->frase = new char[strlen(info->frase) + 1];
  strcpy(copia->frase, info->frase);
  return copia;
}

void liberar_info(info_t &info){
  delete[] info->frase;
  delete info;
}

/* Retorno el numero contenido en la estructura */
int numero_info(info_t info){
  return info->num;
}

/* Retorno la frase contenida en la estructura */
char *frase_info(info_t info){
  return info->frase;
}

/* Comparo los numeros y los strings contenidos en la estructura */
bool son_iguales(info_t i1, info_t i2){
  return i1->num == i2->num && !strcmp(i1->frase, i2->frase);
}

bool es_valida_info(info_t info){
  return (info->num != INT_MAX);
}

char *info_a_texto(info_t info){
  char copia_num[11];
  sprintf(copia_num, "%d", info->num);
  char *texto = new char [strlen(copia_num) + strlen(info->frase) + 4];
  strcpy(texto, "(");
  strcat(texto, copia_num);
  strcat(texto, ",");
  strcat(texto, info->frase);
  strcat(texto, ")");
  return texto;
}

/* 
   - El nuevo numero sera la suma de los numeros en i1 e i2
   - Reservo memoria para el nuevo string
   - Concateno los strings
   - Llamo a la funcion crear_info pasando los datos que procese como parametro
   - Retorno la nueva estructura
*/
info_t combinar_info(info_t i1, info_t i2){
  int num = i1->num + i2->num;
  char *str = new char [strlen(i1->frase) + strlen(i2->frase) + 1];
  strcpy(str, i1->frase);
  strcat(str, i2->frase);
  info_t ret = crear_info(num, str);
  return ret;
}

info_t leer_info(int max){
  info_t leido;
  int num;
  char *cadena = new char[max + 1];
  char simbolo;
  bool error = false;
  scanf(" %c", &simbolo);
  if(simbolo != '(')
    error = true;
  else{
    scanf("%d", &num);
    scanf(" %c", &simbolo);
    if(simbolo != ',')
      error = true;
    else{
      int pos = 0;
      int c = getchar();
      while((c != ')') && (c != '\n')){
        cadena[pos] = c;
        pos++;
        c = getchar();
      }
      cadena[pos] = '\0';
      if(c == '\n'){
        error = true;
        ungetc('\n', stdin);
      }
    }
  }
  if(error){
    char *nulo = new char[1];
    nulo[0] = '\0';
    leido = crear_info(INT_MAX, nulo);
  }else{
    char *frase = new char[strlen(cadena) + 1];
    strcpy(frase, cadena);
    leido = crear_info(num, frase);
  }
  delete[] cadena;
  return leido;
}
