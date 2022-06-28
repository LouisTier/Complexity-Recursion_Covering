/*
    Programme de :
    Louis BERTHIER
    Adrien RUGGIERO
    Nathan WITKOWICZ
*/

//Processeur : Intel(R) Core(TM) i5-8265U CPU @ 1.60GHz wuth Turbo Boost up to 3.9GHz

//19 dates que l'on arrive pas à recouvrir, par rapport à 12*31*7 dates



#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>



#define NB_FORMES_INITIALES 10 //Au début on a 10 formes différentes dans le tp
#define NB_FORMES 40 //On a 10 pièces avec chacune 4 rotations (non abstraction des doubles symétries)

/*
    Les figures 4,9 et 10 sont doubles symétriques. Les autres pièces ne présentent aucune symétrie.
    Ainsi au lieu d'avoir 40 pièces différentes, on en aura 34.
*/
#define NB_FORMES_TOTAL 34 //En enlevant les doubles symétries, on a 34 figures à générer
#define NB_ROTATION 4 //Chaque forme a 4 positions : 0° / 90° / 180° et 270°

#define DIM_X_CALENDRIER 13 //Dimension en abscisse de notre calendrier étendu pour permettre la pose de l'ensemble des pièces
#define DIM_Y_CALENDRIER 14 // Dimension en ordonné de notre calendrier étendu pour permettre la pose de l'ensemble des pièces
#define DIM_X_FORME 4 //Chaque figure a une dimension en "abscisse" de 4
#define DIM_Y_FORME 4 //Chaque figure a une dimension en "ordonnée" de 4



char arrayCalendar[DIM_Y_CALENDRIER][DIM_X_CALENDRIER]; //Notre calendrier étendu
int caseDispos; //Compteur pour améliorer la complexité afin de savoir combien il reste de cases où l'on peut poser une forme dans le calendrier
char dateVide[(DIM_X_CALENDRIER - 6) * (DIM_Y_CALENDRIER - 6)][2]; //On reconsidère les dimensions de notre calendrier initial : 7*8
int cptFormeSansDoublon = 0;
char forme[DIM_Y_FORME][DIM_X_FORME]; //On définit une forme quelconque
char formeCopy[DIM_Y_FORME][DIM_X_FORME]; //Nous sert de bascule/tampon pour les copies et les rotations
char formesInitiales[NB_FORMES_INITIALES][DIM_Y_FORME][DIM_X_FORME] = //On initialise nos 10 formes
{
    //1ere forme
    {
        0,1,0,0,
        0,1,0,0,
        0,1,0,0,
        1,1,0,0
    }
    ,
    //2eme forme
    {
        0,0,0,0,
        0,1,0,0,
        0,1,0,0,
        1,1,1,0
    }
    ,
    //3eme forme
    {
        0,0,0,0,
        1,1,1,0,
        1,0,0,0,
        1,0,0,0
    }
    ,
    //4eme forme
    {
        0,0,0,0,
        1,1,0,0,
        0,1,0,0,
        0,1,1,0
    }
    ,
    //5eme forme
    {
        0,1,0,0,
        0,1,0,0,
        1,1,0,0,
        1,0,0,0
    }
    ,
    //6eme forme
    {
        0,0,0,0,
        0,0,0,0,
        1,0,1,0,
        1,1,1,0
    }
    ,
    //7eme forme
    {
        0,0,0,0,
        1,0,0,0,
        1,1,0,0,
        1,1,0,0
    }
    ,
    //8eme forme
    {
        0,0,0,0,
        0,1,0,0,
        0,1,0,0,
        1,1,0,0
    }
    ,
    //9eme forme
    {
        0,0,0,0,
        1,0,0,0,
        1,1,0,0,
        0,1,0,0
    }
    ,
    //10eme forme
    {
        0,0,0,0,
        0,0,0,0,
        0,0,0,0,
        1,1,1,1
    }
};
int formeEtendue[7][7] = {}; //Va nous permettre de centrer en haut à gauche notre pièce pour la placer correctement sur le calendrier
int nombreSolution = 1; //Nous permet pour une date de connaitre le nombre de solutions/configurations trouvées
//Avec les doublons
char formes[NB_FORMES][DIM_Y_FORME][DIM_X_FORME]; //L'ensemble de nos formes avec doublons
char numFormes[NB_FORMES]; //Le numéro de l'ensemble de nos formes avec doublons
//Sans les doublons
char possibilite[NB_FORMES_TOTAL][DIM_Y_FORME][DIM_X_FORME]; //On crée un tableau de 34 variables de taille 4x4 qui correspondent à l'ensemble de nos formes finales (sans doublons)
char identite[NB_FORMES_TOTAL]; //Permet d'accéder au numéro de la forme (1 à 10 + les rotations) sans les doublons



