#include <iostream>
#include <cstdlib>
#include <string>
#include <time.h>
#include "../include/functions.h"
#include "../include/map.h"
#include "../include/npc.h"

using namespace std;



//Random number between two numbers and the third is random seed
int random_number(int a, int b, int seed)
{
	int new_rand;
	srand((unsigned)time(NULL)+seed);   //Αρχικοποιούμε γεννήτρια τυχαίων αριθμών με τυχαίο seed
	if (a == b)     //Τυχαίος ακέραιος ανάμεσα στο α και το α είναι μόνο το α
		return a;
	else if (a>b)
		new_rand = rand() % (a - b + 1) + b;      //Και στις δύο περιπτώσεις η παράσταση είναι ανάμεσα στα α και β
	else
		new_rand = rand() % (b - a + 1) + a;
	
	return new_rand;
}

//Ελέγχει αν είναι αριθμός και επιστρέφει true ή false
bool is_number(string s)
{
	int i;
	for (i=0;i<s.length();i++)
		if (isdigit(s[i])==0)   //Αν υπάρχει χαρακτήρας που δεν είναι ψηφίο επιστρέφουμε false
			return false;
	
	return true;  //Αλλιώς επιστρέφουμε true
}

//Επιστρέφει το width, height που θέλουμε για τον πίνακα
void map_dimensions(int* width, int* height)
{
	string w="0", h="0";
	while (true)
	{
		cout << "Please input map width (1-100, 0 for exit): ";   //Εισαγωγή πλάτους με έλεγχο εγκυρότητας
		cin>>w;
		if (is_number(w))
			if (stoi(w)>=0 && stoi (w)<100)
				break;
			else
				cout << "Number is too short <0 or too high >100 input again " << endl;
		else
			cout << w << " is not a number input again" << endl;
	}
	if (stoi(w)!=0)
	{
		while (true)
		{
			cout << "Please input map height (1-100, 0 for exit): ";     
			cin>>h;
			if (is_number(h))
				if (stoi(h)>=0 && stoi (h)<100)    //Εισαγωγή ύψους με έλεγχο εγκυρότητας
					break;
				else
					cout << "Number is too short <0 or too high >100 input again " << endl;
			else
				cout << h << " is not a number input again" << endl;
		}
	}
	*width=stoi(w);     //Επιστροφή δύο αριθμών στις θέσεις μνήμης των w και h
	*height=stoi(h);
}