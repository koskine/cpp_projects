/* Rasse
 *
 * Kuvaus:
 * Ohjelma, joka käsittelee ratikkalinjoihin liittyviä tietoja.
 * Suorituksen alussa tiedot luetaan tiedostosta ja talletetaan tietorakenteeseen
 * map<string, vector<pair<string, float>>>, johon viittaan jatkossa nimellä Tramway. Tiedoston luvussa
 * on varauduttu kolmeen virhetilanteeseen: annettua tiedoston nimeä ei löydy, tiedosto on väärässä
 * formaatissa sekä se, että tiedostossa on useampi pysäkki samalla nimellä tai etäisyydellä.
 * Kun tiedot on tallennettu onnistuneesti Tramway-tietorakenteeseen, alkaa ohjelman varsinainen suoritus.
 * Käyttäjä voi syöttää seuraavat komennot:
 * LINES: ei muita parametrejä, tulostaa kaikki tietorakenteesta löytyvät linjat aakkosjärjestyksessä.
 * LINE: parametrinä linjan nimi, tulostaa kaikki kyseisen linjan pysäkit siinä järjestyksessä kuin ne linjalla ovat.
 * STOPS: ei muita parametrejä, tulostaa kaikki tietorakenteesta löytyvät pysäkit aakkosjärjestyksessä
 * STOP: parametrinä pysäkin nimi, tulostaa kaikki linjat, joilta pysäkki löytyy.
 * DISTANCE: parametreinä linja ja kaksi pysäkkiä, kertoo kahden pysäkin välisen etäisyyden tietyllä linjalla
 * ADDLINE: parametrinä linjan nimi, lisää uuden linjan, jossa ei ole pysäkkejä, tietorakenteeseen.
 * ADDSTOP: parametreinä linja, pysäkin nimi ja pysäkin etäisyys lähtöpysäkiltä, lisää uuden pysäkin linjalle
 *          tietorakenteeseen
 * REMOVE: parametrinä pysäkin nimi, poistaa pysäkin kaikilta linjoilta.
 * QUIT: Lopettaa ohjelman suorituksen paluuarvolla EXIT_SUCCESS.
 *
 * Ohjelman kirjoittaja:
 * Nimi: Iiro Koskinen
 * Opiskelijanumero: H299947
 * Käyttäjätunnus: mhiiko
 * E-mail: iiro.koskinen@tuni.fi
 *
 * Huomioita toteutuksesta:
 * Kommentoin ohjelman suomeksi, mutta muuttujat ovat englanniksi, koska koodipohjan print_rasse funktio oli
 * englanniksi, samoin kuin ohjelman käyttöliittymä.
 *
 * */

#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <utility>
#include <string>
#include <algorithm>

using namespace std;
// Käytetään Tramway:ta kuvaamaan tietorakennetta johon talletetaan kaikki linjat ja niihin liittyvät tiedot.
// Jälkikäteen ajateltuna map<string, map<string, float>> olisi varmaan ollut fiksumpi tietorakenne, mutta
// tässä kohtaa sen vaihtaminen on hieman myöhäistä.
using Tramway = map<string, vector<pair<string, float>>>;

// The most magnificent function in this whole program.
// Prints a RASSE
void print_rasse()
{
    std::cout <<
                 "=====//==================//===\n"
                 "  __<<__________________<<__   \n"
                 " | ____ ____ ____ ____ ____ |  \n"
                 " | |  | |  | |  | |  | |  | |  \n"
                 " |_|__|_|__|_|__|_|__|_|__|_|  \n"
                 ".|                  RASSE   |. \n"
                 ":|__________________________|: \n"
                 "___(o)(o)___(o)(o)___(o)(o)____\n"
                 "-------------------------------" << std::endl;
}
// Funktio joka jakaa syötteenä saadun merkkijonon osiin
// Parametrit: jaettava merkkijono, jakomerkki ja merkki, jonka jälkeen jakomerkki jätetään huomiotta
// Paluuarvo: vektori, joka sisältää merkkijonoja
vector<string> split(const string s, const char delimiter, char ignore_char)
{
    vector<string> result;
    string temp = "";
    // Totuusarvo, joka kertoo otetaanko jakomerkki huomioon
    bool ignoring = false;
    for (char c : s)
    {
        if (c == delimiter && !ignoring)
        {
            if (temp != "")
            {
                result.push_back(temp);
                temp = "";
                continue;
            }
        }
        // Kun törmätään ignore_char merkkiin, muutetaan muuttujan ignoring totuusarvo kunnes
        // törmätään merkkiin uudestaan
        if (c == ignore_char)
        {
            ignoring = !ignoring;
            continue;
        }
        temp += c;
    }
    if (temp != "")
    {
        result.push_back(temp);
    }
    return result;
}



