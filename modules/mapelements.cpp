#include <iostream>
#include "../include/mapelements.h"

using namespace std;

////////////////////////////////////////////////////////////////////////
//  Nodes
///////////////////////////////////////////////////////////////////////

//Constructor Node
Node::Node():entity(NULL), has_potion(false) {}

//Destructor Node
Node::~Node() {}

//Βοηθητικές συναρτήσεις για τον έλενχο του map
void Node::set_healthy(bool heal)
{
	healthy = heal;
}

//Βοηθητικές συναρτήσεις για τον έλενχο του map
int Node::get_healthy() const
{
	return healthy;
}

//Επιστρέφει το entity αν υπάρχει αλλιώς NULL
Entity* Node::get_entity() const
{
	return entity;
}

//Θέτει στο node το νεο entity
void Node::set_entity(Entity* ent)
{
	entity = ent;
}

void Node::set_has_potion(bool a)
{
    has_potion = a;
}

bool Node::get_has_potion() const
{
    return has_potion;
}


////////////////////////////////////////////////////////////////////////////
// Trees
////////////////////////////////////////////////////////////////////////////

//Constructor Trees
Trees::Trees() {}

//Destructor Node
Trees::~Trees() {}

//Eπιστρέφει false γιατί είναι tree και δεν έχουν πρόσβαση σε αυτό
bool Trees::get_access() const
{
    return false;
}

//Επιστρέφει τον χαρακτήρα που συμβολίζεται το δέντρο
string Trees::get_char() const
{
    return "&&&";
}


////////////////////////////////////////////////////////////////////////////
// Water
////////////////////////////////////////////////////////////////////////////

//Constructor Water
Water::Water() {}

//Destructor Water
Water::~Water()  {}

//Eπιστρέφει false γιατί είναι water και δεν έχουν πρόσβαση σε αυτό
bool Water::get_access() const
{
    return false;
}

//Επιστρέφει τον χαρακτήρα που συμβολίζεται το water
string Water::get_char() const
{
    return "~~~";
}


////////////////////////////////////////////////////////////////////////////
// Earth
////////////////////////////////////////////////////////////////////////////

//Constructor Trees
Earth::Earth() {}

//Destructor Node
Earth::~Earth()  {}

//Eπιστρέφει true γιατί είναι earth και είναι προσβάσιμος κόμβος
bool Earth::get_access() const
{
    return true;
}

//Επιστρέφει τον χαρακτήρα που συμβολίζεται το earth
string Earth::get_char() const
{
    return "***";
}



