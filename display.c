/** Display helper functions.
 * Includes: display_column, initialise_display
 * Jonathon Howe 12/10/2019
*/
#include "navswitch.h"
#include "pio.h"
#include "display.h"


void display_column (uint8_t row_pattern, uint8_t current_column)
{
    /** Set the pattern on the LED matrix in column current_column (int
     * in range 0 to 4 (inclusive)) to row_pattern (integer that in
     * binary represents the LED pattern. */

    static int last_col = 0;
    pio_output_high(cols[last_col]);

    pio_output_low(cols[current_column]);
    int current_row = 0;

    while (current_row < 7) {
        if ((row_pattern >> current_row) & 1) {
            pio_output_low(rows[current_row]);
        } else {
            pio_output_high(rows[current_row]);
        }
        current_row ++;
    }
    last_col = current_column;

    for (int i = 0; i < 7; i ++) {
        pio_output_high(rows[i]);
    }

    pio_output_high(cols[last_col]);
}


void initialise_display(void)
{
    /** Initialise all rows and columns of the LED matrix display */
    for (int col = 0; col < 5; col ++) {
        pio_config_set(cols[col], PIO_OUTPUT_HIGH);
    }
    for (int row = 0; row < 7; row ++) {
        pio_config_set(rows[row], PIO_OUTPUT_HIGH);
    }
}
