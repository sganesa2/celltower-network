//
// Created by srini on 02/11/2020.
//
#include"PhoneTower.h"
#include <stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>

//struct phone to hold details of each phone
struct phone
{
    char phoneid[30];
    Tower connectedTower;
    int phoneX,phoneY;
};

// allocate and return a new Phone with specified ID and location
Phone createPhone(const char *id, int x, int y)
{
    Phone new_phone;
    new_phone = (Phone) malloc(sizeof(*new_phone));
    if((x<0)||(y<0)) //negative values cannot be used
    {
        return NULL;
    }
    // initialize details of the new phone
    strcpy(new_phone->phoneid,id);
    new_phone->phoneX = x;
    new_phone->phoneY = y;
    new_phone->connectedTower=NULL;
    TowerNode h;
    h = allTowers(); //get the list of all towers except null tower to help find out the tower with which the phone should be associated
    int towerX; //to hold the x coordinate of each tower
    int towerY; //to hold the y coordinate of each tower
    int a,b,diffX,diffY; // a and b are duplicates of towerX and towerY
    double phone_towDist, min; //min will hold the smallest distance between the tower and the phone
    char phone_towName[30]; //this array is to hold the name of each tower
    if(h==NULL) //if there are no towers created yet, just associate the phone with the nullTower
    {
        addPhone(nullTower(),new_phone);
        new_phone->connectedTower = nullTower();
        return new_phone;
    }
    TowerNode p;
    p=h;
    towerLocation(p->tower,&towerX,&towerY); //initialize values of towerX and towerY
    a = towerX; //set a and b equal to towerX and towerY
    b = towerY;
    //diffX and diffY help us compute the distance between two points
    diffX = abs(a-(new_phone->phoneX))*abs(a-(new_phone->phoneX));
    diffY = abs(b-(new_phone->phoneY))*abs(b-(new_phone->phoneY));
    min = sqrt(diffY+diffX); //min is the smallest distance between the tower and the phone
    char minTowName[30]; //to hold the name of the closest tower
    strcpy(minTowName,towerID(p->tower));
    Tower minTowerDist; //the tower that is closest to the phone
    minTowerDist=p->tower; //right now the closest tower is the first tower in the allTowers() list
    p=p->next; //move on to the next tower to compare its closeness with the phone with min
    while(p)
    {
        towerLocation(p->tower,&towerX,&towerY);
        a = towerX;
        b = towerY;
        diffX = abs(a-(new_phone->phoneX))*abs(a-(new_phone->phoneX));
        diffY = abs(b-(new_phone->phoneY))*abs(b-(new_phone->phoneY));
        phone_towDist = sqrt(diffY+diffX);
        strcpy(phone_towName,towerID(p->tower));
        if(min>phone_towDist) //change min if its not the smallest distance
        {
            min=phone_towDist;
            minTowerDist = p->tower; // change the closest tower to the new tower
        }
        else if(min==phone_towDist) //if the distances are equal compare based on names(alphabetical order)
        {
            if(strcasecmp(minTowName,phone_towName)>0)
            {
                strcpy(minTowName,phone_towName);
                minTowerDist = p->tower;
            }
        }
        p=p->next;
    }
    if(min<3) //if min is within the range of the phone, add the phone to the tower's list of phones, and also change the phone's associated tower
    {
        addPhone(minTowerDist,new_phone);
        new_phone->connectedTower=minTowerDist;
    }
    else //if its not within range associate phone with null tower
    {
        new_phone->connectedTower = nullTower();
        addPhone(new_phone->connectedTower,new_phone);
    }
    return new_phone;
}

// given a Phone, return a pointer to its ID string
const char * phoneID(Phone p)
{
    return p->phoneid;
}

// given a Phone, return its x and y location using the provided pointers
void phoneLocation(Phone p, int *xaddr, int *yaddr)
{
    *xaddr = p->phoneX;
    *yaddr = p->phoneY;
}

// given a Phone, return the Tower to which it is currently connected
Tower phoneTower(Phone p)
{
    if(p!=NULL) //proceed only if the phone even exists
    {
        return p->connectedTower;
    }
    else
    {
        return NULL;
    }
}

