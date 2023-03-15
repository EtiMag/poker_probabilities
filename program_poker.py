import subprocess


def position(e, l_):
    try:
        return l_.index(e)
    except ValueError:
        return -1


def garde_deux_decimales(f):
    return round(f, 2)


def conversion(carte_):
    carte = carte_.upper()
    if not isinstance(carte, str):
        print("Mauvais type")
        return -1
    if (len(carte) != 3):
        print("Mauvaise longueur")
        return -1
    hauteur_str = carte[0]
    couleur_str = carte[1:]
    l_hauteurs_int = list(range(0, 13))
    l_hauteurs_str = [str(k) for k in range(2, 10)] + ["T", "V", "D", "R", "A"]
    l_couleurs_int = list(range(0, 4))
    l_couleurs_str = ["TR", "CA", "PI", "CO"]
    hauteur_index = position(hauteur_str, l_hauteurs_str)
    if hauteur_index == -1:  # si la hauteur est invalide
        print("Mauvaise hauteur")
        return -1
    couleur_index = position(couleur_str, l_couleurs_str)
    if couleur_index == -1:  # si la couleur est invalide
        print("Mauvaise couleur")
        return -1
    hauteur = l_hauteurs_int[hauteur_index]
    couleur = l_couleurs_int[couleur_index]
    return 4*hauteur + couleur


def demande_carte(texte):
    while True:
        carte = input(texte)
        if carte == "fin":
            return "fin"
        if carte == "exit":
            return "exit"
        carte = conversion(carte)
        if carte != -1:
            return carte


def demande_nb_adversaires():
    while True:
        nb_adversaires = input("Quel est le nombre max d'adversaires en jeu ?")
        if (nb_adversaires == "fin") or (nb_adversaires == "exit"):
            return nb_adversaires
        nb_adversaires = int(nb_adversaires)
        if (nb_adversaires >= 0):
            return nb_adversaires
        print("Mauvais nombre")


def demande_montant_pot():
    while True:
        pot = input("Quel est le montant du pot ? ")
        if (pot == "fin") or (pot == "exit"):
            return pot
        pot = int(pot)
        if (pot >= 0):
            return pot
        print("Mauvais montant")


def demande_montant_mise():
    while True:
        mise = input("Quel est la mise demandee ? ")
        if (mise == "fin") or (mise == "exit"):
            return mise
        mise = int(mise)
        if (mise > 0):
            return mise
        print("Mauvais montant")


def demande_deja_mise():
    while True:
        deja_mise = input("Quel est le montant deja mise par vous a ce tour ?")
        if (deja_mise == "fin") or (deja_mise == "exit"):
            return deja_mise
        deja_mise = int(deja_mise)
        if (deja_mise >= 0):
            return deja_mise
        print("Mauvais montant")


def demande_phase_suivante():
    while True:
        res = input("Passage à la phase suivante ? [y, n, fin, exit]")
        if (res == "fin") or (res == "exit"):
            return res
        if res == 'y':
            return True
        if res == 'n':
            return False


def calcule_proba_victoire(main1, main2, flop1, flop2, flop3,
                           turn, river, phase, nb_adversaires):
    process = subprocess.run(
        ["./poker", str(main1), str(main2),
         str(flop1), str(flop2), str(flop3),
         str(turn), str(river),
         str(phase), str(nb_adversaires)],
        capture_output=True
    )
    return float(process.stdout)


def gere_partie(compteur_parties):
    print("\nDébut de la partie n°" + str(compteur_parties))
    print()
    # initialisation
    phase = 0
    compteur_tours_encheres = 0
    flop1, flop2, flop3, turn, river = -1, -1, -1, -1, -1
    nb_adversaires_max = -1
    while (phase < 4):
        compteur_tours_encheres_phase = 0
        passage_phase_suivante = False
        if (phase == 0):
            # demande la main
            main1 = demande_carte("Première carte de votre main ? ")
            if (main1 == "fin") or (main1 == "exit"):
                return main1
            main2 = demande_carte("Deuxième carte de votre main ? ")
            if (main2 == "fin") or (main2 == "exit"):
                return main2
        if (phase == 1):
            # phase 1 : flop ouvert
            print("\nTour du Flop")
            print()
            flop1 = demande_carte("Première carte du flop ? ")
            if (flop1 == "fin") or (flop1 == "exit"):
                return flop1
            flop2 = demande_carte("Deuxième carte du flop ? ")
            if (flop2 == "fin") or (flop2 == "exit"):
                return flop2
            flop3 = demande_carte("Troisième carte du flop ? ")
            if (flop3 == "fin") or (flop3 == "exit"):
                return flop3
        elif (phase == 2):
            # phase 2 : flop et turn ouverts
            print("\nTour de la turn")
            print()
            turn = demande_carte("Turn ? ")
            if (turn == "fin") or (turn == "exit"):
                return turn
        elif (phase == 3):
            # phase 3 : flop, turn et river ouverts
            print("\nTour de la river")
            print()
            # demande la river
            river = demande_carte("River ? ")
            if (river == "fin") or (river == "exit"):
                return river
        while not passage_phase_suivante:
            print("\nTour d'enchères !")
            nb_adversaires_max = 4
            l_nb_adversaires = list(range(1, nb_adversaires_max + 1))
            # calcul les probas de victoire
            l_probas_victoire = []
            l_cote_probas = []
            for nb_adversaires in l_nb_adversaires:
                l_probas_victoire.append(
                    calcule_proba_victoire(main1, main2, flop1, flop2, flop3, 
                                           turn, river, phase, nb_adversaires)
                )
                l_cote_probas.append(
                    (1 - l_probas_victoire[-1]) / l_probas_victoire[-1]
                )
            
            for i in range(nb_adversaires_max):
                l_cote_probas[i] = garde_deux_decimales(l_cote_probas[i])
    
            # Affichage 
            print("Nb adversaires ", end=" ")
            print(l_nb_adversaires)
            print("cotes proba    ", end=" ")
            print(l_cote_probas)
            passage_phase_suivante = demande_phase_suivante()
            if (passage_phase_suivante == "fin") or (
                    passage_phase_suivante == "exit"):
                return passage_phase_suivante
            if (passage_phase_suivante):
                phase += 1
            else:
                compteur_tours_encheres_phase += 1
            compteur_tours_encheres += 1
    return "fin"


def main():
    print("Bienvenue dans le calculateur de probabilités pour le poker !")
    print("Codé par Etienne Maeght \n")

    print("Rappel du format de saisie des cartes :")
    print("Voici quelques exemples")
    print("As de coeur -> ACo ou aco ou ACO")
    print("10 de trèfle -> TTr ou ttr ou TTR")
    print("Dame de pique -> DPi ou dpi ou DPI")
    print()
    print('''A tout moment, quitter la partie en entrant "fin"'''
          ''' ou quitter le programme en entrant "exit" \n''')
    compteur_parties = 0
    while True:
        compteur_parties += 1
        retour = gere_partie(compteur_parties)
        if (retour != "fin") and (retour != "exit"):
            print("erreur")
            return 1
        if retour == "exit":
            print("\nSortie du programme, " +
                  str(compteur_parties) + " parties jouées.")
            return 0


if __name__ == "__main__":
    main()
