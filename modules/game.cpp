#include <iostream>
#include "../include/game.h"
#include "../include/functions.h"

using namespace std;

//Δημιουργεία παιχνιδιού 
bool Game::create_game() 
{    
    // Map Creation
    cout << "Creating map..." << endl;
    int width, height;
    map_dimensions(&width, &height);
    if (width==0 && height==0)
        return false;
    while(true)
    {
        map = new Map(width, height);
        map->create_map();
        if (map->check_map()) 
            break;
        delete map;
    }
    is_day = true;

    // Player Creation
    cout << "\nCreating player..." << endl;
    player = new Avatar (random_number(0, 2, 0), "team", 1);
    if (player->set_support_team() != 1)
        return false;
    player->place_on_map(map);

    // NPC creation
    cout << "Creating NPC monsters..." << endl;
    int available_earth = (map->get_num_earth()-2)/2;
    int team_number=random_number(1, (map->get_size_x()*map->get_size_y())/15, 5);
    int seed = 5;
    while (team_number > available_earth)
        team_number=random_number(1, (map->get_size_x()*map->get_size_y())/15, seed++);

    for (int i=0; i<team_number; i++)
    {
        Vampire* v = new Vampire(random_number(0, 2, i), "vampires", 10, false, random_number(1, 3, i+1), random_number(1,2, i+2));
        v->place_on_map(map);

        Werewolf* w = new Werewolf(random_number(0, 2, i+3), "werewolves", 10, false, random_number(1, 3, i+4), random_number(1,2, i+5));
        w->place_on_map(map);

        vampires.push_back(v);
        werewolves.push_back(w);
    }
    return true;
}

//Εκτέλεση του παιχνιδιού μέχρι τον τερματισμό του
void Game::playing() 
{
    cout << "Game about to start..." << endl;
    string key;
    int day_counter = 1;
    while (true) 
    {
        if (is_day)     // Έλενχος ώρας και μέρας
            cout << "It is morning " << 6-day_counter << " hours to become night" << endl;
        else
            cout << "It is night " << 6-day_counter << " hours to become morning" << endl;
        map->show_map();
        cout <<"Please input your choice" << endl;
        cout <<"Press to move w-a-s-d-q-e-z-x" << endl;
        cout <<"Press h to heal ally, j to heal all with potion, p to pause, 0 to terminate" << endl;
        cin>>key;
        if (key == "0")     // Τερματισμός του παιχνιδιού 
            break;
        else if (key=="a"||key=="d"||key=="s"||key=="w"||key=="q"||key=="e"||key=="z"||key=="x") //κίνηση του avatar 
            movement(key);        
        else if (key == "p" || key=="P") //παυση του παιχνιδιού
        {
            if (pause())
                break;
            continue;
        }
        else if (key == "j" || key == "J") //θεραπεία όλων των σημμάχων
        {
            heal_all();
            continue;
        }
        else if (key == "h" || key == "H") //θεραπεία κοντινού σημμάχου
        {    
            heal_neighbours();
            continue;
        }
        npc_behavior();
        if (erase_npcs())
            break;
        npc_movement();     //κίνηση των npc χαρακτήρων

        //εδω  λειπεί ένα μήνυμα
        
        if (day_counter == 6)
        {
            is_day = !is_day;
            day_counter = 1;
        }
        day_counter ++;
    }

}

