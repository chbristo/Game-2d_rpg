#pragma once
#include <string>


//Random number between two numbers and the third is random seed
int random_number(int, int, int);

//Ελένχει αν είναι αριθμός και επιστρέφει true ή false
bool is_number(std::string);

//Επιστρέφει το width, height που θέλουμε για τον πίνακα
void map_dimensions(int* width, int* height);
