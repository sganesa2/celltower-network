// ECE 209 - Fall 2020, Program 4
// Simulation of a cell phone network
// A set of Towers is connected to each other to route calls
// A set of Phones is connected to each Tower

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "PhoneTower.h"

/* helper functions */
int getTowers(FILE*);
int getPhones(FILE*);
void purgeInput();
void printTowers();
void printPhones(PhoneNode);
void call(Phone, Phone);

// User Interface
// (1) reads tower and phone information from files
// (2) allows user to retrieve information about system, and to route calls
int main(int argc, char **argv) {
    FILE *phoneFile = NULL;
    FILE *towerFile = NULL;
    char filename[25];
    char cmd[8], id1[13], id2[13];
    Tower t;
    Phone p1, p2;
    int x, y;

    if (argc > 1) {
        if (argc != 3) {
            printf("Arguments: <tower_file> <phone_file>\n");
            return EXIT_FAILURE;
        }
        printf("Tower file: %s\n", argv[1]);
        towerFile = fopen(argv[1], "r");
        if (!towerFile) {
            printf("Cannot open file: %s\n", argv[1]);
            return EXIT_FAILURE;
        }
        printf("Phone file: %s\n", argv[2]);
        phoneFile = fopen(argv[2], "r");
        if (!phoneFile) {
            printf("Cannot open file: %s\n", argv[2]);
            return EXIT_FAILURE;
        }
    }
    else {
        // read information about towers
        while (!towerFile) {
            printf("Tower file: ");
            fflush(stdout);
            scanf("%24s", filename);
            // read and ignore the rest of the line
            scanf("%*[^\n]");
            towerFile = fopen(filename, "r");
            if (!towerFile) {
                printf("Cannot open file: %s\n", filename);
            }
        }

        // read information about phones
        while (!phoneFile) {
            printf("Phone file: ");
            fflush(stdout);
            scanf("%24s", filename);
            // read and ignore the rest of the line
            scanf("%*[^\n]");
            phoneFile = fopen(filename, "r");
            if (!phoneFile) {
                printf("Cannot open file: %s\n", filename);
            }
        }
    }
    
    /* Read towers and connections from file, add to system */
    if (!getTowers(towerFile)) {
        return EXIT_FAILURE;
    }
    fclose(towerFile);
    
    /* Read phones from file, add to system */
    if (!getPhones(phoneFile)) {
        return EXIT_FAILURE;
    }
    fclose(phoneFile);
    
    /* Command loop -- get command from user and execute */
    while (1) {
        printf("? "); fflush(stdout);
        scanf("%7s", cmd);
        if (strcmp(cmd, "quit") == 0) break;
        else if (strcmp(cmd, "towers") == 0) {
            printTowers();
        }
        else if (strcmp(cmd, "phones") == 0) {
            /* read tower name */
            scanf("%12s", id1);
            t = findTower(id1);
            if (t == NULL) {
                printf("Unable to find tower: %s\n", id1);
            }
            else printPhones(towerPhones(t));
        }
        else if (strcmp(cmd, "call") == 0) {
            /* read phone id's */
            scanf("%12s %12s", id1, id2);
            p1 = findPhone(id1);
            if (p1 == NULL) {
                printf("Unable to find phone: %s\n", id1);
            }
            p2 = findPhone(id2);
            if (p2 == NULL) {
                printf("Unable to find phone: %s\n", id2);
            }
            if (p1 && p2) call(p1, p2);
        }
        else if (strcmp(cmd, "move") == 0) {
            /* read phone id and new location */
            x = y = -1;
            scanf("%12s %d %d", id1, &x, &y);
            p1 = findPhone(id1);
            if (p1 == NULL) {
                printf("Unable to find phone: %s\n", id1);
            }
            if ((x >= 0) && (y >= 0)) {
                if (p1) movePhone(p1, x, y);
            }
            else {
                printf("Illegal location (%d,%d)\n", x, y);
            }
        }
        else {
            printf("Unknown command.\n");
        }
        purgeInput();  /* get rid of any extra stuff on the line */
    }
    

    return EXIT_SUCCESS;
}

