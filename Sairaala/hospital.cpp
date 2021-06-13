#include "hospital.hh"
#include "utils.hh"
#include <iostream>
#include <set>

Hospital::Hospital()
{
}

Hospital::~Hospital()
{
    // Deallocating staff
    for( std::map<std::string, Person*>::iterator
         iter = staff_.begin();
         iter != staff_.end();
         ++iter )
    {
        delete iter->second;
    }

    // Remember to deallocate patients also
    for( std::map<std::string, Person*>::iterator
         iter = current_patients_.begin();
         iter != current_patients_.end();
         ++iter )
    {
        delete iter->second;
    }

}

void Hospital::recruit(Params params)
{
    std::string specialist_id = params.at(0);
    if( staff_.find(specialist_id) != staff_.end() )
    {
        std::cout << ALREADY_EXISTS << specialist_id << std::endl;
        return;
    }
    Person* new_specialist = new Person(specialist_id);
    staff_.insert({specialist_id, new_specialist});
    std::cout << STAFF_RECRUITED << std::endl;
}

void Hospital::enter(Params params)
{
    // Ensiksi tarkistetaan onko annettu henkilö jo sairaalassa. Tämä saadaan
    // selville käymällä läpi map current_patients
    std::string person_id = params.at(0);
    if( current_patients_.find(person_id) != current_patients_.end() )
    {
        std::cout << ALREADY_EXISTS << person_id << std::endl;
        return;
    }

    // Seuraavaksi käydään läpi onko annettua henkilöä ylipäätään olemassa ja
    // jos ei ole, niin luodaan henkilö ja lisätään henkilöön osoittava osoitin
    // mappiin all_people
    if (all_people_.find(person_id) == all_people_.end())
    {

        Person* patient_to_be_added = new Person(person_id);
        all_people_.insert({person_id, patient_to_be_added});
    }
    // Luodaan uusi osoitin henkilöön ja lisätään se tarvittaessa mappeihin
    //current_patients ja all_patients
    Person* new_patient = all_people_.at(person_id);
    current_patients_.insert({person_id, new_patient});
    if (all_patients_.find(person_id) == all_patients_.end())
    {
        all_patients_.insert({person_id, new_patient});
    }
    // luodaan uusi hoitojakso
    CarePeriod* new_careperiod = new CarePeriod(utils::today, new_patient);

    std::map<Person*, std::vector<CarePeriod*>>::iterator
            patient_iter = careperiods_.find(new_patient);
    if (patient_iter != careperiods_.end())
    {
        patient_iter->second.push_back(new_careperiod);
    }
    else
    {
        std::vector<CarePeriod*> new_vector;
        careperiods_.insert({new_patient, new_vector});
        careperiods_.at(new_patient).push_back(new_careperiod);
    }

    std::cout << PATIENT_ENTERED << std::endl;

}

void Hospital::leave(Params params)
{
    std::string patient_id = params.at(0);
    if (current_patients_.find(patient_id) == current_patients_.end())
    {
        std::cout << CANT_FIND << patient_id << std::endl;
        return;
    }

    Person* patient = current_patients_.at(patient_id);
    std::vector<CarePeriod*> patients_careperiods = careperiods_.at(patient);
    CarePeriod *current_careperiod = patients_careperiods.back();
    current_careperiod->close(utils::today);


    current_patients_.erase(patient_id);
    std::cout << PATIENT_LEFT << std::endl;


}

void Hospital::assign_staff(Params params)
{
    std::string staff_id = params.at(0);
    std::string patient_id = params.at(1);
    if (staff_.find(staff_id) == staff_.end())
    {
        std::cout << CANT_FIND << staff_id << std::endl;
        return;
    }
    else if (current_patients_.find(patient_id) == current_patients_.end())
    {
        std::cout << CANT_FIND << patient_id << std::endl;
        return;
    }
    else
    {
        Person* staff = staff_.at(staff_id);
        Person* patient = current_patients_.at(patient_id);
        std::vector<CarePeriod*> patients_careperiods = careperiods_.at(patient);
        CarePeriod* current_careperiod = patients_careperiods.back();
        current_careperiod->add_staff(staff);

        std::map<Person*, std::vector<CarePeriod*>>::iterator
                staff_iter = staffs_careperiods_.find(staff);
        if (staff_iter != staffs_careperiods_.end())
        {
            staff_iter->second.push_back(current_careperiod);
        }
        else
        {
            std::vector<CarePeriod*> new_vector;
            staffs_careperiods_.insert({staff, new_vector});
            staffs_careperiods_.at(staff).push_back(current_careperiod);
        }

        std::cout << STAFF_ASSIGNED << patient_id << std::endl;
    }
}

