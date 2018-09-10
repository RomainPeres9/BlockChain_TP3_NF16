#include "tp3.h"

//initialisation de transaction
T_Transaction* creerTransaction()
{
    T_Transaction* transac;
    transac = malloc(sizeof(T_Transaction));
    if( transac != NULL ) //Verifier que le malloc s'est bien fait
    {
        transac->idetu = -1;
        transac->montant = 0;
        //strcpy(transac->description,"");
        transac->description = " ";
        transac->suivant = NULL;
        transac->precedent = NULL;
        
        return transac;
    }
    else
    {
        printf("ERROR\n");
        return NULL;
    }
}

//initialisation de block
T_Block* creerBlock()
{
    T_Block* block;
    block = malloc(sizeof(T_Block));
    if( block != NULL )
    {
        block->idblock = 0;
        block->listeTransaction = NULL;
        block->precedent = NULL;
        block->suivant = NULL;
        block->date.jours = 0;
        block->date.annee = 0;
        block->date.mois = 0;
        
        return block;
    }
    else
    {
        printf("ERROR\n");
        return NULL;
    }
}

//1
T_Transaction *ajouterTransaction(int idEtu, float montant, char *descr, T_Transaction *listeTransaction)
{
    T_Transaction* transacAjoutee;
    transacAjoutee = creerTransaction();
    transacAjoutee->idetu = idEtu;
    transacAjoutee->montant = montant;
    //strcpy(transacAjoutee->description, descr);
    transacAjoutee->description = descr;
    
    if( listeTransaction != NULL )
    {
        transacAjoutee->suivant = listeTransaction;
        listeTransaction->precedent = transacAjoutee;
        listeTransaction = transacAjoutee;
        
        return listeTransaction;
    }
    else
    {
        //printf("La liste est vide\n");
        listeTransaction = transacAjoutee;

        return listeTransaction;
    }
}

//2
BlockChain ajouterBlock(BlockChain bc)
{
    T_Block* block;
    block = creerBlock();
    printf("\nEntrez la date du jour (JJ/MM/AAAA) : ");
    scanf("%d/%d/%d",&block->date.jours,&block->date.mois,&block->date.annee);
    if( bc != NULL )
    {
        block->idblock = bc->idblock + 1;
        block->suivant = bc;
        bc->precedent = block->suivant;
        bc = block;
        
        return block;
    }
    else
    {
        block->idblock = 0;
        bc = block;
        
        return block;
    }
}

//3
float totalTransactionEtudiantBlock(int idEtu, T_Block b)
{
    T_Transaction* temp;
    float sommeEATcoin = 0;
    temp = b.listeTransaction;
    if( temp != NULL )
    {
        while(temp != NULL)
        {
            if( temp->idetu == idEtu )
            {
                sommeEATcoin += temp->montant;
            }
            temp = temp->suivant;
        }
        return sommeEATcoin;
    }
    else
    {
        return sommeEATcoin;
    }
}

//4
float soldeEtudiant(int idEtu, BlockChain bc)
{
    T_Block* tempB;
    float sommeEATcoinTotale = 0;
    tempB = bc;
    if( tempB != NULL )
    {
        while( tempB != NULL )
        {
            T_Transaction* tempT;
            tempT = tempB->listeTransaction;
            if( tempT != NULL )
            {
                while( tempT != NULL )
                {
                    if( tempT->idetu == idEtu)
                    {
                        sommeEATcoinTotale += tempT->montant;
                    }
                    tempT = tempT->suivant;
                }
            }
            tempB = tempB->suivant;
        }
        return sommeEATcoinTotale;
    }
    else
    {
        return sommeEATcoinTotale;
    }
}

//5
void crediter(int idEtu, float montant, char *descr, BlockChain bc)
{
    bc->listeTransaction = ajouterTransaction(idEtu, montant, descr, bc->listeTransaction);
}

