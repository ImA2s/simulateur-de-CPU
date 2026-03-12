#include "cpu.h"

// EXPRESSIONS REGULIERES D'ADRESSAGE
#define IMMEDIATE_ADRESSING "^[0-9]+$"
#define REGISTER_ADRESSING "^[A-Z]{2}$"
#define DIRECT_MEMORY_ADRESSING "^\\[[0-9]+\\]$"
#define INDIRECT_MEMORY_ADRESSING "^\\[([A-Z]{2})\\]$"
#define SEGMENT_OVERRIDE_ADRESSING "^\\[([A-Z]{2}):([A-Z]{2})\\]$"

CPU* cpu_init(int memory_size) {

  // créer la structure CPU et initialiser ses champs, vérification que la structure et tous ses champs sont bien alloués
  CPU* cpu = (CPU*)malloc(sizeof(CPU));
  if(!cpu) {
    fprintf(stderr, "malloc error for cpu in cpu_init\n");
    return NULL;
  }
  cpu->memory_handler = memory_init(memory_size);
  if(!cpu->memory_handler) {
    fprintf(stderr, "failed initiation of memory_handler in cpu_init\n");
    free(cpu);
    return NULL;
  }
  cpu->context = hashmap_create();
  if(!cpu->context) {
    fprintf(stderr, "failed creation of context hashmap in cpu_init\n");
    destroy_memory(cpu->memory_handler);
    free(cpu);
    return NULL;
  }
  cpu->constant_pool=hashmap_create();
  if(!cpu->constant_pool) {
    fprintf(stderr, "failed creation of context hashmap in cpu_init\n");
    destroy_memory(cpu->memory_handler);
    hashmap_destroy(cpu->context);
    free(cpu);
    return NULL;
  }

  // créer les registres généraux (AX, BX, CX, DX) dans le context
  int* ax_reg = (int*)malloc(sizeof(int));
  if(!ax_reg) {
    fprintf(stderr, "malloc failed for AX register value in cpu_init\n");
    destroy_memory(cpu->memory_handler);
    hashmap_destroy(cpu->context);
    hashmap_destroy(cpu->constant_pool);
    free(cpu);
    return NULL;
  }
  *ax_reg = 0;
  int* bx_reg = (int*)malloc(sizeof(int));
  if(!bx_reg) {
    fprintf(stderr, "malloc failed for BX register value in cpu_init\n");
    free(ax_reg);
    destroy_memory(cpu->memory_handler);
    hashmap_destroy(cpu->context);
    hashmap_destroy(cpu->constant_pool);
    free(cpu);
    return NULL;
  }
  *bx_reg = 0;
  int* cx_reg = (int*)malloc(sizeof(int));
  if(!cx_reg) {
    fprintf(stderr, "malloc failed for CX register value in cpu_init\n");
    free(ax_reg);
    free(bx_reg);
    destroy_memory(cpu->memory_handler);
    hashmap_destroy(cpu->context);
    hashmap_destroy(cpu->constant_pool);
    free(cpu);
    return NULL;
  }
  *cx_reg = 0;
  int* dx_reg = (int*)malloc(sizeof(int));
  if(!dx_reg) {
    fprintf(stderr, "malloc failed for DX register value in cpu_init\n");
    free(ax_reg);
    free(bx_reg);
    free(cx_reg);
    destroy_memory(cpu->memory_handler);
    hashmap_destroy(cpu->context);
    hashmap_destroy(cpu->constant_pool);
    free(cpu);
    return NULL;
  }
  *dx_reg = 0;

  if(!hashmap_insert(cpu->context, "AX", (void*)ax_reg)) {
    fprintf(stderr, "failed initialization of AX register in cpu_init\n");
    free(ax_reg);
    free(bx_reg);
    free(cx_reg);
    free(dx_reg);
    destroy_memory(cpu->memory_handler);
    hashmap_destroy(cpu->context);
    hashmap_destroy(cpu->constant_pool);
    free(cpu);
    return NULL;
  }
  if(!hashmap_insert(cpu->context, "BX", (void*)bx_reg)) {
    fprintf(stderr, "failed initialization of BX register in cpu_init\n");
    free(ax_reg);
    free(bx_reg);
    free(cx_reg);
    free(dx_reg);
    destroy_memory(cpu->memory_handler);
    hashmap_destroy(cpu->context);
    hashmap_destroy(cpu->constant_pool);
    free(cpu);
    return NULL;
  }
  if(!hashmap_insert(cpu->context, "CX", (void*)cx_reg)) {
    fprintf(stderr, "failed initialization of CX register in cpu_init\n");
    free(ax_reg);
    free(bx_reg);
    free(cx_reg);
    free(dx_reg);
    destroy_memory(cpu->memory_handler);
    hashmap_destroy(cpu->context);
    hashmap_destroy(cpu->constant_pool);
    free(cpu);
    return NULL;
  }
  if(!hashmap_insert(cpu->context, "DX", (void*)dx_reg)) {
    fprintf(stderr, "failed initialization of DX register in cpu_init\n");
    free(ax_reg);
    free(bx_reg);
    free(cx_reg);
    free(dx_reg);
    destroy_memory(cpu->memory_handler);
    hashmap_destroy(cpu->context);
    hashmap_destroy(cpu->constant_pool);
    free(cpu);
    return NULL;
  }

  // créer les registres IP, ZF, SF
  int* ip_reg = (int*)malloc(sizeof(int));
  if(!ip_reg) {
    fprintf(stderr, "malloc failed for IP register value in cpu_init\n");
    free(ax_reg);
    free(bx_reg);
    free(cx_reg);
    free(dx_reg);
    destroy_memory(cpu->memory_handler);
    hashmap_destroy(cpu->context);
    hashmap_destroy(cpu->constant_pool);
    free(cpu);
    return NULL;
  }
  *ip_reg = 0;
  int* zf_reg = (int*)malloc(sizeof(int));
  if(!zf_reg) {
    fprintf(stderr, "malloc failed for ZF register value in cpu_init\n");
    free(ax_reg);
    free(bx_reg);
    free(cx_reg);
    free(dx_reg);
    free(ip_reg);
    destroy_memory(cpu->memory_handler);
    hashmap_destroy(cpu->context);
    hashmap_destroy(cpu->constant_pool);
    free(cpu);
    return NULL;
  }
  *zf_reg = 0;
  int* sf_reg = (int*)malloc(sizeof(int));
  if(!sf_reg) {
    fprintf(stderr, "malloc failed for SF register value in cpu_init\n");
    free(ax_reg);
    free(bx_reg);
    free(cx_reg);
    free(dx_reg);
    free(ip_reg);
    free(zf_reg);
    destroy_memory(cpu->memory_handler);
    hashmap_destroy(cpu->context);
    hashmap_destroy(cpu->constant_pool);
    free(cpu);
    return NULL;
  }
  *sf_reg = 0;

  if(!hashmap_insert(cpu->context, "IP", (void*)ip_reg)) {
    fprintf(stderr, "failed initialization of IP register in cpu_init\n");
    free(ax_reg);
    free(bx_reg);
    free(cx_reg);
    free(dx_reg);
    free(ip_reg);
    free(zf_reg);
    free(sf_reg);
    destroy_memory(cpu->memory_handler);
    hashmap_destroy(cpu->context);
    hashmap_destroy(cpu->constant_pool);
    free(cpu);
    return NULL;
  }
  if(!hashmap_insert(cpu->context, "ZF", (void*)zf_reg)) {
    fprintf(stderr, "failed initialization of ZF register in cpu_init\n");
    free(ax_reg);
    free(bx_reg);
    free(cx_reg);
    free(dx_reg);
    free(ip_reg);
    free(zf_reg);
    free(sf_reg);
    destroy_memory(cpu->memory_handler);
    hashmap_destroy(cpu->context);
    hashmap_destroy(cpu->constant_pool);
    free(cpu);
    return NULL;
  }
  if(!hashmap_insert(cpu->context, "SF", (void*)sf_reg)) {
    fprintf(stderr, "failed initialization of SF register in cpu_init\n");
    free(ax_reg);
    free(bx_reg);
    free(cx_reg);
    free(dx_reg);
    free(ip_reg);
    free(zf_reg);
    free(sf_reg);
    destroy_memory(cpu->memory_handler);
    hashmap_destroy(cpu->context);
    hashmap_destroy(cpu->constant_pool);
    free(cpu);
    return NULL;
  }

  // créer les registres de pile et allouer le segment correspondant
  if(!create_segment(cpu->memory_handler, "SS", 0, 128)) {
    fprintf(stderr, "failed allocation of stack segment in cpu_init\n");
    free(ax_reg);
    free(bx_reg);
    free(cx_reg);
    free(dx_reg);
    free(ip_reg);
    free(zf_reg);
    free(sf_reg);
    destroy_memory(cpu->memory_handler);
    hashmap_destroy(cpu->context);
    hashmap_destroy(cpu->constant_pool);
    free(cpu);
    return NULL;
  }

  int* sp_reg = (int*)malloc(sizeof(int));
  if(!sp_reg) {
    fprintf(stderr, "malloc failed for SP register value in cpu_init\n");
    free(ax_reg);
    free(bx_reg);
    free(cx_reg);
    free(dx_reg);
    free(ip_reg);
    free(zf_reg);
    free(sf_reg);
    destroy_memory(cpu->memory_handler);
    hashmap_destroy(cpu->context);
    hashmap_destroy(cpu->constant_pool);
    free(cpu);
    return NULL;
  }
  *sp_reg = ((Segment*)hashmap_get(cpu->memory_handler->allocated, "SS"))->size - 1;
  int* bp_reg = (int*)malloc(sizeof(int));
  if(!bp_reg) {
    fprintf(stderr, "malloc failed for BP register value in cpu_init\n");
    free(ax_reg);
    free(bx_reg);
    free(cx_reg);
    free(dx_reg);
    free(ip_reg);
    free(zf_reg);
    free(sf_reg);
    free(sp_reg);
    destroy_memory(cpu->memory_handler);
    hashmap_destroy(cpu->context);
    hashmap_destroy(cpu->constant_pool);
    free(cpu);
    return NULL;
  }
  *bp_reg = 0;

  if(!hashmap_insert(cpu->context, "SP", (void*)sp_reg)) {
    fprintf(stderr, "failed initialization of SP register in cpu_init\n");
    free(ax_reg);
    free(bx_reg);
    free(cx_reg);
    free(dx_reg);
    free(ip_reg);
    free(zf_reg);
    free(sf_reg);
    free(sp_reg);
    free(bp_reg);
    destroy_memory(cpu->memory_handler);
    hashmap_destroy(cpu->context);
    hashmap_destroy(cpu->constant_pool);
    free(cpu);
    return NULL;
  }
  if(!hashmap_insert(cpu->context, "BP", (void*)bp_reg)) {
    fprintf(stderr, "failed initialization of BP register in cpu_init\n");
    free(ax_reg);
    free(bx_reg);
    free(cx_reg);
    free(dx_reg);
    free(ip_reg);
    free(zf_reg);
    free(sf_reg);
    free(sp_reg);
    free(bp_reg);
    destroy_memory(cpu->memory_handler);
    hashmap_destroy(cpu->context);
    hashmap_destroy(cpu->constant_pool);
    free(cpu);
    return NULL;
  }

  // créer le registre de l'extra segment
  int* es_reg = (int*)malloc(sizeof(int));
  if(!es_reg) {
    fprintf(stderr, "malloc failed for ES register value in cpu_init\n");
    free(ax_reg);
    free(bx_reg);
    free(cx_reg);
    free(dx_reg);
    free(ip_reg);
    free(zf_reg);
    free(sf_reg);
    free(sp_reg);
    free(bp_reg);
    destroy_memory(cpu->memory_handler);
    hashmap_destroy(cpu->context);
    hashmap_destroy(cpu->constant_pool);
    free(cpu);
    return NULL;
  }
  *es_reg = -1;

  if(!hashmap_insert(cpu->context, "ES", (void*)es_reg)) {
    fprintf(stderr, "failed initialization of ES register in cpu_init\n");
    free(ax_reg);
    free(bx_reg);
    free(cx_reg);
    free(dx_reg);
    free(ip_reg);
    free(zf_reg);
    free(sf_reg);
    free(sp_reg);
    free(bp_reg);
    free(es_reg);
    destroy_memory(cpu->memory_handler);
    hashmap_destroy(cpu->context);
    hashmap_destroy(cpu->constant_pool);
    free(cpu);
    return NULL;
  }

  return cpu;

}

 void cpu_destroy(CPU *cpu){

  if(cpu) {

    destroy_memory(cpu->memory_handler);
    printf("cpu memory destroyed\n");
    hashmap_destroy(cpu->context);
    printf("cpu context destroyed\n");
    hashmap_destroy(cpu->constant_pool);
    printf("cpu constant_pool destroyed\n");
    free(cpu);
    printf("cpu destroyed\n");
  }

}

