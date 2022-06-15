# Outils nécéssaires pour la compilation du projet
Avec ces outils installés, il devrait suffire de modifier les chemins du makefile et de lancer Make pour que la compilation se fasse. 

Résumé : 
- gcc >= 8
- SystemC 2.3.3 compatible (compilé avec gcc8)
- Cross-compilateur riscv
## GCC
Version >= 8

*Nous utilisons une librairie c++ pour la lecture des fichier ELF, qui nécéssite au moins GCC 8 pour fonctionner. On peut séléctionner cette version à la fac avec la commande `scl enable devtoolset-8 bash`*

## SystemC
Version 2.3.3, compilée avec le même compilateur que le projet

*Pour que SystemC fonctionne, il faut qu'il soit compilé avec le même compilateur que le reste du projet, et donc GCC 8. Cela implique de recompiler SystemC sur les ordi de la fac. Dans l'idéal, il faudrait que la bonne version de SystemC soit sur le réseau.*

# Outils nécéssaires pour l'execution de fichiers C ou assembleurs
## Cross-compilateur riscv
Pour executer les test assembleur et C, il nous faut un cross-compilateur riscv. 

Jean-paul Chaput avait dit 
>Le compilateur gcc 8 (scl enable devtoolset-8 bash) dispose
du support pour le RISC-V. Avec l'option -march=rv32i .
Il y a d'autres variantes de l'ISA possibles. Voir
man gcc.

La page affichée avec `man gcc` ne contient aucune mention de risc-v, ou rv, ou quoi que ce soit lié à l'architecture. (j'ai fait la recherche *et* j'ai défilé pour vérifier). De plus, l'option `-march=rv32i` n'a aucun effet et les instruction riscv ne sont pas reconnues. 

*Solution* : j'ai recompilé, sur les ordi de la fac, le cross-compilateur risc-v officiel [https://github.com/riscv-collab/riscv-gnu-toolchain](https://github.com/riscv-collab/riscv-gnu-toolchain), avec les options correctes. La procédure était un petit peu hasardeuse, donc c'est probablement mieux de récupérer directement les binaires fonctionnels sur la sessions de Louis. 

Il suffirait de mettre ces binaires sur le réseau pour que tout le monde y ait accès (~300mo). 

(nb : pour ubuntu, nous utilisons des binaires précompilés disponibles sur github. Ces binaire n'existaient pas pour scientific linux, c'est pourquoi nous avons du recompiler gcc)