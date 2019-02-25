
void divide(cadena_t &c1, cadena_t &c2, cadena_t &chain){
  localizador_t slow, fast;
  slow = fast = inicio_cadena(chain);
  while(es_localizador(siguiente(fast, chain)) && es_localizador(siguiente(siguiente(fast, chain), chain))){
    slow = siguiente(slow, chain);
    fast = siguiente(siguiente(fast, chain), chain);
  }
  c1 = separar_segmento(inicio_cadena(chain), slow, chain);
  c2 = separar_segmento(inicio_cadena(chain), final_cadena(chain), chain);
}

void sorted_merge(cadena_t c1, cadena_t c2, cadena_t &merged){
  if(es_vacia_cadena(c1))
    insertar_segmento_despues(c2, final_cadena(merged), merged);
  else if(es_vacia_cadena(c2))
    insertar_segmento_despues(c1, final_cadena(merged), merged);
  localizador_t head1 = inicio_cadena(c1);
  localizador_t head2 = inicio_cadena(c2);
  if(numero_info(info_cadena(head1, c1)) <= numero_info(info_cadena(head2, c2))){
    insertar_al_final(info_cadena(head1, c1), merged);
    if(!es_final_cadena(head1, c1)){
      c1 = separar_segmento(siguiente(head1, c1), final_cadena(c1), c1);
      sorted_merge(c1, c2, merged);
    }else
      insertar_segmento_despues(c2, final_cadena(merged), merged);
  }else{
    insertar_al_final(info_cadena(head2, c2), merged);
    if(!es_final_cadena(head2, c2)){
      c2 = separar_segmento(siguiente(head2, c2), final_cadena(c2), c2);
      sorted_merge(c1, c2, merged);
    }else
      insertar_segmento_despues(c1, final_cadena(merged), merged);
  }
}

void ordenar(cadena_t &cad){
  if(!es_vacia_cadena(cad) && es_localizador(siguiente(inicio_cadena(cad), cad))){
    localizador_t head = inicio_cadena(cad);
    if(!es_localizador(head) || !es_localizador(siguiente(head, cad)))
      return;
    cadena_t c1, c2;
    divide(c1, c2, cad);
    ordenar(c1);
    ordenar(c2);
    sorted_merge(c1, c2, cad);
  }
}