void* store(MemoryHandler* handler, const char* segment_name, int pos, void* data){

  // vérification si segment_name est bien un segment alloué (dans ce cas, found correspond aux données de ce segment(start, size, next), sinon found=NULL)
  void* found = hashmap_get(handler->allocated, segment_name);
  if(found){
    Segment* seg = (Segment*)found;
    if(seg->size > pos) {
      handler->memory[seg->start + pos]=data;
      return data ;
    }
  }
  return NULL ;

}

void* load(MemoryHandler* handler, const char* segment_name, int pos) {

  // vérifier si le segment demandé est bien un segment alloué
  Segment* seg = (Segment*)hashmap_get(handler->allocated, segment_name);
  // si le segment est bien un segment alloué et que pos ne dépasse pas la taille du segment
  if(seg && pos < seg->size) return handler->memory[seg->start + pos];

  return NULL;

}

void allocate_variables(CPU* cpu, Instruction** data_instructions, int data_count) {

  int space = 0;
  int nbE, currL;
  int i;

  // calcul de l'espace nécessaire pour l'allocation du segment DS (si il y a des tableaux, chaque élément compte pour un espace)
  for(i = 0; i < data_count; i++) {

    char* op2 = data_instructions[i]->operand2;
    nbE = 1;
    currL = 0;
    char c = op2[currL];
    while(c != '\0') {
      if(c == ',') nbE++;
      currL++;
      c = op2[currL];
    }

    space += nbE;

  }

  // allouer le segment DS dans le premier segment libre répondant à la contrainte d'espace
  Segment* ptr = cpu->memory_handler->free_list;
  int allocated = 0;
  while(ptr && !(allocated = create_segment(cpu->memory_handler, "DS", ptr->start, space))) {
    ptr = ptr->next;
  }

  // si le segment a pu être alloué
  if(allocated) {
    // récupérer les informations du segment DS (start, size notamment)
    Segment* seg = (Segment*)(hashmap_get(cpu->memory_handler->allocated, "DS"));
    int start = seg->start; // variable utilisée pour désigner le début d usegment DS et éviter de réécrire chaque fois seg->start
    int allocated_variables=0; // compteur du nombre de variables déjà allouées (nécessaire pour pouvoir allouer correctement un tableau de variables)
    // pour chaque instruction du tableau data_instructions
    for(i = 0; i < data_count; i++) {

      // récupération sous la forme d'une chaine de caractères la valeur à allouer
      char* val_to_allocate = (char*)malloc(sizeof(char));
      if(!val_to_allocate) {
        fprintf(stderr, "failed malloc to allocate %s as char\n", data_instructions[i]->mnemonic);
        continue;
      }

      char* op2 = data_instructions[i]->operand2; // lecture dans le deuxième opérande de l'instruction courante
      *val_to_allocate = op2[0];
      nbE = 1;
      char currC = 1;
      // lecture jusqu'à la fin de la chaine représentant la variable (ou le tableau de variables)
      while(*(op2+currC) != '\0') {

        // cas d'un  tableau de variables, quand on rencontre une virgule
        if(*(op2+currC) == ',') {

          // dans ce cas, allouer la valeur construite avant la virgule, puis reprendre la lecture de la suite du tableau après la virgule
          val_to_allocate = (char*)realloc(val_to_allocate, (++nbE)*sizeof(char));
          val_to_allocate[nbE-1] = '\0';

          int* int_to_allocate = (int*)malloc(sizeof(int));
          if(!int_to_allocate) {
          fprintf(stderr, "failed malloc to allocate %s as int\n", val_to_allocate);
          continue;
          }

          *int_to_allocate = atoi(val_to_allocate); // conversion de string vers int de la valeur à allouer
          cpu->memory_handler->memory[start+allocated_variables] = (void*)int_to_allocate; // placement au bon endroit dans le tableau memory
          allocated_variables++;

          // mise à jour des variables pour pouvoir continuer la lecturede la suite du tableau
          nbE = 0;
          free(val_to_allocate);
          val_to_allocate = (char*)malloc(sizeof(char));
          val_to_allocate[0] = *(op2+(++currC));
        } else {
          // ajout du nouveau chiffre lu dans la chaine de la valeur à allouer
          val_to_allocate = (char*)realloc(val_to_allocate, (++nbE)*sizeof(char));
          val_to_allocate[nbE-1] = *(op2+currC);
          currC++;
        }
      }
      val_to_allocate[nbE] = '\0'; // fin de la chaine de la valeur à allouer

      int* int_to_allocate = (int*)malloc(sizeof(int));
      if(!int_to_allocate) {
        fprintf(stderr, "failed malloc to allocate %s as int\n", val_to_allocate);
        continue;
      }

      *int_to_allocate = atoi(val_to_allocate); // conversion de string vers int de la valeur à allouer
      cpu->memory_handler->memory[start+allocated_variables] = (void*)int_to_allocate; // placement au bon endroit dans le tableau memory
      allocated_variables++;
      free(val_to_allocate);

    }
  } else {
    fprintf(stderr, "unable to allocate segment \"DS\" - no free segment fitting the requirements was found\n");
  }

}

