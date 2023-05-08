// Types and functions for Phone
// ECE 209 - Fall 2020, Program 4
// G. Byrd

// Because of cyclic dependency between Phone and Tower types,
// both are defined in this single header file
// Implementations are separate, in Phone.c and Tower.c

#ifndef PHONE_TOWER_H
#define PHONE_TOWER_H

// Phone is an abstract data type representing a mobile phone
// struct phone is defined only in Phone.c -- that's what makes it abstract
typedef struct phone * Phone;
// PhoneNode is a (non-abstract) data type used to build a list of Phones
typedef struct phoneNode * PhoneNode;

// struct phoneNode is a node in a linked list of phones
// Each node has a data field (which is a Phone -- pointer to struct phone)
// and a next field, which is a pointer to the next node in the list.
struct phoneNode {
    Phone phone;      // data
    PhoneNode next;   // ptr to next node (could be NULL)
};


// Tower is an abstract data type that represents a cell phone tower
// struct tower is defined only in Tower.c -- that's what makes it abstract
typedef struct tower * Tower;
// TowerNode is a (non-abstract) data type used to implement a list of Towers
typedef struct towerNode * TowerNode;

// struct towerNode is a node in a linked list of towers
// Each node has a data field (which is a Tower -- pointer to struct tower)
// and a next field, which is a pointer to the next node in the list.
struct towerNode {
    Tower tower;     // data
    TowerNode next;  // ptr to next node (may be NULL)
};


// ----------------------------------- Phone functions

// allocate and return a new Phone with specified ID and location
Phone createPhone(const char *id, int x, int y);

// given a Phone, return a pointer to its ID string
const char * phoneID(Phone p);

// given a Phone, return its x and y location using the provided pointers
void phoneLocation(Phone p, int *xaddr, int *yaddr);

// given a Phone, return the Tower to which it is currently connected
Tower phoneTower(Phone p);

// find a Phone by its ID string
Phone findPhone(const char *id);

// move existing phone to a new location
void movePhone(Phone p, int newx, int newy);

// ----------------------------------- Phone list functions

// add phone to a list (which may be empty), in alphabetical order
// returns updated list
PhoneNode insertPhone(PhoneNode list, Phone p);

// remove phone from a list (which may be empty)
// deallocate node (but not the phone)
// returns updated list (which may be empty)
PhoneNode deletePhone(PhoneNode list, Phone p);

// ----------------------------------- Tower functions

// allocate a return a new Tower with the specified ID and location
Tower createTower(const char *id, int x, int y);

// given a Tower, return a pointer to its ID string
const char * towerID(Tower t);

// given a Tower, return its location using the pointers provided
void towerLocation(Tower t, int *xaddr, int *yaddr);

// create a connection between two Towers
// t2 will be added to t1's neighbor list
// t1 will be added to t2's neighbor list
void addConnection(Tower t1, Tower t2);

// given a Tower, return a list of Towers connected to that Tower
// list will not be altered or deallocated by caller
TowerNode towerNeighbors(Tower t);

// inform Tower t that Phone p is now connected
void addPhone(Tower t, Phone p);
// inform Tower t that Phone o is no longer connected
void removePhone(Tower t, Phone p);

// given a Tower, return a list of Phones connected to that Tower
// list will not be altered or deallocated by caller
PhoneNode towerPhones(Tower t);

// return the null Tower, allocate if necessary
Tower nullTower();

// return the Tower with matching ID (or NULL if not found)
Tower findTower(const char *id);

// return a list of all towers, in alphabetical order
// list will not be altered or deallocated by caller
TowerNode allTowers();

// routes a call between two phones, start to end
// returns a list of Towers required to connect the call, in routing order
// list may be altered or deallocated by caller
TowerNode routeCall(Phone start, Phone end);

#endif