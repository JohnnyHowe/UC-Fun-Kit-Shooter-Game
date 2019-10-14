/** Functions to help with communications between the two devices.
 *
 * Communications are done with 8 bit numbers.
 * Bit 7: Player signature/number - prevent unit from reading its own signal.
 * Bit 5: Victory? (1 = other player just won, 0 otherwise).
 * Last 3 bits represent the x position of the incoming shot if bit 5 = 0.
 *
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
#define PLAYER_NUMBER_LOOP_RATE 20


void initialise_ir(void)
{
    /** Initialise the infrared driver */
    ir_uart_init();
}


int receive_value(void)
{
    /** Return (int) the x position of the incoming shot transmitted by
     * the other player. This function (calls another function to)
     * properly decode the message. If there is no incoming message,
     * return -1. */
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
    /** Return the player number (0 or 1). Unit talks to other unit and
     * decides what unit is player 0 and player 1. */
    int player_num = 2;
    int done = 0;
    pacer_init(PLAYER_NUMBER_LOOP_RATE);

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
                while (i++ < PLAYER_NUMBER_LOOP_RATE) {
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