//Fonction pour copier une forme dans une autre
void copyMat(char tab[DIM_Y_FORME][DIM_X_FORME],char tabCopy[DIM_Y_FORME][DIM_X_FORME])
{
    //On parcourt la matrice d'entrée
    for (int numLigne = 0; numLigne < DIM_Y_FORME; numLigne++)
    {
        for (int numColonne = 0; numColonne < DIM_X_FORME; numColonne++)
        {
            tab[numLigne][numColonne] = tabCopy[numLigne][numColonne]; //On recopie la valeur dans notre matrice copie
        }
    }
}

//Fonction pour rotationner une forme de 90°
void rotateFormes(char mat[DIM_Y_FORME][DIM_X_FORME],char tam[DIM_Y_FORME][DIM_X_FORME])
{
    //On parcourt la matrice d'entrée
    for (int numLigne = 0; numLigne < DIM_Y_FORME; numLigne++)
    {
        for (int numColonne = 0; numColonne < DIM_X_FORME; numColonne++)
        {
            tam[3-numColonne][numLigne] = mat[numLigne][numColonne]; //On applique l'opération de rotation de 90° sur la matrice
        }
    }
}

//Fonction pour placer une forme dans notre calendrier
char positionnerForme(int ligneCalendrier, int colCalendrier, char forme[DIM_Y_FORME][DIM_X_FORME])
{
    //On parcourt la matrice d'entrée
    for (int ligneForme = 0; ligneForme < DIM_Y_FORME; ligneForme++)
    {
        for (int colForme = 0; colForme < DIM_X_FORME; colForme++)
        {
            arrayCalendar[ligneCalendrier+ligneForme][colCalendrier+colForme] = arrayCalendar[ligneCalendrier+ligneForme][colCalendrier+colForme] + forme[ligneForme][colForme]; //On dépose la forme dans le calendrier
        }
    }

    //On vient vérifier si on a le droit de poser la forme
    for (int ligneForme = 0; ligneForme < DIM_Y_FORME; ligneForme++)
    {
        for (int colForme = 0; colForme < DIM_X_FORME; colForme++)
        {
            if (arrayCalendar[ligneCalendrier+ligneForme][colCalendrier+colForme] == 0 || arrayCalendar[ligneCalendrier+ligneForme][colCalendrier+colForme] > 2) //Si on a 0 (case déjà indisponible) ou 2 (forme déjà déposée) alors on a pas le droit d'en déposer une nouvelle sur cette case
            {

                for (int ligneForme = 0; ligneForme < DIM_Y_FORME; ligneForme++)
                {
                    for (int colForme = 0; colForme < DIM_X_FORME; colForme++)
                    {
                        arrayCalendar[ligneCalendrier+ligneForme][colCalendrier+colForme] = arrayCalendar[ligneCalendrier+ligneForme][colCalendrier+colForme] - forme[ligneForme][colForme]; //On retire la forme puisqu'on a pas le droit de la poser
                    }
                }
                return 0;
            }
        }
    }

    //On retire la forme
    for (int ligneForme=0; ligneForme< DIM_Y_FORME; ligneForme++)
    {
        for (int colForme=0; colForme< DIM_X_FORME; colForme++)
        {
            arrayCalendar[ligneCalendrier+ligneForme][colCalendrier+colForme]=arrayCalendar[ligneCalendrier+ligneForme][colCalendrier+colForme] - forme[ligneForme][colForme]; //On retire la forme
        }
    }
    return 1;
}

