/** Shot struct and helper function definitions.
 * Jonathon Howe 12/10/2019 */


typedef struct {
    /** Player struct, just holds the position right now */
    int xPos;
    int yPos;
} Shot;


void show_shot(Shot* shot);

void show_shots(Shot* shots, int numShots);

void move_shot(Shot* shot);

void update_shots(Shot* shots, int numShots);
