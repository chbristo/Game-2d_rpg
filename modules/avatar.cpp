#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include "../include/functions.h"
#include "../include/avatar.h"



using namespace std;

Avatar::Avatar(int a, string b, int c):Entity(a, b), potions(c){};   //Constructor Avatar

//Destructor Avatar
Avatar::~Avatar(){  };


char Avatar::get_symbol() const   //Σύμβολο του avatar ανάλογα με την ομάδα του
{
    if (team=="vampires")
        return 'V';

    return 'W';
}

int Avatar::set_support_team()
{
    string temp_team;
    cout<<"Please input the team you will be supporting ('v' or 'V' for Vampires, 'w' or 'W' for Werewolves, 0 for exit): ";
    cin>>temp_team;    //Εισαγωγή ομάδας μέχρι να δοθεί κάποιο από τα ζητούμενα
    while (temp_team!="v" && temp_team!="V" && temp_team!="w" && temp_team!="W" && temp_team!="0")
    {
        cout<<"Wrong input. Please select your team again ('v' or 'V' for Vampires, 'w' or 'W' for Werewolves, 0 for exit): ";
        cin>>temp_team;
    }

    if (temp_team=="0")  //Έξοδος αν δοθεί το 0
        return 0;
    
    else if (temp_team=="v" || temp_team=="V")    //Επιλογή ομάδας Vampires
    {
        team="vampires";
        cout<<"You are supporting the Vampires."<<endl<<endl;
    }
    else
    {
        team="werewolves";   //Επιλογή ομάδας Werewolves
        cout<<"You are supporting the Werewolves."<<endl<<endl;
    }
    return 1;
}



void Avatar::obtain_potion()
{
    potions++;
}


int Avatar::get_potions()
{
    return potions;
}


void Avatar::reduce_potions()
{
    potions--;
}
