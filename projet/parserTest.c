#include "parser.h"
#include "hachageg.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {

  // HashMap* memloc = hashmap_create();
  // int addr = 0;

  // Instruction* inst = parse_data_instruction("X DW 3", memloc, &addr);
  // Instruction* inst2 = parse_data_instruction("y DB 4", memloc, &addr);
  // Instruction* inst3 = parse_data_instruction("arr DB 5,6,7,8", memloc, &addr);
  // Instruction* inst4 = parse_data_instruction("z DB 9", memloc, &addr);

  // instruction_display(inst);
  // instruction_display(inst2);
  // instruction_display(inst3);
  // instruction_display(inst4);

  // for(int i = 0; i < memloc->size; i++) {
  //   if(memloc->table[i].value != NULL && memloc->table[i].value != TOMBSTONE) {
  //     printf("%s -> %d\n", memloc->table[i].key, (int)(memloc->table[i].value));
  //   }
  // }

  // HashMap* labels = hashmap_create();

  // Instruction* code1 = parse_code_instruction("loop: MOV AX,6", labels, 0);
  // Instruction* code2 = parse_code_instruction("ADD BX,10", labels, 1);
  // Instruction* code3 = parse_code_instruction("end: JMP loop", labels, 2);

  // instruction_display(code1);
  // instruction_display(code2);
  // instruction_display(code3);

  // for(int i = 0; i < labels->size; i++) {
  //   //printf("i: %d %s -> %p\n", i, labels->table[i].key, (labels->table[i].value));
  //   if(labels->table[i].value != NULL && labels->table[i].value != TOMBSTONE) {
  //     printf("%s -> %d\n", labels->table[i].key, (int)(labels->table[i].value));
  //   }
  // }

  ParserResult* p = parse("pseudo_assembleur_parser.inst");
  parser_display(p);
  free_parser_result(p);

  return 0;

}
