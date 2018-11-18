/**
*@Authors: Anat Bar-Sinai, Omer Segal.
*@version: Last Update: 8.11.2018
*/

#include "../include/Dish.h"


/**
 * Dish constructor.
 *
 * @param d_id
 * @param d_name
 * @param d_price
 * @param d_type
 */
Dish::Dish(int d_id, std::string d_name, int d_price, DishType d_type): id(d_id), name(d_name), price(d_price), type(d_type){
}

/**
 * id getter
 * @return id by int.
 */
int Dish::getId() const {
    return this->id;
}

/**
 * Dish name getter.
 * @return name by string.
 */
std::string Dish::getName() const {
    return this->name;
}

/**
 * Dish price getter.
 * @return price by int.
 */
int Dish::getPrice() const {
    return this->price;
}

/**
 * Dish type getter.
 * @return type by DishType.
 */
DishType Dish::getType() const {
    return this->type;
}