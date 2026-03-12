#include "memory.h"
#include <stdlib.h>
#include <stdio.h>

Segment* segment_init(int start, int size) {

    Segment* seg = (Segment*)malloc(sizeof(Segment));
    if(!seg) {
        printf("malloc failed for Segment in segment_init\n");
        return NULL;
    }
    seg->start = start;
    seg->size = size;
    seg->next = NULL;
    return seg;

}

MemoryHandler* memory_init(int size) {

    MemoryHandler* mem = (MemoryHandler*)malloc(sizeof(MemoryHandler));
    if(!mem) {
        printf("malloc failed for MemoryHandler in memory_init\n");
        return NULL;
    }
    mem->memory = (void*)malloc(size*sizeof(void*));
    for(int i = 0; i < size; i++) {
      mem->memory[i] = NULL;
    }
    mem->total_size = size;

    Segment* seg = segment_init(0,size);
    if(!seg) {
        printf("malloc failed for Segment of MemoryHandler in memory_init\n");
        free(mem);
        return NULL;
    }
    mem->free_list = seg;

    HashMap* map = hashmap_create();
    if(!map) {
        printf("malloc failed for HashMap of MemoryHandler in memory_init\n");
        free(seg);
        free(mem);
        return NULL;
    }
    mem->allocated = map;

    return mem;

}


Segment* find_free_segment(MemoryHandler* handler, int start, int size, Segment** prev) {

    // sortir immédiatement si les paramètres ne sont pas réalistes
    if((size < 1) || (start < 0)) {
        *prev = NULL;
        return NULL;
    }

    // si le premier segment de la liste des segments libres contient le segment demandé
    if(handler->free_list->start <= start && (handler->free_list->start + handler->free_list->size) >= start+size) {
        *prev = NULL;
        return handler->free_list;
    }

    // parcours de la liste des segments libres jusqu'à trouver un segment correspondant
    Segment* seg = handler->free_list;
    while(seg->next) {
        if(seg->next->start <= start && seg->next->size >= size) {
            *prev = seg;
            return seg->next;
        }
        seg = seg->next;
    }

    // si aucun segment rassemblant les critères n'est trouvé
    *prev = NULL;
    return NULL;

}

int create_segment(MemoryHandler* handler, const char* name, int start, int size) {

    Segment* prev;
    Segment* freeSeg = find_free_segment(handler, start, size, &prev);
    if(!freeSeg) {
        printf("segment \"%s\" of %d units at adress @%d couldn't be allocated (not enough space or unavailable adress)\n", name, size, start);
        return 0;
    }

    Segment* new_seg = segment_init(start, size);
    if(!new_seg) {
        printf("malloc failed for Segment new_seg in create_segment\n");
        return 0;
    }

    hashmap_insert(handler->allocated, name, (void*)new_seg);

    Segment* next_seg = freeSeg->next;

    Segment* firstPart, *secondPart;

    firstPart = (start-freeSeg->start > 0) ? segment_init(freeSeg->start, start-freeSeg->start) : NULL;
    secondPart = ((freeSeg->start + freeSeg->size)-(start+size) > 0) ? segment_init(start+size, (freeSeg->start + freeSeg->size)-(start+size)) : NULL;

    if(prev) {
        if(firstPart) {
            prev->next = firstPart;
            if(secondPart) {
                firstPart->next = secondPart;
                if(next_seg) secondPart->next = next_seg;
            } else {
                if(next_seg) firstPart->next = next_seg;
            }
        } else {
            if(secondPart) {
                prev->next = secondPart;
                if(next_seg) secondPart->next = next_seg;
            } else {
                prev->next = next_seg ? next_seg : NULL;
            }
        }
    } else {
        if(firstPart) {
            handler->free_list = firstPart;
            if(secondPart) {
                firstPart->next = secondPart;
                if(next_seg) secondPart->next = next_seg;
            } else {
                if(next_seg) firstPart->next = next_seg;
            }
        } else {
            if(secondPart) {
                handler->free_list = secondPart;
                if(next_seg) secondPart->next = next_seg;
            } else {
                handler->free_list = next_seg ? next_seg : NULL;
            }
        }
    }

    free(freeSeg);

    printf("%d units allocated at adress @%d for segment \"%s\"\n", size, start, name);

    return 1;
}

int remove_segment(MemoryHandler* handler, const char* name) {

    Segment* seg = (Segment*)hashmap_get(handler->allocated, name);
    if(!seg) {
        printf("segment \"%s\" does not correspond to any allocated segment\n", name);
        return 0;
    }

    if(!handler->free_list) {
        handler->free_list = seg;
    } else {
        Segment* ptr = handler->free_list;
        if(ptr->start == seg->start + seg->size) {
            seg->next = ptr->next;
            seg->size += ptr->size;
            free(ptr);
            handler->free_list = seg;
        } else if(ptr->start > seg->start + seg->size) {
            seg->next = ptr;
            handler->free_list = seg;
        } else {
            while(ptr) {

                if(seg->start == ptr->start + ptr->size) { // si le segment à libérer commence à la fin du segment courant
                    ptr->size += seg->size;
                    free(seg);
                    if(ptr->next && (ptr->start + ptr->size == ptr->next->start)) { // si le "nouveau" segment libre finit au début du suivant, fusion des deux
                        Segment* tmp = ptr->next;
                        ptr->size += tmp->size;
                        ptr->next = tmp->next;
                        free(tmp);
                    }
                    break;
                }

                if(ptr->next && ptr->next->start > seg->start + seg->size) {
                    seg->next = ptr->next;
                    if(ptr == handler->free_list){
                        if(ptr->start > seg->start)
                            handler->free_list = seg;
                        else
                            ptr->next = seg;
                    }
                    break;
                }

                if(ptr->next && (ptr->next->start == seg->start + seg->size)) {
                    Segment* tmp = ptr->next;
                    seg->next = tmp->next;
                    seg->size += tmp->size;
                    free(tmp);
                    if(ptr == handler->free_list) handler->free_list = seg;
                    break;
                }

                if(!ptr->next) {
                    ptr->next = seg;
                    break;
                }

                ptr = ptr->next;
            }
        }
    }

    hashmap_remove(handler->allocated, name);
    return 1;

}

void memory_display(MemoryHandler* handler) {

    printf("\n********** MEMORY **********\n");
    printf("Total size : %d units\n", handler->total_size);

    if(handler->free_list) {
        printf("Free memory :\n");
        Segment* seg = handler->free_list;
        while(seg) {
            printf("[%d, %d]\n", seg->start, seg->size);
            seg = seg->next;
        }
    } else {
        printf("No free memory\n");
    }

    if(isEmpty_hashmap(handler->allocated)) {
        printf("No allocated memory\n");
    } else {
        printf("Allocated memory :\n");
        HashMap* map = handler->allocated;
        Segment* seg;
        for(int i = 0; i < map->size; i++) {
            if(map->table[i].value != NULL && map->table[i].value != TOMBSTONE) {
                seg = (Segment*)(map->table[i].value);
                printf("%s : [%d, %d]\n", map->table[i].key, seg->start, seg->size);
            }
        }
    }

}

void destroy_memory(MemoryHandler* handler) {
    hashmap_destroy(handler->allocated);
    Segment* seg = handler->free_list, *tmp;
    while(seg) {
        tmp = seg;
        seg = seg->next;
        free(tmp);
    }
    for(int i = 0; i < handler->total_size - 1; i++) {
      if(handler->memory[i] != NULL) {
        free(handler->memory[i]);
      }
    }
    free(handler->memory);
    free(handler);
}
