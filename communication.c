/** Functions to help with communications between the two devices.
 *
 * Communication is done with 8 bit binary numbers in the form 0bSCN
 * S (7): Player setting mode. If S = 1 then we are setting the player numbers.
 * C (6):   If S = 1:   Stage of player set mode. 0 = waiting for reply, 1 = confirming number.
 *          If S = 0:   Player setting mode complete (0 = not done, 1 = complete).
 * N (5): Player number being taken by unit transmitting message.
 *
 * Includes: initialise_ir, transmit_shot, receive_value
 * Jonathon Howe, Tomoya Sakai 14/10/2019
 */
#include <stdlib.h>
#include <avr/io.h>
#include "ir_uart.h"
#include "pacer.h"
#include "tinygl.h"

#include "game_display.h"
#include "communication.h"

#define I_AM_PLAYER_ZERO 0b1000000
#define I_AM_PLAYER_ONE 0b0100000
#define PLAYER_CONFIRMED 0b11000000



void initialise_ir(void)
{
    /** Initialising the infrared driver
     * Parameters: None
     * Outputs: None
     * */
    ir_uart_init();
}


int receive_value(void)
{
    /** Return (int) the x position of the incoming shot transmitted by
     * the other player. This function (calls another function to)
     * properly decode the message. */
    int message = -1;
    if (ir_uart_read_ready_p()) {
        message = ir_uart_getc();
    }
    return message;
}


int check_bit(int number, int bit)
{
    /** Return the bit in position bit of message in binary.
     * e.g. check_bit(0b0010, 1) -> 1
     *      check_bit(0b0010, 2) -> 0
     * message (int): Binary number to have bit selected from.
     * bit (int): Slot of message (binary) to return */
    int cleared_message = number &= (1 << bit);
    int bit_value = cleared_message >> bit;
    return bit_value;
}


int get_player_number(void)
{
    /** talk to the other unit and decide what player is 0 and 1 */
    int player_num = 2;
    int done = 0;
    pacer_init(20);

    while (!done) {
        pacer_wait();

        if (!ir_uart_read_ready_p()) {    // This is the first unit to turn on
            ir_uart_putc(I_AM_PLAYER_ZERO);
            display_column(0b0001000, 2);  // I AM PLAYER ZERO
            player_num = 0;
        } else {
            uint8_t received_message = ir_uart_getc();
            if (received_message == PLAYER_CONFIRMED) {
                ir_uart_putc(I_AM_PLAYER_ZERO);
                done = 1;
                PORTC |= (1 << 2);
            } else if (received_message == I_AM_PLAYER_ZERO) {
                display_column(0b1000001, 2);  // I AM PLAYER ONE
                player_num = 1;

                int i = 0;
                while (i++ < 20) {
                    ir_uart_putc(PLAYER_CONFIRMED);
                    PORTC |= (1 << 2);
                    pacer_wait();
                }
                done = 1;
            }
        }
    }
    PORTC &= ~(1 << 2);
    return player_num;
}
