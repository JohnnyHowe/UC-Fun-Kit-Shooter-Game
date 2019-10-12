/** Player struct and helper function definitions.
 * Jonathon Howe 12/10/2019
*/


typedef struct {
    /** Player struct, just holds the position right now */
    int xPos;
} Player;


void move_player(Player* player);

void show_player(Player* player);