void print_data_segment(CPU* cpu) {

  if(cpu) {
    // récupération du segment DS si il existe dans le cpu
    Segment* ds = hashmap_get(cpu->memory_handler->allocated, "DS");
    if(ds) {
      printf("Data Segment:\n");
      for(int i = 0; i < ds->size; i++) {
        if(cpu->memory_handler->memory[ds->start+i]) printf("%d (@%d)   |   ", *((int*)cpu->memory_handler->memory[ds->start+i]), ds->start+i);
      }
    } else {
      printf("no data segment allocated");
    }
    printf("\n");
  }

}

int matches(const char* pattern, const char* string) {

  regex_t regex;
  int result = regcomp(&regex, pattern, REG_EXTENDED);
  if(result) {
    fprintf(stderr, "Regex compilation failed for pattern: %s\n", pattern);
    return 0;
  }
  result = regexec(&regex, string, 0, NULL, 0);
  regfree(&regex);
  return result == 0;

}

void* immediate_addressing(CPU* cpu, const char* operand){

  // test si l'opérande est bien un adressage immédiat
  if(!matches(IMMEDIATE_ADRESSING, operand)) {
    //fprintf(stderr, "operand is not an immediate adressing\n");
    return NULL;
  }

  // récupérer la velur dans la constant_pool du cpu, l'y ajouter si celle-ci ne s'y trouve pas
  void* val = hashmap_get(cpu->constant_pool, operand);
  if(val) return val;
  int* alloc_val = (int*)malloc(sizeof(int));
  if(!alloc_val) {
    fprintf(stderr, "failed malloc for alloc_val in immediate_adressing\n");
    return NULL;
  }
  *alloc_val = atoi(operand);
  hashmap_insert(cpu->constant_pool, operand, (void*)alloc_val);
  return (void*)alloc_val;

}

