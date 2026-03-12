#ifndef _HACHAGEG_H_
#define  _HACHAGEG_H_

#define TOMBSTONE (( void *) -1)
#define TABLE_SIZE 128

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/* ********** Définition des types ********** */

typedef struct hashentry{
  char *key ;
  void *value ;
} HashEntry;

typedef struct hashmap{
  int size;
  HashEntry *table ;
} HashMap ;


/* ********** Prototypes des  fonctions ********** */

// hachage de la clef en un indice dans la table de hachage. Renvoie un entier dans tous les cas
unsigned long simple_hash(const char *str);

// création d'une table de hachage générique. Renvoie un pointeur sur cette table si celle-ci est correctement créée, NULL sinon
HashMap *hashmap_create();

// insèretion d'un élément de valeur value dans la table de hachage map à partir de sa clé key. Renvoie 1 si l'insertion s'est bien passée, 0 sinon
int hashmap_insert(HashMap* map, const char* key, void* value);

// récupération d'un élément à partir de sa clef dans la table map. Renvoie la valeur trouvée, NULL si cette clef ne correspond à aucun espace de la table
void* hashmap_get(HashMap* map, const char* key);

// affichage de la table de hachage
void hashmap_display(HashMap* map);

// vérifie si la table de hachage est vide. Renvoie 1 si la table est vide, 0 sinon
int isEmpty_hashmap(HashMap* map);

// suppression de l'élément identifié par key dans map en remplaçant sa clef et sa valeur par TOMBSTONE. Renvoie 1 si la suppression s'est bien passée, 0 sinon
int hashmap_remove(HashMap *map, const char *key);

// libération de toute la mémoire associée à la table map
void hashmap_destroy(HashMap *map);

#endif