void Hospital::add_medicine(Params params)
{
    std::string medicine = params.at(0);
    std::string strength = params.at(1);
    std::string dosage = params.at(2);
    std::string patient = params.at(3);
    if( not utils::is_numeric(strength, true) or
        not utils::is_numeric(dosage, true) )
    {
        std::cout << NOT_NUMERIC << std::endl;
        return;
    }
    std::map<std::string, Person*>::const_iterator
            patient_iter = current_patients_.find(patient);
    if( patient_iter == current_patients_.end() )
    {
        std::cout << CANT_FIND << patient << std::endl;
        return;
    }
    patient_iter->second->add_medicine(medicine, stoi(strength), stoi(dosage));
    std::cout << MEDICINE_ADDED << patient << std::endl;
}

void Hospital::remove_medicine(Params params)
{
    std::string medicine = params.at(0);
    std::string patient = params.at(1);
    std::map<std::string, Person*>::const_iterator
            patient_iter = current_patients_.find(patient);
    if( patient_iter == current_patients_.end() )
    {
        std::cout << CANT_FIND << patient << std::endl;
        return;
    }
    patient_iter->second->remove_medicine(medicine);
    std::cout << MEDICINE_REMOVED << patient << std::endl;
}

void Hospital::print_patient_info(Params params)
{
    std::string patient_id = params.at(0);
    if (all_patients_.find(patient_id) == all_patients_.end())
    {
        std::cout << CANT_FIND << patient_id << std::endl;
        return;
    }
    Person* patient = all_patients_.at(patient_id);
    std::vector<CarePeriod*> list_of_careperiods = careperiods_.at(patient);
    for (CarePeriod* careperiod : list_of_careperiods)
    {
        std::cout << "* Care period: ";
        careperiod->get_start().print();
        if (!careperiod->get_end().is_default())
        {
            std::cout << " - ";
            careperiod->get_end().print();
            std::cout << std::endl;
        }
        else
        {
            std::cout << " -" << std::endl;
        }
        std::cout << "  - Staff: ";

        std::set<Person*> staff_set = careperiod->get_staff();
        std::set<std::string> staff_names;
        if (staff_set.size() == 0)
        {
            std::cout << "None" << std::endl;
        }
        else
        {
            for (Person* staff_member : staff_set)
            {
                staff_names.insert(staff_member->get_id());
            }
            std::string seperator = "";
            for (std::string caretaker : staff_names)
            {
                std::cout << seperator << caretaker;
                seperator = " ";
            }
            std::cout << std::endl;
        }
    }
    std::cout << "* Medicines: ";
    patient->print_medicines("  - ");
}

void Hospital::print_patient_info_from_string(std::string name)
{
    std::string patient_id = name;
    if (all_patients_.find(patient_id) == all_patients_.end())
    {
        std::cout << CANT_FIND << patient_id << std::endl;
        return;
    }
    Person* patient = all_patients_.at(patient_id);
    std::vector<CarePeriod*> list_of_careperiods = careperiods_.at(patient);
    for (CarePeriod* careperiod : list_of_careperiods)
    {
        std::cout << "* Care period: ";
        careperiod->get_start().print();
        if (!careperiod->get_end().is_default())
        {
            std::cout << " - ";
            careperiod->get_end().print();
            std::cout << std::endl;
        }
        else
        {
            std::cout << " -" << std::endl;
        }
        std::cout << "  - Staff: ";

        std::set<Person*> staff_set = careperiod->get_staff();
        std::set<std::string> staff_names;
        if (staff_set.size() == 0)
        {
            std::cout << "None" << std::endl;
        }
        else
        {
            for (Person* staff_member : staff_set)
            {
                staff_names.insert(staff_member->get_id());
            }
            std::string seperator = "";
            for (std::string caretaker : staff_names)
            {
                std::cout << seperator << caretaker;
                seperator = " ";
            }
            std::cout << std::endl;
        }
    }
    std::cout << "* Medicines: ";
    patient->print_medicines("  - ");
}