//Fonction pour indiquer si un pixel est isolé ou non afin de le rendre indisponible lors de la pose des formes
char rechercheCasesIsolees(void)
{
    //On parcourt notre calendrier
    for(int numLigne = 0; numLigne < DIM_Y_CALENDRIER; numLigne++)
    {
        for(int numCol = 0; numCol < DIM_X_CALENDRIER; numCol++)
        {
            //On regarde si on a un pixel isolé
            if((arrayCalendar[numLigne][numCol] == 1)&& //On détecte notre pixel où on peut poser une forme
               ((arrayCalendar[numLigne+1][numCol] != 1)&& //On regarde si le pixel sur la droite est occupé
                (arrayCalendar[numLigne-1][numCol] != 1)&& //On regarde si le pixel sur la gauche est occupé
                (arrayCalendar[numLigne][numCol+1] != 1)&& //On regarde si le pixel sur le haut est occupé
                (arrayCalendar[numLigne][numCol-1] != 1))) //On regarde si le pixel sur le bas est occupé
            {
                return 0; //Si c'est le cas on passe à l'étape suivante
            }
        }
    }
    return 1;
}

//Fonction pour décaler le plus en haut à gauche possible une forme
void repositionnerForme(int numForme)
{
    int posX,posY;
    int ligneForme,colForme;

    //On parcourt les lignes de la forme pour connaitre l'indice sur les ordonnées
    for(ligneForme = 0; ligneForme < DIM_Y_FORME; ligneForme++)
    {
        int cpt = 0;

        for(colForme = 0; colForme < DIM_X_FORME; colForme++)
        {
            if (possibilite[numForme][ligneForme][colForme] == 1)
            {
                cpt++; //On incrémente quand on rencontre une partie de la forme
            }
        }

        if(cpt != 0) //Des qu'on a détecté une partie de notre forme
        {
            posY = ligneForme;
            break;
        }
    }

    //On parcourt les colonnes de la forme pour connaitre l'indice sur les abscisses
    for(colForme = 0; colForme < DIM_X_FORME; colForme++)
    {
        int cpt = 0;

        for(ligneForme = 0; ligneForme < DIM_Y_FORME; ligneForme++)
        {
            if (possibilite[numForme][ligneForme][colForme] == 1)
            {
                cpt++; //On incrémente quand on rencontre une partie de la forme
            }
        }

        if(cpt != 0) //Des qu'on a détecté une partie de notre forme
        {
            posX = colForme;
            break;
        }
    }

    //On vient créer le décalage de notre forme
    for(ligneForme = 0; ligneForme < DIM_Y_FORME; ligneForme++)
    {
        for(colForme = 0; colForme < DIM_X_FORME; colForme++)
        {
            formeEtendue[ligneForme][colForme] = possibilite[numForme][ligneForme][colForme];
        }
    }

    //On vient stocker notre forme déplacée en haut à gauche dans notre tableau contenant l'ensemble des formes finales sans doublon
    for(ligneForme = 0; ligneForme < DIM_Y_FORME; ligneForme++)
    {
        for(colForme = 0; colForme < DIM_X_FORME; colForme++)
        {
            possibilite[numForme][ligneForme][colForme] = formeEtendue[ligneForme+posY][colForme+posX];
        }
    }

}

//Fonction pour indiquer les cases utilisables ou non dans le calendrier aggrandi
void remplirCalendrier(void)
{
    //On parcourt notre calendrier aggrandi
    for (int ligneCalendrier = 0; ligneCalendrier < DIM_Y_CALENDRIER; ligneCalendrier++)
    {
        for (int colCalendrier = 0; colCalendrier < DIM_X_CALENDRIER; colCalendrier++)
        {
            if ((ligneCalendrier > 2 && ligneCalendrier < 5) && (colCalendrier < 9 && colCalendrier > 2)) //On se ramène  aux dimensions du premier calendrier (7*8)
            {
                arrayCalendar[ligneCalendrier][colCalendrier] = 1; //Case initialement disponible
            }
            else if ((ligneCalendrier > 4 && ligneCalendrier < 10) && (colCalendrier > 2 && colCalendrier < 10))
            {
                arrayCalendar[ligneCalendrier][colCalendrier] = 1; //Case initialement disponible
            }

            else if ((ligneCalendrier == 10) && (colCalendrier > 6 && colCalendrier < 10))
            {
                arrayCalendar[ligneCalendrier][colCalendrier] = 1; //Case initialement disponible
            }
            else //Si une case fait partie du calendrier étendue on l'indique par un -1
            {
                arrayCalendar[ligneCalendrier][colCalendrier] = -1; //Case indisponible
            }
        }
    }
}

