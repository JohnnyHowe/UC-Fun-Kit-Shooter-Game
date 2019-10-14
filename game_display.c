/** Display helper functions.
 * Jonathon Howe, Tomoya Sakai 12/10/2019 */
#include "navswitch.h"
#include "game_display.h"
#include "tinygl.h"


void display_character (char character)
{
    /** Call tinygl_text with the character null terminated.
     * character (char): Character to show. */
    char buffer[2];
    buffer[0] = character;
    buffer[1] = '\0';
    tinygl_text (buffer);
}


void display_column (uint8_t row_pattern, uint8_t current_column)
{
    /** Set the pattern on the LED matrix in column current_column (int
     * in range 0 to 4 (inclusive)) to row_pattern (integer that in
     * binary represents the LED pattern.
     * row_pattern (uint8_t): Binary number representing the row pattern to use.
     * current_column (uint8_t): What column to put the row pattern in. */
    static int last_col = 0;
    pio_output_high(cols[last_col]);

    pio_output_low(cols[current_column]);
    int current_row = 0;

    while (current_row < NUM_OF_ROWS) {
        if ((row_pattern >> current_row) & 1) {
            pio_output_low(rows[current_row]);
        } else {
            pio_output_high(rows[current_row]);
        }
        current_row ++;
    }
    last_col = current_column;

    for (int i = 0; i < NUM_OF_ROWS; i ++) {
        pio_output_high(rows[i]);
    }

    pio_output_high(cols[last_col]);
}


void initialise_display(void)
{
    /** Initialise all rows and columns of the LED matrix display */

    for (int col = 0; col < GAME_WIDTH; col ++) {
        pio_config_set(cols[col], PIO_OUTPUT_HIGH);
    }
    for (int row = 0; row < GAME_HEIGHT; row ++) {
        pio_config_set(rows[row], PIO_OUTPUT_HIGH);
    }
}


void clear_display(void)
{
    /** Turn all rows and columns of the LED matrix to high */
    for (int i = 0; i < GAME_WIDTH; i++) {
        pio_output_high(cols[i]);
    }
    for (int j = 0; j < GAME_HEIGHT; j++) {
        pio_output_high(rows[j]);
    }
}