// Ελένχει αν πρέπει να αφαιρεθεί κάποιο npc από τις λίστες "αν έχει πεθάνει"
// Και ελένχει αν μια ομάδα έχει κερδίσει για τον τερματισμό του παιχνιδιού
bool Game::erase_npcs() 
{
    for (int i=0; i<vampires.size(); i++) {
        if (vampires.at(i)->get_health() <= 0) {        //Αν κάποιο vampire δεν έχει health
            for (int y=0; y<map->get_size_y(); y++) {
                for (int j=0; j<map->get_size_x(); j++) {
                    if (map->map[y][j]->get_entity() == vampires.at(i)) {
                        map->map[y][j]->set_entity(NULL);    //Βρες το vampire στον χάρτη και διέγραψέ το
                    }
                }
            }
            delete vampires.at(i);    //Διέγραψε το vampire από τη μνήμη και το vector
            vampires.erase(vampires.begin() + i);
        }
    }
    for (int i=0; i<werewolves.size(); i++) {        //Διαγραφή werewolf με τον ίδιο τρόπο
        if (werewolves.at(i)->get_health() <= 0) {
            for (int y=0; y<map->get_size_y(); y++) {
                for (int j=0; j<map->get_size_x(); j++) {
                    if (map->map[y][j]->get_entity() == werewolves.at(i)) {
                        map->map[y][j]->set_entity(NULL);
                    }
                }
            }
            delete werewolves.at(i);
            werewolves.erase(werewolves.begin() + i);
        }
    }

    if (vampires.size()==0 && werewolves.size()==0)      //Αν το πλήθοςκαι των δύο ομάδων φτάσει το 0
    {
        cout<<"All vampires and werewolves have died."<<endl;
        return true;
    }
    else if (vampires.size()==0)   //Αν το πλήθος μιας ομάδας φτάσει το 0, η άλλη ομάδα κερδίζει
    {
        cout<<"All vampires have died. Werewolves win."<<endl;
        return true;
    }
    else if (werewolves.size()==0)
    {
        cout<<"All werewolves have died. Vampires win."<<endl;
        return true;
    }
    return false;
}

//Θεραπεία των γειτονικών σημμάχων 
void Game::heal_neighbours() {
    if (player->get_medicine() == 0)
    {
        cout <<"You have no medicine to cure allies" <<endl;
        return ;
    }
    int start_y=player->get_position_y()-1;
    int end_y=player->get_position_y()+1;
    if (player->get_position_y()==0)
        start_y=0;
    if (player->get_position_y()==map->get_size_y()-1)
        end_y=map->get_size_y()-1;

    int start_x = player->get_position_x()-1;
    int end_x = player->get_position_x()+1;
    if (player->get_position_x()==0)
        start_x=0;
    if (player->get_position_x()==map->get_size_x()-1)
        end_x=map->get_size_x()-1;

    vector<NPC*> neighbours;
    for (int i=start_y; i<=end_y; i++)
    {
        for (int j=start_x; j<=end_x; j++)
        {
            if (map->map[i][j]->get_entity()!=NULL && (player->get_position_x() != j || player->get_position_y() != i))
            {
                NPC* npc= (NPC*) (map->map[i][j]->get_entity());
                if (npc->get_team()==player->get_team() && npc->get_health()<10)
                    neighbours.push_back(npc);
            }
        }
    }
    if (neighbours.size()==0)
        cout<<"No neighbouring allies."<<endl;
    else
    {
        string ally = "a";
        cout<<"Choose ally to heal (numbers 1-"<<neighbours.size()<<" starting from top left going clockwise, other number to heal nobody): ";
        cin>>ally;
        while (is_number(ally)==false)
        {
            cout<<"Wrong input."<<endl;
            cout<<"Choose ally to heal (numbers 1-"<<neighbours.size()<<" starting from top left going clockwise, other number to heal nobody): ";
            cin>>ally;
        }
        if (stoi(ally)>=1 && stoi(ally)<=neighbours.size()) 
            neighbours.at(stoi(ally)-1)->heal();
    }
    neighbours.clear();
}

// Ο παίκτης θεραπεύει όλους τους συμπαίκτες του
void Game::heal_all() 
{
    if (player->get_potions() == 0)
    {
        cout <<"You have no potions to cure them all"<<endl;
        return ;
    }
    if (player->get_symbol()=='V') 
    {
        if (is_day == false)
        {
            cout << "It is night you cant heal the Vampires" << endl;
            return;
        }
        for (int i=0; i<vampires.size();i++)
            vampires.at(i)->fully_heal();
    }
    else 
    {   
        if (is_day)
        {
            cout << "It is morning you cant heal the Werewolves" << endl;
            return;
        }
        for (int i=0; i<werewolves.size();i++)
            werewolves.at(i)->fully_heal();        
    }
    cout<<"All team members healed."<<endl;
    player->reduce_potions();
}

//Παύση του παιχνιδιού μέχρι ο παίκτης να συνεχίσει το παιχνίδι
bool Game::pause() 
{
    cout<<"Active vampires: "<<vampires.size()<<endl;
    cout<<"Active werewolves: "<<werewolves.size()<<endl;
    cout<<"Potions:"<<player->get_potions()<<endl;
    string key;
    cout << "Game is paused press p or P to continue or 0 to exit" << endl;
    cin>>key;
    while (key!="p" && key!="P" && key!="0")
    {
        cout << "Game is paused press p or P to continue or 0 to exit" << endl;
        cin>>key;
    }
    if (key == "0")
        return true;
    return false;
}


