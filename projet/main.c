#include "cpu.h"

int main() {

  CPU* cpu = cpu_init(1024);

  ParserResult* p = parse("pseudo_assembleur_code.inst");

  allocate_variables(cpu, p->data_instructions, p->data_count);

  resolve_constants(p);

  allocate_code_segment(cpu, p->code_instructions, p->code_count);

  run_program(cpu);

  cpu_destroy(cpu);

  return 0;

}
