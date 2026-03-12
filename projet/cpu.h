#ifndef _CPU_H_
#define _CPU_H_

#include "hachageg.h"
#include "memory.h"
#include "parser.h"
#include <regex.h>

/* ********** Définition des types ********** */

typedef struct {
  MemoryHandler* memory_handler; // gestionnaire de mémoire
  HashMap* context; // Registres (AX, BX, CX, DX)
  HashMap* constant_pool ;
} CPU;


/* ********** Prototypes des fonctions ********** */

// initialise un CPU avec une mémoire de taille memort_size. Renvoie un pointeur sur ce cpu, NULL si l'initialisation échoue
CPU* cpu_init(int memory_size);

// libère la mémoire associée à un cpu
void cpu_destroy(CPU *cpu);

// sauvegarde dans le tableau memory du handler la donnée data à la position pos dans le segment segment_name. Renvoie la donnée si la sauvegarde réussit, NULL sinon
void* store(MemoryHandler* handler, const char* segment_name, int pos, void* data);

// récupère la donnée stockée à la position pos du segment segment_name dans le tableau memory du handler. Renvoie la donnée si celle-ci est trouvée, NULL sinon (par exemple si pos dépasse la taille du segment)
void* load(MemoryHandler* handler, const char* segment_name, int pos);

// alloue un segment dans le cpu pour stocker les variables présentes déclarées dans data_instructions
void allocate_variables(CPU* cpu, Instruction** data_instructions, int data_count);

// affiche les données du segment data_segment (DS) du cpu
void print_data_segment(CPU* cpu);

// vérifier le match d'une expression régulière dans une chaîne. Renvoie 1 si il y a un match, 0 sinon
int matches(const char* pattern, const char* string);

// récupérer la donnée indiquée par l'opérande par adressage direct. Renvoie un pointeur sur cette donnée
void* immediate_addressing(CPU* cpu, const char* operand);

// récupérer la donnée stockée par adressage par registre. Renvoie un pointeur sur cette donnée, NULL si le registre n'existe pas dans le CPU
void* register_addressing(CPU* cpu, const char* operand);

// récupérer la donnée stockée à dans le segment data à l'adresse indiquée par l'opérande, par adressage direct en mémoire. Renvoie un pointeur sur cette donnée, NULL si l'adresse n'est pas dans le segment data ou que la donnée n'existe pas
void* memory_direct_addressing(CPU* cpu, const char* operand);

// récupérer la donnée par adressage indirect par registre. Renvoie un pointeur sur cette donnée, NULL si cette donnée n'est pas trouvée dans l'adresse du DS indiquée dans le registre, ou que le registre n'existe pas dans le cpu
void* register_indirect_addressing(CPU* cpu, const char* operand);

// copier la valeur de src vers dest
void handle_MOV(CPU* cpu, void* src, void* dest);

// résout le type d'adressage. Renvoie un pointeur sur la donnée à la position donnée par l'opérande, NULL si cette donnée n'existe pas
void* resolve_adressing(CPU* cpu, const char* operand);

// set up CPU environment for testing
CPU* setup_test_environment();

// supprimer les caractères séparateurs à la fin d'une chaine. Renvoie la chaine trimée
char* trim(char* str);

// remplace dans str les valeurs correspondant à des clefs de la table value
int search_and_replace(char** str, HashMap* values);

// remplace les constantes dans le résultat d'un parser
int resolve_constants(ParserResult *result);

// alloue le segment code et y stocke les instructions du tablea code_instructions
void allocate_code_segment(CPU *cpu, Instruction **code_instructions, int code_count);

// affiche les instructions stockées dans le segment code (CS) du cpu
void print_code_segment(CPU* cpu);

// gere l'instruction depuis src vers dst dans le cpu
int handle_instruction(CPU* cpu, Instruction* instr, void* src, void* dest);

// execute l'instruction en résolvant l'adresse des opérandes
int execute_instruction(CPU* cpu, Instruction* instr);

// recherche la prochaine instruction dans le segment code du cpu
Instruction* fetch_next_instruction(CPU* cpu);

// exécute un programme préalablement chargé dans le cpu en mode pasà pas
int run_program(CPU* cpu);

// fonction auxiliare, affichage des informations du cpu
void cpu_display(CPU* cpu);

// empile value. Renvoie 1 si l'opération s'est bien passée, -1 sinon
int push_value(CPU* cpu, int value);

// dépile la valeur du sommet de la pile dans des. Renvoie 1 si l'opération s'est bien passée, -1 sinon
int pop_value(CPU* cpu, int* dest);

// récupérer la donnée par adressage avec prefixe de segment explicite. Renvoie la donnée trouvée en cas de réussite, NULL sinon
void* segment_override_adressing(CPU* cpu, const char* operand);

// cherche le premier segment libre correspondant à la stratégie demandée pour allouer ES de taille size. Renvoie l'adresse de début de ce segement libre s'il est trouvé, -1 sinon
int find_free_adress_strategy(MemoryHandler* handler, int size, int strategy);

// alloue le segment ES dans le cpu. Renvoie 1 si l'allocation se passe bien, -1 si elle échoue
int alloc_es_segment(CPU* cpu);

// libere le segment ES. Renvoie 1 si la libération s'effectue correctement, -1 sinon
int free_es_segment(CPU* cpu);

#endif
