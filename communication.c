#include "shot.h"
#include "communication.h"
#include <stdlib.h>
#include <avr/io.h>


void initialise_ir(void)
{
    ir_uart_init();
}


void transmit(Shot* shots, int num_shots)
{
    /* Given an array of shots, if any are ready to be transmitted, do
     * just that */
    int x_pos = pos_to_transmit(shots, num_shots);
    if (x_pos != -1) {
        ir_uart_putc(x_pos);
        PORTC |= (1 << 2);
    } else {
        PORTC &= ~(1 << 2);
    }
}
