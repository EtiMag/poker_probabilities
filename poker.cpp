#include <iostream>
#include <cassert>
#include <array>
#include <algorithm>
#include "poker.hpp"

int puissance(int nombre, int exposant) { // ne pas appeler avec un entier < 1
    return (exposant == 1) ? nombre : nombre * puissance(nombre, exposant - 1);
}

int conversion(std::string carte) {
    /*
    Convertit un string de 3 caractères en un entier qui donne un numéro de carte
    */
   char c1 = carte[0];
   char c2 = carte[1];
   char c3 = carte[2];
   int valeur = -1;
   int couleur = -1;
   switch(c1)
   {
       case '2':
       valeur = 0;
       break;
       case '3':
       valeur = 1;
       break;
       case '4':
       valeur = 2;
       break;
       case '5':
       valeur = 3;
       break;
       case '6':
       valeur = 4;
       break;
       case '7':
       valeur  = 5;
       break;
       case '8':
       valeur = 6;
       break;
       case '9':
       valeur = 7;
       break;
       case 'T':
       valeur = 8;
       break;
       case 'V':
       valeur = 9;
       break;
       case 'D':
       valeur = 10;
       break;
       case 'R':
       valeur = 11;
       break;
       case 'A':
       valeur = 12;
       break;
       default:
       std::cout << "Hauteur invalide" << std::endl;
       assert(0==1);
   }
   switch(c2) {
       case 'T':
        if (c3 == 'r') {
            couleur = 0;
        }
        else {
            assert (0 == 1);
        }
        break;
        case 'P':
        if (c3 == 'i') {
            couleur = 2;
        }
        else {
            assert (0 == 1);
        }
        break;
        case 'C':
        if (c3 == 'a') {
            couleur = 1;
        }
        else if (c3 == 'o')
        {
            couleur = 3;
        }
        else {
            assert (0 == 1);
        }
        break;
        default:
        std::cout << "Couleur invalide" << std::endl;
        assert (0 == 1);
   }
   return 4*valeur + couleur;
}

void test_conversion() {
    std::string carte1 = "TTr";
    std::cout << "carte1 = " << carte1 << std::endl;
    std::cout << "Numero = " << conversion(carte1) << std::endl;
    std::string carte2 = "2Co";
    std::cout << "carte2 = " << carte2 << std::endl;
    std::cout << "Numero = " << conversion(carte2) << std::endl;
    std::string carte3 = "5Ca";
    std::cout << "carte3 = " << carte3 << std::endl;
    std::cout << "Numero = " << conversion(carte3) << std::endl;

}

/// Class state_of_game

state_of_game::state_of_game() {    
    nb_competitors = -1;
}
state_of_game::~state_of_game() {
}

state_of_game::state_of_game(std::array<int, 2> main_, int nb_competitors_) {// important : l'initialisation des cartes se fait a  -1
    std::random_device rd;// entier aleatoire machine
    mersenne_twister = std::mt19937 (rd());// On cree le generateur aleatoire mersenne twister
    distrib = std::uniform_int_distribution<> (0, 51); // generateur uniforme d'entiers entre 0 et 49
    index = 0; // On initialise l'index (nombre de tirages passes a 0 car i y a eu 0 tirages)
    nb_cartes_tirees = 0;
    nb_competitors = nb_competitors_; // On initialise le nombre d'adversaires
    main[0] = main_[0]; // On initialise la main
    main[1] = main_[1];
    init_toutes_cartes(); // met toutes les cartes a -1
}

