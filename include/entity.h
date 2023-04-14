#pragma once
#include "map.h"

class Map;

//Ο γενικός τύπος οντότητας που εμφανίζεται στο παιχνίδι. Κοινός πρόγονος όλων των οντοτήτων.
class Entity   
{
	protected:
		int position_x, position_y, medicine;   //Οι συντεταγμένες μιας οντότητας στον χάρτη και το γιατρικό που διαθέτει
		std::string team;    //Η ομάδα που υποστηρίζει μια οντότητα ("Vampires" ή "Werewolves")
	
	public:
		Entity(int, std::string);  //Constructor
		virtual ~Entity() = 0;   //Pure virtual destructor

		void place_on_map(Map*);    //Τοποθέτηση μιας οντότητας τυχαία στον χάρτη 

		void move(int, int);   //Κίνηση μιας οντότητας σε δύο νέες συντεταγμένες
		int get_position_x() const;   //Επιστροφή συντεταγμένων της οντότητας στον χάρτη
		int get_position_y() const;


		int get_medicine();	   //Επιστροφή γιατρικού της οντότητας
		std::string get_team() const;    //Επιστροφή ομάδας της οντότητας
		virtual char get_symbol() const=0;  //Επιστροφή συμβόλου οντότητας στον χάρτη
};