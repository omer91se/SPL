/**
*@Authors: Anat Bar-Sinai, Omer Segal.
*@version: Last Update: 14.11.2018
*/

#include <iostream>
#include <climits>
#include "../include/Customer.h"

using namespace std;
/**
 * Constructor.
 * @param c_name.
 * @param c_id.
 */
Customer::Customer(string c_name, int c_id):name(c_name), id(c_id){}

int Customer::getId() const{
    return this->id;
}

string Customer::getName() const {
    return this->name;
}

//VEG
VegetarianCustomer::VegetarianCustomer(string name, int id) : Customer(name, id), type("veg") {}

/**
 * Orders for the vegetarian customer (lowest id VEG dish and most expencive BVG).
 *
 * @param menu.
 * @return vector of dishes id..
 */
vector<int> VegetarianCustomer::order(const vector<Dish> &menu) {
    vector<int> dishes(2);
    bool food = false;
    int max_price = INT_MIN;
    int bevId = -1;

    for (vector<Dish>::const_iterator dish = menu.begin(); dish != menu.end(); ++dish) {
        if (dish->getType() == VEG & food)
            dishes.push_back((dish->getId()));

        if (dish->getType() == BVG & dish->getPrice() >max_price) {
            max_price = dish->getPrice();
            bevId = dish->getId();
        }
    }
    dishes.push_back(bevId);
    return dishes;
}
//TODO
string VegetarianCustomer::toString() const {
    return type;
}





//CHP
CheapCustomer::CheapCustomer(string name, int id) : Customer(name, id), type("chp") {

}

/**
 * Orders for the cheap customer (lowest price dish).
 *
 * @param menu
 * @return vector of dishes id.
 */
vector<int> CheapCustomer::order(const vector<Dish> &menu) {
    int min_price = INT_MAX;
    int dishId = -1;
    vector<int> dishes(1);

    for (vector<Dish>::const_iterator dish = menu.begin(); dish != menu.end(); ++dish) {
        if(dish->getPrice()<min_price) {
            min_price = dish->getPrice();
            dishId = dish->getId();
        }
    }
    dishes.push_back(dishId);
    return dishes;
}

//TODO
string CheapCustomer::toString() const {
    return type;
}





//SPC
SpicyCustomer::SpicyCustomer(string name, int id) : Customer(name, id), firstOrder(true), type("spc")
{}

/**
 * Orders for the spicy customer (If first order then most expensive SPC dish, else cheapest BVG).
 *
 * @param menu
 * @return vector of dishes id.
 */
vector<int> SpicyCustomer::order(const vector<Dish> &menu) {
    int max_price = INT_MIN;
    int min_price = INT_MAX;
    int dishId = -1;
    vector<int> dishes(1);


    for (vector<Dish>::const_iterator dish = menu.begin(); dish != menu.end(); ++dish){
        if(firstOrder) {
            if (dish->getType() == SPC && max_price < dish->getPrice()) {
                max_price = dish->getPrice();
                dishId = dish->getId();
            }

        }
        else
            if(dish->getType() == BVG && min_price > dish->getPrice() ){
                min_price = dish->getPrice();
                dishId = dish->getId();
            }
    }
    firstOrder = false;
    dishes.push_back(dishId);
    return dishes;
}

//TODO
string SpicyCustomer::toString() const {
    return type;
}





//ALC
AlchoholicCustomer::AlchoholicCustomer(string name, int id) : Customer(name, id), last_price(0), last_id(-1), type("alc") {

}

/**
 * Orders for the alcoholic customer (next cheapest ALC from last order).
 *
 * @param menu
 * @return vector of dishes id.
 */
vector<int> AlchoholicCustomer::order(const vector<Dish> &menu) {
    int min_price = INT_MAX;
    int dishId = -1;
    vector<int> dishes(1);


    for (vector<Dish>::const_iterator dish = menu.begin(); dish != menu.end(); ++dish){

        //Finds the cheapest ALC that was not ordered yet.
        if(dish->getId() != last_id && dish->getType() == ALC && dish->getPrice() >= last_price && dish->getPrice() < min_price) {
            //Make sure not to order an ALC with the same lowest price (which was ordered in the past).
            if(!(dish->getPrice() == last_price && dish->getId()<last_id))
            min_price = dish->getPrice();
            dishId = dish->getId();
        }
    }
    last_id = dishId;
    last_price = min_price;
    dishes.push_back(dishId);

    return dishes;
}

//TODO
string AlchoholicCustomer::toString() const {
    return type;
}
