/* Class CarePeriod
 * ----------
 * COMP.CS.110 SPRING 2021
 * ----------
 * Class for describing a patient's care period in hospital.
 *
 * Note: Students need to implement almost all of this class by themselves.
 * */
#ifndef CAREPERIOD_HH
#define CAREPERIOD_HH

#include "person.hh"
#include "date.hh"
#include "utils.hh"
#include <string>
#include<set>

class CarePeriod
{
public:
    // Constructor, start date given as a string (ddmmyyyy).
    CarePeriod(const std::string& start, Person* patient);

    // Constructor, start date given as a Date object.
    CarePeriod(const Date& start, Person* patient);

    // Destructor.
    ~CarePeriod();

    // More public methods
    void close(const Date &today);

    void add_staff(Person* staff_member);

    // Selvät getter methodit
    Date get_start();
    Date get_end();
    std::set<Person*> get_staff();
    Person* get_patient();

private:
    Person* patient_;
    Date start_;
    Date end_;

    // Set johon talletetaan kaikki hoitojaksolla työskentelevät hoitajat
    // Valitsin setin helpottamaan nimien tulostusta aakkosjärjestyksessä,
    // mutta se ei toiminutkaan heti, koska settiin talletettiin pointereita
    // eikä Person-olioita.
    std::set<Person*> staff_;
};

#endif // CAREPERIOD_HH
