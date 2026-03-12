#include "cpu.h"
#include "memory.h"
#include "hachageg.h"
#include "parser.h"

int main(void) {

  CPU* cpu = cpu_init(1024);
  if(!cpu) return 1;

  // create_segment(cpu->memory_handler, "test", 100, 200);

  // int* n = (int*)malloc(sizeof(int));
  // *n = 1;
  // store(cpu->memory_handler, "test", 75, (void*)n);

  // for(int i = 0; i < 1024; i++) {
  //   if(cpu->memory_handler->memory[i])
  //     printf("i: %d -> %d\n", i, *(int*)(cpu->memory_handler->memory[i]));
  // }

  //printf("pos 75 : %d\n", *(int*)(load(cpu->memory_handler, "test", 75)));

  ParserResult* p = parse("pseudo_assembleur_parser.inst");

  printf("----------\nparser avant resolv_constants:\n");
  parser_display(p);
  printf("----------\n");

  allocate_variables(cpu, p->data_instructions, p->data_count);

  resolve_constants(p);

  allocate_code_segment(cpu, p->code_instructions, p->code_count);

  print_code_segment(cpu);

  print_data_segment(cpu);

  printf("----------\nparser apres resolve_constants\n");
  parser_display(p);

  free_parser_result(p);

  cpu_destroy(cpu);

  return 0;

}