// Ελένχει την συμπεριφορά τον NPC
// Μετακινούνται αν υπάρχει κάποιο πιο δυνατό αντίπαλο npc με τυχαίο τρόπο
// Θεραπεύει με τυχαίο τρόπο αν το npc είναι της ίδιας ομάδας
// και προκαλούν damage 
void Game::npc_behavior()
{
    for (int i=0; i < vampires.size(); i++)
        vampires.at(i)->set_has_moved(false);

    for (int i=0; i < werewolves.size(); i++)
        werewolves.at(i)->set_has_moved(false);

    for (int y=0; y < map->get_size_y(); y++)
    {
        for (int x=0; x < map->get_size_x(); x++)
        {
            if (map->map[y][x]->get_entity() == NULL)
                continue;
            if (x == player->get_position_x() && y == player->get_position_y())
                continue;
            
            NPC* npc = (NPC*) (map->map[y][x]->get_entity());
            int start_y = npc->get_position_y()-1;
            int end_y = npc->get_position_y()+1;
            if (npc->get_position_y() == 0)
                start_y = 0;
            if (npc->get_position_y() == map->get_size_y()-1)
                end_y = map->get_size_y()-1;

            int start_x = npc->get_position_x()-1;
            int end_x = npc->get_position_x()+1;
            if (npc->get_position_x() == 0)
                start_x = 0;
            if (npc->get_position_x() == map->get_size_x()-1)
                end_x = map->get_size_x()-1;

            for (int i=start_y; i<=end_y; i++)
            {
                for (int j=start_x; j<=end_x; j++)
                {
                    if (map->map[i][j]->get_entity() == NULL)
                        continue;
                    if (i==player->get_position_y() && j==player->get_position_x())
                        continue;
                    if (i==y && j==x)
                        continue;
                    NPC* neighbor= (NPC*) (map->map[i][j]->get_entity());
                    if (neighbor->get_team() == npc->get_team())
                    {
                        npc->heal_other(neighbor, 1);
                        continue;
                    }
                    if (neighbor->get_power() > npc->get_power())
                        continue;

                    int moveaway=random_number(0, 0, i+j);  
                    if (moveaway==1)
                    {                    
                        int start_i=i-1;
                        int end_i=i+1;
                        if (i==0)
                            start_i=0;
                        if (i==map->get_size_y()-1)
                            end_i=map->get_size_y()-1;

                        int start_j=j-1;
                        int end_j=j+1;
                        if (j==0)
                            start_j=0;
                        if (j==map->get_size_x()-1)
                            end_j=map->get_size_x()-1;
                        
                        int rand_x=random_number(start_i, end_i, 17);
                        int rand_y=random_number(start_j, end_j, 23);
                        if (map->map[rand_y][rand_x]->get_access() && map->map[rand_y][rand_x]->get_entity() == NULL)
                        {
                            neighbor->move(rand_x, rand_y);
                            map->map[i][j]->set_entity(NULL);
                            map->map[rand_y][rand_x]->set_entity(neighbor);
                            neighbor->set_has_moved(true);
                        }
                    }
                    else 
                        npc->damage(neighbor);   // damage
                }
            }
        }
    }
}

