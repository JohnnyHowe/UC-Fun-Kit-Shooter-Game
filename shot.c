/** A bunch of helper functions for the shot struct thing.
 * Jonathon Howe, Tomoya Sakai 12/10/2019 */
#include <avr/io.h>
#include "shot.h"
#include "game_display.h"
#include "ir_uart.h"
#include "communication.h"


void transmit_shot(Shot* shots, int num_shots, int player_number)
{
    /** Given the array of shots, check to see if any are ready to be
     * transmitted to the other kit and if so, transmit.
     * shots (Shot*): shot array to check and possibly transmit from.
     * num_shots (int): length of shots array.
     * player_number (int): Player/unit number to transmit from. */
    int x_pos = pos_to_transmit(shots, num_shots);
    if (x_pos != -1) {
        uint8_t message = x_pos;
        message |= (player_number << SIGNATURE_BIT);
        ir_uart_putc(message);
    }
}


void show_shot(Shot* shot)
{
    /** Show a single shot on the LED matrix..
     * shot (Shot*): pointer to shot to show. */
    if (shot->y_pos >= 0 && shot->y_pos < GAME_HEIGHT) {
        display_column(1 << shot->y_pos, shot->x_pos);
    }
};


void show_shots(Shot* shots, int num_shots)
{
    /** Show all the shots in the given shot array.
     * shots (Shot*): Array of shots to show.
     * num_shots (int): length of shots. */
    for (int i = 0; i < num_shots; i++) {
        show_shot(&shots[i]);
    }
};


void move_shot(Shot* shot)
{
    /** Move the given shot forward one unit.
     * shot (Shot*): shot to move. */
    shot->y_pos += shot->direction;
};


void update_shots(Shot* shots, int num_shots)
{
    /** Update the array of shots given (move)
     * shots (Shot*): array of shots to update.
     * num_shots (int): length of shots array. */
    for (int i = 0; i < num_shots; i++) {
        move_shot(&(shots[i]));
    }
}


int pos_to_transmit(Shot* shots, int num_shots)
{
    /** Return the x position to transmit. If there is not shot that
     * should be transmitted, -1 is returned.
     * shots (Shot*): Array of shots to potentially transmit from.
     * num_shots (int): length of shots array. */
    int x_pos = -1;
    int i = 0;
    while (i < num_shots && x_pos == -1)
    {
        if (shots[i].direction == 1 && shots[i].y_pos == GAME_HEIGHT - 1) {
            x_pos = shots[i].x_pos;
        }
        i ++;
    }
    return x_pos;
}


Shot process_shot(int player_number, uint8_t message)
{
    /** Return a shot with the x position received from message.
     * If the x position is not in the range 0 to 4 (inclusive), set the
     * shots x pos to -1.
     * player_number (int): Receiving player/unit number.
     * message (uint8_t): Incoming binary message. */
    Shot shot = {-1, 6, -1};
    int incoming_player_num = check_bit(message, SIGNATURE_BIT);
    message &= ~(1 << SIGNATURE_BIT);
    int x_pos = message;

    if (x_pos >= 0 && x_pos < GAME_WIDTH && incoming_player_num != player_number) {
        shot.x_pos = GAME_WIDTH - x_pos - 1;
    }
    return shot;
}


void set_null_shot(Shot* shot)
{
    /** Set the direction of the shot to 0 and the x position to -1
     * shot (Shot*): Shot to forget about. */
    shot->x_pos = -1;
    shot->direction = 0;
}
