#include <iostream>
#include <cassert>
#include <array>
#include <random>

const int nb_combin_7_cartes = 99884400;// Nombre de tirages de 7 ccartes
const int nb_combin_5_cartes = 2118760;// Nombre de tirages de 5 cartes

class combin
{
    /*
    Codification d'une plus haute combinaison de cartes
    */
   public:
    combin();
    ~combin();
    int set_cartes(std::array<int, 7>); // prend les cartes en entrée et calcule les champs, sans calculer le score. Renvoie 1 si execution correcte
    int type;// tyoe de la combinaison
    std::array<int, 7> valeurs;// valeurs concernées (ex hauteurs des cartes)
    int long_valeurs;// longueur du tableau des valeurs, qui peut ne pas etre rempli en entier
    int score;
    int* cartes; // champ servant a stocker les cartes pendant le calcul du score. Sert a n pas reinstancier la memoire a chaque appel a set_cartes
    int* tab_valeurs; // champ servant a stocker les valeurs (hauteurs - 2) des cartes pendant le calcul du score. Sert a ne pas reinstancier la memoire a chaque appel a set_cartes
    int* tab_couleurs; // meme principe
    std::array<int, 9> puiss_13;
    int get_score(); // getter de score
    void calcule_score();// calcule le score a partir des autres champs. Plante si les autres champs sonnt incommplets
    void affiche();
    // champs interens utilises dans set_cartes
    int* carte_point;
    bool quinte;
    bool couleur;
    bool quinte_flush;
    bool carre;
    bool full;
    bool brelan;
    bool paire;
    bool double_paire;
    int position_quinte; // position de la quinte a renvoyer (carte la plus haute, ou celle de plus grand indice si egalite des cartes les plus hautes)
    int compteur_couleurs; // intervient dans la detection QF
    int couleur_de_la_couleur;
    int increment_prov[7];// en position i, le nombre de valeurs distinctes de la plus longue suite de valeurs egales ou qui augmentent de 1 se finissant en i (inclus)
    int nb_coeur;
    int nb_pique;
    int nb_trefle;
    int nb_carreau;
    int position_plus_haute_carte;
    int j;
    int valeur_prov;
    int position_paire_full;
    int position_deuxieme_paire;
    // exemple : pour la suite 0 0 2 2 5 6 7 on a increment_prov = 1 1 1 1 1 2 3 eet pour 0 1 2 3 3 4 9 on a 1 2 3 4 4 5 1
    int egal_prov[7]; // contient la longueur de la plus longue suite de valeurs egales se finissant en i (inclus)
    int max_nb_cartes_valeur_egales;
    int position_max_nb_cartes_valeurs_egales;
};
class state_of_game
{
    /*
    contient un état du jeu, avec toutes les mains, le flop, la turn et la river abattues
    */
   public:
    state_of_game();
    state_of_game(std::array<int, 2>, int);// construit un etat en initialisant a -1 les cartes (sauf la main)
    ~state_of_game();
    int main_gagnante(); // Donne la maiin gagnante selon la codification : -1 -> joueur ; 1 - un des adversaires (peut être modifié pour donner l'adversaire gagnant)
    int index; // index de la main
    int nb_cartes_tirees; // nombre total d'appels au generateur aleatoire
    std::array<int, 2> main;// Contient les deux cartes de la main du joueur, pas forcément triées
    int nb_competitors;// Nombre d'adversaires
    std::array<int, 20> jeux_competitors;// Mains des adversaires. de longueur 20 car max 10 adversaires
    std::array<int, 3> flop;// flop
    int turn;// turn
    int river;// river
    int seed;
    int* liste_des_cartes_provisoire;
    std::mt19937 mersenne_twister;
    std::uniform_int_distribution<> distrib;
    combin combin_variable;
    void maj_flop();// met à jour le flop (tirage aleatoire)
    void maj_turn();// met à jour la turn (tirage aleatoire)
    void maj_river();// met à jour la river (tirage aleatoire)
    void maj_competitors();// met a jour les jeux des adversaires (tirage aleatoire)
    void init_competitors(); // met les cartes des adversaires a -1
    void init_flop(); // met les cartes du flop a -1
    void init_turn(); // met la turn a -1
    void init_river(); // met la river a -1
    void init_toutes_cartes(); // met toutes les cartes sauf la main a -1
    int tire_une_carte();// tire une carte differente de toues les autres cartes du flop, turn, rver, main et jeux des adversaires
    void set_flop(std::array<int, 3>);
    void set_turn(int);
    void set_river(int);
    void set_nb_competitors(int);
    void affiche();
};

class game
{
    /*
    Une partie
    */
   public:
    game(int, int, int);
    ~game();
    int turn;// valeur de la turn (non forcément initialisé)
    int river;// valeur de la river (non forcément initialisé)
    std::array<int, 3> flop;// valeur du flop (non forcément initialisé)
    int phase;// phase du jeu
    std::array<int, 2> main;
    int nb_competitors;
    /*
    phase = 0 -> pas de flop, pas de turn, pas de river
    phase = 1 -> flop visible, pas de turn, pas de river
    phase = 2 -> flop visible, turn visible, pas de river
    phase = 3 -> flop visible, turn visible, river visible
    */
    state_of_game current;// Etat hypothétique du jeu, amené à changer
    float proba_victoire();// renvoie la probabilité de victoire, compte tenu de la main, de la phase et des cartes visibles
    void augmente_phase();
    void set_phase(int);
    void set_flop(int, int, int);
    void set_turn(int);
    void set_river(int);
    void set_nb_competitors(int);

};
