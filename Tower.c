//
// Created by srini on 02/11/2020.
//
#include"PhoneTower.h"
#include <stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>

TowerNode routeInternal(Tower t, Phone p, Tower prev); //function declaration for routeInternal() function. This will be used by routeCall() function
static TowerNode retHead = {NULL}; // head of the route list connecting start Phone to end Phone. This will be used by routeCall() and routeInternal()

//struct tower to hold the details of each tower.
struct tower
{
    char towerName[30];
    int xval,yval;
    TowerNode neighbourList; // head to the list of neighbours of a particular tower
    PhoneNode phoneHead; // head to list of phones associated with the tower
};

static struct tower Tower_null = {"None",0,0,NULL,NULL}; //statically allocated a null tower and initialized its details
static TowerNode towerHead=NULL; // statically allocated a head to the list of towers

//nullTower() to return the header to the statically allocated null tower struct
Tower nullTower()
{
    return &Tower_null;
}

// return a list of all towers, in alphabetical order
// list will not be altered or deallocated by caller
TowerNode allTowers()
{
    return towerHead;
}

// allocate a return a new Tower with the specified ID and location
Tower createTower(const char *id, int x, int y)
{
    if((x<0)||(y<0)) //look out for negative coordinates
    {
        return NULL;
    }

    Tower tow; //  malloc the new tower to be added
    tow = (Tower) malloc(sizeof(*tow));
    //initialize its fields
    strcpy(tow->towerName,id);
    tow->xval = x;
    tow->yval = y;
    tow->neighbourList = NULL;
    tow->phoneHead =NULL;
    TowerNode towNode; // the new tower node to be added to the list
    towNode= (TowerNode) malloc(sizeof(*towNode));
    towNode->tower = tow;
    towNode->next = NULL;
    if(towerHead==NULL) //if the list is empty just change the head to new node
    {
        towerHead = towNode;
        towNode->next = NULL;
        return tow;
    }
    TowerNode p, prev;
    p= towerHead;
    prev=NULL;
    while((p!=NULL)&&(strcasecmp(towNode->tower->towerName,p->tower->towerName)>0)) // adding nodes in alphabetical order while inserting
    {
        prev=p;
        p=p->next;
    }
    if(prev==NULL)
    {
        towerHead = towNode;
    }
    else
    {
        prev->next = towNode;
    }
    towNode->next=p;
    return tow;
}

// given a Tower, return a pointer to its ID string
const char * towerID(Tower t)
{
    return t->towerName;
}

// given a Tower, return its location using the pointers provided
void towerLocation(Tower t, int *xaddr, int *yaddr)
{
    *xaddr = (t->xval);
    *yaddr= (t->yval);
}

// create a connection between two Towers
// t2 will be added to t1's neighbor list
// t1 will be added to t2's neighbor list
void addConnection(Tower t1, Tower t2)
{
    //add a connection only if these two towers even exist in alltowers() or as nullTower()
    if((t1!=NULL)&&(t2!=NULL)) {
        TowerNode p1;
        TowerNode p2;
        p1 = t1->neighbourList; //take in head of neighbour list of both the towers t1 and t2
        p2 = t2->neighbourList;
        while ((p1 != NULL) && (strcasecmp(p1->tower->towerName, t2->towerName) != 0))  //check if the towers are already neighbours
        {
            p1 = p1->next;
        }
        while ((p2 != NULL) && (strcasecmp(p2->tower->towerName, t1->towerName) != 0))
        {
            p2 = p2->next;
        }
        if (p1 == NULL) // if they are not neighbours, add t2 to t1's neighbour list
        {
            TowerNode new;
            TowerNode p;
            TowerNode prev;
            new = (TowerNode) malloc(sizeof(*new)); // new neighbour to be added
            //initialize its fields with details of t2
            new->tower= t2;
            new->next = NULL;
            if (t1->neighbourList == NULL) // add t2 as first item if the neighbour list for t1 is empty
            {
                t1->neighbourList = new;
            }
            else
            {
                p = t1->neighbourList;
                prev = NULL;
                while ((p != NULL) && (strcasecmp(new->tower->towerName, p->tower->towerName) > 0))  // if t1's neighbour list is not empty, push t2 as a neighbour into the list in alphabetical order
                {
                    prev = p;
                    p = p->next;
                }
                if (prev == NULL)
                {
                    t1->neighbourList = new;
                }
                else
                {
                    prev->next = new;
                }
                new->next = p;
            }
        }
        if (p2 == NULL) //same as previous case but now we are looking at t2
        {
            TowerNode new1;
            TowerNode i;
            TowerNode iprev;
            new1 = (TowerNode) malloc(sizeof(*new1));
            new1->tower= t1;
            new1->next = NULL;
            if (t2->neighbourList == NULL)
            {
                t2->neighbourList = new1;
            }
            else
            {
                i = t2->neighbourList;
                iprev = NULL;
                while ((i != NULL) && (strcasecmp(new1->tower->towerName, i->tower->towerName) > 0))
                {
                    iprev = i;
                    i = i->next;
                }
                if (iprev == NULL)
                {
                    t2->neighbourList = new1;
                }
                else
                {
                    iprev->next = new1;
                }
                new1->next = i;
            }
        }
    }
}

