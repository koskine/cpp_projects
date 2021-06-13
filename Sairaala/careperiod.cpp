#include "careperiod.hh"
#include <iostream>

CarePeriod::CarePeriod(const std::string& start, Person* patient):
    patient_(patient), start_(start)
{
}

CarePeriod::CarePeriod(const Date &start, Person* patient):
    patient_(patient), start_(start)
{
}

CarePeriod::~CarePeriod()
{
}

void CarePeriod::close(const Date &today)
{
    end_ = today;
}

void CarePeriod::add_staff(Person *staff_member)
{
    staff_.insert(staff_member);
}

Date CarePeriod::get_start()
{
    return start_;
}

Date CarePeriod::get_end()
{
    return end_;
}

std::set<Person*> CarePeriod::get_staff()
{
    return staff_;
}

Person *CarePeriod::get_patient()
{
    return patient_;
}
