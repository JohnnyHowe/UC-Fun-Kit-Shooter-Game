/** A bunch of helper functions for the shot struct thing.
 * Includes:
 * Jonathon Howe 12/10/2019
*/
#include "shot.h"
#include "display.h"
#include <avr/io.h> // Just for debugging things


void show_shot(Shot* shot)
{
    /** Given a Shot pointer, show it if it is on the screen. */
    if (shot->yPos >= 0 && shot->yPos <= 4) {
        display_column(1 << shot->xPos, shot->yPos);
    }
};


void show_shots(Shot* shots, int numShots)
{
    /** Given a (pointer to) an array of shots and the number of shots
     * in said array, show them all (run show_shot on them) */
    for (int i = 0; i < numShots; i++) {
        show_shot(&shots[i]);
    }
};


void move_shot(Shot* shot)
{
    /** Move the given shot (pointer) forward one unit. */
    shot->yPos -= shot->direction;
};


void update_shots(Shot* shots, int numShots)
{
    /** Update the array of shots given (move) */
    for (int i = 0; i < numShots; i++) {
        move_shot(&(shots[i]));
    }
}


int pos_to_transmit(Shot* shots, int num_shots)
{
    /* Given an array of shots, if one is at y = 0 and it is outgoing,
     * return its x position, if no shots exist, return -1 */
    int x_pos = -1;
    int i = 0;
    while (i < num_shots && x_pos == -1)
    {
        if (shots[i].direction == 1 && shots[i].yPos == 0) {
            x_pos = shots[i].xPos;
        }
        i ++;
    }
    return x_pos;
}

