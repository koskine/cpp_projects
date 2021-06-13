/* Muistipeli (päätiedosto)
 *
 *
 * Ohjelman toiminta:
 *
 * Aluksi ohjelma kysyy pelaajilta pelilaudalle tulevien korttien kokonaismäärää.
 * Korttien määrän on oltava parillinen ja positiivinen kokonaisluku, muutoin
 * pelaajilta kysytään tätä uudestaan, kunnes validi syöte löytyy. Seuraavaksi kysytään
 * siemenlukua, jonka perusteella korttien sijainti arvotaan pelilaudalle. Tämän jälkeen
 * kysytään pelaajien määrää, jolle pätevät muutoin samat vaatimukset, kuin korttien
 * määrälle, mutta luvun ei tarvitse olla parillinen. Ennen pelin aloitusta on vielä
 * listattava osallistuvien henkilöiden nimet välilyönnillä erotettuna. Kun tarvittavat
 * tiedot ovat kasassa, pelilauta tulostuu näkyviin ensimmäisen kerran. Pelilauta on
 * näkyvillä ennen arvausta silloisessa tilassaan, sekä arvauksen jälkeen mahdollisesti
 * päivittyneessä tilassa.
 *
 * # vastaa kuvapuoli alaspäin olevaa korttia
 * . vastaa jo arvattua ja pelilaudalta poistettua korttia
 * (A, B, C ...) vastaa kuvapuoli ylöspäin olevaa korttia vaihtelevin kirjaimin
 *
 * Pelin aikana pelaajien vuoro vaihtelee siten, että ensimmäisenä listattu pelaaja aloitaa,
 * jonka jälken vuoro kulkee listauksen mukaan järjestyksessä, kuitenkin niin, että parin
 * löydettyään samainen pelaaja saa jatkaa vuoroaan, kunnes paria ei enää löydy käännetyistä
 * korteista.
 *
 * Jokainen pelaaja antaa vuorollaan kahdelle kortille kaksi koordinaattia (x,y- muodossa),
 * jolloin pelilaudalta kääntyy näkyviin nämä kaksi korttia, mikäli annetut koordinaatit
 * löytyvät pelilaudalta ja kortti on täten olemassa. Mikäli koordinaatit eivät ole valideja,
 * kysytään samalta pelaajalta uusien korttien koordinaatteja. Tämän jälkeen tulostetaan
 * käännettävät kortit pelilaudalle sekä kerrotaan, ovatko nämä kaksi korttia parit vaiko eivät.
 * Yhden kierroksen päätteeksi tulostetaan vielä kaikkien pelaajien pistetilanne kyseisen kierroksen
 * jälkeen.
 *
 * Peli jatkuu niin kauan kuin pelilaudalla on kortteja. Kun viimeinen korttipari käännetään, peli
 * loppuu ja tulostetaan voittaja pisteineen tai tasapelitilanteesa montako voittajaa löytyy ja mikä
 * pistemäärä siivitti voittoon.
 *
 *
 * Ohjelman kirjoittajat
 *
 * Iiro Koskinen                Iris Matinlauri
 * H299947                      H299798
 * mhiiko                       smirma
 * iiro.koskinen@tuni.fi        iris.matinlauri@tuni.fi
 *
 * */

#include <player.hh>
#include <card.hh>
#include <iostream>
#include <vector>
#include <random>

using namespace std;

const string INPUT_AMOUNT_OF_CARDS = "Enter the amount of cards (an even number): ";
const string INPUT_SEED = "Enter a seed value: ";
const string INPUT_AMOUNT_OF_PLAYERS = "Enter the amount of players (one or more): ";
const string INPUT_CARDS = "Enter two cards (x1, y1, x2, y2), or q to quit: ";
const string INVALID_CARD = "Invalid card.";
const string FOUND = "Pairs found.";
const string NOT_FOUND = "Pairs not found.";
const string GIVING_UP = "Why on earth you are giving up the game?";
const string GAME_OVER = "Game over!";

using Game_row_type = vector<Card>;
using Game_board_type = vector<vector<Card>>;

// Muuntaa annetun numeerisen merkkijonon vastaavaksi kokonaisluvuksi
// (kutsumalla stoi-funktiota).
// Jos annettu merkkijono ei ole numeerinen, palauttaa nollan
// (mikä johtaa laittomaan korttiin myöhemmin).
unsigned int stoi_with_check(const string& str)
{
    bool is_numeric = true;
    for(unsigned int i = 0; i < str.length(); ++i)
    {
        if(not isdigit(str.at(i)))
        {
            is_numeric = false;
            break;
        }
    }
    if(is_numeric)
    {
        return stoi(str);
    }
    else
    {
        return 0;
    }
}