//6
int payer(int idEtu, float montant, char *descr, BlockChain bc)
{
    int somme;
    somme = montant + soldeEtudiant(idEtu, bc);
    if(bc != NULL)
    {
        if( somme > 0 )
        {
            bc->listeTransaction = ajouterTransaction(idEtu, montant, descr, bc->listeTransaction);
            return 1;
        }
        else
        {
            //printf("L'etudiant n'a plus de sous\n");
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

//7
void consulter(int idEtu, BlockChain bc)
{
    int i;
    T_Transaction* tabTransac[5];
    for(i=0;i<5;i++)//Initialisation du tableau dans lequel sera stocké les pointeurs, pointant vers les transactions
    {
        tabTransac[i] = NULL;
    }
    
    int tabidBloc[5];
    for(i=0;i<5;i++)//Initialisation du tableau dans lequel sera stocké les idblock des transactions
    {
        tabidBloc[i] = -1;
    }
    
    i=0;
    T_Block* tempB;
    tempB = bc;
    if( tempB != NULL )//pour verifier s'il y a au moins 1 bloc dans la blockchain
    {
        T_Transaction* tempT;
        while( (tempB != NULL) && (i != 5) )
        {
            tempT = tempB->listeTransaction;
            if( tempT != NULL )//pour verifier s'il y a au moins 1 transaction d'enregistrée dans le bloc
            {
                while( (tempT != NULL) && (i != 5) )
                {
                    if(tempT->idetu == idEtu)
                    {
                        tabTransac[i] = tempT;
                        tabidBloc[i] = tempB->idblock;
                        i++;
                    }
                    tempT = tempT->suivant;
                }
            }
            tempB = tempB->suivant;
        }
        int j;
        //AFFICHAGE
        if(i > 0)
        {
            printf("********** HISTORIQUE ETUDIANTS %d **********\n",idEtu);
            printf("\n");
            printf("SOLDE = %f\n",soldeEtudiant(idEtu,bc));
            printf("\n%d DERNIERES TRANSACTIONS :\n\n",i);
            for(j=0;j<i;j++)
            {
                printf("->transaction %d :\n  idblock = %d\n  description = %s  montant = %f\n\n",j+1,tabidBloc[j],tabTransac[j]->description,tabTransac[j]->montant);
            }
        }
        else
        {
            printf("0 transactions effectuées\n");
        }
    }
    else
    {
        printf("<< Aucun bloc n'a ete cree >>\n");
    }
}

//8
int transfert(int idSource, int idDestination, float montant, char *descr, BlockChain bc)
{
    if(bc != NULL)
    {
        int x;
        float montantSource = -montant;
        x = payer(idSource, montantSource, descr, bc);

        if(x == 1)
        {
            payer(idDestination, montant, descr, bc);
        }
        return x;
    }
    return 0;
}

//9
void exprt(BlockChain bc)
{
    FILE *fp = NULL;
    fp = fopen("transactions.txt","w+");
    BlockChain tempB;
    tempB = bc;
    while(tempB != NULL)
    {
        T_Transaction* tempT;
        tempT = tempB->listeTransaction;
        while(tempT != NULL)
        {
            fprintf(fp,"%d/%d/%d;%d;%f;%s",tempB->date.jours,tempB->date.mois,tempB->date.annee,tempT->idetu,tempT->montant,tempT->description);
            //fseek(fp,0,SEEK_SET);
            tempT=tempT->suivant;
        }
        tempB = tempB->suivant;
    }
    if(fp != NULL)
    {
        printf("yes");
    }
    fclose(fp);
}

//10

//fonction qui vide le buffer
void clean_stdin(void)
{
    int c;
    
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

//liberation des transactions
void liberationTransaction(BlockChain bc)
{
    T_Block* tempB;
    tempB = bc;
    if(tempB != NULL)
    {
        T_Transaction* tempT;
        T_Transaction* tempTprecedent;
        tempT = tempB->listeTransaction;
        tempTprecedent = tempB->listeTransaction;
        while(tempT != NULL)
        {
            tempT = tempT->suivant;
            if(tempT != NULL)
            {
                //detacher la transaction
                tempT->precedent = NULL;
                tempTprecedent->suivant = NULL;
            }
            free(tempTprecedent);
            tempTprecedent = tempT; //replacer le pointeur
        }
        tempB->listeTransaction = NULL;
        printf("<< Toutes les transactions du bloc %d ont ete libere >>\n\n",tempB->idblock);
    }
}


void menu()
{
    BlockChain BC = NULL;
    int choix = 0;
    while(choix != 8)
    {
        printf("\n                         MENU                        \n\n");
        printf("1 : Afficher la liste des blocs de la BlockChain\n");
        printf("2 : Afficher toutes les transactions d'un bloc\n");
        printf("3 : Afficher toutes les transactions du jour pour un etudiant\n");
        printf("4 : Afficher l'historique pour un etudiant\n");
        printf("5 : Crediter un compte\n");
        printf("6 : Payer un repas\n");
        printf("7 : Transferer des EATCoins entre 2 etudiants\n");
        printf("8 : Quitter\n");
        printf("9 : Exporter toutes les transactions vers un fichier\n");
        printf("10 : Importer des transactions depuis un fichier\n");
        printf("11 : Nouveau jour\n");
        printf("\n[QUE VOULEZ VOUS FAIRE] ");
        scanf("%d",&choix);
        printf("\n");
        switch (choix)
        {
            case 1:
            {
                T_Block* temp;
                temp = BC;
                if(temp != NULL)
                {
                    printf("********** LISTE DES BLOCS **********\n\n");
                    while(temp != NULL)
                    {
                        printf("->Bloc %d\n",temp->idblock);
                        temp = temp->suivant;
                    }
                }
                else
                {
                    printf("<< Aucun bloc n'a ete cree >>\n");
                    printf("\n->Voulez-vous en creer un ? (Saisir '1' si oui) ");
                    int rep;
                    scanf("%d",&rep);
                    if(rep == 1)
                    {
                        BC = ajouterBlock(BC);
                    }
                }
            }
                break;
                
            case 2:
            {
                printf("->Saisir l'identifiant du bloc : ");
                int id;
                scanf("%d",&id);
                T_Block* tempB;
                tempB = BC;
                while( (tempB != NULL) && (tempB->idblock != id) )
                {
                    tempB = tempB->suivant;
                }
                if(tempB == NULL)
                {
                    printf("\n<< Aucun bloc trouvé avec cet identifiant >>\n");
                }
                else
                {
                    T_Transaction* tempT;
                    tempT = tempB->listeTransaction;
                    int i=0;
                    printf("\n********** AFFICHAGE DES TRANSACTIONS DU BLOC %d **********\n",id);
                    while(tempT != NULL)
                    {
                        printf("\n->transaction %d :\n  idEtudiant = %d\n  description = %s  montant = %f\n",i,tempT->idetu,tempT->description,tempT->montant);
                        i++;
                        tempT = tempT->suivant;
                    }
                    if(i == 0)
                    {
                        printf("\n0 transactions effectuées\n");
                    }
                }
            }
                break;
                
            case 3://afficher toutes les transactions du jour pour un etudiant
            {
                if(BC != NULL)
                {
                    T_Transaction* temp;
                    temp = BC->listeTransaction;
                    if(temp != NULL)
                    {
                        printf("->Saisir l'identifiant de l'etudiant : ");
                        int id;
                        scanf("%d",&id);
                        printf("\n********* AFFICHAGE DES TRANSACTIONS DU JOUR DE L'ETUDIANT %d **********\n",id);
                        int i=0;
                        while(temp != NULL)
                        {
                            if(temp->idetu == id)
                            {
                                printf("\n->transaction %d :\n  description = %s  montant = %f\n\n",i,temp->description,temp->montant);
                                i++;
                            }
                            temp = temp->suivant;
                        }
                        if(i == 0)
                        {
                            printf("<< Aucune transaction n'a ete effectuee par l'etudiant %d, aujourd'hui >>\n",id);
                        }
                    }
                    else
                    {
                        printf("<< Aucune transaction n'a ete effectuee aujourd'hui >>\n");
                    }
                }
                else
                {
                    printf("<< La BlockChain ne contient aucun bloc >>\n");
                    printf("\n->Voulez-vous en creer un ? (Saisir '1' si oui) ");
                    int rep;
                    scanf("%d",&rep);
                    if(rep == 1)
                    {
                        BC = ajouterBlock(BC);
                    }
                }
            }
                break;
                
            case 4://afficher l'historique d'un etudiant
            {
                if(BC == NULL)
                {
                    printf("\n<< La BlockChain ne contient aucun bloc >>\n");
                    printf("\n->Voulez-vous en creer un ? (Saisir '1' si oui) ");
                    int rep;
                    scanf("%d",&rep);
                    if(rep == 1)
                    {
                        BC = ajouterBlock(BC);
                    }
                }
                else
                {
                    printf("->Saisir l'identifiant de l'etudiant : ");
                    int id;
                    scanf("%d",&id);
                    printf("\n");
                    consulter(id,BC);
                }
            }
                break;
        
            case 5://crediter un compte
            {
                if(BC == NULL)
                {
                    printf("<< La BlockChain ne contient aucun bloc >>\n");
                    printf("\n->Voulez-vous en creer un ? (Saisir '1' si oui) ");
                    int rep;
                    scanf("%d",&rep);
                    if(rep == 1)
                    {
                        BC = ajouterBlock(BC);
                    }
                }
                else
                {
                    printf("->Saisir l'identifiant de l'etudiant à crediter : ");
                    int id;
                    scanf("%d",&id);
                    printf("\n->Saisir le montant : ");
                    float montant;
                    scanf("%f",&montant);
                    clean_stdin();
                    printf("\n->Entrer une description : ");
                    char description[100];
                    fgets(description, 100, stdin);
                    crediter(id, montant, description, BC); //S'il n'y a pas de BlockChain existante, l'a créer !
                    printf("\n<< Compte crediter >>\n");
                }
            }
                break;
                
            case 6:
            {
                if(BC == NULL)
                {
                    printf("<< La BlockChain ne contient aucun bloc >>\n");
                    printf("\n->Voulez-vous en creer un ? (Saisir '1' si oui) ");
                    int rep;
                    scanf("%d",&rep);
                    if(rep == 1)
                    {
                        BC = ajouterBlock(BC);
                    }
                }
                else
                {
                    printf("->Saisir l'identifiant de l'etudiant qui paie : ");
                    int id;
                    scanf("%d",&id);
                    printf("\n->Saisir le montant : ");
                    float montant;
                    scanf("%f",&montant);
                    clean_stdin();
                    printf("\n->Entrer une description : ");
                    char description[100];
                    fgets(description, 100, stdin);
                    int x;
                    x = payer(id, montant, description,BC);
                    if( x == 0 )
                    {
                        printf("\n<< L'etudiant n'a pas assez d'EATCoins sur son compte >>\n");
                    }
                    else
                    {
                        printf("\n<< Le paiement a ete effectue >>\n");
                    }
                }
            }
                break;
                
            case 7://Transférer des EATCoins entre 2 etudiants
            {
                if(BC == NULL)
                {
                    printf("<< La BlockChain ne contient aucun bloc >>\n");
                    printf("\n->Voulez-vous en creer un ? (Saisir '1' si oui) ");
                    int rep;
                    scanf("%d",&rep);
                    if(rep == 1)
                    {
                        BC = ajouterBlock(BC);
                    }
                }
                else
                {
                    printf("->Saisir l'identifiant de l'etudiant qui effectue le transfert : ");
                    int id1;
                    scanf("%d",&id1);
                    
                    printf("\n->Saisir l'identifiant de l'etudiant qui recoit le transfert : ");
                    int id2;
                    scanf("%d",&id2);
                    while(id2 == id1)
                    {
                        printf("\n->Veuillez resaisir l'identifiant du compte de l'etudiant receveur : ");
                        scanf("%d",&id2);
                    }
                    printf("\n->Saisir le montant du transfert : ");
                    float montant;
                    scanf("%f",&montant);
                    clean_stdin();
                    printf("\n->Saisir une description : ");
                    char description[100];
                    fgets(description, 100, stdin);
                    int x;
                    x = transfert(id1, id2, montant, description, BC);
                    if(x == 1)
                    {
                        printf("\n<< Transfert effectue >>\n");
                    }
                    else
                    {
                        printf("\n<< Transfert refuse (l'etudiant %d n'a pas assez de sous) >>\n ",id1);
                    }
                }
            }
                break;
                
            case 8://liberer les bloc et transactions!!
            {
                T_Block* temp;
                T_Block* tempPrecedent;
                temp = BC;
                tempPrecedent = BC;
                while(temp != NULL)
                {
                    temp = temp->suivant;
                    liberationTransaction(tempPrecedent);
                    //detacher le bloc
                    if(temp != NULL)
                    {
                        temp->precedent = NULL;
                        tempPrecedent->suivant = NULL;
                    }
                    //liberer le bloc
                    free(tempPrecedent);
                    //On replace le pointeur
                    tempPrecedent = temp;
                }
                BC = NULL;
                printf("<< Tous les blocs ont bien ete libere >>\n\n");
            }
                break;
                
            case 9:
            {
                exprt(BC);
            }
                break;
                
            case 10:
                break;
            case 11:
            {
                if(BC != NULL)
                {
                    printf("-> Etes vous sure de changer de jour ? (si oui saisir 1) ");
                    int rep;
                    scanf("%d",&rep);
                    if(rep == 1)
                    {
                        BC = ajouterBlock(BC);
                    }
                }
                else
                {
                    printf("<< La BlockChain ne contient aucun bloc >>\n");
                    printf("\n->Voulez-vous en creer un ? (Saisir '1' si oui) ");
                    int rep;
                    scanf("%d",&rep);
                    if(rep == 1)
                    {
                        BC = ajouterBlock(BC);
                    }
                }
            }
                break;
                
            default:
                printf("<< Veuillez resaisir votre choix >>\n");
                break;
        }
    }
}

