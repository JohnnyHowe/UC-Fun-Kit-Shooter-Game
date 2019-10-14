/** A bunch of helper functions for the player.
 * Includes: move_player, show_player
 * Jonathon Howe 12/10/2019 */
#include <avr/io.h> // Just for debugging things
#include <stdlib.h>
#include "player.h"
#include "navswitch.h"
#include "game_display.h"


Player new_player(void)
{
    /** Make and return a Player with the default attribute values. */
    Player player = {2, {}, 0};
    return player;
}


int is_hit(Player* player)
{
    int hit = 0;
    int i = 0;
    while (i < player->num_shots && !hit) {
        if (player->shots[i].y_pos == 0 && player->shots[i].x_pos == player->x_pos) {
            hit = 1;
        }
        i ++;
    }
    return hit;
}


void update_player(Player* player)
{
    move_player(player);
    player_shoot(player);
    show_player(player);
}


void move_player(Player* player)
{
    /** Given a pointer to a Player, increase or decrese its x coordinate
     * (pos) if the navswitch is pushed north or south repectively
     * if it will stay on screen (in range 0 to 6 (inclusive) */

    if (navswitch_push_event_p (NAVSWITCH_EAST) && player->x_pos < 4) {
        player->x_pos += 1;
    } else if (navswitch_push_event_p (NAVSWITCH_WEST) && player->x_pos > 0) {
        player->x_pos -= 1;
    }
};


Shot new_shot(Player* player)
{
    /** Given the pointer to the player that clicked the shoot button,
     * create a new shot at (player.x_pos, 3) */
    Shot shot = {player->x_pos, 1, 1};
    return shot;
}


void player_shoot(Player* player)
{
    /** Given a Player pointer, add a Shot to player.shots in the nav
     * button is pressed and the player has less than 10 shots. */
    if (navswitch_push_event_p(NAVSWITCH_PUSH) && can_shoot(player->shots, player->num_shots)) {
        player->shots[player->num_shots] = new_shot(player);
        player->num_shots++;
    }
}


int can_shoot(Shot* shots, int num_shots)
{
    /* Given an array of shots, check whether the player can shoot.
     * so num_shots < maxShots and the most recent outgoing shot is not
     * in the first or second position (column 2 and 3) */
    int shot_okay = 1;
    int i = 0;
    while (i < num_shots && shot_okay) {
        if (shots[i].direction > 0 && (shots[i].y_pos == 1 || shots[i].y_pos == 2)) {
            shot_okay = 0;
        }
        i ++;
    }
    return (shot_okay && num_shots < MAX_SHOTS);
}


void receive_shot(Player* player, int player_number)
{
    /* If a shot is received from the IR sensor, add it to the
     * player.shot array. */
    Shot shot = process_shot(player_number);
    if (player->num_shots < MAX_SHOTS && shot.x_pos != -1) {
        player->shots[player->num_shots] = shot;
        player->num_shots ++;
    }
}


void show_player(Player* player)
{
    /** Given the pointer to a Player, light the LED in columm 4 and the
     * row corresponding to the player x pos (player.pos). */
    display_column(1, player->x_pos);
}


void refresh_shots(Player* player)
{
    /* Remove the invalid shots (y > 4) and shuffle the valid ones down */
    int i = 0;
    int j = 0;
    while (i < player->num_shots) {
        if (player->shots[i].y_pos <= 6 && player->shots[i].y_pos >= 0) {
            player->shots[j] = player->shots[i];
            j ++;
        }
        i ++;
    }
    player->num_shots = j;
    // shot_collision(player);
}


void shot_collision(Player* player)
{
    /** If two shots have collided, set their y_pos to -1 and their
     * direction to 0 so the refresh_shots function gets rid
     * of them. */
    int i = 0;
    int j = 0;
    while (i < player->num_shots) {
        while (j < player->num_shots) {
            if (shots_collided(&(player->shots[i]), &(player->shots[j]))) {
                // player->shots[i] = NULL_SHOT;
                // player->shots[j] = NULL_SHOT;
                set_null_shot(&(player->shots[i]));
                set_null_shot(&(player->shots[j]));
                //player->shots[i].direction = 0;
                //player->shots[j].direction = 0;
                //player->shots[i].x_pos = -1;
                //player->shots[j].x_pos = -1;
            }
            j ++;
        }
        i ++;
    }
}


int shots_collided(Shot* shot1, Shot* shot2)
{
    /** return whether the 2 shots have just collided. */
    int collided = 0;
    if (shot1->direction + shot2->direction == 0) {
        if (shot1->x_pos == shot2->x_pos) {
            if (abs(shot1->y_pos - shot2->y_pos) <= 1) {
                collided = 1;
            }
        }
    }
    return collided;
}

