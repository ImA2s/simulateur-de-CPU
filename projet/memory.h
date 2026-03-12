#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "hachageg.h"

/* ********** Définition des types ********** */

typedef struct segment {
    int start; // position of the beggining (adress) of the segment in memory
    int size; // size of the segment in memory units
    struct segment* next; // pointer to the next segmet in the linked list
} Segment;

typedef struct memoryHandler {
    void** memory; // array of pointers to allocated memory
    int total_size; // total size of the memory
    Segment* free_list; // linked list of free memory segments
    HashMap* allocated; // hash table (name, segment)
} MemoryHandler;


/* ********** Prototypes des fonctions ********** */

// permet d'initialiser un gestionnaire de mémoire. Renvoie un pointeur sur ce gestionnaire si l'initialisation se passe bien, NULL sinon
MemoryHandler* memory_init(int size);

// création d'un segment avec les attributs correspondants, et le champ next à NULL. Renvoie un pointeur sur le segment si la création se passe bien, NULL sinon
Segment* segment_init(int start, int size);

// cherche un segment libre selon les paramètres donnés. Renvoie un pointeur sur ce segment s'il existe, NULL sinon
Segment* find_free_segment(MemoryHandler* handler, int start, int size, Segment** prev);

// allocation d'un nouveau segment dans le handler. Renvoie 1 si l'allocation s'est bien passée, 0 sinon
int create_segment(MemoryHandler* handler, const char* name, int start, int size);

// enlève un segment mémoire de la zone mémoire allouée pour le remettre à disposition dans la zone libre. Renvoie 1 si la libération du segment se passe bien, 0 sinon
int remove_segment(MemoryHandler* handler, const char* name);

// affichage de l'état de la mémoire
void memory_display(MemoryHandler* handler);

// libérer toute la mémoire associée à un handler
void destroy_memory(MemoryHandler* handler);

#endif