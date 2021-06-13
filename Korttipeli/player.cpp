/* Muistipelissä käytettävän Player-luokan toteutustiedosto
*
* Ohjelman kirjoittajat
*
* Iiro Koskinen                Iris Matinlauri
* H299947                      H299798
* mhiiko                       smirma
* iiro.koskinen@tuni.fi        iris.matinlauri@tuni.fi
*
**/

#include "player.hh"



Player::Player(std::string name, int points):
    name_(name), points_(points)
{

}

std::string Player::get_name() const
{
    return name_;
}

unsigned int Player::number_of_pairs() const
{
    return points_;
}

void Player::add_card(Card &card)
{
    cards_collected_.push_back(card);
    ++points_;
}