void* register_addressing(CPU* cpu, const char* operand){

  // vérifier que l'opérande est bien un adressage par registre
  if(!matches(REGISTER_ADRESSING, operand)){
    //fprintf(stderr, "operand is not a register adressing\n");
    return NULL;
  }

  // récupérer la valeur stockées dans le registre
  void *val = hashmap_get(cpu->context,operand);
  if(!val){
    fprintf(stderr, "empty register %s\n", operand);
    return NULL;
  }
  return val ;
}

void* memory_direct_addressing(CPU* cpu, const char* operand){

  // vérifier si l'opérande est bien un adressage mémoire direct
  if(!matches(DIRECT_MEMORY_ADRESSING, operand)) {
    //fprintf(stderr, "operand is not a memory direct adressing\n");
    return NULL;
  }

  // récupérer l'adresse mémoire demandée
  char* str_addr = NULL;
  int i = 0, added = 0;
  char c = operand[0];
  while(c != '\0') {
    if(c != '[' && c != ']') {
      if(!str_addr) {
        str_addr = (char*)malloc(sizeof(char));
        *str_addr = c;
        added++;
      } else {
        str_addr = (char*)realloc(str_addr, ++added);
        *(str_addr+added-1) = c;
      }
    }
    c = operand[++i];
  }
  str_addr = (char*)realloc(str_addr, added+1);
  *(str_addr+added) = '\0';

  // convertir l'adresse mémoire en nombre et récupérer la donnée à cette adresse dans le segment data
  int addr = atoi(str_addr);
  return load(cpu->memory_handler, "DS", addr);

}