// Funktio, joka lisää syötetiedostosta luettujen linjojen ja pysäkkien tiedot Tramway-tietorakenteeseen.
// Parametrit: Tramway-tietorakenne ja merkkijonoja sisältävä vektori
// Paluuarvo: Totuusarvo tietojen tallentamisen onnistumisesta
bool add_tramway(Tramway& tramway, vector<string> parts)
{
    if (tramway.count(parts.at(0)) == 0)
    {
        // Jos vektorin sisältämää linjaa ei vielä ole, lisätään se map:iin ja alustetaan sen hyötykuormaksi tyhjä vektori
        tramway[parts.at(0)] = {};
    }

    if (parts.size() == 2)
    {
        // Koska split-funktio ei tallenna tyhjiä alkioita vektoriin, tiedetään että, jos vektori sisältää
        // vain kaksi alkiota, on kyseessä lähtöpysäkki, jolloin etäisyydeksi saadaan 0
        // Koska lähtöpysäkki on aina tiedostossa linjan ensimmäinen pysäkki, ei tarvitse tarkistaa onko se
        // jo lisätty
        pair<string, float> pair_to_be_added (parts.at(1), 0);
        tramway.at(parts.at(0)).push_back(pair_to_be_added);
    }
    else
    {
        // Koska split-funktio palauttaa vektorin, jonka alkiot ovat merkkijonoja, täytyy etäisyys muuttaa
        // floatiksi. Käytetään siihen string-kirjastosta löytyvää stof valmisfunktiota
        float string_to_float = stof(parts.at(2));
        // Ennen uuden pysäkin lisäämistä täytyy tarkistaa, ettei kyseistä pysäkkiä tai etäisyyttä ole vielä lisättynä.
        // Tutkitaan ensin pysäkkien nimet
        for (auto data_pair : tramway.at(parts.at(0)))
        {
            if (data_pair.first == parts.at(1))
            {
                return false;
            }
        }
        // Seuraavaksi tutkitaan vielä etäisyydet
        for (auto data_pair : tramway.at(parts.at(0)))
        {
            if (data_pair.second == string_to_float)
            {
                return false;
            }
        }
        // Jos ongelmia ei ilmene, lisätään uusi pysäkki ja sen etäisyys mappiin
        pair<string, float> pair_to_be_added (parts.at(1), string_to_float);
        tramway.at(parts.at(0)).push_back(pair_to_be_added);
    }
    return true;
}
// Funktio, joka kysyy syötetiedoston nimen, lukee tiedoston ja tallentaa tiedot Tramway-tietorakenteeseen
// add_tramway-funktioita hyödyntäen
// Parametrit: Tramway-tietorakenne
// Paluuarvo: totuusarvo tiedoston lukemisen onnistumiselle.
bool read_file(Tramway& tramway)
{

    // Kysytään syötetiedoston nimi ja yritetään avata se
    string file_name;
    cout << "Give a name for input file: ";
    getline(cin, file_name);

    ifstream file(file_name);
    if (not file)
    {
        cout << "Error: File could not be read." << endl;
        return false;
    }
    else
    {
        string line;
        while (getline(file, line))
        {

            vector<string> parts = split(line, ';', '"');
            // Jos vektorissa ei ole kahta tai kolmea alkiota, tiedetään, että syötetiedoston
            // formaatti on väärä
            if (parts.size() < 2 or parts.size() > 3)
            {
                cout << "Error: Invalid format in file." << endl;
                return false;
            }
            if (not add_tramway(tramway, parts))
            {
                cout << "Error: Stop/line already exists." << endl;
                return false;
            }

        }
        file.close();
    }
    return true;
}
// Funktio, joka tulostaa kaikki tietorakenteesta löytyvät linjat aakkosjärjestyksessä
// Parametrit: Tramway-tietorakenne
void print_lines(Tramway tramway)
{
    cout << "All tramlines in alphabetical order:" << endl;
    for (auto data_pair : tramway)
    {
        cout << data_pair.first << endl;
    }
}
// Funktio, joka tulostaa tietyn linjan tiedot siinä järjestyksessä, missä pysäkit ovat linjalla
// Parametrit: Tramway-tietorakenne, linjan nimi merkkijonona
void print_line(Tramway tramway, string line)
{
    cout << "Line " << line << " goes through these stops in the order they are listed:" << endl;
    for (auto data_pair : tramway.at(line))
    {
        cout << "- " << data_pair.first << " : " << data_pair.second << endl;
    }
}
// Funktio, joka selvittää onko annettu pysäkki annetulla linjalla
// Parametrit: Tramway-tietorakenne, linjan nimi merkkijonona, pysäkin nimi merkkijonona
// Paluuarvo: totuusarvo siitä onko pysäkki linjalla
bool is_stop_on_line(Tramway tramway, string line, string stop)
{
    // Käydään läpi annettu linja ja katsotaan löytyykö pysäkki
    for (auto data_pair : tramway.at(line))
    {
        if (data_pair.first == stop)
        {
            return true;
        }
    }
    return false;
}
// Funktio, joka selvittää onko annettu etäisyys annetulla linjalla
// Parametrit: Tramway-tietorakenne, linjan nimi merkkijonona, etäisyys merkkijonona
// Paluuarvo: totuusarvo siitä onko etäisyys linjalla
bool is_distance_on_line(Tramway tramway, string line, string distance_as_string)
{
    // Muutetaan annettu etäisyys merkkijonosta floatiksi
    float distance = stof(distance_as_string);
    // Käydään läpi annettu linja ja katsotaan löytyykö etäisyys
    for (auto data_pair : tramway.at(line))
    {
        if (data_pair.second == distance)
        {
            return true;
        }
    }
    return false;
}
// Funktio, joka selvittää onko pysäkki olemassa
// Parametrit: Tramway-tietorakenne, pysäkin nimi merkkijonona
// Paluuarvo: totuusarvo pysäkin olemassaolosta
bool is_stop(Tramway tramway, string stop)
{
    // Käydään läpi kaikki linjat ja katsotaan löytyykö pysäkki joltain niistä
    for (auto data_pair : tramway)
    {
        if (is_stop_on_line(tramway, data_pair.first, stop))
        {
            return true;
        }
    }
    return false;
}
// Funktio, jolla tarkistetaan miltä linjoilta annettu pysäkki löytyy
// Parametrit: Tramway-tietorakenne, pysäkin nimi merkkijonona
void stop_on_lines(Tramway tramway, string stop)
{
    // Luodaan vektori johon talletetaan kaikki linjat jolta pysäkki löytyy
    vector<string> lines;
    // Käydään linjat läpi yksitellen ja lisätään ne vektoriin jos pysäkki löytyy linjalta
    for (auto data_pair : tramway)
    {
        if (is_stop_on_line(tramway, data_pair.first, stop))
        {
            lines.push_back(data_pair.first);
        }
    }
    cout << "Stop " << stop << " can be found on the following lines:" << endl;
    for (string line : lines)
    {
        cout << " - " << line << endl;
    }
}
// Funktio joka tulostaa kaikki olemassa olevat pysäkit
// Parametrit: Tramway-tietorakenne
void print_stops(Tramway tramway)
{
    // Luodaan vektori johon talletetaan kaikki olemassa olevat pysäkit
    vector<string> all_stops;
    // Iteraattori jolla tarkistetaan onko pysäkki jo lisätty all_stops-vektoriin
    vector<string>::iterator it;
    // Käydään läpi kaikki mapin avaimet ja niitä vastaavat vektorit
    for (auto data_pair : tramway)
    {
        for (auto pair : data_pair.second)
        {
            it = find(all_stops.begin(), all_stops.end(), pair.first);
            if (it == all_stops.end())
            {
                all_stops.push_back(pair.first);
            }
        }
    }
    // Järjestetään pysäkit sisältävä vektori aakkosjärjestykseen ja tulostetaan pysäkit
    sort(all_stops.begin(),all_stops.end());
    cout << "All stops in alphabetical order:" << endl;
    for (string stop : all_stops)
    {
        cout << stop << endl;
    }
    // Lopuksi tyhjennetään vektori, jotta pysäkkien poistaminen päivittyy vektoriin.
    all_stops.clear();
}
// Funktio, joka kertoo annetun pysäkin etäisyyden lähtöpysäkistä annetulla linjalla
// Parametrit: Tramway-tietorakenne, linjan nimi merkkijonona, pysäkin nimi merkkijonona
// Paluuarvo: Float, pysäkin etäisyys lähtöpysäkistä
float find_location(Tramway tramway, string line, string stop)
{
    // Käydään läpi annetun linjan vektori ja palautetaan pysäkkiä vastaava etäisyys
    for ( auto data_pair : tramway.at(line))
    {
        if (data_pair.first == stop)
        {
            return data_pair.second;
        }
    }
    // Koska tiedämme kutsutun pysäkin olevan olemassa, eikä ohjeissa käsketty erikseen tarkastella tilannetta, jossa annettu pysäkki ei olisi
    // annetulla linjalla ei tarvitse varautua tilanteeseen jossa sijaintia ei saisi palautettua. QT-creator antaa kuitenkin varoituksen jos
    // en lisää jotain palautusta tähän kohtaa, joten palautan negatiivisen luvun, jolloin voisin tarvittaessa käyttää tämän luvun etumerkkiä
    // ehtona, sille onko pysäkki linjalla vai ei.
    return -1;
}
// Funktio, joka laskee kahden pysäkin välisen etäisyyden
// Parametrit: Tramway-tietorakenne, linjan ja pysäkkien nimet merkkijonoina
// Paluuarvo: float, pysäkkien välinen etäisyys
float calculate_distance(Tramway tramway, string line, string stop1, string stop2)
{
    // Selvitetään ensin molempien pysäkkien etäisyys lähtöpysäkistä
    float location1 = find_location(tramway, line, stop1);
    float location2 = find_location(tramway, line, stop2);
    float distance = location1 - location2;
    // Jos etäisyydeksi saadaan negatiivinen luku, palautetaan sen itseisarvo
    if (distance < 0)
    {
        return -distance;
    }
    else
    {
        return distance;
    }
}
// Funktio, jolla lisätään uusi pysäkki etäisyyksineen annetulle linjalle
// Parametrit: Tramway-tietorakenne, linjan ja pysäkin nimet sekä pysäkin etäisyys lähtöpysäkistä
// merkkijonona
void add_stop(Tramway& tramway, string line, string stop, string distance_as_string)
{
    // Muutetaan ensimmäisenä annettu etäisyys floatiksi
    float distance = stof(distance_as_string);
    // Muodostetaan pysäkin nimestä ja etäisyydestä pari, jonka tulemme lisäämään oikeaan kohtaan vektoria
    pair<string, float> new_stop (stop, distance);
    vector<pair<string, float>>::iterator it = tramway.at(line).begin();
    // Käydään läpi linjavektoria kunnes vastaan tulee alkio, jonka etäisyys on suurempi kuin uuden pysäkin
    // tämän jälkeen lisätään uusi pysäkki enne sitä kauempana olevaa pysäkkiä
    for (auto data_pair : tramway.at(line))
    {
        if (data_pair.second > new_stop.second)
        {
            tramway.at(line).emplace(it, new_stop);
            return;
        }
        ++it;
    }
    // Jos uuden pysäkin etäisyys on suurempi kuin olemassa olevien pysäkkien, lisätään se vektorin perään
    tramway.at(line).push_back(new_stop);
}
// Funktio, joka poistaa annetun pysäkin kaikilta linjoilta
// Parametrit: Tramway-tietorakenne, pysäkin nimi merkkijonona
void remove_stop(Tramway& tramway, string stop)
{
    // Käydään läpi kaikki linjat
    for (auto data_pair : tramway)
    {
        // Apumuuttuja, jolla saadaan poistettavan pysäkin indeksi
        int remove_index = 0;
        for (auto pair : data_pair.second)
        {
            // Poistetaan pysäkin sisältävä pari linjalta
            if (pair.first == stop)
            {
                tramway[data_pair.first].erase(tramway[data_pair.first].begin()+remove_index);
            }
            ++remove_index;
        }
    }
}
// Funktio, jossa toteutetaan ohjelman käyttöliittymä
// Parametrit: Tramway-tietorakenne
// Paluuarvo: Totuusarvo, kertoo main-funktiolle onnistuiko ohjelman suoritus.
// Paluuarvo ei saa muita arvoja kuin true, jolloin komennolla QUIT ohjelman suoritus
// päättyy paluuarvolla EXIT_SUCCESS
bool interface(Tramway& tramway)
{
    // Käyttöliittymä pyörii loputtomassa silmukassa kunnes sen suoritus lopetetaan
    // QUIT komennolla
    for (;;)
    {
    string line;
    std::cout << "tramway> ";
    getline(cin, line);
    vector<string> parts = split(line, ' ', '"');

    string command = parts.at(0);

    // Muutetaan syötteen ensimmäinen osa isoiksi kirjaimiksi
    for (char& c : command)
    {
        c = toupper(c);
    }
    if (command == "LINES")
    {
        print_lines(tramway);
    }
    else if (command == "LINE")
    {
        if (parts.size() < 2)
        {
            cout << "Error: Invalid input." << endl;
            continue;
        }
        else if (tramway.count(parts.at(1)) == 0)
        {
            cout << "Error: Line could not be found." << endl;
            continue;
        }
        else
        {
            print_line(tramway, parts.at(1));
        }
    }
    else if (command == "STOPS")
    {
        print_stops(tramway);
    }
    else if (command == "STOP")
    {
        if (parts.size() < 2)
        {
            cout << "Error: Invalid input." << endl;
            continue;
        }
        else if (!is_stop(tramway, parts.at(1)))
        {
            cout << "Error: Stop could not be found." << endl;
            continue;
        }
        else
        {
            stop_on_lines(tramway, parts.at(1));
        }
    }
    else if (command == "DISTANCE")
    {
        if (parts.size() < 4)
        {
            cout << "Error: Invalid input." << endl;
            continue;
        }
        else if (tramway.count(parts.at(1)) == 0)
        {
            cout << "Error: Line could not be found." << endl;
            continue;
        }
        else if (!is_stop(tramway, parts.at(2)) or !is_stop(tramway, parts.at(3)))
        {
            cout << "Error: Line could not be found." << endl;
            continue;
        }
        else
        {
            float distance = calculate_distance(tramway, parts.at(1), parts.at(2), parts.at(3));
            cout << "Distance between " << parts.at(2) << " and " << parts.at(3) << " is " << distance << endl;
        }
    }
    else if (command == "ADDLINE")
    {
        if (parts.size() < 2)
        {
            cout << "Error: Invalid input." << endl;
            continue;
        }
        else if (tramway.count(parts.at(1)) != 0)
        {
            cout << "Error: Stop/line already exists." << endl;
            continue;
        }
        else
        {
            // Lisätään map:iin linja jolla ei ole vielä pysäkkejä
            tramway[parts.at(1)] = {};
            cout << "Line was added." << endl;
        }
    }
    else if (command == "ADDSTOP")
    {

        if (parts.size() < 4)
        {
            cout << "Error: Invalid input." << endl;
            continue;
        }
        else if (tramway.count(parts.at(1)) == 0)
        {
            cout << "Error: Line could not be found." << endl;
            continue;
        }
        else if (is_stop_on_line(tramway, parts.at(1), parts.at(2)) or is_distance_on_line(tramway,parts.at(1), parts.at(3)))
        {
            cout << "Error: Stop/line already exists." << endl;
            continue;
        }
        else
        {
            add_stop(tramway, parts.at(1), parts.at(2), parts.at(3));
            cout << "Stop was added." << endl;
        }

    }
    else if (command == "REMOVE")
    {
        if (parts.size() < 2)
        {
            cout << "Error: Invalid input." << endl;
            continue;
        }
        else if (!is_stop(tramway, parts.at(1)))
        {
            cout << "Error: Stop could not be found." << endl;
            continue;
        }
        else
        {
            remove_stop(tramway, parts.at(1));
            cout << "Stop was removed from all lines." << endl;
        }
    }
    else if (command == "QUIT")
    {
        return true;
    }
    // Jos annettu komento ei ole mikään edellä mainituista tulostetaan virheilmoitus
    // ja palataan silmukan alkuun
    else
    {
        cout << "Error: Invalid input." << endl;
        continue;
    }
    }
}

// Short and sweet main.
int main()
{
    // Tietorakenne johon talletetaan kaikki linjat ja niitä koskevat tiedot
    Tramway tramway;
    print_rasse();
    if (not read_file(tramway))
    {
        return EXIT_FAILURE;
    }
    if (not interface(tramway))
    {
        return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;
}
