/** A bunch of helper functions for the player.
 * Includes: move_player, show_player
 * Jonathon Howe 12/10/2019
*/

#include "player.h"
#include "navswitch.h"
#include "display.h"


void move_player(Player* player)
{
    /** Given a pointer to a Player, increase or decrese its x coordinate
     * (pos) if the navswitch is pushed north or south repectively
     * if it will stay on screen (in range 0 to 6 (inclusive) */

    if (navswitch_push_event_p (NAVSWITCH_SOUTH) && player->pos < 6) {
        player->pos += 1;
    } else if (navswitch_push_event_p (NAVSWITCH_NORTH) && player->pos > 0) {
        player->pos -= 1;
    }
};


void show_player(Player* player)
{
    /** Given the pointer to a Player, light the LED in columm 4 and the
     * row corresponding to the player x pos (player.pos). */
    display_column(1 << player->pos, 4);
}
