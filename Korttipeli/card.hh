/*Muistipelissä käytettävän Card-luokan header-tiedosto
*
* Ohjelman kirjoittajat
*
* Iiro Koskinen                Iris Matinlauri
* H299947                      H299798
* mhiiko                       smirma
* iiro.koskinen@tuni.fi        iris.matinlauri@tuni.fi
*
**/

/* Luokka: Card
 * ------------
 * Kuvaa yhtä korttia muistipelissä.
 *
 * COMP.CS.110 K2021
 * */


#ifndef CARD_HH
#define CARD_HH

enum Visibility_type {OPEN, HIDDEN, EMPTY};
const char HIDDEN_CHAR = '#';
const char EMPTY_CHAR = '.';

class Card
{
public:

    // Oletusrakentaja: luo tyhjän kortin.
    Card();

    // Rakentaja: luo piilotetun kortin, jonka kirjaimeksi tulee annettu merkki.
    Card(const char c);

    // Ilmeiset setter- ja getter-metodit.
    void set_letter(const char c);
    void set_visibility(const Visibility_type visibility);
    char get_letter() const;
    Visibility_type get_visibility() const;

    // Kääntää kortin: vaihtaa näkyvyyden avoimesta piilotettuun ja päin vastoin.
    void turn();

    // Tulostaa kortin sen tämänhetkisen tilanteen (näkyvyyden) mukaisesti.
    void print() const;


private:

    char letter_;
    Visibility_type visibility_;
};

#endif // CARD_HH
