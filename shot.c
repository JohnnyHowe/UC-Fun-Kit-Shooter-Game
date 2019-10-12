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
    shot->yPos--;
};


void update_shots(Shot* shots, int numShots)
{
    /** Update the array of shots given (move) */
    for (int i = 0; i < numShots; i++) {
        move_shot(&(shots[i]));
    }
};
