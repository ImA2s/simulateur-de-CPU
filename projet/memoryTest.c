#include "hachageg.h"
#include "memory.h"

void menu() {
  printf("\n---------- MEMORY MANAGEMENT MENU ----------\n");
  printf("0 - Close\n");
  printf("1 - Allocate a segment\n");
  printf("2 - Free a segment\n");
  printf("3 - Show memory informations\n");
}

int main(){
  // HashMap *h=hashmap_create();

  // if(hashmap_insert(h,"hello",(void*)1)){
  //     printf("OK\n");
  // }
  // hashmap_insert(h, "heoll", ((void*)2));
  // hashmap_display(h);

  // printf("get hello : %d\n", (int)hashmap_get(h, "hello"));
  // printf("get heoll : %d\n", (int)hashmap_get(h, "heoll"));

  // printf("remove heoll : %s\n", hashmap_remove(h, "heoll") ? "yes" : "no");

  // hashmap_display(h);
  // hashmap_destroy(h);

  // MemoryHandler* handler = memory_init(1024);
  // memory_display(handler);
  // printf("\ntry allocation 'testSeg' seg 800 300\n");
  // create_segment(handler, "testSeg", 800, 300);
  // memory_display(handler);
  // printf("\nallocation seg 'data' 200 150\n");
  // create_segment(handler, "data", 200, 150);
  // memory_display(handler);
  // printf("\ntry allocation seg 'anotherSeg' 100 200\n");
  // create_segment(handler, "anotherSeg", 100, 200);
  // memory_display(handler);
  // printf("\nallocation seg 'rodata' 0 200\n");
  // create_segment(handler, "rodata", 0, 200);
  // memory_display(handler);
  // printf("\nallocation seg 'cache' 600 424\n");
  // create_segment(handler, "cache", 600, 424);
  // memory_display(handler);
  // printf("\nallocation seg 'heap' 350 250\n");
  // create_segment(handler, "heap", 350, 250);
  // memory_display(handler);
  // printf("\nfree of 'aSeg'\n");
  // remove_segment(handler, "aSeg");
  // memory_display(handler);
  // printf("\nfree of 'rodata'\n");
  // remove_segment(handler, "rodata");
  // memory_display(handler);
  // printf("\nfree of 'heap'\n");
  // remove_segment(handler, "heap");
  // memory_display(handler);
  // printf("\nfree of 'data'\n");
  // remove_segment(handler, "data");
  // memory_display(handler);
  // printf("\nfree of 'cache'\n");
  // remove_segment(handler, "cache");
  // memory_display(handler);
  // printf("\nfree of 'heap'\n");
  // remove_segment(handler, "heap");
  // memory_display(handler);

  //destroy_memory(handler);

  int mem_size;
  printf("Memory init - type the size of the memory (in memory units) :\n");
  scanf(" %d", &mem_size);

  MemoryHandler* handler = memory_init(mem_size);
  if(!handler) {
    printf("fatal error at memory init\n");
    printf("exit\n");
    return 1;
  }
  
  int opt;
  do {
    menu();
    if(scanf(" %d", &opt) == 1) {
      switch(opt) {
        case 1:
          {
            char name[256];
            int start, size;
            printf("Type the name, start adress and size of the segment to allocate :\n");
            if(scanf("%s %d %d", name, &start, &size) == 3) {
              create_segment(handler, name, start, size);
            } else {
              printf("Format error\n");
            }
          }
          break;
        case 2:
          {
            char name[256];
            printf("Type the name of the segment to free :\n");
            scanf("%s", name);
            remove_segment(handler, name);
          }
          break;
        case 3:
          {
            memory_display(handler);
          }
          break;
      }
    } else {
      printf("unknown command\n");
    }
  } while(opt != 0);

  destroy_memory(handler);

return 0;
}