void* register_indirect_addressing(CPU* cpu, const char* operand){

  // vérifier que l'opérande est bien un adressage mémoire indirect par registre
  if(!matches(INDIRECT_MEMORY_ADRESSING, operand)) {
    //fprintf(stderr, "operand is not a register indirect adressing\n");
    return NULL;
  }

  // récupérer le nom du resgistre et son contenu
  char* registre = (char*)malloc(sizeof(char));
  int i = 0, added = 0;
  char c = operand[0];
  while(c != '\0') {
    if(c != '[' && c != ']') {
      registre = (char*)realloc(registre, (++added)*sizeof(char));
      registre[added-1] = c;
    }
    c = operand[++i];
  }
  registre[added] = '\0';
  void *val_void = hashmap_get(cpu->context,registre);
  if(!val_void){
    free(registre);
    return NULL;
  }
  free(registre);

  // récupérer la donnée stockée dans le segment data à l'adresse récupérée dans le registre
  int* val_int = (int*)val_void;
  return load(cpu->memory_handler, "DS", *val_int);
}

void handle_MOV(CPU* cpu, void* src, void* dest) {

  // copie avec cast en int de la valeur pointée par src dans l'espace pointé par dest
  int* int_src = (int*)src;
  *((int*)dest) = *int_src;

}

void* resolve_adressing(CPU* cpu, const char* operand) {

  // tester l'adressage immédiat
  void* data;
  data = immediate_addressing(cpu, operand);
  if(data != NULL) {
    return data;
  }
  // si échec de l'adressage immédiat, tester l'adressage par registre
  data = register_addressing(cpu, operand);
  if(data != NULL) {
    return data;
  }
  // si également échec de l'adressage par registre, tester l'adressage mémoire direct
  data = memory_direct_addressing(cpu, operand);
  if(data != NULL) {
    return data;
  }
  // si échec aussi, tester l'adressage indirect par registre
  data = register_indirect_addressing(cpu, operand);
  if(data != NULL) {
    return data;
  }
  // si encore échec, tester l'adressage avec préfixe de segment explicite
  data = segment_override_adressing(cpu, operand);
  return data; // si l'adressage a échoué, data==NULL, on renvoie donc bien la bonne valeur dans chaque cas de figure

}

CPU* setup_test_environment() {

  // initialiser le cpu
  CPU* cpu = cpu_init(1024);
    if(!cpu) {
      fprintf(stderr, "Error: CPU initialization failed\n");
      return NULL;
    }

    // initialiser les registres avec des valeurs spécifiques
    int* ax = (int*)hashmap_get(cpu->context, "AX");
    int* bx = (int*)hashmap_get(cpu->context, "BX");
    int* cx = (int*)hashmap_get(cpu->context, "CX");
    int* dx = (int*)hashmap_get(cpu->context, "DX");

    *ax = 3;
    *bx = 6;
    *cx = 100;
    *dx = 0;

    // créer et initialiser le segment de données
    if(!hashmap_get(cpu->memory_handler->allocated, "DS")) {
      create_segment(cpu->memory_handler, "DS", cpu->memory_handler->free_list->start, 20);

      // initialiser le segment de données avec des valeurs de test
      for(int i = 0; i < 10; i++) {
        int* value = (int*)malloc(sizeof(int));
        *value = i * 10 + 5;
        store(cpu->memory_handler, "DS", i, value);
      }
    }
    printf("Test environment initialized.\n");
    return cpu;
}

char* trim(char* str) {

  while((*str == ' ') ||(*str == '\t') || (*str == '\n') || (*str == '\r')) str++;

  char* end = str + strlen(str) - 1;
  while((end > str) && ((*end == ' ') ||(*end == '\t') || (*end == '\n') || (*end == '\r'))) {
    *end = '\0';
    end--;
  }
  return str;

}

