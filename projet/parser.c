#include "parser.h"
#include <stdlib.h>
#include <stdio.h>

Instruction* parse_data_instruction(const char* line, HashMap* memory_locations, int* addr) {

  // lecture de la ligne et stockage des informations dans des variables
  char mnemonic[256], operand1[256], operand2[256];

  sscanf(line, "%s %s %s", mnemonic, operand1, operand2);

  // création de la structure Instruction et vérification de la bonne création
  Instruction* inst = (Instruction*)malloc(sizeof(Instruction));
  if(!inst) {
    fprintf(stderr, "erreur parse_data_instruction malloc Instruction*\n");
    return NULL;
  }

  // lecture du deuxième opérande pour savoir le nombre d'unités mémoires à réserver
  int nbE = 1, currL = 0;
  char c = operand2[currL];
  while(c != '\0') {
    if(c == ',') nbE++;
    currL++;
    c = operand2[currL];
  }

  // allocation d'un espace mémoire pour stocker l'adresse (malloc nécessaire pour la table de hachage)
  int* num = (int*)malloc(sizeof(int));
  if(!num) {
    free(inst->mnemonic);
    free(inst->operand1);
    free(inst->operand2);
    free(inst);
    return NULL;
  }
  *num = *addr;
  //printf("num pointer: %p -> %d\n", num, *num);
  hashmap_insert(memory_locations, mnemonic, (void*)(num)); // insertion des éléments dans la table memory_locations
  *addr += nbE;

  // insertion des données dans la structure Instruction et renvoi de la structure
  inst->mnemonic = strdup(mnemonic);
  inst->operand1 = strdup(operand1);
  inst->operand2 = strdup(operand2);

  return inst;

}

Instruction* parse_code_instruction(const char* line, HashMap* labels, int code_counts) {

    // lecture de la ligne et stockage des informations dans des variables
    char bloc1[256] = "\0", bloc2[256] = "\0", bloc3[256] = "\0";

    sscanf(line, "%s %s %s", bloc1, bloc2, bloc3);

    // variables contenant les informations qui seront stockées dans la structure Instruction, ainsi que le label (si besoin)
    char* mnemonic = NULL;
    char operand1[128] = "\0";
    char operand2[128] = "\0";
    char et[256] = "\0";

    if((strcmp(bloc2, "") == 0) && (strcmp(bloc3, "") == 0)) { // instruction sans operand (PUSH, POP, ALLOC, FREE) et sans label
        
        // copie du mnemonic
        mnemonic = strdup(bloc1);

    } else if((strcmp(bloc2, "") != 0) && (strcmp(bloc3, "") == 0)) { // si le bloc 3 est vide, on a 2 cas possibles

        // CAS 1 : la ligne est une instruction sans operand (PUSH, POP, ALLOC, FREE) avec un label
        // CAS 2 : la ligne est une instruction classique (MOV, ADD, JMP, ...) sans label
        
        // on définit si le premier bloc est un label, dans ce cas on se trouvera dans le cas 1
        int is_label = 0;
        int i = 0;
        while(bloc1[i] != '\0') {
            if(bloc1[i] == ':') { // si on trouve le caractère ':', c'est que le premier bloc est un label, on se trouve alors dans le premier cas
                is_label = 1;
                break;
            }
            i++;
        }

        if(is_label) { // CAS 1

            // réécriture de l'étiquette sans le ':'
            int currL = 0;
            char c = bloc1[currL];
            currL = 0;
            while(c != ':') {
            et[currL] = bloc1[currL];
            currL++;
            c = bloc1[currL];
            }
            et[currL] = '\0';
            printf("found label: %s\n", et);

            // copie du mnemonic
            mnemonic = strdup(bloc2);

        } else { // CAS 2

            // gestion des operands
            int currL = 0;
            char c = bloc2[currL];
            // vérification si présence d'un deuxième opérande
            int op2 = 0;
            while(c !='\0') {
                if(c == ',') {
                    op2 = 1;
                    break;
                }
            currL++;
            c = bloc2[currL];
            }

            if(op2) { // si 2 operands
                // séparation des deux opérandes
                currL = 0;
                while(bloc2[currL] != ',') {
                    operand1[currL] = bloc2[currL];
                    currL++;
                }
                operand1[currL] = '\0';
                currL++;
                int op2ind = 0;
                while(bloc2[currL] != '\0') {
                    operand2[op2ind] = bloc2[currL];
                    currL++;
                    op2ind++;
                }
                operand2[currL] = '\0';

            } else { // réécriture de l'operand 1 dans la variable correspondante

                currL = 0;
                while(bloc2[currL] != '\0') {
                    operand1[currL] = bloc2[currL];
                    currL++;
                }
                operand1[currL] = '\0';

            }

            // copie du mnemonic
            mnemonic = strdup(bloc1);
            
        }

    } else { // instruction classique avec operand et label

        // réécriture de l'étiquette sans le ':'
        int currL = 0;
        char c = bloc1[currL];
        currL = 0;
        while(c != ':') {
        et[currL] = bloc1[currL];
        currL++;
        c = bloc1[currL];
        }
        et[currL] = '\0';
        printf("found label: %s\n", et);

        // gestion des operands
        currL = 0;
        c = bloc3[currL];
        // vérification si présence d'un deuxième opérande
        int op2 = 0;
        while(c !='\0') {
            if(c == ',') {
                op2 = 1;
                break;
            }
            currL++;
            c = bloc3[currL];
        }

        if(op2) { // si 2 operands
            // séparation des deux opérandes
            currL = 0;
            while(bloc3[currL] != ',') {
                operand1[currL] = bloc3[currL];
                currL++;
            }
            operand1[currL] = '\0';
            currL++;
            int op2ind = 0;
            while(bloc3[currL] != '\0') {
                operand2[op2ind] = bloc3[currL];
                currL++;
                op2ind++;
            }
            operand2[currL] = '\0';

        } else { // réécriture de l'operand 1 dans la variable correspondante

            currL = 0;
            while(bloc3[currL] != '\0') {
                operand1[currL] = bloc3[currL];
                currL++;
            }
            operand1[currL] = '\0';

        }

        // copie du mnemonic
        mnemonic = strdup(bloc2);

    }

    // création de la structure Instruction et insertion des données (et label s'il y a)
    Instruction* inst = (Instruction*)malloc(sizeof(Instruction));
    if(inst == NULL) {
        fprintf(stderr, "Instruction malloc failed in parse_code_instruction\n");
        return NULL;
    }
    inst->mnemonic = mnemonic;
    inst->operand1 = strdup(operand1);
    inst->operand2 = strdup(operand2);

    if(et[0] != '\0') { // s'il y a un label

        int* num = (int*)malloc(sizeof(int));
        if(!num) {
            fprintf(stderr, "malloc failed for label num in parse_code_instruction\n");
            free(inst->mnemonic);
            free(inst->operand1);
            free(inst->operand2);
            return NULL;
        }
        *num = code_counts;
        hashmap_insert(labels, et, (void*)num);

    }

    return inst;

}