//Fonction pour remplir notre tableau avec l'ensemble des formes finales
void remplirPossibilite(void)
{
    //On remplit notre tableau avec l'ensemble des 40 formes possibles
    for (int numForme = 0; numForme < NB_FORMES; numForme = numForme + 4)
    {
        copyMat(forme,formesInitiales[numForme/4]);
        for (int numRotation = 0; numRotation < NB_ROTATION; numRotation++)
        {
            copyMat(formes[numForme+numRotation],forme);
            rotateFormes(forme,formeCopy);
            copyMat(forme,formeCopy);
            numFormes[numForme+numRotation] = numForme/4;
        }
    }

    //On remplit notre tableau uniquement avec les formes possibles différentes : on tient compte de la double symétrie des pièces 4,9 et 10
    for(int numForme=0; numForme< NB_FORMES; numForme++)
    {
        if (numForme == 15 || numForme == 35 || numForme == 39) //Si on atteint la 2ème rotation de la forme 4,9 ou 10 alors on passe pour éviter une copie (double symétrie)
        {
            cptFormeSansDoublon = cptFormeSansDoublon + 2; //On passe à la nouvelle forme
        }
        else
        {
            identite[numForme-cptFormeSansDoublon]=numFormes[numForme];
            copyMat(possibilite[numForme-cptFormeSansDoublon],formes[numForme]);
        }
    }

    //On déplace notre forme vers le haut à gauche
    for(int numForme = 0; numForme < NB_FORMES_TOTAL; numForme++)
    {
        repositionnerForme(numForme);
    }

    //On affiche nos 34 formes
    for(int numForme = 0; numForme < NB_FORMES_TOTAL; numForme++)
    {
        printf("Forme numero : %d \n", numForme+1);
        for(int numLigne = 0; numLigne< DIM_Y_FORME; numLigne++)
        {
            printf("\n");
            for (int numCol = 0; numCol< DIM_X_FORME; numCol++)
            {
                printf(" %d ",possibilite[numForme][numLigne][numCol]);
            }

        }
        printf("\n\n");
    }
}

//Fonction pour permettre à l'utilisateur de donner la date qu'il souhaite étudier
void indiquerPosition(int numMois, int numQuantieme, int numJour)
{
    //Variables qui correspondent à la position en abscisse et ordonnée de la date indiquée
    int absMois, absQuantieme, absJour;
    int ordMois, ordQuantieme, ordJour;
    int numJourReindexe = (numJour-1) % 7;

    //On vient récupérer la position du jour indiqué sachant que 0 = Lundi et Dimanche = 6
    absJour = numJourReindexe + 3;
    ordJour = floor((numJour-1)/7) + 5;
    arrayCalendar[ordJour][absJour] = -1; //On vient indiquer que la case est indisponible car elle correspond à la date choisie

    //On vient récupérer les coordonnées du mois indiqué sachant que 0 = Janvier et 11 = Décembre
    if (numMois <=6) //On gère les cas avec les dimensions du nouveau calendrier
    {
        absMois = numMois + 2;
        ordMois = 3;
    }
    else
    {
        absMois = numMois - 4;
        ordMois = 4;
    }
    arrayCalendar[ordMois][absMois]=-1; //On vient indiquer que la case est indisponible car elle correspond à la date choisie

    //On vient récupérer les coordonnées du jour indiqué
    if (numQuantieme <= 4) //On gère les cas avec les dimensions du nouveau calendrier
    {
        absQuantieme = numQuantieme + 5;
        ordQuantieme = 9;
    }
    else
    {
        absQuantieme = numQuantieme + 2;
        ordQuantieme = 10;
    }
    arrayCalendar[ordQuantieme][absQuantieme] = -1; //On vient indiquer que la case est indisponible car elle correspond à la date choisie
}

