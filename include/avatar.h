#pragma once
#include <vector>
#include "entity.h"
#include "map.h"
#include "npc.h"

class Avatar : public Entity   //Ο avatar που ελέγχει ο παίκτης
{
	int potions;   //Ο αριθμός μαγικών φίλτρων πλήρους επούλωσης που έχει ο avatar
 
	public:
		Avatar(int, std::string, int);   //Constructor
		~Avatar();   //Destructor
		
		char get_symbol() const;   //Επιστρέφει το σύμβολο του avatar στον χάρτη, 'V' ή 'W' ανάλογα με την ομάδα του
		
		int set_support_team();   //Καθορισμός ομάδας του avatar

		void obtain_potion();   //Απόκτηση νέου μαγικού φίλτρου από τον avatar
		int get_potions();   //Επιστρέφει τον αριθμό μαγικών φίλτρων του avatar
		void reduce_potions();  //Μείωνει τα μαγικά φίλτρα του avatar κατά 1
};