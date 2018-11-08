/**
*@Authors: Anat Bar-Sinai, Omer Segal.
*@version: Last Update: 8.11.2018
*/

#include <iostream>
#include "../include/Customer.h"

/**
 * Constructor
 * @param c_name
 * @param c_id
 */
Customer::Customer(std::string c_name, int c_id):name(c_name), id(c_id){}

int Customer::getId() const{
    return this->id;
}

std::string Customer::getName() const {
    return this->name;
}






VegetarianCustomer::VegetarianCustomer(std::string name, int id) : Customer(name, id) {

}

std::vector<int> VegetarianCustomer::order(const std::vector<Dish> &menu) {
    return std::vector<int>();
}

std::string VegetarianCustomer::toString() const {
    return std::__cxx11::string();
}






CheapCustomer::CheapCustomer(std::string name, int id) : Customer(name, id) {

}

std::vector<int> CheapCustomer::order(const std::vector<Dish> &menu) {
    return std::vector<int>();
}

std::string CheapCustomer::toString() const {
    return std::__cxx11::string();
}






SpicyCustomer::SpicyCustomer(std::string name, int id) : Customer(name, id) {

}

std::vector<int> SpicyCustomer::order(const std::vector<Dish> &menu) {
    return std::vector<int>();
}

std::string SpicyCustomer::toString() const {
    return std::__cxx11::string();
}






AlchoholicCustomer::AlchoholicCustomer(std::string name, int id) : Customer(name, id) {

}

std::vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu) {
    return std::vector<int>();
}

std::string AlchoholicCustomer::toString() const {
    return std::__cxx11::string();
}