ParserResult* parse(const char* filename) {

  // ouverture du fichier et vérification de la bonne ouverture
  FILE* f = fopen(filename, "r");
  if(!f) {
    fprintf(stderr, "failed to open file \"%s\" in parse\n", filename);
    return NULL;
  }

  // création du ParserResult, vérification de la bonne création et initialisation des champs
  ParserResult* parser = (ParserResult*)malloc(sizeof(ParserResult));
  if(!parser) {
    fprintf(stderr, "malloc failed for ParserResult in parse\n");
    fclose(f);
    return NULL;
  }

  parser->data_instructions = NULL;
  parser->data_count = 0;
  parser->code_instructions = NULL;
  parser->code_count = 0;
  parser->labels = hashmap_create();
  if(!parser->labels) {
    fprintf(stderr, "failed creation of hashmap labels in parse\n");
    free(parser);
    fclose(f);
    return NULL;
  }
  parser->memory_locations = hashmap_create();
  if(!parser->memory_locations) {
    fprintf(stderr, "failed creation of hashmap memory_location in parse\n");
    hashmap_destroy(parser->labels);
    free(parser);
    fclose(f);
    return NULL;
  }

  // lecture du fichier jusqu'à tomber sur le début d'un segment .DATA ou .CODE
  char buffer[256];
  fgets(buffer, 256, f);
  while((strncmp(buffer, ".DATA", 5) != 0) && (strncmp(buffer, ".CODE", 5) != 0) && !feof(f)) {
    fgets(buffer, 256, f);
  }

  // savoir si le segment rencontré est .DATA ou .CODE
  int is_data = strncmp(buffer, ".DATA", 5)==0 ? 1 : 0;

  if(is_data) {

    // lecture du segment .DATA
    int addr = 0;
    fgets(buffer, 256, f);
    while(strncmp(buffer, ".", 1) != 0 && !feof(f)) { // tant qu'on ne rencontre pas un nouveau segment et que la fin du fichier n'est pas atteinte (si il n'y a pas de segment .CODE par exemple)
      if(strcmp(buffer, "\n") != 0) { 
        if(parser->data_count == 0) {
          parser->data_instructions = (Instruction**)malloc(sizeof(Instruction*));
          parser->data_instructions[parser->data_count] = parse_data_instruction(buffer, parser->memory_locations, &addr);
          parser->data_count++;
        } else {
          parser->data_instructions = (Instruction**)realloc(parser->data_instructions, (++parser->data_count)*sizeof(Instruction*));
          parser->data_instructions[parser->data_count-1] = parse_data_instruction(buffer, parser->memory_locations, &addr);
        }
      }
      fgets(buffer, 256, f);
    }
    if((strcmp(buffer, "\n") != 0) && (strncmp(buffer, ".", 1) != 0) && !feof(f)) { 
        if(parser->data_count == 0) {
          parser->data_instructions = (Instruction**)malloc(sizeof(Instruction*));
          parser->data_instructions[parser->data_count] = parse_data_instruction(buffer, parser->memory_locations, &addr);
          parser->data_count++;
        } else {
          parser->data_instructions = (Instruction**)realloc(parser->data_instructions, (++parser->data_count)*sizeof(Instruction*));
          parser->data_instructions[parser->data_count-1] = parse_data_instruction(buffer, parser->memory_locations, &addr);
        }
      }

    // vérification si le nouveau bloc rencontré est bien .CODE, sinon lecture jsuq'à trouver le segment .CODE
    while(strncmp(buffer, ".CODE", 5) != 0 && !feof(f)) {
      fgets(buffer, 256, f);
    }

    // lecture du segment .CODE
    fgets(buffer, 256, f);
    while((!feof(f)) && (strncmp(buffer, ".", 1) != 0)) { // tant qu'on ne rencontre pas de nouveau segment et que la fin du fichier n'est pas atteinte (si .CODE est le dernier segment par exemple)
      if(strcmp(buffer, "\n") != 0) {
        if(parser->code_count == 0) {
          parser->code_instructions = (Instruction**)malloc(sizeof(Instruction*));
          parser->code_instructions[parser->code_count] = parse_code_instruction(buffer, parser->labels, parser->code_count);
          parser->code_count++;
        } else {
          parser->code_instructions = (Instruction**)realloc(parser->code_instructions, (++parser->code_count)*sizeof(Instruction*));
          parser->code_instructions[parser->code_count-1] = parse_code_instruction(buffer, parser->labels, parser->code_count-1);
        }
      }
      fgets(buffer, 256, f);
    }
    if((strcmp(buffer, "\n") != 0) && (strncmp(buffer, ".", 1) != 0) && !feof(f)) {
      if(parser->code_count == 0) {
        parser->code_instructions = (Instruction**)malloc(sizeof(Instruction*));
        parser->code_instructions[parser->code_count] = parse_code_instruction(buffer, parser->labels, parser->code_count);
        parser->code_count++;
      } else {
        parser->code_instructions = (Instruction**)realloc(parser->code_instructions, (++parser->code_count)*sizeof(Instruction*));
        parser->code_instructions[parser->code_count-1] = parse_code_instruction(buffer, parser->labels, parser->code_count-1);
      }
    }

  } else { // même code que précédemment en changeant les instructions de .DATA par les instructions de .CODE et inversement

    // lecture du segment .CODE
    fgets(buffer, 256, f);
    while((!feof(f)) && (strncmp(buffer, ".", 1) != 0)) { // tant qu'on ne rencontre pas de nouveau segment et que la fin du fichier n'est pas atteinte (s'il n'y a pas de segment .DATA par exemple)
      if(strcmp(buffer, "\n") != 0) {
        if(parser->code_count == 0) {
          parser->code_instructions = (Instruction**)malloc(sizeof(Instruction*));
          parser->code_instructions[parser->code_count] = parse_code_instruction(buffer, parser->labels, parser->code_count);
          parser->code_count++;
        } else {
          parser->code_instructions = (Instruction**)realloc(parser->code_instructions, (++parser->code_count)*sizeof(Instruction*));
          parser->code_instructions[parser->code_count-1] = parse_code_instruction(buffer, parser->labels, parser->code_count-1);
        }
      }
      fgets(buffer, 256, f);
    }
    if((strcmp(buffer, "\n") != 0) && (strncmp(buffer, ".", 1) != 0) && !feof(f)) {
      if(parser->code_count == 0) {
        parser->code_instructions = (Instruction**)malloc(sizeof(Instruction*));
        parser->code_instructions[parser->code_count] = parse_code_instruction(buffer, parser->labels, parser->code_count);
        parser->code_count++;
      } else {
        parser->code_instructions = (Instruction**)realloc(parser->code_instructions, (++parser->code_count)*sizeof(Instruction*));
        parser->code_instructions[parser->code_count-1] = parse_code_instruction(buffer, parser->labels, parser->code_count-1);
      }
    }

    // vérification si le nouveau bloc rencontré est bien .DATA, sinon lecture jsuq'à trouver le segment .CODE
    while(strncmp(buffer, ".DATA", 5) != 0 && !feof(f)) {
      fgets(buffer, 256, f);
    }

    // lecture du segment .DATA
    int addr = 0;
    fgets(buffer, 256, f);
    while(strncmp(buffer, ".", 1) != 0 && !feof(f)) { // tant qu'on ne rencontre pas un nouveau segment et que la fin du fichier n'est pas atteinte (si .DATA est le dernier segment du fichier par exemple)
      if(strcmp(buffer, "\n") != 0) {
        if(parser->data_count == 0) {
          parser->data_instructions = (Instruction**)malloc(sizeof(Instruction*));
          parser->data_instructions[parser->data_count] = parse_data_instruction(buffer, parser->memory_locations, &addr);
          parser->data_count++;
        } else {
          parser->data_instructions = (Instruction**)realloc(parser->data_instructions, (++parser->data_count)*sizeof(Instruction*));
          parser->data_instructions[parser->data_count-1] = parse_data_instruction(buffer, parser->memory_locations, &addr);
        }
      }
      fgets(buffer, 256, f);
    }
    if((strcmp(buffer, "\n") != 0) && (strncmp(buffer, ".", 1) != 0) && !feof(f)) { 
        if(parser->data_count == 0) {
          parser->data_instructions = (Instruction**)malloc(sizeof(Instruction*));
          parser->data_instructions[parser->data_count] = parse_data_instruction(buffer, parser->memory_locations, &addr);
          parser->data_count++;
        } else {
          parser->data_instructions = (Instruction**)realloc(parser->data_instructions, (++parser->data_count)*sizeof(Instruction*));
          parser->data_instructions[parser->data_count-1] = parse_data_instruction(buffer, parser->memory_locations, &addr);
        }
      }

  }

  fclose(f);

  return parser;

}