// Täyttää pelilaudan (kooltaan rows * columns) tyhjillä korteilla.
void init_with_empties(Game_board_type& g_board, unsigned int rows, unsigned int columns)
{
    g_board.clear();
    Game_row_type row;
    for(unsigned int i = 0; i < columns; ++i)
    {
        Card card;
        row.push_back(card);
    }
    for(unsigned int i = 0; i < rows; ++i)
    {
        g_board.push_back(row);
    }
}

// Etsii seuraavan tyhjän kohdan pelilaudalta (g_board) aloittamalla
// annetusta kohdasta start ja jatkamalla tarvittaessa alusta.
// (Kutsutaan vain funktiosta init_with_cards.)
unsigned int next_free(Game_board_type& g_board, unsigned int start)
{
    // Selvitetään annetun pelilaudan rivien ja sarakkeiden määrät
    unsigned int rows = g_board.size();
    unsigned int columns = g_board.at(0).size();

    // Aloitetaan annetusta arvosta
    for(unsigned int i = start; i < rows * columns; ++i)
    {
        if(g_board.at(i / columns).at(i % columns).get_visibility() == EMPTY) // vaihdettu
        {
            return i;
        }
    }
    // Jatketaan alusta
    for(unsigned int i = 0; i < start; ++i)
    {
        if(g_board.at(i / columns).at(i % columns).get_visibility() == EMPTY)
        {
            return i;
        }
    }
    // Tänne ei pitäisi koskaan päätyä
    std::cout << "No more empty spaces" << std::endl;
    return rows * columns - 1;
}

// Alustaa annetun pelilaudan (g_board) satunnaisesti arvotuilla korteilla
// annetun siemenarvon (seed) perusteella.
void init_with_cards(Game_board_type& g_board, int seed)
{
    // Selvitetään annetun pelilaudan rivien ja sarakkeiden mä?ä?rä?t
    unsigned int rows = g_board.size();
    unsigned int columns = g_board.at(0).size();

    // Arvotaan täytettävä sijainti
    std::default_random_engine randomEng(seed);
    std::uniform_int_distribution<int> distr(0, rows * columns - 1);

    // Hylätään ensimmäinen satunnaisluku (joka on aina jakauman alaraja)
    distr(randomEng);

    // Jos arvotussa sijainnissa on jo kortti, valitaan siitä seuraava tyhjä paikka.
    // (Seuraava tyhjä paikka haetaan kierteisesti funktion next_free avulla.)
    for(unsigned int i = 0, c = 'A'; i < rows * columns - 1; i += 2, ++c)
    {
        // Lisätään kaksi samaa korttia (parit) pelilaudalle
        for(unsigned int j = 0; j < 2; ++j)
        {
            unsigned int cell = distr(randomEng);
            cell = next_free(g_board, cell);
            g_board.at(cell / columns).at(cell % columns).set_letter(c);
            g_board.at(cell / columns).at(cell % columns).set_visibility(HIDDEN);
        }
    }
}

// Tulostaa annetusta merkistä? c koostuvan rivin,
// jonka pituus annetaan parametrissa line_length.
// (Kutsutaan vain funktiosta print.)
void print_line_with_char(char c, unsigned int line_length)
{
    for(unsigned int i = 0; i < line_length * 2 + 7; ++i)
    {
        cout << c;
    }
    cout << endl;
}

// Tulostaa vaihtelevankokoisen pelilaudan reunuksineen.
void print(const Game_board_type& g_board)
{
    // Selvitetään annetun pelilaudan rivien ja sarakkeiden määrät
    unsigned int rows = g_board.size();
    unsigned int columns = g_board.at(0).size();

    print_line_with_char('=', columns);
    cout << "|   | ";
    for(unsigned int i = 0; i < columns; ++i)
    {
        cout << i + 1 << " ";
    }
    cout << "|" << endl;
    print_line_with_char('-', columns);
    for(unsigned int i = 0; i < rows; ++i)
    {
        cout << "| " << i + 1 << " | ";
        for(unsigned int j = 0; j < columns; ++j)
        {
            g_board.at(i).at(j).print();
            cout << " ";
        }
        cout << "|" << endl;
    }
    print_line_with_char('=', columns);
}