int state_of_game::tire_une_carte() {
    int compteur = 0;
    int tirage = -1;
    bool b = false; // indique si on a tire une carte qui etait dans le jeu d'un adversaire
    // debug
    /*
    if (index > 10000000) {
        std::cout << "index > 10000" << std::endl;
        assert(0 == 1);
    }
    */
    // end of debug
    index++;
    while (++compteur < 20) {
        tirage = distrib(mersenne_twister);
        nb_cartes_tirees++;
        b = false;
        if ((tirage != main[0]) && (tirage != main[1]) && (tirage != turn) && (tirage != river) && (tirage != flop[0]) && (tirage != flop[1]) && (tirage != flop[2]))  { // si le tirage n'est ni das la main, ni dans le turn ou la river
            
            for (int i = 0; i < nb_competitors; i++) {
                if ((tirage == jeux_competitors[2*i]) || (tirage == jeux_competitors[2*i + 1])) { // si on a tire une carte qui etait dans le jeu d'un adversaire
                    b = true; 
                }
            }
            
            if (!b) // si la carte n'etait pas dans le jeu d'un adversaire
            {return tirage;}
        }

        // debug
        /*
        if (compteur == 19) {
        std::cout << std::endl;
        std::cout << "Dans tire_une_carte() " << std::endl;
        std::cout << std::endl;
        std::cout << "carte tiree = " << tirage << std::endl;
        std::cout << "main dans tire_une_carte() = " << main[0] << " " << main[1] << std::endl;
        
        for (int i = 0; i < nb_competitors; i++) {
            std::cout << "adversaire numero " << i << std::endl;
            std::cout << "cartes  = " << jeux_competitors[2*i] << " " << jeux_competitors[2*i + 1] << std::endl;
        }
        std::cout << "flop = " << flop[0] << " " << flop[1] << " " << flop[2] << std::endl;
        std::cout << "turn = " << turn << std::endl;
        std::cout << "river = " << river << std::endl;
    
        std::cout << "nouveau tour " << nb_cartes_tirees << std::endl;
        assert (0 == 1);
        }
        */
        // end of debug
    }
    assert(compteur < 20);
    return 0;
}

void state_of_game::init_competitors() {
    for (int i = 0; i < nb_competitors; i++) {
        jeux_competitors[2*i] = -1;
        jeux_competitors[2*i + 1] = -1;
    }
}
void state_of_game::init_flop() {
    flop[0] = -1; flop[1] = -1; flop[2] = -1;
}

void state_of_game::init_turn() {
    turn = -1;
}

void state_of_game::init_river() {
    river = -1;
}
void state_of_game::init_toutes_cartes() {
    init_competitors();
    init_flop();
    init_turn();
    init_river();
}

void test_tire_une_carte(state_of_game s) {
    std::cout << "tirage = " << s.tire_une_carte() << std::endl;
}


void state_of_game::maj_competitors() {
    for (int i = 0; i < nb_competitors; i++) {
        jeux_competitors[2*i] = tire_une_carte();
        jeux_competitors[2*i + 1] = tire_une_carte();
    }
    //index++;
}

void state_of_game::maj_river() {
    river = tire_une_carte(); // la carte est forcément différente des autres cartes deja tirees : main, flop et jeux adversaires
    //index++;
}

void state_of_game::maj_turn() {
    turn = tire_une_carte();
    //index++;
}

void state_of_game::maj_flop() {
    flop[0] = tire_une_carte();
    flop[1] = tire_une_carte();
    flop[2] = tire_une_carte();
    //index++;
}

void state_of_game::set_flop(std::array<int, 3> flop_) { // setter pour e flop
    flop[0] = flop_[0]; flop[1] = flop_[1]; flop[2] = flop_[2];
}

void state_of_game::set_turn(int turn_) {
    turn = turn_;
}

void state_of_game::set_river(int river_) {
    river = river_;
}

int state_of_game::main_gagnante() {
    std::array<int, 7> cartes;
    int score_main;
    int score_adversaire;
    // calcul du score de la meilleure combin du joueur
    cartes[0] = main[0]; cartes[1] = main[1]; cartes[2] = flop[0]; cartes[3] = flop[1]; cartes[4] = flop[2]; cartes[5] = turn; cartes[6] = river;
    combin_variable.set_cartes(cartes);
    score_main = combin_variable.get_score();
    //std::cout << "Score du joueur = " << score_main << std::endl;
    //combin_variable.affiche();
    // calcul du score de la meilleure combin des adversaires
    for (int i = 0; i < nb_competitors; i++ ) {
        cartes[0] = jeux_competitors[2*i];
        cartes[1] = jeux_competitors[2*i + 1];
        combin_variable.set_cartes(cartes);
        score_adversaire = combin_variable.get_score();
        //std::cout << "Score adversaire " << i << " = " << score_adversaire << std::endl;
        //combin_variable.affiche();
        if (score_main > score_adversaire) {
            return 1; // cas ou l'adversaire i a un score strictement plus bas -> l'adversaire gagne
        }
    }
    return -1; // cas ou les scores des adversaires sont tous superieurs ou egaux a celui du joueur -> le joueur gagne
}

