/** A bunch of helper functions for the shot struct thing.
 * Includes:
 * Jonathon Howe 12/10/2019
*/
#include "shot.h"
#include "display.h"
#include <avr/io.h> // Just for debugging things
#include "communication.h"


void show_shot(Shot* shot)
{
    /** Given a Shot pointer, show it if it is on the screen. */
    if (shot->y_pos >= 0 && shot->y_pos <= 4) {
        display_column(1 << shot->x_pos, shot->y_pos);
    }
};


void show_shots(Shot* shots, int num_shots)
{
    /** Given a (pointer to) an array of shots and the number of shots
     * in said array, show them all (run show_shot on them) */
    for (int i = 0; i < num_shots; i++) {
        show_shot(&shots[i]);
    }
};


void move_shot(Shot* shot)
{
    /** Move the given shot (pointer) forward one unit. */
    shot->y_pos -= shot->direction;
};


void update_shots(Shot* shots, int num_shots)
{
    /** Update the array of shots given (move) */
    for (int i = 0; i < num_shots; i++) {
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
        if (shots[i].direction == 1 && shots[i].y_pos == 0) {
            x_pos = shots[i].x_pos;
        }
        i ++;
    }
    return x_pos;
}


Shot process_shot(void)
{
    /* If a value between 0 and 6 (inclusive) is received by the IR
     * sensor, use it as the x coordinate for a new shot, and return
     * it. If not, return a shot with -1 x position. */
    Shot shot = {-1, 0, -1};
    int x_pos = receive_value();
    if (x_pos >= 0 && x_pos <= 6) {
        shot.x_pos = 6 - x_pos;
    }
    return shot;
}