void Hospital::print_care_periods_per_staff(Params params)
{
    std::string staff_name = params.at(0);
    if (staff_.find(staff_name) == staff_.end())
    {
        std::cout << CANT_FIND << staff_name << std::endl;
        return;
    }


    Person* staff_member = staff_.at(staff_name);
    if (!staffs_careperiods_.count(staff_member))
    {
        std::cout << "None" << std::endl;
        return;
    }
    std::vector<CarePeriod*> list_of_careperiods =
            staffs_careperiods_.at(staff_member);


    for (CarePeriod* careperiod : list_of_careperiods)
    {
        careperiod->get_start().print();
        if (!careperiod->get_end().is_default())
        {
            std::cout << " - ";
            careperiod->get_end().print();
            std::cout << std::endl;
        }
        else
        {
            std::cout << " -" << std::endl;
        }
        std::cout << "* Patient: " << careperiod->get_patient()->get_id()
                  << std::endl;
    }
}

void Hospital::print_all_medicines(Params)
{
    std::map<std::string, std::set<std::string>> all_medicines;

    for (auto patient : all_patients_)
    {
        std::vector<std::string> patients_meds = patient.second->get_medicines();
        for ( std::string medicine : patients_meds)
        {
            if (all_medicines.find(medicine) != all_medicines.end())
            {
                all_medicines.at(medicine).insert(patient.first);
            }
            else
            {
                std::set<std::string> new_set;
                all_medicines.insert({medicine, new_set});
                all_medicines.at(medicine).insert(patient.first);
            }
        }

    }
    if (all_medicines.size() == 0)
    {
        std::cout << "None" << std::endl;
        return;
    }
    for (auto meds : all_medicines)
    {
        std::cout << meds.first << " prescribed for" << std::endl;
        for (std::string patient : meds.second)
        {
            std::cout << "* " << patient << std::endl;
        }
    }

}

void Hospital::print_all_staff(Params)
{
    if( staff_.empty() )
    {
        std::cout << "None" << std::endl;
        return;
    }
    for( std::map<std::string, Person*>::const_iterator iter = staff_.begin();
         iter != staff_.end();
         ++iter )
    {
        std::cout << iter->first << std::endl;
    }
}

void Hospital::print_all_patients(Params)
{
    if (all_patients_.size() == 0)
    {
        std::cout << "None" << std::endl;
        return;
    }
    for (auto patient : all_patients_)
    {
        std::cout << patient.first << std::endl;
        print_patient_info_from_string(patient.first);
    }
}

void Hospital::print_current_patients(Params)
{
    if (current_patients_.size() == 0)
    {
        std::cout << "None" << std::endl;
        return;
    }
    for (auto patient : current_patients_)
    {
        std::cout << patient.first << std::endl;
        print_patient_info_from_string(patient.first);
    }
}

void Hospital::set_date(Params params)
{
    std::string day = params.at(0);
    std::string month = params.at(1);
    std::string year = params.at(2);
    if( not utils::is_numeric(day, false) or
        not utils::is_numeric(month, false) or
        not utils::is_numeric(year, false) )
    {
        std::cout << NOT_NUMERIC << std::endl;
        return;
    }
    utils::today.set(stoi(day), stoi(month), stoi(year));
    std::cout << "Date has been set to ";
    utils::today.print();
    std::cout << std::endl;
}

void Hospital::advance_date(Params params)
{
    std::string amount = params.at(0);
    if( not utils::is_numeric(amount, true) )
    {
        std::cout << NOT_NUMERIC << std::endl;
        return;
    }
    utils::today.advance(stoi(amount));
    std::cout << "New date is ";
    utils::today.print();
    std::cout << std::endl;
}

std::map<Person *, std::vector<CarePeriod *> > Hospital::get_careperiods()
{
    return careperiods_;
}