// Kysyy käyttäjältä tulon ja sellaiset tulon tekijät, jotka ovat
// mahdollisimman lähellä toisiaan.
int ask_product_and_calculate_factors(unsigned int& smaller_factor, unsigned int& bigger_factor)
{
    unsigned int product = 0;
    while(not (product > 0 and product % 2 == 0))
    {
        std::cout << INPUT_AMOUNT_OF_CARDS;
        string product_str = "";
        std::getline(std::cin, product_str);
        product = stoi_with_check(product_str);
    }

    for(unsigned int i = 1; i * i <= product; ++i)
    {
        if(product % i == 0)
        {
            smaller_factor = i;
        }
    }
    bigger_factor = product / smaller_factor;
    return product;
}

// Funktio tarkistaa, ovatko pelaajan antamat koordinaatit valideja
// Parametrit: koordinaatit sisältävä vektori ja pelilauta (vektorin sisällä oleva vektori)
// Paluuarvo: totuusarvo, koordinaattien validiudesta
bool are_coordinates_correct(std::vector<unsigned int> coordinates, Game_board_type g_board)
{
    // Jos koordinaatit ovat laudalla pienempiä kuin 0, on syöte ollut luku, joka on pienempi kuin 1
    // tai jokin muu kuin numero
    for (int i : coordinates)
    {
        if (i <= -1)
        {
            return false;
        }

    }
    unsigned int rows = g_board.size();
    unsigned int columns = g_board.at(0).size();

    // Tutkitaan, ovatko syötetyt x-koordinaatit valideja
    if (coordinates.at(0) >= columns or coordinates.at(2) >= columns)
    {
        return false;
    }
    // Tutkitaan, ovatko syötetyt y-koordinaatit valideja
    else if (coordinates.at(1) >= rows or coordinates.at(3) >= rows)

    {
        return false;
    }
    // Tutkitaan, ovatko koordinaatit samat
    else if (coordinates.at(0) == coordinates.at(2) and  coordinates.at(1) == coordinates.at(3))
    {
        return false;
    }
    // Tutkitaan, onko koordinaatin kohdalla jo laudalta poistettu kortti
    else if (g_board.at(coordinates.at(1)).at(coordinates.at(0)).get_visibility() == EMPTY or
             g_board.at(coordinates.at(3)).at(coordinates.at(2)).get_visibility() == EMPTY)
    {
        return false;
    }
    else
    {
        return true;
    }


}

// Funktio, joka selvittää voittajan, kun peli on saatu päätökseen. Sen palauttaman vektorin pituuden avulla
// saadaan selvitettyä onko voittajia yksi vai useampia
// Parametri: vektori kaikista Player-olioista
// Paluuarvo: vektori, joka sisältää voittaja Player-olion/oliot
std::vector<Player> who_wins(std::vector<Player> player_vector)
{
    // Alustetaan voittaja-vektori siten, että se palauttaa oletuksena player_vectorin ensimmäisen
    // alkion, ellei jollain muulla pelaajalla ole suurempia pisteitä
    std::vector<Player> winner_vector;
    winner_vector.push_back(player_vector.at(0));
    // Käydään läpi player_vectorin alkioita ja lisätään ne sopivassa tilanteessa voittaja-vektoriin
    for (unsigned long int i = 1; i < player_vector.size(); ++i)
    {
        // Jos tutkittavalla oliolla on enemmän pisteitä kuin voittaja-vektorista
        // löytyvällä oliolla, tyhjennetään vektori ja lisätään johtoon siirtynyt olio sinne.
        if (player_vector.at(i).number_of_pairs() > winner_vector.at(0).number_of_pairs())
        {
            winner_vector.clear();
            winner_vector.push_back(player_vector.at(i));
        }
        // Jos tutkittavalla oliolla on saman verran pisteitä kuin voittaja-vektorista
        // löytyvällä oliolla, lisätään uusi olio vektoriin, poistamatta siellä jo olevia olioita.
        else if (player_vector.at(i).number_of_pairs() == winner_vector.at(0).number_of_pairs())
        {
            winner_vector.push_back(player_vector.at(i));
        }
    }

    return winner_vector;

}


