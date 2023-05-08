# celltower-network

Download the driver file cellmain.c, along with two additional files: Phone.c will implement phone-related functions, and Tower.c will implement tower-related functions that are initialized in the header file PhoneTower.h(this file also needs to be downloaded).

## Description of the program:

This program will model a cellular phone network. The system consists of a number of towers (base stations) and a number of phones. A phone communicates with a single tower -- the tower that is closest to the phone, within its transmission range. A tower can communicate with any phone that is associated
with that tower, and it may communicate with a specified set of other towers. In this program, the entire cell phone network is modelled using a grid of points in R2. All phones and towers are present at grid points in the R2 grid.

The user will have an interface that allows him/her to perform the following actions:
– List all towers.
– List all phones associated with a particular tower.
– Change the location of a phone.
– Place a call from one phone to another, displaying the connections (phone-to-tower and tower-to-tower) that are used to connect the phones.

The following is the algorithm for placing a call:
Consider a call from phone 1 to phone 5. Phone 1 is associated with tower A, but phone 5 is not. Tower
A contacts its first neighbor (B) and asks it to find phone 5. Since phone 5 is not associated with B, and
B has no neighbors, it tells A that it cannot find a route. A then asks its next neighbor, C. Since tower C is not associated with phone 5, it asks its neighbor D. Tower D responds affirmatively, so C replies to A with the list of towers in the partial route: tower C / tower D. Tower A then prepends itself to the list of towers, to create the final result: phone 1 / tower A / tower C / tower D / phone 5. In other words, to complete a call from phone 1 to phone 5, the phone contacts tower A, which relays the call to tower C, which relays the call to tower D, which completes the call to phone 5. If no neighbor is able to find a route, tower A responds that it is unable to complete the call.

## **Note for the user**:
When the program runs, it will ask the user for two file names. The first file gives information about the towers in the system. The second file gives information about the phones in the system. The formats of these files is described below.

A tower is specified by the following line:
tower <ID> <location-x> <location-y>
A connection is specified by the following line:
conn <tower1_ID> <tower2_ID>
The towers mentioned in a connection statement must have been created by a previous tower statement.
The phone file has ID and location information:
<ID> <location-x> <location-y>

## List of important functionalities of the program:
1. Creating towers and phones(returns a pointer to the phone or tower, and gives a phone id when the appropriate pointer is passed)
2. Provide phone and tower location(in the form of x,y-coordinates)
3. Provide a list of neighbouring towers and phones
4. Add phones or towers to your list of neighbouring phones and towers, Find if a tower or phone exists(using tower or phone id)
5. ROUTE CALLS. The route calls feature is the main functionality of my program.
