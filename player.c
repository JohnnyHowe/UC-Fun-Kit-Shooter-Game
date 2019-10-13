/** A bunch of helper functions for the player.
 * Includes: move_player, show_player
 * Jonathon Howe 12/10/2019
*/
#include <avr/io.h> // Just for debugging things
#include "player.h"
#include "navswitch.h"
#include "display.h"


Player new_player(void)
{
    /** Make and return a Player with the default attribute values. */
    Player player = {3, {}, 0};
    return player;
}


void move_player(Player* player)
{
    /** Given a pointer to a Player, increase or decrese its x coordinate
     * (pos) if the navswitch is pushed north or south repectively
     * if it will stay on screen (in range 0 to 6 (inclusive) */

    if (navswitch_push_event_p (NAVSWITCH_SOUTH) && player->xPos < 6) {
        player->xPos += 1;
    } else if (navswitch_push_event_p (NAVSWITCH_NORTH) && player->xPos > 0) {
        player->xPos -= 1;
    }
};


Shot new_shot(Player* player)
{
    /** Given the pointer to the player that clicked the shoot button,
     * create a new shot at (player.xPos, 3) */
    Shot shot = {player->xPos, 3, 1};
    return shot;
}


void player_shoot(Player* player)
{
    /** Given a Player pointer, add a Shot to player.shots in the nav
     * button is pressed and the player has less than 10 shots. */
    if (navswitch_push_event_p(NAVSWITCH_PUSH) && can_shoot(player->shots, player->numShots)) {
        player->shots[player->numShots] = new_shot(player);
        player->numShots++;
    }
}


int can_shoot(Shot* shots, int numShots)
{
    /* Given an array of shots, check whether the player can shoot.
     * so numShots < maxShots and the most recent outgoing shot is not
     * in the first position (column 3) */
    int shot_okay = 1;
    int i = 0;
    while (i < numShots && shot_okay) {
        if (shots[i].direction > 0 && shots[i].yPos == 3) {
            shot_okay = 0;
        }
        i ++;
    }
    return (shot_okay && numShots < MAX_SHOTS);
}


void show_player(Player* player)
{
    /** Given the pointer to a Player, light the LED in columm 4 and the
     * row corresponding to the player x pos (player.pos). */
    display_column(1 << player->xPos, 4);
}