void state_of_game::affiche() {
    std::cout << "Affichage d'un objet de state_of_game : " << std::endl;
    std::cout << "int index = " << index << std::endl;
    std::cout << "int nb_cartes_tirees = " << nb_cartes_tirees << std::endl;
    std::cout << "main = " << main[0] << " " << main[1] << std::endl;
    std::cout << "int nb_competitors = " << nb_competitors << std::endl;
    std::cout << "jeux competitors " << std::endl;
    if (nb_competitors == -1) {
        std::cout << "Non initialise" << std::endl;
    }
    else
    {
        for (int i = 0; i < nb_competitors; i++) {
            std::cout << "Adversaire numero " << i << " a pour jeu ";
            std::cout << jeux_competitors[2*i] << " (soit valeur = " << jeux_competitors[2*1]/4 << " et couleur = " << jeux_competitors[2*i]%4 << ") " << jeux_competitors[2*i + 1] <<" (soit valeur = " << jeux_competitors[2*i + 1]/4 << " et couleur = " << jeux_competitors[2*i+1]%4 <<")" << std::endl;
        }
    }
    std::cout << "std::array<int, 3> flop = " << flop[0] << " (" << flop[0]/4 << " " << flop[0]%4 << ") " " " << flop[1]<< " (" << flop[1]/4 << " " << flop[1]%4 << ") " << " " << flop[2]<< " (" << flop[2]/4 << " " << flop[2]%4 << ") " << std::endl;
    std::cout << "int turn = " << turn  << " (" << turn/4 << " " << turn%4 << ") " << std::endl;
    std::cout << "int river = " << river << " " << river/4 << " " << river%4<< std::endl;
    
}
void state_of_game::set_nb_competitors(int nb_competitors_) {
    nb_competitors = nb_competitors_;
}

/// class combin
combin::combin() {
    score = -1;
    type = -1;
    long_valeurs = -1;
    puiss_13[0] = 1;
    for (int i = 1; i < 9; i++) { // ne surtout pas faire commencer a 0
        puiss_13[i] = puissance(13, i);
        //std::cout << "puiss_13[" << i << "] = " << puiss_13[i] << std::endl;
    }
    for (int i = 0; i < 7; i++) {
        valeurs[i] = -1;
    }

    cartes = new int[7]; // combin de 7 cartes
    tab_valeurs = new int[7];
    tab_couleurs = new int[7];
    }

combin::~combin() {
    delete[] cartes;
    delete[] tab_valeurs;
    delete[] tab_couleurs;
}

int combin::get_score() {
    calcule_score();
    return score;
}