int search_and_replace(char** str, HashMap* values) {

  if((!str) ||(!(*str)) || (!values)) return 0;

  int replaced = 0;
  char* input = *str;

  // Iterate through all keys in the hashmap
  for(int i = 0; i < values->size; i++) {
    if(values->table[i].key && values->table[i].key != (void*)-1) {
      char* key = values->table[i].key;
      int value = *((int*)values->table[i].value);

      //printf("s&r, key: %s, value: %d\n", key, value);

      // Find potential substring match
      char* substr = strstr(input, key);
      if(substr) {
        // Construct replacement buffer
        char replacement[64];
        snprintf(replacement, sizeof(replacement), "%d", value);

        // Calculate lengths
        int key_len = strlen(key);
        int repl_len = strlen(replacement);
        //int remain_len = strlen(substr + key_len);

        // Create new string
        char* new_str = (char*)malloc(strlen(input) - key_len + repl_len + 1);
        strncpy(new_str, input, substr-input);
        new_str[substr-input] = '\0';
        strcat(new_str, replacement);
        strcat(new_str, substr+key_len);

        // Free and update original string
        free(input);
        *str = new_str;
        input = new_str;

        replaced = 1;
      }
    }
  }

  // Trim the final string
  if(replaced) {
    char* trimmed = trim(input);
    if(trimmed != input) {
      memmove(input, trimmed, strlen(trimmed)+1);
    }
  }

  return replaced;

}


int resolve_constants(ParserResult *result){

  Instruction** code = result->code_instructions; // récupérer le tableau d'insctructions .CODE du parser
  for(int i = 0; i < result->code_count; i++) {
    Instruction* inst = code[i];
    if(strcmp(inst->operand2, "")==0) { // si il n'y a pas de deuxième opérande, c'est une étiquette qu'il faut remplacer
      search_and_replace((char**)(&inst->operand1), result->labels);
    } else { // sinon, on remplace, si besoin, les opérandes par leurs valeurs en mémoire
      search_and_replace((char**)(&inst->operand1), result->memory_locations);
      search_and_replace((char**)(&inst->operand2), result->memory_locations);
    }
  }

  return 1;

 }

void allocate_code_segment(CPU *cpu, Instruction **code_instructions, int code_count){

  Segment* ptr = cpu->memory_handler->free_list;
  int allocated = 0;
  while(ptr && !(allocated = create_segment(cpu->memory_handler, "CS", ptr->start, code_count))) {
    ptr = ptr->next;
  }

  if(allocated) {

    for(int i = 0; i < code_count; i++) {
      Instruction* inst = code_instructions[i];
      store(cpu->memory_handler, "CS", i, (void*)inst);
    }

    *((int*)hashmap_get(cpu->context, "IP")) = 0;

  }

}

void print_code_segment(CPU* cpu) {

  if(cpu) {
    // récupération du segment DS si il existe dans le cpu
    Segment* cs = hashmap_get(cpu->memory_handler->allocated, "CS");
    if(cs) {
      printf("Code Segment:\n");
      for(int i = 0; i < cs->size; i++) {
        if(cpu->memory_handler->memory[cs->start+i]) {
          printf("@%d : ", cs->start+i);
          instruction_display((Instruction*)(cpu->memory_handler->memory[cs->start+i]));
        }
      }
    } else {
      printf("no code segment allocated");
    }
    printf("\n");
  }

}

int handle_instruction(CPU* cpu, Instruction* instr, void* src, void* dest) {

  char* mnemonic = strdup(instr->mnemonic); // récupérer le mnemonic (pour avoir une écriture moins lourde par la suite)
  int return_value = 1; // valeur à retourner (vaudra 0 si une instruction échoue)

  if(strcmp(mnemonic, "MOV") == 0) { // instruction MOV

    handle_MOV(cpu, src, dest);

  } else if(strcmp(mnemonic, "ADD") == 0) { // instruction ADD

    int isrc = *((int*)src);
    int idest = *((int*)dest);

    *((int*)dest) = isrc+idest;

  } else if(strcmp(mnemonic, "CMP") == 0) { // instruction CMP

    int isrc = *((int*)src);
    int idest = *((int*)dest);

    int comp = idest - isrc;

    if(comp == 0) {
      *((int*)hashmap_get(cpu->context, "ZF")) = 1;
    } else {
      *((int*)hashmap_get(cpu->context, "ZF")) = 0;
    }
    if(comp < 0) {
      *((int*)hashmap_get(cpu->context, "SF")) = 1;
    } else {
      *((int*)hashmap_get(cpu->context, "SF")) = 0;
    }

  } else if(strcmp(mnemonic, "JMP") == 0) { // instruction JMP

    *((int*)hashmap_get(cpu->context, "IP")) = atoi(instr->operand1);

  } else if(strcmp(mnemonic, "JZ") == 0) { // instruction JZ

    if(*((int*)hashmap_get(cpu->context, "ZF")) == 1) {
      *((int*)hashmap_get(cpu->context, "IP")) = atoi(instr->operand1);
    }

  } else if(strcmp(mnemonic, "JNZ") == 0) { // instruction JNZ

    if(*((int*)hashmap_get(cpu->context, "ZF")) == 0) {
      *((int*)hashmap_get(cpu->context, "IP")) = atoi(instr->operand1);
    }

  } else if(strcmp(mnemonic, "HALT") == 0) { // instruction HALT

    Segment* cs = (Segment*)hashmap_get(cpu->memory_handler->allocated, "CS");
    *((int*)hashmap_get(cpu->context, "IP")) = cs->size;

  } else if(strcmp(mnemonic, "PUSH") == 0) { // instruction PUSH

    int* push = (src == NULL) ? (int*)hashmap_get(cpu->context, "AX") : (int*)src;
    return_value = push_value(cpu, *push);
    if(!return_value) fprintf(stderr, "unable to push value on stack\n");

  } else if(strcmp(mnemonic, "POP") == 0) { // instruction POP

    return_value = pop_value(cpu, (src == NULL) ? resolve_adressing(cpu, "AX") : src);
    if(!return_value) fprintf(stderr, "unable to pop value of stack\n");

  } else if(strcmp(mnemonic, "ALLOC") == 0) { // instruction ALLOC

    if(!(return_value = alloc_es_segment(cpu))) fprintf(stderr, "unable to allocate ES segment\n");

  } else if(strcmp(mnemonic, "FREE") == 0) { // instruction FREE

    if(!(return_value = free_es_segment(cpu))) fprintf(stderr, "unable to free ES segment\n");

  } else if(strcmp(mnemonic, "JS") == 0) { // instruction JS

    if(*((int*)hashmap_get(cpu->context, "SF")) == 1) {
      *((int*)hashmap_get(cpu->context, "IP")) = atoi(instr->operand1);
    }

  } else if(strcmp(mnemonic, "JNS") == 0) { // instruction JNS

    if(*((int*)hashmap_get(cpu->context, "SF")) == 0) {
      *((int*)hashmap_get(cpu->context, "IP")) = atoi((char*)instr->operand1);
    }

  }

  free(mnemonic);

  return return_value;

}

