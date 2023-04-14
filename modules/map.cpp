#include <iostream>
#include "../include/functions.h"
#include "../include/map.h"
#include "../include/npc.h"

using namespace std;

//Constructor Map
Map::Map(int x, int y):size_x(x), size_y(y)  {}

//Destructor Map διαγραφεί και όλα τα nodes
Map::~Map() 
{
    for (int i=0; i < size_y; i++) 
    {
        for (int j=0; j < size_x; j++)
            delete map[i][j];
        map[i].clear();
    }
}


//Δημιουργια του map και με τυχαίο τρόπο τον τύπο που θα έχει το κάθε node
void Map::create_map()
{
    map.resize(size_y);
    for (int i = 0 ; i <size_y ; i++)
        map[i].resize(size_x);

    num_earth = 0;
    Node *new_node;
    for (int i=0; i < size_y; i++) {
        for (int j=0; j < size_x; j++) {
            int rand_num = random_number(1, 100, i*size_y+j);
            if (rand_num < 80) {
                new_node = new Earth();
                num_earth ++;
            }
            else if (rand_num < 90) {
                new_node = new Trees();
            }
            else {
                new_node = new Water();
            }
            map[i][j] = new_node;
        }
    }
	int rand_x, rand_y;
    rand_x=random_number(0, get_size_x()-1, 1);    //Καθορισμός δύο τυχαίων συντεταγμένων στον χάρτη
    rand_y=random_number(0, get_size_y()-1, 2);
    int seed=3;
    while (map[rand_y][rand_x]->get_access()==false )
    {
        rand_x=random_number(0, get_size_x()-1, seed++);    //Καθορισμός νέων συντεταγμένων μέχρι το node που δείχνουν να είναι γη και όχι κατειλημμένο από οντότητα
        rand_y=random_number(0, get_size_y()-1, seed++);
    }
	map[rand_y][rand_x]->set_has_potion(true);
}

// Δειχνεί τον χάρτι και με τα τους χαρακτήρες
void Map::show_map() 
{
    for (int i=0; i < size_y; i++) {
        for (int j=0; j < size_x; j++) {
            if (map[i][j]->get_entity() == NULL && map[i][j]->get_has_potion()==false)
                cout << map[i][j]->get_char() << "  ";
            else if (map[i][j]->get_entity() == NULL && map[i][j]->get_has_potion()==true)
                cout<<"*P*" << "  ";
            else if (map[i][j]->get_entity()->get_symbol()=='v' || map[i][j]->get_entity()->get_symbol()=='w')
                cout << map[i][j]->get_entity()->get_symbol() << ((NPC*) (map[i][j]->get_entity()))->get_health()<<"  ";
            else
                cout << map[i][j]->get_entity()->get_symbol()<<"  ";
        }
        cout << endl;
    }
}

// Ελένχει αν ο χάρτης είναι σωστός. Σωστός θεωρείται ένας χάρτης που ο παικτής μπορεί να κινηθεί σε όλα
// τα nodes που είναι προσβάσιμα. Ο τρόπος που ο χάρτης κάνει αυτή την λειτουργία είναι:
// Βρίσκει το πρώτο node που είναι earth και το κάνει healthy. Μετά για κάθε άλλο earth node
// ελένχει αν γειτονεύει με κάποιο healthy και μετά γίνεται και αυτό. Επαναλαμβάνουμε αυτή την
// διαδικασία για κάποιο βάθος και μετά ελένχουμε αν όλα τα earth nodes είναι healthy και 
// κρίνουμε έτσι αν ο χάρτης είναι σωστός
bool Map::check_map()
{
    bool not_found = true;
    for (int i=0; i < size_y; i++)
    {
        for (int j=0; j < size_x; j++)
        {
            if (map[i][j]->get_access() && not_found)
            {
                not_found = false;
                map[i][j]->set_healthy(true);
            }
            else
                map[i][j]->set_healthy(false);
        }
    }
    for (int k=0; k < size_y; k++)
    {
        for (int i=0; i < size_y; i++)
        {
            for (int j=0; j < size_x; j++)
            {
                if (map[i][j]->get_access())
                {
                    if (i == 0 and j == 0) 
                    {
                        if (map[1][0]->get_healthy() or map[1][1]->get_healthy() or map[0][1]->get_healthy())
                            map[0][0]->set_healthy(true);
                    } 
                    else if (i==0 and j < size_x-1)
                    {    
                        if (map[0][j-1]->get_healthy() or map[0][j+1]->get_healthy() or map[1][j-1]->get_healthy() or map[1][j]->get_healthy() or map[1][j+1]->get_healthy())
                            map[0][j]->set_healthy(true);
                    }
                    else if (i==0 and j == size_x - 1)
                    {
                        if (map[0][j-1]->get_healthy() or map[0][j]->get_healthy() or map[1][j-1]->get_healthy() or map[1][j]->get_healthy())
                            map[0][j]->set_healthy(true);
                    }
                    else if (i == size_y-1 and j == 0)
                    {
                        if (map[i-1][j]->get_healthy() or map[i-1][j+1]->get_healthy() or map[i][j+1]->get_healthy())
                            map[i][j]->set_healthy(true);
                    }
                    else if (i == size_y-1 and j < size_x-1)
                    {
                        if (map[i][j-1]->get_healthy() or map[i][j+1]->get_healthy() or map[i-1][j-1]->get_healthy() or map[i-1][j]->get_healthy() or map[i-1][j+1]->get_healthy())
                            map[i][j]->set_healthy(true);
                    }
                    else if (i == size_y-1 and j == size_x - 1)
                    {
                        if (map[i][j-1]->get_healthy() or map[i][j]->get_healthy() or map[i-1][j-1]->get_healthy() or map[i-1][j]->get_healthy())
                            map[i][j]->set_healthy(true);
                    }
                    else if (j == 0)
                    {
                        if (map[i][j]->get_healthy() or map[i-1][j]->get_healthy() or map[i+1][1]->get_healthy() or map[i][1]->get_healthy() or map[i-1][1]->get_healthy() or map[i+1][1]->get_healthy())
                            map[i][j]->set_healthy(true);
                    }
                    else if (j == size_x - 1)
                    {
                        if (map[i][j]->get_healthy() or map[i-1][j]->get_healthy() or map[i+1][1]->get_healthy() or map[i][j-1]->get_healthy() or map[i-1][j-1]->get_healthy() or map[i+1][j-1]->get_healthy())
                            map[i][j]->set_healthy(true);
                    }
                    else
                    {
                        if (map[i][j-1]->get_healthy() or map[i][j+1]->get_healthy() or map[i-1][j]->get_healthy() or map[i-1][j-1]->get_healthy() or map[i-1][j+1]->get_healthy() or map[i+1][j]->get_healthy() or map[i+1][j-1]->get_healthy() or map[i+1][j+1]->get_healthy())
                            map[i][j]->set_healthy(true);
                    }
                }
            }
        }    
    }
    for (int i=0; i < size_y; i++)
        for (int j=0; j < size_x; j++)
            if (map[i][j]->get_access() and not map[i][j]->get_healthy())
                return false;
    return true;
}


// Μέγεθος του άξονα x
int Map::get_size_x() const
{
    return size_x;
}

// Μέγεθος του άξονα y
int Map::get_size_y() const
{
    return size_y;
}

// Πλήθος nodes που είναι earth
int Map::get_num_earth() const
{
    return num_earth;
}