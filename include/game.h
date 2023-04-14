#pragma once
#include "map.h"
#include "npc.h"
#include "avatar.h"


// Είναι η βασική κλάση του παιχνιδιού που χειρίζεται τις βασικές συναρτήσεις 
// οπως την δημιουργία του παιχνιδιού την διαφορές καταστάσεις και την έξοδο του
// περιέχει όλες τις συμαντικές λίστες και πληροφορίες
// δημιουργεί ποιο καθαρό ευανάγνωστό κώδικα και διευκολύνει στην επέκταση και 
// διαχωρισμό του παιχνιδιού σε καταστάσεις.
class Game {
    Map* map;   // Χάρτης παιχνιδιού
    Avatar* player; // Παίκτης
    std::vector<Vampire*> vampires;     //Vector με όλα τα vampire που υπάρχουν
    std::vector<Werewolf*> werewolves;  //Vector με όλα τα werewolves που υπάρχουν
    
    bool is_day;    //Αν είναι μερα true αλλιώς false
    
    public:
        Game();
        ~Game();

        bool create_game();     //Δημιουργεία παιχνιδιού 

        void movement(std::string);     //Κίνηση παίκτη/avatar
        void playing();     //Η κατάσταση που ο παίκτης παίζει 
        bool pause();       //Προσωρινή παύση που παιχνιδιού
        
        void npc_movement();    //Κινησεις των npc παικτών δλδ Vampires και werewolves

        void heal_all();    //Θεράπευσει των όλων των σημμάχων του avatar
        void heal_neighbours();     //Θεραπεύει τους γείτονες
        void npc_behavior();    //Συμπεριφορά των npc χαρακτήρων
        
        bool erase_npcs();
};