int combin::set_cartes(std::array<int, 7> cartes_) { // return 0 si exécution corrrecte
    score = -1; // initialisation
    type = -1;
    long_valeurs = -1;
    for (int i = 0; i < 7; i++) {
        valeurs[i] = -1;
    }
    // fin init
    for (int i = 0; i < 7; i++) { // l'etape de copie est importante pour ne pas modifier les valeurs du tableau passe en argument
        cartes[i] = cartes_[i];
    }
    // tri
    std::sort(cartes, cartes + 7);
    // extraction des valeurs et couleurs. Le tri est un tri alphabétique par valeur puis couleur car cartes[i] = 4* tab_valeurs[i] + tab_couleurs[i]
    for (int i = 0; i < 7; i++) {
        tab_valeurs[i] = cartes[i]/4;
        tab_couleurs[i] = cartes[i]%4;
    }

    // Declarations
    quinte = false;
    couleur = false;
    quinte_flush = false;
    carre = false;
    full = false;
    brelan = false;
    paire = false;
    double_paire = false;
    position_quinte = -1; // position de la quinte a renvoyer (carte la plus haute, ou celle de plus grand indice si egalite des cartes les plus hautes)
    compteur_couleurs = 0; // intervient dans la detection QF
    couleur_de_la_couleur = -1;
    //increment_prov[7];// en position i, le nombre de valeurs distinctes de la plus longue suite de valeurs egales ou qui augmentent de 1 se finissant en i (inclus)
    nb_coeur = 0;
    nb_pique = 0;
    nb_trefle = 0;
    nb_carreau = 0;
    position_plus_haute_carte = -1;
    j = -1;
    valeur_prov = -1;
    position_paire_full = -1;
    position_deuxieme_paire = -1;
    // exemple : pour la suite 0 0 2 2 5 6 7 on aa increment_prov = 1 1 1 1 1 2 3 eet pour 0 1 2 3 3 4 9 on a 1 2 3 4 4 5 1
    //int egal_prov[7]; // contient la longueur de la plus longue suite de valeurs egales se finissant en i (inclus)
    max_nb_cartes_valeur_egales = 1;
    position_max_nb_cartes_valeurs_egales = 0;

    // remplissage de increment_prov et egal_prov
    increment_prov[0] = 1;
    egal_prov[0] = 1;
    for (int i = 1; i < 7 ; i++) {
        if (tab_valeurs[i] == tab_valeurs[i - 1]) {
            egal_prov[i] = egal_prov[i - 1] + 1;
            increment_prov[i] = increment_prov[i-1];
        }
        else if (tab_valeurs[i] == tab_valeurs[i - 1] + 1)
        {
            egal_prov[i] = 1;
            increment_prov[i] = increment_prov[i - 1] + 1;
        }
        else
        {
            increment_prov[i] = 1;
            egal_prov[i] = 1;
        }
        if (increment_prov[i] >= 5) {
            quinte = true;
            position_quinte = i;
        }
        if (egal_prov[i] >= max_nb_cartes_valeur_egales) { // si le nombre max de valeurs egales d'affile a ete depasse
            max_nb_cartes_valeur_egales = egal_prov[i]; // on met a jour ce max
            position_max_nb_cartes_valeurs_egales = i; // on met a jour sa position
        }
        /* debug
        std::cout << "increment_prov[" << i << "] = " << increment_prov[i] << std::endl;
        std::cout << "egal_prov[" << i << "] = " << egal_prov[i] << std::endl;
        */
    }
    assert(max_nb_cartes_valeur_egales <= 4);
    /*
    // Debug
    if (quinte) {
        std::cout << "quinte est true" << std::endl;
        std::cout << "position de la quinte " << position_quinte << std::endl;
        std::cout << "increment_prov vaut ";
        for (int d = 0; d < 7; d++) {
            std::cout << increment_prov[d] << " ";
        }
        std::cout << std::endl;
    }
    // end of debug
    */

    // calcul du nombre de couleurs
    for (int i = 0; i < 7; i++) {
        switch(tab_couleurs[i]) {
            case 0:
            nb_trefle++;
            break;
            case 1:
            nb_carreau++;
            break;
            case 2:
            nb_pique++;
            break;
            case 3:
            nb_coeur++;
            break;
            default:
            assert(0 == 1);
        }
    }
    /*
    // debug
    std::cout << "nb_coeur = " << nb_coeur << std::endl;
    std::cout << "nb_pique = " << nb_pique << std::endl;
    // end of debug
    */
    couleur = ((nb_trefle >= 5) || (nb_carreau >= 5) || (nb_pique >= 5) || (nb_coeur >= 5));
    if (couleur) {
        /*
        // debug
        std::cout << "couleur est true" << std::endl;
        // end of debug
        */
        if (nb_trefle >= 5) {
            couleur_de_la_couleur = 0;
        }
        else if (nb_carreau >= 5)
        {
            couleur_de_la_couleur = 1;
        }
        else if (nb_pique >= 5)
        {
            couleur_de_la_couleur = 2;
        }
        else
        {
            couleur_de_la_couleur = 3;
        }
        /*
        // debug
        std::cout << "la couleur de la couleur est " << couleur_de_la_couleur << std::endl;
        // end of debug
        */
    }

    // detection quinte flush
    if (quinte && couleur) {
        /*
        // debug
        std::cout << "quinte && couleur est true" << std::endl;
        // end of debug
        */
        j = position_quinte;
        while (j > -1) { // on compte le nombre de cartes de la bonne couleur dans les cartes de la quinte, pour voir si il y en a au moins 5
            if (increment_prov[j] > 1) {
                compteur_couleurs += (tab_couleurs[j] == couleur_de_la_couleur) ? 1 : 0;
            }
            else {
                break;
            }
            /*
            // debug
            if (increment_prov[j] == 1) {
                std::cout << "bug sur le break : on est encore dans la booucle while" << std::endl;
                assert(0 == 1);
            }
            // end of debug
            */
            j--;
        }
        int valeur_prov = tab_valeurs[j];
        while (j > -1) {
            if (tab_valeurs[j] == valeur_prov) {
                compteur_couleurs += (tab_couleurs[j] == couleur_de_la_couleur) ? 1 : 0;
            }
            else {
                break;
            }
            j--;
        }
        quinte_flush = (compteur_couleurs >= 5);
    }

    /*
    // debug
    if (quinte_flush) {
        std::cout << "quinte_flush est true" << std::endl;
    }
    // end of debug
    */

    // fin detection quinte flush

    // return quinte flush
    if (quinte_flush) {
        type = 0;
        // detection de la plus haute carte
        for (int i = position_quinte; i> -1; i--) {
            if (tab_couleurs[i] == couleur_de_la_couleur) {
                position_plus_haute_carte = i;
                break;
            }
        }
        valeurs[0] = tab_valeurs[position_plus_haute_carte];
        long_valeurs = 1;
        // remplissage des autres valeurs (detection des deux cartes qui ne sont pas dans la quinte flush)
        if (position_plus_haute_carte == 4) { // necessairement, les deux cartes au dessus de la quinte ne sont pas dedans
            long_valeurs = 3;
            valeurs[1] = tab_valeurs[6];
            valeurs[2] = tab_valeurs[5];
            return 0;
        }
        if (position_plus_haute_carte == 5) {
            long_valeurs = 3;
            valeurs[1] = tab_valeurs[6]; // la derniere ne peut pas etre dans la quinte flush
            for (int i = 0; i < 6; i++) { // on cherche si une carte n'a pas la bonne couleur
                if (tab_couleurs[i] != couleur_de_la_couleur) { // si c'est le cas, on a fini
                    valeurs[2] = tab_valeurs[i];
                    return 0;
                }
            }
            // si ce n'est pas le cas (pas de return, alors la carte restante est forcement la premiere)
            valeurs[2] = tab_valeurs[0];
            return 0;
        }
        if (position_plus_haute_carte == 6) {
            j = position_plus_haute_carte;
            while (j > -1) { // parcours des cartes de la quinte, en mettant de cote celles qui ne sont pas de la bonne couleur
                if (increment_prov[j] > 1) {
                    if (tab_couleurs[j] != couleur_de_la_couleur) {
                        valeurs[long_valeurs] = tab_valeurs[j];
                        long_valeurs++;
                    }
                }
                else {
                    break;
                }
                j--;
            }
            valeur_prov = tab_valeurs[j];
            while (j > -1) {
                if (tab_valeurs[j] == valeur_prov) {
                    if (tab_couleurs[j] != couleur_de_la_couleur) {
                        valeurs[long_valeurs] = tab_valeurs[j];
                        long_valeurs++;
                    }
                }
                else {
                    break;
                }
                j--;
            } // fin du parcours
            assert(long_valeurs < 4); // on assure qu'on a bien mis pas plus de 2 cartes de cote
            if (long_valeurs == 3) { // selon le nombre de cartes que l'on a mises de cote
                return 1; 
            }
            else if (long_valeurs == 2)
            {
                valeurs[2] = tab_valeurs[0];
                long_valeurs++;
                return 0;
            }
            else if (long_valeurs == 1)
            {
                valeurs[1] = tab_valeurs[1];
                valeurs[2] = tab_valeurs[0];
                long_valeurs = 3;
                return 0;
            }
            assert (0 == 1);
        }
        assert (0 == 1); // cas ou on a pas pu renvoyer la quinte flush alors qu'il y en a une
    }
    position_plus_haute_carte = -1; // pour eviter toute erreur

    // fin return quinte flush

    // detection carre
    if (max_nb_cartes_valeur_egales == 4) {
        carre = true;
    }
    /*
    // debug
    if (carre) {
        std::cout << "carre est true" << std::endl;
    }
    // end of debug
    */
    // fin detection carre

    // return carre
    if (carre) {
        type = 1;
        long_valeurs = 1;
        valeurs[0] = tab_valeurs[position_max_nb_cartes_valeurs_egales];
        for (int i = 6; i > position_max_nb_cartes_valeurs_egales; i-- ) {
            valeurs[long_valeurs] = tab_valeurs[i];
            long_valeurs++;
        }
        for (int i = position_max_nb_cartes_valeurs_egales - 4; i > -1; i--) {
            valeurs[long_valeurs] = tab_valeurs[i];
            long_valeurs++;
        }
        // debug
        //affiche();
        assert (long_valeurs == 4);
        // end of debug
        return 0;
    }
    // fin return carre

    // detection full
    if (max_nb_cartes_valeur_egales == 3) {
        for (int i = 6; i > position_max_nb_cartes_valeurs_egales; i--) { // A faire : changer le sens du parcours et mettre un break
            if (egal_prov[i] >= 2) {
                full = true;
                position_paire_full = i;
                break;
            }
        }
        if (!full) {
            for (int i = position_max_nb_cartes_valeurs_egales  - 3; i > -1; i--) {
                if (egal_prov[i] >= 2) {
                    full = true;
                    position_paire_full = i;
                    break;
                }
            }
        }
    }

    // fin detection full

    // return full
    if (full) {
        type = 2;
        valeurs[0] = tab_valeurs[position_max_nb_cartes_valeurs_egales];
        valeurs[1] = tab_valeurs[position_paire_full];
        long_valeurs = 2;
        if (position_max_nb_cartes_valeurs_egales < position_paire_full) {
            for (int i = 6; i > position_paire_full; i--) {
                valeurs[long_valeurs] = tab_valeurs[i];
                long_valeurs++;
            }
            for (int i = position_paire_full - 2; i > position_max_nb_cartes_valeurs_egales; i--) {
                valeurs[long_valeurs] = tab_valeurs[i];
                long_valeurs++;
            }
            for (int i = position_max_nb_cartes_valeurs_egales - 3; i > -1; i--) {
                valeurs[long_valeurs] = tab_valeurs[i];
                long_valeurs++;
            }
        }
        else
        {
            for (int i = 6; i > position_max_nb_cartes_valeurs_egales; i--) {
                valeurs[long_valeurs] = tab_valeurs[i];
                long_valeurs++;
            }
            for (int i = position_max_nb_cartes_valeurs_egales - 3; i > position_paire_full; i--) {
                valeurs[long_valeurs] = tab_valeurs[i];
                long_valeurs++;
            }
            for (int i = position_paire_full - 2; i > -1; i--) {
                valeurs[long_valeurs] = tab_valeurs[i];
                long_valeurs++;
            }
        }
        assert (long_valeurs == 4);
        return 0;
    }
    // fin return full
    // detection couleur - fait plus haut, pour utilisation pour QF
    // fin detection couleur

    // return couleur
    if (couleur) {
        type = 3;
        // premier parcours sur les cartes de la bonne couleur
        long_valeurs = 0;
        for (int i = 6; i > -1; i--) {
            if (tab_couleurs[i] == couleur_de_la_couleur) {
                valeurs[long_valeurs] = tab_valeurs[i];
                long_valeurs++;
            }
        }
        for (int i = 6; i > -1; i--) {
            if (tab_couleurs[i] != couleur_de_la_couleur) {
                valeurs[long_valeurs] = tab_valeurs[i];
                long_valeurs++;
            }
        }
        // debug
        assert (long_valeurs == 7);
        // end of debug
        return 0;
    }
    // detection quinte -> faite plus haut pour la quinte flush
    // fin detection quinte

    // return quinte
    if (quinte) {
        type = 4;
        long_valeurs = 1;
        valeurs[0] = tab_valeurs[position_quinte];
        for (int i = 6; i > position_quinte; i--) {
            valeurs[long_valeurs] = tab_valeurs[i];
            long_valeurs++;
        }
        for (int i = position_quinte - 5; i > -1; i--) {
            valeurs[long_valeurs] = tab_valeurs[i];
            long_valeurs++;
        }
        // debug
        assert (long_valeurs == 3);
        // fin debug
        return 0;
    }
    // fin return quinte

    // detection brelan
    if (max_nb_cartes_valeur_egales == 3) {
        brelan = true;
    }
    // fin detection brelan
    // return brelan
    if (brelan) {
        type = 5;
        long_valeurs = 1;
        valeurs[0] = tab_valeurs[position_max_nb_cartes_valeurs_egales];
        for (int i = 6; i > position_max_nb_cartes_valeurs_egales; i--) {
            valeurs[long_valeurs] = tab_valeurs[i];
            long_valeurs++;
        }
        for (int i = position_max_nb_cartes_valeurs_egales - 3; i > -1; i--) {
            valeurs[long_valeurs] = tab_valeurs[i];
            long_valeurs++;
        }
        // debug
        assert (long_valeurs == 5);
        // end of debug
        return 0;
    }
    // fin return brelan
    
    // detection double paire
    if (max_nb_cartes_valeur_egales == 2) {
        paire = true;
        for (int i = position_max_nb_cartes_valeurs_egales - 2; i > -1; i--) {
            if (egal_prov[i] == 2) {
                double_paire = true;
                position_deuxieme_paire = i;
                break;
            }
        }
    }
    // fin detection double paire

    // return double paire
    if (double_paire) {
        type = 6;
        valeurs[0] = tab_valeurs[position_max_nb_cartes_valeurs_egales];
        valeurs[1] = tab_valeurs[position_deuxieme_paire];
        long_valeurs = 2;
        for (int i = 6; i > position_max_nb_cartes_valeurs_egales; i--) {
            valeurs[long_valeurs++] = tab_valeurs[i];
        }
        for (int i = position_max_nb_cartes_valeurs_egales - 2; i > position_deuxieme_paire; i--) {
            valeurs[long_valeurs++] = tab_valeurs[i];
        }
        for (int i = position_deuxieme_paire - 2; i > -1; i--) {
            valeurs[long_valeurs++] = tab_valeurs[i];
        }
        // debug
        assert (long_valeurs == 5);
        // end of debug
        return 0;
    }
    // fin return double paire

    // detection paire -> fait avec la detection double paire
    // fin detection paire

    // return paire
    if (paire) {
        type = 7;
        valeurs[0] = tab_valeurs[position_max_nb_cartes_valeurs_egales];
        long_valeurs = 1;
        for (int i = 6; i > position_max_nb_cartes_valeurs_egales ; i--) {
            valeurs[long_valeurs++] = tab_valeurs[i];
        }
        for (int i = position_max_nb_cartes_valeurs_egales - 2; i > -1; i--) {
            valeurs[long_valeurs++] = tab_valeurs[i];
        }
        // debug
        assert(long_valeurs == 6);
        // end of debug
        return 0;
    }
    // fin return paire

    // return hauteur
    long_valeurs = 0;
    type = 8;
    for (int i = 6; i > -1; i--) {
        valeurs[long_valeurs++] = tab_valeurs[i];
    }
    // debbug
    assert(long_valeurs == 7);
    // end od debug
    return 0;
    // fin return hauteur
}