int main()
{
    Game_board_type game_board;

    unsigned int factor1 = 1;
    unsigned int factor2 = 1;
    // Muuttuja jonka avulla määritetään pelin lopetus
    int number_of_cards = ask_product_and_calculate_factors(factor1, factor2);
    init_with_empties(game_board, factor1, factor2);

    string seed_str = "";
    std::cout << INPUT_SEED;
    std::getline(std::cin, seed_str);
    int seed = stoi_with_check(seed_str);
    init_with_cards(game_board, seed);


    int player_amount = 0;
    while (not (player_amount > 0))
    {
        string player_amount_str = "";
        std::cout << INPUT_AMOUNT_OF_PLAYERS;
        std::getline(std::cin, player_amount_str);
        player_amount = stoi_with_check(player_amount_str);
    }


    std::vector<Player> player_vector;
    std::cout << "List " << player_amount << " players: ";
    // Luodaan ilmoitetun pelaajamäärän verran Player-olioita ja lisätään ne pelaaja-vektoriin
    for (int i = 0; i < player_amount; ++i)
    {
        string player_name = "";
        std::cin >> player_name;
        Player player_to_be_added(player_name);
        player_vector.push_back(player_to_be_added);
    }

    print(game_board);

    // Muuttuja joka kertoo montako paria on löytynyt, käytetään pelin lopettamisen määrittämiseen
    int pairs_found = 0;

    // Jatkuva silmukka, joka kierrättää pelivuoroa kunnes peli loppuu
    for(;;)
    {

        // Muuttuja jonka-avulla seurataan vuorossa olevaa pelaajaa
        unsigned long int player_index = 0;
        // Silmukka jolla käydään läpi kaikki pelaajavektorin alkiot
        while (player_index < player_vector.size())
        {


            // Vektori johon kerätään syötetyt koordinaatit, koordinaattien oikeellisuuden tarkastamista
            // ja niitä vastaavien korttien kääntämistä varten
            std::vector<unsigned int> coordinate_vector;
            // Silmukka jolla kysytään koordinaatteja, kunnes syöte on validi
            for(;;)
            {


                std::cout << player_vector.at(player_index).get_name() << ": " << INPUT_CARDS;
                for (int i = 0; i < 4; i++)
                {

                    string coordinate_to_be_added_str = "";
                    std::cin >> coordinate_to_be_added_str;

                    if (coordinate_to_be_added_str == "q")
                    {
                        std::cout << "Why on earth you are giving up the game?" << std::endl;
                        return EXIT_SUCCESS;
                    }
                    else
                    {
                        unsigned int coordinate_to_be_added = stoi_with_check(coordinate_to_be_added_str);
                        // Koska pelilaudan koordinaatti (1,1) vastaa pelilautavektorin koordinaattia (0,0)
                        // tulee kaikki syötetyt arvot pienentää yhdellä
                        --coordinate_to_be_added;
                        coordinate_vector.push_back(coordinate_to_be_added);
                    }

                }
                if (not are_coordinates_correct(coordinate_vector, game_board))
                {

                    std::cout << "Invalid card." << std::endl;
                    coordinate_vector.clear();
                }
                else
                {
                    break;
                }
            }
            // Luodaan muuttujat, jotka vastaavat arvattuja kortteja
            Card& card_1 = game_board.at(coordinate_vector.at(1)).at(coordinate_vector.at(0));
            Card& card_2 = game_board.at(coordinate_vector.at(3)).at(coordinate_vector.at(2));

            // Käännetään kortit oikein päin ja tulostetaan pelilauta kääntämisen jälkeen
            card_1.turn();
            card_2.turn();

            print(game_board);



            // Jos arvatut kortit ovat pari, lisätään kortti oikein arvanneelle pelaajalle ja
            // poistetaan kortit laudalta
            if ((card_1.get_letter() == card_2.get_letter()))
            {
                player_vector.at(player_index).add_card(card_1);
                card_1.set_visibility(EMPTY);
                card_2.set_visibility(EMPTY);
                std::cout << "Pairs found." << std::endl;
                ++ pairs_found;

            }
            // Jos kortit eivät ole pari, käännetään ne taas väärin päin ja vaihdetaan vuorossa olevaa pelaajaa
            else
            {
                card_1.turn();
                card_2.turn();
                std::cout << "Pairs not found." << std::endl;
                ++ player_index;
            }

            // Tulostetaan pistetilanne
            for (Player i : player_vector)
            {
                std::cout << "*** " << i.get_name() << " has " << i.number_of_pairs() << " pair(s)." << std::endl;
            }


            print(game_board);



            // Koska laudalla olevien parien määrä on tietenkin puolet korttien määrästä peli loppuu kun
            // kaikki parit on poistettu pelilaudalta
            if (pairs_found == number_of_cards / 2)
            {
                std::cout << "Game over!" << std::endl;
                std::vector<Player> winners = who_wins(player_vector);
                // Määritetään voittajavektorin koon perusteella voittajien määrä
                if (winners.size() > 1)
                {
                    std::cout << "Tie of " << winners.size() << " players with " << winners.at(0).number_of_pairs() << " pairs." << std::endl;
                }
                else
                {
                    std::cout << winners.at(0).get_name() << " has won with " << winners.at(0).number_of_pairs() << " pairs." << std::endl;
                }
                return EXIT_SUCCESS;

            }
        }
    }



    return EXIT_SUCCESS;
}

