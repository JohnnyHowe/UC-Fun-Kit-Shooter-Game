/** Player struct and helper function definitions.
 * Jonathon Howe 12/10/2019
*/


typedef struct {
    /** Player struct, just holds the position right now */
    int pos;    // Just the x coordinate, x is just a bad name
} Player;


void move_player(Player* player);
