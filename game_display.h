/** Display functions and rows and cols array definitions.
 * Full docstrings are in the game_display.c file.
 * Jonathon Howe, Tomoya Sakai 12/10/2019 */
#include "pio.h"

#define NUM_OF_COL 5
#define NUM_OF_ROWS 7
#define GAME_WIDTH NUM_OF_COL
#define GAME_HEIGHT NUM_OF_ROWS

/** Define PIO pins driving LED matrix rows.  */
static const pio_t rows[] =
{
    LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO,
    LEDMAT_ROW4_PIO, LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO,
    LEDMAT_ROW7_PIO
};

/** Define PIO pins driving LED matrix columns.  */
static const pio_t cols[] =
{
    LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
};

void display_column (uint8_t row_pattern, uint8_t current_column);

void initialise_display(void);

void display_character (char character);

void clear_display(void);
