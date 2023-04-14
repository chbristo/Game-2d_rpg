#pragma once
#include "entity.h"
#include <string>

class Entity;

// Ο γενικός τύπος των map element
class Node 
{
    Entity* entity;  //Αν υπαρχεί κάποιο entity σε αυτο το node αν όχι θα έχει την τιμή NULL
    bool healthy;    //Βοηθητική μεταβλητη για να ελένχει αν ο map λειτουργεί σωστά
    bool has_potion;
    public:
        Node();
        virtual ~Node() = 0;
        
        //Οι vistual συναρτήσεις είναι υλοποιημένες στις από κάτω κλάσεις
        virtual	bool get_access() const = 0;
        virtual std::string get_char() const = 0;
        
        void set_entity(Entity*);
        Entity* get_entity()    const;

        void set_healthy(bool );   //Βοηθητικές συναρτήσεις για το map
        int get_healthy()   const;

        void set_has_potion(bool);
        bool get_has_potion() const;
};


// Οι παρακάτω classes είναι κενές γιατί οι πληροφορίες βρίσκονται στις συναρτήσεις
// και οι πληροφορίες είναι πάντα σταθερές

// Δεντρό
class Trees: public Node
{
public:
	Trees();
	~Trees();
    bool get_access()   const;
    std::string get_char()  const;
};

//Νερο
class Water: public Node
{
public:
	Water();
	~Water();
    bool get_access()   const;
    std::string get_char()  const;
};

//Γη
class Earth: public Node
{
public:
	Earth();
	~Earth();
    bool get_access()   const;
    std::string get_char()  const;
};