// Read tower info from file.
// Two kinds of formatted lines:
//    tower <id> <x> <y>
//    conn <id1> <id2>
// Returns 1 if successful, 0 otherwise.
int getTowers(FILE *towerFile) {
    Tower t1, t2;
    char name1[13], name2[13], type[6];
    int xLoc, yLoc;
    
    t1 = nullTower();
    if (t1 == NULL) {
        printf("Error creating null tower.\n");
        return 0;
    }
    while (fscanf(towerFile, "%5s", type) != EOF) {
        if (strcmp(type, "tower") == 0) {
            fscanf(towerFile, "%12s %d %d", name1, &xLoc, &yLoc);
            t1 = createTower(name1, xLoc, yLoc);
            if (t1 == NULL) {
                printf("Error creating tower: %s (%d,%d)\n", name1, xLoc, yLoc);
                return 0;
            }
        }
        else if (strcmp(type, "conn") == 0) {
            name1[0] = '\0';
            name2[0] = '\0';      
            fscanf(towerFile, "%12s %12s", name1, name2);
            t1 = findTower(name1);
            t2 = findTower(name2);
            if ((t1 == NULL) || (t2 == NULL)) {
                printf("Unable to add connection between %s and %s.\n", 
                        name1, name2);
                return 0;
            }
            else {
                addConnection(t1, t2);
            }
            
        }
        else {
            printf("Bogus entry in towers file.\n");
            return 0;
        }
    }
    return 1;
}

// Read phone info from file.
// Format is: <id> <x> <y>
// Returns 1 if successful, 0 otherwise.
int getPhones(FILE *phoneFile) {
    Phone p;
    char id[13];
    int xLoc, yLoc;

    while (fscanf(phoneFile, "%12s %d %d", id, &xLoc, &yLoc) != EOF) {
        p = createPhone(id, xLoc, yLoc);
        if (p == NULL) {
            printf("Error creating phone: %s (%d,%d)\n", id, xLoc, yLoc);
            return 0;
        }
    }
    return 1;
}

// Read and throw away all characters on standard input, up to and including a linefeed.
void purgeInput() {
    char c;
    do {
        scanf("%c", &c);
    } while (c != '\n');
}

// Retrieve list of all towers, and print one per line.
void printTowers() {
    TowerNode n;
    Tower t;
    TowerNode nbr;
    int x, y, first;
    for (n=allTowers(); n != NULL; n = n->next) {
        t = n->tower;
        printf("%s ", towerID(t));
        towerLocation(t, &x, &y);
        printf("Loc: (%d,%d) Connected to: ", x, y);
        nbr = towerNeighbors(t);
        first = 1;
        if (nbr == NULL) {
            printf("<none>\n");
        }
        else {
            for (; nbr != NULL; nbr = nbr->next) {
                if (!first) printf(",");
                printf("%s", towerID(nbr->tower));
                first = 0;
            }
            printf("\n");
        }
    }
}

// given a list of phones, print the phone and associated tower
void printPhones(PhoneNode list) {
    PhoneNode n;
    Phone p;
    Tower t;
    int x, y;
    for (n=list; n!=NULL; n = n->next) {
        p = n->phone;
        phoneLocation(p, &x, &y);
        t = phoneTower(p);
        printf("%s Loc:(%d,%d) Tower: %s\n", 
                phoneID(p), x, y, towerID(t));
    }
}

// route a call from orig to recv, and print the route if found
void call(Phone orig, Phone recv) {
    TowerNode route;
    route = routeCall(orig, recv);
    if (route == NULL) {
        printf("Call cannot be completed -- no route found.\n");
    } else {
        printf("Phone %s ", phoneID(orig));
        for (; route != NULL; route = route->next) {
            printf("/ Tower %s ", towerID(route->tower));
        }
        printf("/ Phone %s\n", phoneID(recv));
    }
}