//Κίνηση του χαρακτήρα ανάλογα με το move
void Game::movement(string move) 
{
    int pos_x = player->get_position_x();
    int pos_y = player->get_position_y();
    if (move == "w")
    {
        if (pos_y-1 < 0 ) 
            cout << "I cant move outside the map" << endl;
        else if (map->map[pos_y-1][pos_x]->get_entity() != NULL)
            cout << "I cant move there, there is an entity" << endl;
        else if (map->map[pos_y-1][pos_x]->get_access() )
        {
            player->move(pos_x, pos_y-1);
            map->map[pos_y-1][pos_x]->set_entity(player);
            map->map[pos_y][pos_x]->set_entity(NULL);
            if ( map->map[pos_y-1][pos_x]->get_has_potion()==true)
            {
                player->obtain_potion();
                map->map[pos_y-1][pos_x]->set_has_potion(false);
                cout<<"Potion obtained"<<endl;
            }
        }
        else
            cout << "This node is not accessible" << endl;
    }else if (move == "s")
    {
        if (pos_y+1 >= map->get_size_y() ) 
            cout << "I cant move outside the map" << endl;
        else if (map->map[pos_y+1][pos_x]->get_entity() != NULL)
            cout << "I cant move there, there is an entity" << endl;
        else if (map->map[pos_y+1][pos_x]->get_access() )
        {
            player->move(pos_x, pos_y+1);
            map->map[pos_y+1][pos_x]->set_entity(player);
            map->map[pos_y][pos_x]->set_entity(NULL);
            if ( map->map[pos_y+1][pos_x]->get_has_potion()==true)
            {
                player->obtain_potion();
                map->map[pos_y+1][pos_x]->set_has_potion(false);
                cout<<"Potion obtained"<<endl;
            }
        }
        else
            cout << "This node is not accessible" << endl;
    }else if (move == "d" || move == "D")
    {
        if (pos_x+1 >= map->get_size_x()) 
            cout << "I cant move outside the map" << endl;
        else if (map->map[pos_y][pos_x+1]->get_entity() != NULL)
            cout << "I cant move there, there is an entity" << endl;
        else if (map->map[pos_y][pos_x+1]->get_access() )
        {
            player->move(pos_x+1, pos_y);
            map->map[pos_y][pos_x+1]->set_entity(player);
            map->map[pos_y][pos_x]->set_entity(NULL);
            if ( map->map[pos_y][pos_x+1]->get_has_potion()==true)
            {
                player->obtain_potion();
                map->map[pos_y][pos_x+1]->set_has_potion(false);
                cout<<"Potion obtained"<<endl;
            }
        }
        else
            cout << "This node is not accessible" << endl;
    }
    else if (move == "a")
    {
        if (pos_x-1 < 0 ) 
            cout << "I cant move outside the map" << endl;
        else if (map->map[pos_y][pos_x-1]->get_entity() != NULL)
            cout << "I cant move there, there is an entity" << endl;
        else if (map->map[pos_y][pos_x-1]->get_access() )
        {
            player->move(pos_x-1, pos_y);
            map->map[pos_y][pos_x-1]->set_entity(player);
            map->map[pos_y][pos_x]->set_entity(NULL);
            if ( map->map[pos_y][pos_x-1]->get_has_potion()==true)
            {
                player->obtain_potion();
                map->map[pos_y][pos_x-1]->set_has_potion(false);
                cout<<"Potion obtained"<<endl;
            }
        }
        else 
            cout << "This node is not accessible" << endl;
    }
    else if (move == "q")
    {
        if (pos_x-1 < 0 || pos_y-1<0 ) 
            cout << "I cant move outside the map" << endl;
        else if (map->map[pos_y-1][pos_x-1]->get_entity() != NULL)
            cout << "I cant move there, there is an entity" << endl;
        else if (map->map[pos_y-1][pos_x-1]->get_access() )
        {
            player->move(pos_x-1, pos_y-1);
            map->map[pos_y-1][pos_x-1]->set_entity(player);
            map->map[pos_y][pos_x]->set_entity(NULL);
            if ( map->map[pos_y-1][pos_x-1]->get_has_potion()==true)
            {
                player->obtain_potion();
                map->map[pos_y-1][pos_x-1]->set_has_potion(false);
                cout<<"Potion obtained"<<endl;
            }
        }
        else
            cout << "This node is not accessible" << endl;
    }
    else if (move == "x")
    {
        if (pos_x+1 >= map->get_size_x() || pos_y+1 >= map->get_size_y() ) 
            cout << "I cant move outside the map" << endl;
        else if (map->map[pos_y+1][pos_x+1]->get_entity() != NULL)
            cout << "I cant move there, there is an entity" << endl;
        else if (map->map[pos_y+1][pos_x+1]->get_access() )
        {
            player->move(pos_x+1, pos_y+1);
            map->map[pos_y+1][pos_x+1]->set_entity(player);
            map->map[pos_y][pos_x]->set_entity(NULL);
            if ( map->map[pos_y+1][pos_x+1]->get_has_potion()==true)
            {
                player->obtain_potion();
                map->map[pos_y+1][pos_x+1]->set_has_potion(false);
                cout<<"Potion obtained"<<endl;
            }
        }
        else
            cout << "This node is not accessible" << endl;
    }
    else if (move == "e")
    {
        if (pos_x+1 >= map->get_size_x() || pos_y-1 <0) 
            cout << "I cant move outside the map" << endl;
        else if (map->map[pos_y-1][pos_x+1]->get_entity() != NULL)
            cout << "I cant move there, there is an entity" << endl;
        else if (map->map[pos_y-1][pos_x+1]->get_access() )
        {
            player->move(pos_x+1, pos_y-1);
            map->map[pos_y-1][pos_x+1]->set_entity(player);
            map->map[pos_y][pos_x]->set_entity(NULL);
            if ( map->map[pos_y-1][pos_x+1]->get_has_potion()==true)
            {
                player->obtain_potion();
                map->map[pos_y-1][pos_x+1]->set_has_potion(false);
                cout<<"Potion obtained"<<endl;
            }
        }
        else
            cout << "This node is not accessible" << endl;
    }
    else if (move == "z")
    {
        if (pos_x-1 < 0 || pos_y+1 >= map->get_size_y() ) 
            cout << "I cant move outside the map" << endl;
        else if (map->map[pos_y+1][pos_x-1]->get_entity() != NULL)
            cout << "I cant move there, there is an entity" << endl;
        else if (map->map[pos_y+1][pos_x-1]->get_access() )
        {
            player->move(pos_x-1, pos_y+1);
            map->map[pos_y+1][pos_x-1]->set_entity(player);
            map->map[pos_y][pos_x]->set_entity(NULL);
            if ( map->map[pos_y+1][pos_x-1]->get_has_potion()==true)
            {
                player->obtain_potion();
                map->map[pos_y+1][pos_x-1]->set_has_potion(false);
                cout<<"Potion obtained"<<endl;
            }
        }
        else
            cout << "This node is not accessible" << endl;
    }                    
}

