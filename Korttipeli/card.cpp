/* Muistipelissä käytettävän Card-luokan toteutustiedosto
*
* Ohjelman kirjoittajat
*
* Iiro Koskinen                Iris Matinlauri
* H299947                      H299798
* mhiiko                       smirma
* iiro.koskinen@tuni.fi        iris.matinlauri@tuni.fi
*
**/

#include "card.hh"
#include <iostream>

Card::Card():
    letter_(EMPTY_CHAR), visibility_(EMPTY)
{

}

Card::Card(const char c):
    letter_(c), visibility_(HIDDEN)
{

}

void Card::set_letter(const char c)
{
    letter_ = c;
}

void Card::set_visibility(const Visibility_type visibility)
{
    visibility_ = visibility;
}

char Card::get_letter() const
{
    return letter_;
}

Visibility_type Card::get_visibility() const
{
    return visibility_;
}

void Card::turn()
{
    if(visibility_ == HIDDEN)
    {
        visibility_ = OPEN;
    }
    else if(visibility_ == OPEN)
    {
        visibility_ = HIDDEN;
    }
    else // if(visibility_ == EMPTY)
    {
        std::cout << "Cannot turn an empty place." << std::endl;
    }
}

void Card::print() const
{
    if (visibility_ == EMPTY)
    {
        std::cout << "." << std::flush;
    }
    else if (visibility_ == HIDDEN)
    {
        std::cout << "#" << std::flush;
    }
    else
    {
        std::cout << letter_ << std::flush;
    }
}


