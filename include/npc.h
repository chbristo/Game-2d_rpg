#pragma once
#include <string>
#include "entity.h"
// #include "functions.h"
#include "map.h"
#include "mapelements.h"


//NPC δηλαδή non-playable character, ο γενικός τύπος που περιλαμβάνει vampires και werewolves

class NPC : public Entity
{
    protected:
        int health;   //Επίπεδο υγείας, μέγιστο το 10
        const int power, defense;   //Επίπεδο δύναμης και άμυνας
        bool has_moved;  //Αν το npc έχει κινηθεί στον τρέχοντα γύρο

    public:
        NPC(int, std::string, int, bool, const int, const int);  //Constructor
        virtual ~NPC()=0;   //Pure virtual destructor

        virtual char get_symbol() const=0;  //Σύμβολο στον χάρτη

        void fully_heal();   //Θεραπεύει πλήρως το npc, επίπεδο υγείας 10
        void heal();   //Αυξάνει το επίπεδο υγείας κατά 1
        int get_health() const;   //Επιστρέφει το επίπεδο υγείας
        void heal_other(NPC*, int);   //Το npc θεραπέυει ένα άλλο npc χρησιμοποιώντας γιατρικό
        
        bool get_has_moved();  //Επιστρέφει αν το npc έχει κινηθεί
        void set_has_moved(bool );  //Μεταβάλλει την τιμή του has_moved

        void damage(NPC*);  //Το npc μειώνει το επίπεδο υγείας ενός άλλου npc
        int get_power();   //Επιστρέφει το επίπεδο δύναμης npc
};



class Vampire : public NPC   //Vampires, 1 από τα 2 είδη npc
{

public:
	Vampire(int , std::string , int , bool , const int , const int);  //Constructor
	~Vampire();    //Destructor
    char get_symbol() const;  //Επιστρέφει το σύμβολο των Vampires, 'v'
};


class Werewolf : public NPC   //Werewolves
{
public:
	Werewolf(int , std::string , int , bool, const int , const int);  //Constructor
	~Werewolf();   //Destructor
    char get_symbol() const;   //Επιστρέφει το σύμβολο των Werewolves, 'w'

};