//Npc movement
void Game::npc_movement() {
    for (int i=0; i < werewolves.size(); i++) {
        int move = random_number(0, 4, i);
        int pos_x = werewolves.at(i)->get_position_x();
        int pos_y = werewolves.at(i)->get_position_y();
        if (move == 1)
        {
            if (pos_y-1 < 0)
                return;
            if (map->map[pos_y-1][pos_x]->get_access()&&map->map[pos_y-1][pos_x]->get_entity()==NULL)
            {
                werewolves.at(i)->move(pos_x, pos_y-1);
                map->map[pos_y-1][pos_x]->set_entity(werewolves.at(i));
                map->map[pos_y][pos_x]->set_entity(NULL);
            }
        }   
        else if (move == 2)
        {
            if (pos_y+1 >= map->get_size_y())
                return;
            if (map->map[pos_y+1][pos_x]->get_access()&&map->map[pos_y+1][pos_x]->get_entity()==NULL)
            {
                werewolves.at(i)->move(pos_x, pos_y+1);
                map->map[pos_y+1][pos_x]->set_entity(werewolves.at(i));
                map->map[pos_y][pos_x]->set_entity(NULL);
            }
        }
        else if (move == 3)
        {
            if (pos_x+1>=map->get_size_x())
                return ;
            if (map->map[pos_y][pos_x+1]->get_access()&&map->map[pos_y][pos_x+1]->get_entity()==NULL)
            {
                werewolves.at(i)->move(pos_x+1, pos_y);
                map->map[pos_y][pos_x+1]->set_entity(werewolves.at(i));
                map->map[pos_y][pos_x]->set_entity(NULL);
            }
        }
        else if (move == 4)
        {
            if (pos_x-1 < 0)
                return;
            if (map->map[pos_y][pos_x-1]->get_access()&&map->map[pos_y][pos_x-1]->get_entity()==NULL)
            {
                werewolves.at(i)->move(pos_x-1, pos_y);
                map->map[pos_y][pos_x-1]->set_entity(werewolves.at(i));
                map->map[pos_y][pos_x]->set_entity(NULL);
            }
        }
    }

    for (int i=0; i < vampires.size(); i++) {
        int move = random_number(0, 8, i);
        int pos_x =  vampires.at(i)->get_position_x();
        int pos_y =  vampires.at(i)->get_position_y();
        if (move == 1)
        {
            if (pos_y-1 < 0)
                return;
            if (map->map[pos_y-1][pos_x]->get_access()&&map->map[pos_y-1][pos_x]->get_entity()==NULL)
            {
                vampires.at(i)->move(pos_x, pos_y-1);
                map->map[pos_y-1][pos_x]->set_entity(vampires.at(i));
                map->map[pos_y][pos_x]->set_entity(NULL);
            }
        }   
        else if (move == 2)
        {
            if (pos_y+1 >= map->get_size_y())
                return;
            if (map->map[pos_y+1][pos_x]->get_access()&&map->map[pos_y+1][pos_x]->get_entity()==NULL )
            {
                vampires.at(i)->move(pos_x, pos_y+1);
                map->map[pos_y+1][pos_x]->set_entity(vampires.at(i));
                map->map[pos_y][pos_x]->set_entity(NULL);
            }
        }
        else if (move == 3)
        {
            if (pos_x+1>=map->get_size_x())
                return;
            if (map->map[pos_y][pos_x+1]->get_access()&&map->map[pos_y][pos_x+1]->get_entity()==NULL )
            {
                vampires.at(i)->move(pos_x+1, pos_y);
                map->map[pos_y][pos_x+1]->set_entity(vampires.at(i));
                map->map[pos_y][pos_x]->set_entity(NULL);
            }
        }
        else if (move == 4)
        {
            if (pos_x-1 <0)
                return;
            if (map->map[pos_y][pos_x-1]->get_access()&&map->map[pos_y][pos_x-1]->get_entity()==NULL )
            {
                vampires.at(i)->move(pos_x-1, pos_y);
                map->map[pos_y][pos_x-1]->set_entity(vampires.at(i));
                map->map[pos_y][pos_x]->set_entity(NULL);
            }
        }
        else if (move == 5)
        {
            if (pos_x-1 < 0 || pos_y-1<0 ) 
                return;
            if (map->map[pos_y-1][pos_x-1]->get_access()&&map->map[pos_y-1][pos_x-1]->get_entity()==NULL && vampires.at(i)->get_has_moved()==false)
            {
                vampires.at(i)->move(pos_x-1, pos_y-1);
                map->map[pos_y-1][pos_x-1]->set_entity(vampires.at(i));
                map->map[pos_y][pos_x]->set_entity(NULL);
            }
        }
        else if (move == 6)
        {
            if (pos_x+1 >= map->get_size_x() || pos_y+1 >= map->get_size_y() ) 
                return;
            if (map->map[pos_y+1][pos_x+1]->get_access()&&map->map[pos_y+1][pos_x+1]->get_entity()==NULL )
            {
                vampires.at(i)->move(pos_x+1, pos_y+1);
                map->map[pos_y+1][pos_x+1]->set_entity(vampires.at(i));
                map->map[pos_y][pos_x]->set_entity(NULL);
            }
        }
        else if (move == 7)
        {
            if (pos_x+1 >= map->get_size_x() || pos_y-1 <0) 
                return;
            if (map->map[pos_y-1][pos_x+1]->get_access()&&map->map[pos_y-1][pos_x+1]->get_entity()==NULL )
            {
                vampires.at(i)->move(pos_x+1, pos_y-1);
                map->map[pos_y-1][pos_x+1]->set_entity(vampires.at(i));
                map->map[pos_y][pos_x]->set_entity(NULL);
            }
        }
        else if (move == 8)
        {
            if (pos_x-1 < 0 || pos_y+1 >= map->get_size_y()) 
                return;
            if (map->map[pos_y+1][pos_x-1]->get_access()&&map->map[pos_y+1][pos_x-1]->get_entity()==NULL )
            {
                vampires.at(i)->move(pos_x-1, pos_y+1);
                map->map[pos_y+1][pos_x-1]->set_entity(vampires.at(i));
                map->map[pos_y][pos_x]->set_entity(NULL);
            }
        }    
    }
}



//Constructor Game
Game::Game():map(NULL), player(NULL)  {}

//Destructor Map και δίνει την εντολή να διαγραφτούν όλα τα υπόλοιπα στοιχεία του χάρτη
Game::~Game()
{
    delete map;
    delete player;

    for (int i=0; i < vampires.size(); i++)
        delete vampires[i];
    vampires.clear();
    for (int i=0; i < werewolves.size(); i++)
        delete werewolves[i];
    werewolves.clear();

}
