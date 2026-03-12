#include "hachageg.h"

//convertir une chaîne de caractères en un indice dans la table de hachage
unsigned long simple_hash(const char *str){
  unsigned long res=0;
  int i=0;
  while(str[i]!='\0'){
    res+=str[i];
    i++;
  }
  return res % TABLE_SIZE;
}

HashMap* hashmap_create() {

  // allocation mémoire et vérification de la réussite de l'allocation
  HashMap* hm = (HashMap*)malloc(sizeof(HashMap));
  if(!hm) {
    printf("erreur d'allocation HashMap\n");
    return NULL;
  }

  // initialisation des champs
  hm->size = TABLE_SIZE;
  
  hm->table = (HashEntry*)malloc(hm->size * sizeof(HashEntry));
  if(!hm->table) {
    printf("erreur d'allocation table HashMap\n");
    free(hm);
    return NULL;
  }
  
  for(int i = 0; i < hm->size; i++) {
    hm->table[i].key = NULL;
    hm->table[i].value = NULL;
  }

  return hm;

}


int hashmap_insert(HashMap* map, const char* key, void* value) {
    
  int pos = (int)simple_hash(key);

  //printf("mapPos (%d) before : key : %p -> %s ; value : %p -> %d\n", pos, map->table[pos].key, map->table[pos].key, map->table[pos].value, (int)(map->table[pos].value));

  //probing linéaire en cas de collision
  int cpt = 0;
  while(map->table[pos].value != NULL && map->table[pos].value != TOMBSTONE && cpt < map->size) {
    pos = (pos + 1) % map->size;
    cpt++;
  }

  if(cpt == map->size) {
    printf("insertion impossible : table pleine\n");
    return 0;
  }
  
  map->table[pos].key = strdup(key);
  map->table[pos].value = value;
  //printf("insert key : %p -> %s ; value : %p -> %d at pos map->table[%d]\n", map->table[pos].key, map->table[pos].key, map->table[pos].value, (int)map->table[pos].value, pos);
  return 1;
}


void* hashmap_get(HashMap* map, const char* key) {
  if (!map || !key) return NULL;
  int pos = (int)(simple_hash(key));
  int cpt = 0;
  // parcourir la table à partir de la première clef demandée (pour gérer le probing linéaire)
  while(cpt < TABLE_SIZE) {
      if(map->table[pos].key != NULL && map->table[pos].key != TOMBSTONE && strcmp(key, map->table[pos].key) == 0 && map->table[pos].value != NULL && map->table[pos].value != TOMBSTONE) {
          return map->table[pos].value;
      }
      pos = (pos + 1)%TABLE_SIZE;
      cpt++;
  }
  return NULL;
}

void hashmap_display(HashMap* map) {
  if(map) {
    for(int i = 0; i < map->size; i++) {
      if(map->table[i].value != NULL && map->table[i].value != TOMBSTONE) {
        //printf("map->table[%d].key = %s ; map->table[%d].value = %d\n", i, map->table[i].key, i, *((int*)map->table[i].value));
        printf("\"%s\" -> %d\n", map->table[i].key, *((int*)map->table[i].value));
      }
    }
  }
}

int isEmpty_hashmap(HashMap* map) {
  for(int i = 0; i < map->size; i++) {
    if(map->table[i].value != NULL && map->table[i].value != TOMBSTONE) return 0;
  }
  return 1;
}

int hashmap_remove(HashMap *map, const char *key){
  if (!map || !key) return 0;
  int pos = (int)(simple_hash(key));
  int cpt = 0;
  // parcourir la table (pour gérer le probing linéaire, et supprimer bonne case)
  while(cpt < map->size) {
      if(map->table[pos].key != NULL && map->table[pos].key != TOMBSTONE && strcmp(key, map->table[pos].key) == 0) {
          map->table[pos].key = TOMBSTONE;
          map->table[pos].value=TOMBSTONE;
          return 1;
      }
      pos = (pos + 1) % map->size;
      cpt++;
  }
  return 0;
}

void hashmap_destroy(HashMap *map){
   if(map){
    for(int i=0;i<map->size;i++){
      if(map->table[i].key && map->table[i].key != TOMBSTONE) {
        free(map->table[i].key);
      }
      if(map->table[i].value && map->table[i].value != TOMBSTONE) {
        //printf("lib val %d : %p %d\n", i, map->table[i].value, (int)map->table[i].value);
        free(map->table[i].value);
        //printf("val %d freeed\n", i);
      }
    }
    free(map->table);
    free(map);
  }
}
