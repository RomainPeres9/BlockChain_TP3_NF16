#ifndef tp3_h
#define tp3_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Transaction
{
    int idetu;
    float montant;
    char* description;
    struct Transaction *suivant;
    struct Transaction *precedent;
}T_Transaction;

typedef struct date
{
    int jours;
    int mois;
    int annee;
}Date;

typedef struct Block
{
    int idblock;
    Date date;
    T_Transaction* listeTransaction;
    struct Block* suivant;
    struct Block* precedent;
}T_Block;


typedef T_Block* BlockChain;

/************** LISTE DES FONCTIONS **************/

//creerTransaction
T_Transaction* creerTransaction(void);

//creerBlock
T_Block* creerBlock(void);

//1
T_Transaction *ajouterTransaction(int idEtu, float montant, char *descr, T_Transaction *listeTransaction);

//2
BlockChain ajouterBlock(BlockChain bc);

//3
float totalTransactionEtudiantBlock(int idEtu, T_Block b);

//4
float soldeEtudiant(int idEtu, BlockChain bc);

//5
void crediter(int idEtu, float montant, char *descr, BlockChain bc);

//6
int payer(int idEtu, float montant, char *descr, BlockChain bc);

//7
void consulter(int idEtu, BlockChain bc);

//8
int transfert(int idSource, int idDestination, float montant, char *descr, BlockChain bc);

//9
void exprt(BlockChain bc);

//10


//menu
void menu(void);

//liberation des transactions
void liberationTransaction(BlockChain bc);


#endif /* tp3_h */