int execute_instruction(CPU* cpu, Instruction* instr) {

  void* src = NULL;
  void* dest = NULL;

  int op1 = (strcmp((char*)instr->operand1, "") != 0);
  int op2 = (strcmp((char*)instr->operand2, "") != 0);

  if(op1 && op2) {
    dest = resolve_adressing(cpu, (char*)instr->operand1);
    src = resolve_adressing(cpu, (char*)instr->operand2);
  } else if(op1) {
    src = resolve_adressing(cpu, (char*)instr->operand1);
  }

  return handle_instruction(cpu, instr, src, dest);

}

Instruction* fetch_next_instruction(CPU* cpu) {

  if(!hashmap_get(cpu->memory_handler->allocated, "CS") || !hashmap_get(cpu->context, "IP")) {
    return NULL;
  }

  int* ip = (int*)hashmap_get(cpu->context, "IP");
  Segment* cs = (Segment*)hashmap_get(cpu->memory_handler->allocated, "CS");

  if(*ip > cs->size -1) {
    return NULL;
  }

  Instruction* inst = (Instruction*)load(cpu->memory_handler, "CS", (*ip));

  (*ip)++;

  return inst;

}

int run_program(CPU* cpu) {

  // affichage à l'état initial
  printf("CPU at initialization:\n");
  cpu_display(cpu);

  printf("**********\n");
  printf("q - leave program\n");
  printf("enter - run next instruction\n");
  printf("**********\n");

  // récupération de la première instruction
  Instruction* inst = fetch_next_instruction(cpu);
  
  int executed; // booléen indiquant la réussite de l'instruction

  char opt;

  do {
    scanf("%c", &opt);
    if(opt == 10) {
      instruction_display(inst);
      executed = execute_instruction(cpu, inst);
      if(!executed) {
        printf("instruction { \n");
        instruction_display(inst);
        printf(" } could not be executed\n");
        printf("program terminated\n");
        return 0;
      }
      inst = fetch_next_instruction(cpu);
    }

  } while((opt != 'q') && (inst != NULL));

  // affichage à l'état final
  printf("CPU after run\n");
  cpu_display(cpu);

  return 1;

}

void cpu_display(CPU* cpu) {

  printf("Registres:\n");
  hashmap_display(cpu->context);

  printf("**********\n");

  print_data_segment(cpu);

}

int push_value(CPU* cpu, int value) {

  // récupérer le stack segment
  Segment* ss = (Segment*)hashmap_get(cpu->memory_handler->allocated, "SS");

  if(!ss) {
    fprintf(stderr, "no stack segment allocated\n");
    return -1;
  }

  // récupérer le stack pointer
  int* sp = (int*)hashmap_get(cpu->context, "SP");

  // vérifier qu'il n'y a pas dépassement de pile
  if(*sp == 0) {
    fprintf(stderr, "error (stack overflow)\n");
    return -1;
  }

  // allouer la valeur à stocker dans la pile
  int* alloc_val = (int*)malloc(sizeof(int));
  if(!alloc_val) {
    fprintf(stderr, "error\n");
    return -1;
  }
  *alloc_val = value;

  // insérer la valeur dans la mémoire
  int ins = (store(cpu->memory_handler, "SS", *sp, (void*)alloc_val) != NULL) ? 1 : -1;

  // décrémenter le stack pointer (car la pile va de la plus grande vers la plus petite adresse) si l'insertion s'est bien passée
  if(ins == 1) (*sp) = (*sp)-1;

  return ins;

}

int pop_value(CPU* cpu, int* dest) {

  // récupérer le stack segment
  Segment* ss = (Segment*)hashmap_get(cpu->memory_handler->allocated, "SS");

  if(!ss) {
    fprintf(stderr, "no stack segment allocated\n");
    return -1;
  }

  // récupérer le stack pointer
  int* sp = (int*)hashmap_get(cpu->context, "SP");

  // vérifier qu'il n'y a pas de débordement de pile
  if(*sp == ss->size-1) {
    fprintf(stderr, "error (stack underflow)\n");
    return -1;
  }

  // récupérer la valeur à dépiler dans la mémoire
  int* val = (int*)load(cpu->memory_handler, "SS", *sp+1);

  // vérification que la valeur a bien été dépilée
  if(val == NULL) {
    fprintf(stderr, "unable to pop stack value\n");
    return -1;
  }

  // affectation de la valeur dépilée dans la destination demandée et incrémentation du stack pointer
  *dest = *val;
  (*sp) = (*sp)+1;
  return 1;

}

