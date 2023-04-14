#include <iostream>
#include <cstdlib>
#include <string>
#include "../include/npc.h"
#include "../include/functions.h"

using namespace std;

NPC::NPC(int a, string b, int c, bool d, const int e, const int f):Entity(a, b), health(c), has_moved(d), power(e), defense(f) {};  //Constructor NPC

NPC::~NPC(){};   //Destructor NPC

int NPC::get_power()
{
    return power;
}


int NPC::get_health() const
{
	return health;
}

void NPC::heal()
{
	health++;   //Αυξανεται το επίπεδο υγείας κατά 1
}

void NPC::fully_heal()
{
	health=10;   //Επιστρέφει το επίπεδο υγείας στο μέγιστο 10
}


void NPC::heal_other(NPC* ally, int seed)   //Το npc γιατρέυει τον ally
{
	if (team == ally->team && ally->health < 10 && medicine>0)   //Αν ο ally είναι στην ίδια ομάδα και του λείπει υγεία και έχουμε γαιτρικό
	{
		if (random_number(0, 1, seed) == 1)  //Τυχαία επιλογή
		{
			ally->health++;    //Χρησιμοποιείται 1 γιατρικό και αυξάνεται η υγεία του ally κατά 1
			medicine--;
		}
	}
}

bool NPC::get_has_moved()
{
    return has_moved;
}

void NPC::set_has_moved(bool a )
{
    has_moved = a;
}

void NPC::damage(NPC* enemy)
{
	if (enemy->defense == power);
	{
		enemy->health -= 1;
		return;
	}
	enemy->health -= abs(enemy->defense - power);  //Μειώνεται η υγεία του enemy

}



Vampire::Vampire(int a, string b, int c, bool d, const int e, const int f):NPC(a, b, c, d, e, f){};  //Constructor Vampire

Vampire::~Vampire(){}; //Destructor Vampire

char Vampire::get_symbol() const   //Σύμβολο των Vampires, 'v'
{
 return 'v';
}

Werewolf::Werewolf(int a, string b, int c, bool d, const int e, const int f):NPC(a, b, c, d, e, f){};   //Constructor Werewolf

Werewolf::~Werewolf(){}; //Destructor Werewolf  

char Werewolf::get_symbol() const
{
	return 'w';    //Σύμβολο των Werewolves, 'w'
}