void combin::affiche() {
    std::cout <<" ##### " << std::endl;
    std::cout << "Affichage des champs de la combin : " << std::endl;
    std::cout << std::endl;
    std::cout << "int type = " << type << std::endl;
    std::cout << "std::array<int, 5> valeurs  = ";
    for (int i = 0; i < 7; i++) {
        std::cout << valeurs[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "int long_valeurs = " << long_valeurs << std::endl;
    std::cout << "int score = " << score << std::endl;
    std::cout << "#######" << std::endl;
    std::cout << "Champs provisoires :" << std::endl;
    std::cout << "int* cartes = ";
    for (int i = 0; i < 7; i++) {
        std::cout << cartes[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "int* tab_valeurs = ";
    for (int i = 0; i < 7; i++) {
        std::cout << tab_valeurs[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "int* tab_couleurs = ";
    for (int i = 0; i < 7; i++) {
        std::cout << tab_couleurs[i] << " ";
    }
    std::cout << std::endl;
}


void test_combin() {
    combin combin_instance;
    std::array<int, 7> card_list;
    std::string carte_actuelle;
    /*int liste[7]  {0, 1, 6, 12, 13, 26, 30};

    
    for (int i = 0; i < 7; i++) {
        card_list[i] = liste[i];
    }
    */
   std::cout << "Rentrer les cartes : "<< std::endl;
    for (int i = 0; i < 7; i++) {
        std::cout << "Carte numero " << i+1 << " ";
        std::cin >> carte_actuelle;
        std::cout << std::endl;
        card_list[i] = conversion(carte_actuelle);
    }
    std::cout << "liste des cartes donnees : ";
    for (int i =0; i < 7; i++) {
        std::cout << card_list[i] << " ";
    }
    std::cout << std::endl;
    combin_instance.set_cartes(card_list);

    combin_instance.affiche();
}

void combin::calcule_score() {
    // debug
    assert (type > -1);
    // end of debug
    score = type*puiss_13[7];
    for (int i = 0; i < long_valeurs; i++) {
        score += (12 - valeurs[i])*puiss_13[long_valeurs - i - 1];
    }
}
/// class game

game::game(int main1_, int main2_, int nb_competitors_) {
    turn = -1;
    river = -1;
    flop= {0, 0, 0};
    phase = 0;
    nb_competitors = nb_competitors_;
    main =  {main1_, main2_};

    current = state_of_game (main, nb_competitors);
    
    /*
    // debug
    std::cout << "debug : dans game::game(carte1, carte2, nb_competitors)" << std::endl;
    std::cout << "main dans current = " << current.main[0] << " " << current.main[1] << std::endl;
    
    for (int i = 0; i < nb_competitors; i++) {
        std::cout << "adversaire numero " << i << std::endl;
        std::cout << "cartes  = " << current.jeux_competitors[2*i] << " " << current.jeux_competitors[2*i + 1] << std::endl;
    }
    */
}

game::~game() {
    
}

float game::proba_victoire() {
    float p = 0.;
    int  N = 100000;
    float nb_victoires = 0; // float pour avoir la division sur les float et non la division euclidienne
    switch(phase) { // indispensable d'initialiser les cartes a -1 avant d'appeler les fonctions de maj, car le tirage serait faux
        case 3:
        for (int i = 0; i < N; i++) {
            current.init_competitors();
            current.maj_competitors();
            if (current.main_gagnante() == -1) {
                nb_victoires++;
            }
        }
        break;

        case 2:
        for (int i = 0; i < N; i++) {
            current.init_competitors();
            current.init_river();
            current.maj_competitors();
            current.maj_river();
            if (current.main_gagnante() == -1) {
                nb_victoires++;
            }
        }
        break;

        case 1:
        for (int i = 0; i < N; i++) {
            current.init_competitors();
            current.init_river();
            current.init_turn();
            current.maj_competitors();
            current.maj_river();
            current.maj_turn();
            if (current.main_gagnante() == -1) {
                nb_victoires++;
            }
        }
        break;

        case 0:
        for (int i = 0; i < N; i++) {
            current.init_competitors();
            current.init_river();
            current.init_turn();
            current.init_flop();
            current.maj_competitors();
            current.maj_river();
            current.maj_turn();
            current.maj_flop();
            if (current.main_gagnante() == -1) {
                nb_victoires = nb_victoires + 1;
            }
        }
        break;

        default:
        assert(0 == 1);
        break;
    }
    return nb_victoires/N;
}

void game::augmente_phase() {
    phase++;
}
void game::set_phase(int phase_) {
    phase = phase_;
}
void game::set_flop(int flop1_, int flop2_, int flop3_) {
    flop[0] = flop1_;
    flop[1] = flop2_;
    flop[2] = flop3_;
    current.set_flop(flop);
}
void game::set_turn(int turn_) {
    turn  = turn_;
    current.set_turn(turn);
}
void game::set_river(int river_) {
    river = river_;
    current.set_river(river);
}
void game::set_nb_competitors(int nb_comprtitors_) {
    nb_competitors = nb_comprtitors_;
    current.set_nb_competitors(nb_competitors);
}

int main(int argc, char* argv[], char* envp[]) {
    assert (argc == 10);
    int main1 = atoi(argv[1]);
    int main2 = atoi(argv[2]);
    int flop1 = atoi(argv[3]);
    int flop2 = atoi(argv[4]);
    int flop3 = atoi(argv[5]);
    int turn = atoi(argv[6]);
    int river = atoi(argv[7]);
    int phase = atoi(argv[8]);
    int nb_competitors = atoi(argv[9]);
    game jeu(main1, main2, nb_competitors);
    if (phase == 0) {
        std::cout << jeu.proba_victoire() << std::endl;
        return 0;
    }
    if (phase == 1) {
        jeu.set_phase(phase);
        jeu.set_flop(flop1, flop2, flop3);
        std::cout << jeu.proba_victoire() << std::endl;
        return 0;
    }
    if (phase == 2) {
        jeu.set_phase(phase);
        jeu.set_flop(flop1, flop2, flop3);
        jeu.set_turn(turn);
        std::cout << jeu.proba_victoire() << std::endl;
        return 0;
    }
    if (phase == 3) {
        jeu.set_phase(phase);
        jeu.set_flop(flop1, flop2, flop3);
        jeu.set_turn(turn);
        jeu.set_river(river);
        std::cout << jeu.proba_victoire() << std::endl;
        return 0;
    }

    assert(0 == 1);
    return 1;
}