void free_instruction(Instruction* inst) {

  if(inst) {
  // libération des champs
    free(inst->mnemonic);
    free(inst->operand1);
    free(inst->operand2);

    // libération de la structure
    free(inst);
  }

}

void free_parser_result(ParserResult* result) {

  int i;

  // libération des instructions de .DATA et du champ data_instructions
  for(i = 0; i < result->data_count; i++) {
    free_instruction(result->data_instructions[i]);
  }
  free(result->data_instructions);

  // libération des instructions de .CODE et du champ code_instructions
  for(i = 0; i < result->code_count; i++) {
    free_instruction(result->code_instructions[i]);
  }
  free(result->code_instructions);

  // libération des tabels de hachage
  hashmap_destroy(result->labels);
  hashmap_destroy(result->memory_locations);

  // libération de la structure entière
  free(result);

}

void instruction_display(Instruction* inst) {

  if(inst)
    printf("mnemonique: %s; op1: %s; op2: %s\n", inst->mnemonic, inst->operand1, inst->operand2);

}

void parser_display(ParserResult* p) {

  if(p) {
    // affichage des instructions de .DATA
    printf("data_instructions:\n");
    int i;
    for(i = 0; i < p->data_count; i++) {
      instruction_display(p->data_instructions[i]);
    }
    printf("data_count: %d\n", p->data_count);

    printf("\n**********\n");

    // affichage des instructions de .CODE
    printf("code_instructions:\n");
    for(i = 0; i < p->code_count; i++) {
      instruction_display(p->code_instructions[i]);
    }
    printf("code_count: %d\n", p->code_count);

    printf("\n**********\n");

    // affichage des labels
    printf("labels:\n");
    hashmap_display(p->labels);

    printf("\n**********\n");

    // affichage des memory_locations
    printf("memory_locations:\n");
    hashmap_display(p->memory_locations);
  } else {
    printf("ParserResult vide\n");
  }

}