void* segment_override_adressing(CPU* cpu, const char* operand) {

  // test si l'opérande est bien un adressage avec préfixe de segment explicite
  if(!matches(SEGMENT_OVERRIDE_ADRESSING, operand)) {
    //fprintf(stderr, "operand is not a segment override adressing\n");
    return NULL;
  }

  //char c = operand[0];
  //int i = 0; // compteur pour parcourir l'opérande
  //int pos = 0;
  char seg[3] = "\0", reg[3] = "\0";

  /*
  // boucle pour lire le segment
  while(c != ':') {
    if(c != '[') {
      seg[pos] = c;
      pos++;
    }
    c = operand[++i];
  }
  seg[2] = '\0';

  // boucle pour lire le registre
  pos = 0;
  while(c != ']') {
    reg[pos] = c;
    pos++;
    c = operand[++i];
  }
  reg[2] = '\0';
  */
  seg[0] = operand[1];
  seg[1] = operand[2];
  seg[3] = '\0';
  reg[0] = operand[4];
  reg[1] = operand[5];
  reg[3] = '\0';

  int* reg_val = (int*)register_addressing(cpu, reg);
  if(reg_val == NULL) {
    //fprintf(stderr, "segment_override_adressing : unknown specified register\n");
    return NULL;
  }

  return load(cpu->memory_handler, seg, *reg_val);

}

int find_free_adress_strategy(MemoryHandler* handler, int size, int strategy) {

  switch(strategy) {
    case 0:
      {
        Segment* fseg = handler->free_list;
        while(fseg != NULL) {
          if(fseg->size >= size) {
            return fseg->start;
          }
          fseg = fseg->next;
        }
        return -1;
      }
      break;
    case 1:
      {
        Segment* fseg = handler->free_list;
        int diff = handler->total_size+1;
        int curr_diff = handler->total_size +1;
        int addr = -1;

        while(fseg != NULL) {
          curr_diff = fseg->size - size;
          if((curr_diff >= 0) && (curr_diff < diff)) {
            diff = curr_diff;
            addr = fseg->start;
          }
          fseg = fseg->next;
        }
        return addr;
      }
      break;
    case 2:
      {
        Segment* fseg = handler->free_list;
        int diff = -1;
        int curr_diff = -1;
        int addr = -1;

        while(fseg != NULL) {
          curr_diff = fseg->size - size;
          if((curr_diff >= 0) && (curr_diff > diff)) {
            diff = curr_diff;
            addr = fseg->start;
          }
          fseg = fseg->next;
        }
        return addr;
      }
      break;
    default: return -1;
  }

}

int alloc_es_segment(CPU* cpu) {

  // récupérer les informations stockées dans les registres AX et BX
  int size = *((int*)register_addressing(cpu, "AX"));
  int strategy = *((int*)register_addressing(cpu, "BX"));

  // chercher l'adresse de départ du segment selon la stratégie souhaitée
  int start_addr = find_free_adress_strategy(cpu->memory_handler, size, strategy);
  if(start_addr == -1) {
    *((int*)hashmap_get(cpu->context, "ZF")) = 1;
    return -1;
  }

  // allouer le segment
  if(!create_segment(cpu->memory_handler, "ES", start_addr, size)) {
    *((int*)hashmap_get(cpu->context, "ZF")) = 1;
    return -1;
  }

  // initialiser le segment avec des zéros
  for(int i = 0; i < size; i++) {
    int* dat = (int*)malloc(sizeof(int));
    if(dat == NULL) {
      fprintf(stderr, "malloc failed for data value at adress %i in alloc_es_segment\n", i);
      *((int*)hashmap_get(cpu->context, "ZF")) = 1;
      return -1;
    }
    *dat = 0;
    if(!store(cpu->memory_handler, "ES", i, (void*)dat)) {
      fprintf(stderr, "failed to store data at adress %i in alloc_es_segment\n", i);
      *((int*)hashmap_get(cpu->context, "ZF")) = 1;
      return -1;
    }
  }

  // mettre à jour les registres ZF et ES
  *((int*)hashmap_get(cpu->context, "ZF")) = 0;
  *((int*)hashmap_get(cpu->context, "ES")) = start_addr;

  return 1;

}

int free_es_segment(CPU* cpu) {

  Segment* seg = (Segment*)hashmap_get(cpu->memory_handler->allocated, "ES");
  if(!seg) return 1; // si le segment n'existe pas, il n'y a rien à libérer, on renvoie 1 pour éviter de bloquer le programme
  for(int i = 0; i < seg->size; i++) {
    void* val = load(cpu->memory_handler, "ES", i);
    free(val);
  }
  *((int*)hashmap_get(cpu->context, "ES")) = -1;
  return remove_segment(cpu->memory_handler, "ES");
}