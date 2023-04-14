#include <iostream>

#include "../include/game.h"

#include <vector>

using namespace std;





int main() {

    Game game;     //Δημιουργία παιχνιδιού
    if (game.create_game() == false)
        return 0;
    

    game.playing();   //Παίξιμο παιχνιδιού

    
    return 0;
}