// find a Phone by its ID string
Phone findPhone(const char *id)
{
    TowerNode findTow;
    TowerNode p;
    findTow = allTowers(); //get list of all towers except null tower
    Phone foundPhone=NULL; //foundPhone is the phone to be returned
    if(findTow==NULL) //if no towers have been created yet, check null tower's phone list
    {
        PhoneNode head;
        head = towerPhones(nullTower());
        if(head==NULL) //if null tower also doesn't have any phones then just return NULL
        {
            return NULL;
        }
        else
        {
            PhoneNode pho;
            pho = head;
            while((pho)&&(strcasecmp(pho->phone->phoneid,id)!=0)) //loop to find out the phone within null tower's list of phones
            {
                pho=pho->next;
            }
            if(pho!=NULL)
            {
                return pho->phone;
            }
            else
            {
                return NULL;
            }
        }
    }
    p=findTow;
    while(p)
    {
        PhoneNode i;
        i=towerPhones(p->tower); //list of phones associated with the particular tower
        if(i!=NULL)
        {
            while((i!=NULL)&&(strcasecmp(phoneID(i->phone),id)!=0)) //look for the particular phone based on its id
            {
                i=i->next;
            }
            if(i!=NULL)
            {
                foundPhone = i->phone;
                break;
            }
        }
        p=p->next;
    }
    if(foundPhone!=NULL) //if the ID doesn't exist
    {
        return foundPhone;
    }
    else //if the phone isn't in the allTowers() list, look for it in nullTower's list of phones
    {
        PhoneNode h;
        h = towerPhones(nullTower());
        if(h==NULL)
        {
            return NULL;
        }
        else
        {
            PhoneNode pho1;
            pho1 = h;
            while((pho1)&&(strcasecmp(pho1->phone->phoneid,id)!=0))
            {
                pho1=pho1->next;
            }
            if(pho1!=NULL)
            {
                return pho1->phone;
            }
            else
            {
                return NULL;
            }
        }
    }
}

// move existing phone to a new location
void movePhone(Phone p, int newx, int newy)
{
    p->phoneX = newx; //change the phone's x and y coordinates to their new values
    p->phoneY = newy;
    TowerNode h;
    h = allTowers();
    if(h!=NULL) //very similar to createPhone() please look at that for specific comments
    {
        int towerX;
        int towerY;
        int a, b, diffX, diffY;
        double phone_towDist, min;
        char phone_towName[30];
        TowerNode i;
        i = h;
        towerLocation(i->tower, &towerX, &towerY);
        a = towerX;
        b = towerY;
        diffX = abs(a - (p->phoneX)) * abs(a - (p->phoneX));
        diffY = abs(b - (p->phoneY)) * abs(b - (p->phoneY));
        min = sqrt(diffY + diffX);
        char minTowName[30];
        strcpy(minTowName, towerID(i->tower));
        Tower minTowerDist;
        minTowerDist = i->tower;
        i = i->next;
        while (i) {
            towerLocation(i->tower, &towerX, &towerY);
            a = towerX;
            b = towerY;
            diffX = abs(a - (p->phoneX)) * abs(a - (p->phoneX));
            diffY = abs(b - (p->phoneY)) * abs(b - (p->phoneY));
            phone_towDist = sqrt(diffY + diffX);
            strcpy(phone_towName, towerID(i->tower));
            if (min > phone_towDist)
            {
                min = phone_towDist;
                minTowerDist = i->tower;
            }
            else if (min == phone_towDist)
            {
                if (strcasecmp(minTowName, phone_towName) > 0)
                {
                    strcpy(minTowName, towerID(i->tower));
                    minTowerDist = i->tower;
                }
            }
            i = i->next;
        }
        if ((minTowerDist != p->connectedTower) && (min < 3)) //if the closest tower found above is within range, but is not the tower associated with phone p
        {
            removePhone(p->connectedTower, p); //remove the phone form its associated tower
            p->connectedTower = minTowerDist;
            addPhone(p->connectedTower, p); //add the phone to the new closest tower
        }
        else if ((minTowerDist != p->connectedTower) && (min >= 3))//if min tower found doesn't lie within range, then connect phone p to null tower
        {
            removePhone(p->connectedTower, p);
            p->connectedTower = nullTower();
            addPhone(p->connectedTower, p);
        }
    }
}

// add phone to a list (which may be empty), in alphabetical order
// returns updated list
PhoneNode insertPhone(PhoneNode list, Phone p)
{
    PhoneNode new; //new phone node to be added
    PhoneNode i,prev;
    prev=NULL;
    new = (PhoneNode) malloc(sizeof(*new));
    //initialize its fields with the details of Phone p
    new->phone = p;
    new->next = NULL;
    if(list==NULL) //if the list of phones is empty, just change the head of the list to new
    {
        list = new;
        return list;
    }
    i=list;
    while((i)&&(strcasecmp(i->phone->phoneid,p->phoneid)<0)) //insert phone in alphabetical order
    {
        prev=i;
        i=i->next;
    }
    if(prev==NULL)
    {
        list = new;
    }
    else
    {
        prev->next = new;
    }
    new->next = i;
    return list;
}