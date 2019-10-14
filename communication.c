/** Functions to help with communications between the two devices.
 * Includes: initialise_ir, transmit_shot, receive_value
 * Jonathon Howe, Tomoya Sakai 14/10/2019
 */
#include "shot.h"
#include "communication.h"
#include <stdlib.h>
#include <avr/io.h>


void initialise_ir(void)
{
    /** Initialising the infrared driver
     * Parameters: None
     * Outputs: None
     * */
    ir_uart_init();
}


void transmit_shot(Shot* shots, int num_shots)
{
    /** Given the array of shots, check to see if any are ready to be
     * transmitted to the other kit (if x_pos = -1) and if so, transmit.
     * Parameters: shots, num_shots.
     * Outputs: IR communication to other device
     * */
    int x_pos = pos_to_transmit(shots, num_shots);
    if (x_pos != -1) {
        ir_uart_putc(x_pos);
    }
}

int receive_value(void)
{
    /** Recieve and return the signal transmitted by the other ATMega32u2
     * Parameters: None
     * Output: Receives IR signal iff (ir_uart_read_ready_p() returns True
     * */
    int x_pos = -1;
    if (ir_uart_read_ready_p()) {
        x_pos = ir_uart_getc();
        PORTC |= (1 << 2);
    } else {
        PORTC &= ~(1 << 2);
    }
    return x_pos;
}
