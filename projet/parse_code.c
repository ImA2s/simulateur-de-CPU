#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

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
        while(bloc1[i] != "\0") {
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
        int currL = 0;
        char c = bloc3[currL];
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