//Fonction pour connaitre le nombre de cases où l'on peut déposer nos formes
void rechercheCasesDispos(void)
{
    caseDispos = 0; //Compteur du nombre de cases où l'on peut encore déposer une partie de nos formes

    //On parcourt le calendrier initial (7*8)
    for(int numLigne = 3; numLigne < DIM_Y_CALENDRIER - 3; numLigne++)
    {
        for(int numCol = 3; numCol < DIM_X_CALENDRIER - 3; numCol++)
        {
            if(arrayCalendar[numLigne][numCol] == 1) //Si on a une case disponible
            {
                dateVide[caseDispos][0] = numLigne; //On récupère ses coordonnées
                dateVide[caseDispos][1] = numCol;
                caseDispos++; //On incrémente le compteur
            }
        }
    }
}

//Fonction récursive qui permet de chercher l'ensemble des solutions possibles
void rechercheSolutions (int numFormeInit)
{
    if(numFormeInit < NB_FORMES_INITIALES && rechercheCasesIsolees())
    {
        for (int numForme = 0; numForme < NB_FORMES_TOTAL; numForme++)
        {
            if (identite[numForme] == numFormeInit)
            {
                rechercheCasesDispos(); //On vient compter le nombre de cases libres

                for(int i = 0; i < caseDispos; i++)
                {
                    if(positionnerForme(dateVide[i][0],dateVide[i][1],possibilite[numForme]))
                    {
                        int absDateVide = dateVide[i][0]; //On récupère l'abscisse de chaque case considérée comme libre afin de poser la forme
                        int ordDateVide = dateVide[i][1]; //On récupère l'ordonnée de chaque case considérée comme libre afin de poser la forme

                        //On parcourt notre forme
                        for (int ligneForme = 0; ligneForme < DIM_Y_FORME; ligneForme++)
                        {
                            for (int colForme = 0; colForme < DIM_X_FORME; colForme++)
                            {
                                arrayCalendar[absDateVide+ligneForme][ordDateVide+colForme] = arrayCalendar[absDateVide+ligneForme][ordDateVide+colForme] + possibilite[numForme][ligneForme][colForme]; //On vient déposer notre forme sur le calendrier
                            }
                        }

                        rechercheSolutions(numFormeInit+1); //Appel récursif de la fonction

                        //On parcourt notre forme
                        for (int ligneForme = 0; ligneForme < DIM_Y_FORME; ligneForme++)
                        {
                            for (int colForme = 0; colForme < DIM_X_FORME; colForme++)
                            {
                                arrayCalendar[absDateVide+ligneForme][ordDateVide+colForme] = arrayCalendar[absDateVide+ligneForme][ordDateVide+colForme] - possibilite[numForme][ligneForme][colForme]; //On vient retirer notre forme du calendrier
                            }
                        }
                    }
                }
            }
        }
    }

    if(numFormeInit == NB_FORMES_INITIALES) //Lorsque l'on atteint la dernière forme
    {
        printf("Solution numero : %d \n",nombreSolution);

        //On vient afficher le calendrier solution (normalement identiques)
        for(int numLigne = 0; numLigne < DIM_Y_CALENDRIER; numLigne++)
        {
            printf("\n");
            for(int numCol = 0; numCol < DIM_X_CALENDRIER; numCol++)
            {
                printf(" %2d ",arrayCalendar[numLigne][numCol]);
            }
        }
        printf("\n\n");

        nombreSolution++;
    }
}

int main()
{
    float time_ms = 0; //Initialisation du temps (ms)
    float time_s = 0; //Initialisation du temps (s)

    remplirPossibilite(); //On crée nos différentes formes
    remplirCalendrier(); //On crée notre calendrier
    indiquerPosition(2,14,2); //On indique la date que l'on souhaite étudier : mois, quantième, jour : [0-11] | [0-30] | [0-6]
    rechercheSolutions(0); //On indique qu'on part de la 1ere forme pour étudier toutes les possibilités
    printf("Nombre total de solution : %d \n",nombreSolution-1);

    time_ms = clock(); //On recupere le nombre de "tick"
    time_s = time_ms / CLOCKS_PER_SEC; //On convertit les "tick" en temps "reel" en secondes
    printf("\nLe CPU requiert %f secondes pour l'execution \n", time_s );
}
