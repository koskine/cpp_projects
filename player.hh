/* Muistipelissä käytettävän Player-luokan header-tiedosto
*
* Ohjelman kirjoittajat
*
* Iiro Koskinen                Iris Matinlauri
* H299947                      H299798
* mhiiko                       smirma
* iiro.koskinen@tuni.fi        iris.matinlauri@tuni.fi
*
**/

/* Luokka: Player
 * --------------
 * Kuvaa yhtä pelaajaa muistipelissä.
 *
 * COMP.CS.110 K2021
 * */


#ifndef PLAYER_HH
#define PLAYER_HH

#include "card.hh"
#include <string>
#include <vector>
#include <iostream>

class Player
{
public:

    // Rakentaja: luo annetun nimisen pelaajan.
    Player(std::string name, int points = 0);

    // Palauttaa pelaajan nimen.
    std::string get_name() const;

    // Palauttaa pelaajan tähän asti keräämien parien määrän.
    unsigned int number_of_pairs() const;

    // Siirtää annetun kortin pelilaudalta pelaajalle,
    // eli lisää kortin pelaajan keräämiin kortteihin
    // ja poistaa sen pelilaudalta.
    void add_card(Card& card);

private:

    std::string name_;
    int points_;
    std::vector<Card> cards_collected_;
};

#endif // PLAYER_HH
