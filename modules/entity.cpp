#include <iostream>
#include "../include/entity.h"
#include "../include/functions.h"

using namespace std;


//Constructor Entity
Entity::Entity(int a, string b):medicine(a), team(b) {}

//Destructor Entity
Entity::~Entity() {	}


void Entity::place_on_map(Map* map)
{
    int rand_x, rand_y;
    rand_x=random_number(0, map->get_size_x()-1, 1);    //Καθορισμός δύο τυχαίων συντεταγμένων στον χάρτη
    rand_y=random_number(0, map->get_size_y()-1, 2);
    int seed=3;
    while (map->map[rand_y][rand_x]->get_access()==false || map->map[rand_y][rand_x]->get_entity()!= NULL || map->map[rand_y][rand_x]->get_has_potion()==true)
    {
        rand_x=random_number(0, map->get_size_x()-1, seed++);    //Καθορισμός νέων συντεταγμένων μέχρι το node που δείχνουν να είναι γη και όχι κατειλημμένο από οντότητα
        rand_y=random_number(0, map->get_size_y()-1, seed++);
    }
    move(rand_x, rand_y);    //Τοποθέτηση οντότητας στις συντεταγμένες
    map->map[rand_y][rand_x]->set_entity(this);   
}

int Entity::get_position_x() const
{
	return position_x;
}

int Entity::get_position_y() const
{
	return position_y;
}

void Entity::move(int x, int y) 
{
	position_x = x;    //Καθορισμός νέων συντεταγμένων μιας οντότητας
	position_y = y;
}

int Entity::get_medicine()
{
	return medicine;
}

string Entity::get_team() const
{
	return team;
}