// given a Tower, return a list of Towers connected to that Tower
// list will not be altered or deallocated by caller
TowerNode towerNeighbors(Tower t)
{
    return t->neighbourList;
}

// inform Tower t that Phone p is now connected
void addPhone(Tower t, Phone p)
{
    if((t!=NULL)&&(p!=NULL)) // only works if both the tower and the phone even exist
    {
        PhoneNode h;
        h = insertPhone(t->phoneHead, p); // insertPhone function will insert the Phone p into tower t's list of phones
        t->phoneHead = h;
    }
}

// inform Tower t that Phone o is no longer connected
void removePhone(Tower t, Phone p)
{
    if((t!=NULL)&&(p!=NULL)) // only works if both the tower and the phone even exist
    {
        if(t->phoneHead!=NULL) // if the list of phones is not empty, only then will you be able to remove the phone from the list
        {
            PhoneNode i, prev;
            prev = NULL;
            i = t->phoneHead;
            char str[30];
            strcpy(str, phoneID(p));
            while ((i != NULL) && (strcasecmp(phoneID(i->phone), str) != 0)) // condition that helps us find the exact location of the Phone p in the list
            {
                prev = i;
                i = i->next;
            }
            if (i != NULL) //if i is not null, we actually found a match for the Phone p in the list of phones of Tower t
            {
                if (prev != NULL) //if prev is not null, the phone was found at the end or in between the list
                {
                    prev->next = i->next;
                }
                else // else it was found at start; so just change the head of the list to the next node
                {
                    t->phoneHead = i->next;
                }
            }
        }
    }
}

// given a Tower, return a list of Phones connected to that Tower
// list will not be altered or deallocated by caller
PhoneNode towerPhones(Tower t)
{
    if(t!=NULL) // proceed only if the tower t even exists
    {
        return t->phoneHead;
    }
    else if(strcasecmp(towerID(t),"None")==0) //check if its in the nullTower's list of phones
    {
        return nullTower()->phoneHead;
    }
    return NULL;
}

// return the Tower with matching ID (or NULL if not found)
Tower findTower(const char *id)
{
    TowerNode head;
    head = allTowers(); //get list of all the towers, except nullTower
    if(strcasecmp(id,"None")==0) //if the id is "None" then it is the null tower
    {
        return nullTower();
    }
    else
    {
        TowerNode p;
        if(head==NULL) //if no towers have been added yet
        {
            return NULL;
        }
        p=head;
        while((p!=NULL)&&(strcasecmp(towerID(p->tower),id)!=0)) //condition to find out where that particular tower is present in the list of towers
        {
            p=p->next;
        }
        if(p!=NULL) //if p is not null, the tower was present in the list
        {
            return p->tower;
        }
        else
        {
            return NULL;
        }
    }
}

