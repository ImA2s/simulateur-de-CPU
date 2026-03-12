#ifndef _PARSER_H_
#define _PARSER_H_

#include "hachageg.h"

/* ********** Définition des types ********** */

typedef struct {
  char* mnemonic;
  char* operand1;
  char* operand2;
} Instruction;

typedef struct {
  Instruction** data_instructions;
  int data_count;
  Instruction** code_instructions;
  int code_count;
  HashMap* labels;
  HashMap* memory_locations;
} ParserResult;


/* ********** Prototypes des fonctions ********** */

// parser une ligne du segment .DATA, renvoie un pointeur sur l'instruction correspondant à cette ligne, NULL si un problème est survenu
Instruction* parse_data_instruction(const char* line, HashMap* memory_locations, int* addr);

// parser une ligne du segment .CODE, renvoie un pointeur sur l'instruction correspondant à cette ligne, NULL si un problème est survenu
Instruction* parse_code_instruction(const char* line, HashMap* labels, int code_count);

// parse les segments .CODE et .DATA d'un fichier écrit en pseudo-assembleur. Renvoie un pointeur sur le ParserResult correspondant, NULL si un problème a été rencontré
ParserResult* parse(const char* filename);

// fonction auxiliaire pour free_parser_result, permet de libérer une instruction
void free_instruction(Instruction* inst);

// libere un ParserResult
void free_parser_result(ParserResult* result);

// fonction auxiliaire pour parser_display, permet d'afficher une instruction
void instruction_display(Instruction* inst);

// affiche les informations contenues dans un ParserResult
void parser_display(ParserResult* p);

#endif
