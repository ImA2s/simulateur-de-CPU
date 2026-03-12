# Simulateur de CPU simplifié

## Résumé du projet

Ce projet, réalisé dans le cadre du cours de **Structures de Données (2024–2025)**, consiste à développer un simulateur de CPU simplifié capable d’exécuter un programme écrit en pseudo-assembleur.

L’objectif principal est de créer un système capable de lire un programme, d’analyser ses instructions, de gérer la mémoire nécessaire à son exécution et d’exécuter ces instructions à l’aide d’un modèle simplifié de processeur.

Le simulateur doit être capable de traiter directement le code du programme fourni, de gérer l’allocation et l’organisation de la mémoire, puis d’interpréter et exécuter les instructions décrites dans le pseudo-assembleur.

---

# Structures de données utilisées

Le projet repose sur plusieurs structures de données permettant de représenter les différents éléments nécessaires à la simulation du CPU.

---

## Table de hachage

Deux structures principales sont utilisées pour implémenter une table de hachage générique.

### HashEntry

Permet de stocker une valeur générique (pointeur `void*`) associée à une clé dans la table de hachage.

### HashMap

Représente la table de hachage elle-même.

Elle contient la taille totale de la table (**128 dans ce projet**) ainsi qu’un tableau d’entrées `HashEntry`.

Ces structures permettent notamment de stocker différentes informations comme les registres du CPU ou les emplacements mémoire.

---

# Gestion de la mémoire

La mémoire du simulateur est gérée grâce à plusieurs structures.

### Segment

Représente un segment mémoire sous forme de liste chaînée.

Chaque segment contient :

- une adresse de début  
- une taille  
- un pointeur vers le segment suivant  

### MemoryHandler

Représente l’espace mémoire global.

Il contient :

- un tableau de pointeurs génériques pour stocker les données  
- la taille totale de la mémoire  
- une liste chaînée représentant les segments libres  
- une table de hachage représentant les segments déjà alloués  

Chaque entrée de cette table correspond à un segment mémoire identifié par un nom unique.

---

# Représentation des instructions

Le programme pseudo-assembleur est analysé et transformé en structures internes.

### Instruction

Permet de représenter une instruction du programme.

Elle contient le mnémonique et deux opérandes possibles.

### ParserResult

Contient le résultat du parsing d’un programme.

On y trouve :

- les instructions du segment `.DATA`
- les instructions du segment `.CODE`
- le nombre d’instructions de chaque segment
- une table de hachage associant les labels à leur position dans le code
- une table de hachage associant les variables à leur emplacement en mémoire

---

# Structure CPU

La structure **CPU** représente le processeur simulé.

Elle contient :

- un `MemoryHandler` pour gérer la mémoire  
- une table de hachage représentant les registres du CPU (**AX, BX, CX, DX, ZF, SF, IP, etc.**)  
- une table `constant_pool` permettant un accès rapide aux constantes numériques  

---

# Organisation du projet

Le projet est organisé en plusieurs modules, chacun responsable d’un aspect spécifique du simulateur.

---

## Table de hachage
hachageg.h
hachageg.c

Ces fichiers implémentent une table de hachage générique utilisée dans différentes parties du projet.

---

## Gestion de la mémoire
memory.h
memory.c

Ces fichiers implémentent le système de gestion mémoire du simulateur.

Un fichier de test (`memoryTest.c`) permet de tester l’allocation et la libération de la mémoire à l’aide d’un menu interactif.

---

## Parser
parser.h
parser.c

Ces fichiers implémentent un parser de pseudo-assembleur.

Le programme `parserTest.c` permet de tester l’analyse d’un programme et d’afficher les instructions parsées.

---

## CPU
cpu.h
cpu.c

Ces fichiers implémentent le fonctionnement du CPU et l’exécution des programmes.

Ils utilisent les modules précédents (**hachage, mémoire, parser**) et font également appel à `regex.h` pour gérer les expressions régulières dans certaines fonctions d’adressage.

Le programme `cpuTest.c` permet de tester :

- l’initialisation du CPU
- la manipulation des instructions
- la substitution des variables et labels par leurs adresses
- l’allocation en mémoire des instructions et variables

---

# Programme principal

Le fichier `main.c` permet de tester l’exécution d’un programme pseudo-assembleur plus complexe.

Ce programme :

- charge un fichier d’instructions
- exécute les instructions dans le CPU simulé
- permet une exécution **pas à pas du programme**

---

# Compilation

Le projet contient un **Makefile** qui permet de compiler séparément les différents modules.

La commande `clean` permet de supprimer :

- les fichiers objets (`.o`)
- les exécutables générés

---

# Tests et améliorations possibles

Les différents modules ont été testés grâce aux fichiers de test fournis.

---

## memoryTest

Les tests montrent que le système de gestion mémoire fonctionne correctement et qu’aucune fuite mémoire n’est détectée.

---

## parserTest

Le parser fonctionne de manière satisfaisante, mais un problème est identifié :

Si le fichier pseudo-assembleur ne contient pas de ligne vide après la dernière instruction, celle-ci n’est pas prise en compte.

Les analyses avec **Valgrind** montrent toutefois qu’il n’y a pas de fuite mémoire.

---

## cpuTest

Les fonctions liées au CPU produisent des résultats corrects.

Aucune fuite mémoire n’est détectée, mais certaines erreurs non fatales doivent encore être corrigées.

---

## main

L’exécution complète du programme peut provoquer un crash à la fin de l’exécution.

Si l’utilisateur quitte le programme avant la fin, celui-ci se termine correctement.

Les analyses avec **Valgrind** indiquent la présence de fuites mémoire de plusieurs centaines d’octets, probablement liées aux fonctions de libération de la mémoire.

---

# Conclusion

Ce projet permet de mettre en œuvre plusieurs concepts importants liés aux **structures de données et à l’organisation d’un système simulant un processeur**.

Il combine :

- tables de hachage  
- gestion dynamique de la mémoire  
- parsing de code pseudo-assembleur  
- simulation de registres et d’instructions CPU  

Même si certaines améliorations restent possibles, notamment concernant les performances et la gestion complète de la mémoire, le projet constitue une base fonctionnelle permettant d’exécuter un programme pseudo-assembleur dans un environnement simulé.
