#pragma once
#include <vector>
#include "mapelements.h"


//Ο χάρτης του παιχνιδιού αποτελείται από ένα vector από vectors του node που είναι
//ο γενικός τύπος κάθε map element και περιέχει βασικές πληροφορίες που σχετίζονται
//για τον χάρτη.

class Node;

class Map
{
    int size;          	//Size Grid
    int size_x;			//Πλάτος Grid
    int size_y;			//Mηκος Grid
    int num_earth;      //πλήθος από τον τύπο γης που περιέχεται στο map
    public:
        std::vector<std::vector<Node*>> map;
        
        Map(int, int);
        ~Map();

        void create_map();      //Δημιουργια Grid
        bool check_map();		//Eλενχει αν το map πληροί κάποιος προυποθέσεις
        void show_map();        //Εμφανίζει τα Market NonAcc Normal καθως και την τοποθεσια της ομάδας

        int get_size_x() const;
        int get_size_y() const;
        int get_num_earth() const;
};









