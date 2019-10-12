#include <avr/io.h>
#include "pio.h"
#include "navswitch.h"


/* Define PIO pins driving LED matrix rows.  */
static const pio_t rows[] =
{
    LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO,
    LEDMAT_ROW4_PIO, LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO,
    LEDMAT_ROW7_PIO
};


/* Define PIO pins driving LED matrix columns.  */
static const pio_t cols[] =
{
    LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
};


static void display_column (uint8_t row_pattern, uint8_t current_column)
{
    /* Set the pattern on the LED matrix in column current_column (int
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

    for (int i = 0; i < 5; i ++) {
        pio_output_high(rows[i]);
    }

    pio_output_high(cols[last_col]);
}


void initialise_display(void)
{
    /* Initialise all rows and columns of the LED matrix display */
    for (int col = 0; col < 5; col ++) {
        pio_config_set(cols[col], PIO_OUTPUT_HIGH);
    }
    for (int row = 0; row < 7; row ++) {
        pio_config_set(rows[row], PIO_OUTPUT_HIGH);
    }
}


int move_player(int current_x)
{
    /* Given the current x coordinate of the player, increase or
     * decrese it if the navswitch is pushed north or south repectively
     * if it will stay on screen (in range 0 to 6 (inclusive) */

    if (navswitch_push_event_p (NAVSWITCH_SOUTH) && current_x < 6) {
        current_x += 1;
    } else if (navswitch_push_event_p (NAVSWITCH_NORTH) && current_x > 0) {
        current_x -= 1;
    }
    return current_x;
}


int main(void)
{
    system_init();
    initialise_display();
    navswitch_init();

    int player_x = 3;

    while (1) {
        navswitch_update();
        player_x = move_player(player_x);
        display_column(1 << player_x, 4);


    }
}