// routes a call between two phones, start to end
// returns a list of Towers required to connect the call, in routing order
// list may be altered or deallocated by caller
TowerNode routeCall(Phone start, Phone end)
{
    Tower S;
    if((start==NULL)||(end==NULL)) //proceed only if the start and end phones even exist
    {
        return  NULL;
    }
    if((phoneTower(start)==NULL)||(phoneTower(end)==NULL)) //proceed only if the start and end phones even have a tower associated with them
    {
        return NULL;
    }
    S = phoneTower(start); //start tower
    TowerNode routeHead=NULL; //this will be the head of the route list returned by routeInternal
    routeHead = routeInternal(S,end,NULL);
    return routeHead;
}

//The routeInternal implementation uses the
//following algorithm to find a route from starting tower t to a receiving phone p, as requested by tower prev
TowerNode routeInternal(Tower t, Phone p, Tower prev)
{
    if((prev==NULL)&&(strcasecmp(phoneTower(p)->towerName,t->towerName)==0)) //first case: if start tower is the tower associated with both start and end phone
    {
        TowerNode new;
        new = (TowerNode) malloc(sizeof(*new));
        new->tower = t;
        new->next = NULL;
        retHead = new;
        return retHead;
    }
    else if ((prev==NULL)&&(strcasecmp(phoneTower(p)->towerName,t->towerName)!=0)) //second case: if we are looking at the start tower but it is not the tower associated with end phone
    {
        TowerNode correctNeighbour,i;
        correctNeighbour = towerNeighbors(t); //correctNeighbour is the head of the list of neighbours of tower t
        i=correctNeighbour;
        if(i==NULL) //if no neighbours for the start tower itself
        {
            return NULL;
        }
        Tower R; // R will be the neighbour that the previus tower calls on, to find if it is associated with end phone
        while(i)
        {
            R = i->tower;
            retHead = routeInternal(R,p,t); //retHead will tell us if the tower R, or one of its neighbours is associated with end phone
            if(retHead!=NULL) //if its not null we were able to find a route from start to end phone
            {
                //code to prepend tower t to the route
                TowerNode new,x;
                x=retHead;
                new = (TowerNode) malloc(sizeof(*new));
                new->tower = t;
                new->next = x;
                retHead = new;
                return retHead;
            }
            i=i->next;
        }
        return retHead;
    }
    else if((prev!=NULL)&&(strcasecmp(phoneTower(p)->towerName,t->towerName)==0)) //third case: if we are looking at a tower other than the start tower that is associated with the end phone
    {
        //code to prepend tower t to the route
        TowerNode new;
        new = (TowerNode) malloc(sizeof(*new));
        new->tower = t;
        new->next = NULL;
        retHead = new;
        return retHead;
    }
    else if((prev!=NULL)&&(strcasecmp(phoneTower(p)->towerName,t->towerName)!=0)) //final case: if we are looking at a tower other than start tower, but it hasn't found a route to the end phone yet
    {
        TowerNode correctNeighbour,i;
        correctNeighbour = towerNeighbors(t); //get the list of neighbours for this tower
        i=correctNeighbour;
        int count =0; //count will help us find out the prev tower and exclude it from the search
        while((i!=NULL)&&(i->tower!=prev)) //loop to find out the prev tower
        {
            count++;
            i=i->next;
        }
        Tower R;
        i=correctNeighbour;
        int newCount =0; //newCount will help us exclude the prev tower, when it is incremented to the value of the 'count' variable
        while(i)
        {
            if(newCount==count) //exclude prev tower and move to next tower in the list of neighbours
            {
                i=i->next;
                if(i==NULL)
                {
                    return NULL;
                }
            }
            R = i->tower;
            retHead = routeInternal(R,p,t); //find out the route from neighbour tower R to end phone
            if(retHead!=NULL) // :if we found a route
            {
                TowerNode new,x;
                x=retHead;
                new = (TowerNode) malloc(sizeof(*new));
                new->tower = t;
                new->next = x;
                retHead = new;
                return retHead;
            }
            newCount++; //increment newCount until it reaches count
            i=i->next;
        }
        return retHead;
    }
    return retHead;
}