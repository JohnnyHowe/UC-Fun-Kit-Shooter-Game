/** A bunch of helper functions for the player.
 * Jonathon Howe, Tomoya Sakai 12/10/2019 */
#include <avr/io.h> // Just for debugging things
#include <stdlib.h>
#include "player.h"
#include "navswitch.h"
#include "game_display.h"
#include "ir_uart.h"


Player new_player(void)
{
    /** Return a new Player with the default attribute values. (2, {}, 0)*/
    Player player = {2, {}, 0};
    return player;
}


int is_hit(Player* player)
{
    /** Return whether the player it hit. If any shot in player.shot is
     * in the same spot as the player.
     * player (Player*): Player to check. */
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
    /** Update the player - move, shoot and shot
     * player (Player*): Pointer to player to update. */
    move_player(player);
    player_shoot(player);
    show_player(player);
}


void move_player(Player* player)
{
    /** Move the player if the navswitch is pushed east or west.
     * player (Player*): Pointer to player to move. */

    if (navswitch_push_event_p (NAVSWITCH_EAST) && player->x_pos < 4) {
        player->x_pos += 1;
    } else if (navswitch_push_event_p (NAVSWITCH_WEST) && player->x_pos > 0) {
        player->x_pos -= 1;
    }
};


Shot new_shot(Player* player)
{
    /** Return a new shot at the same x position as the player and y = 1
     * with a direction away from the player (1)
     * player (Player*): Pointer to player that made the shot. */
    Shot shot = {player->x_pos, 1, 1};
    return shot;
}


int shoot_button(void)
{
    /** Return whether the shoot button(s) have been pressed.
     * navswitch push or white button. */
    return ((PIND & (1 << 7)) != 0) || navswitch_push_event_p(NAVSWITCH_PUSH);
}


void player_shoot(Player* player)
{
    /** Add a new shot to player.shots if the button is pressed and
     * can_shoot returns 1
     * player (Player*): The player to (maybe) shoot from */
    if (shoot_button() && can_shoot(player->shots, player->num_shots)) {
        player->shots[player->num_shots] = new_shot(player);
        player->num_shots++;
    }
}


int can_shoot(Shot* shots, int num_shots)
{
    /** Return whether the player can shoot. If the number of shots is
     * less than MAX_SHOTS and it has been at least 3 game ticks since
     * the last time this player shot.
     * shots (Shot*): The players array of shots.
     * num_shots (int): length of shots. */
    int shot_okay = 1;
    int i = 0;
    while (i < num_shots && shot_okay) {
        if (shots[i].direction > 0 && shots[i].y_pos <= 3) {
            shot_okay = 0;
        }
        i ++;
    }
    return (shot_okay && num_shots < MAX_SHOTS);
}


void receive_shot(Player* player, int player_number, uint8_t message)
{
    /** Add a shot to player.shots if one is received from the IR sensor.
     * player (Player*): Pointer to player to receive shot.
     * player_number (int): Player/unit number.
     * message (uint8_t): Message received from the IR sensor. */
    Shot shot = process_shot(player_number, message);
    if (player->num_shots < MAX_SHOTS && shot.x_pos != -1) {
        player->shots[player->num_shots] = shot;
        player->num_shots ++;
    }
}


void show_player(Player* player)
{
    /** Show the player int row 0, column = player.x_pos.
     * player (Player*): Pointer to player to show. */
    display_column(1, player->x_pos);
}


void refresh_shots(Player* player)
{
    /** Remove invalid shots (y < 0 or y > 6) bu shuffling valid shots
     * down and decreasing player.num_shots (if there are invalid shots).
     * player (Player*): Pointer to player with shots array to refresh. */
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
}


void transmit_hit(int player_number)
{
    /** Transmit a signal with bit 5 active (saying i've been hit).
     * player_number (int): Unit/player number. */
    uint8_t message = 0;
    message |= (player_number << 7);
    message |= (1 << 5);
    ir_uart_putc(message);
}
