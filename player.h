/** Player struct and helper function definitions.
 * Jonathon Howe 12/10/2019
*/
#include "shot.h"


typedef struct {
    /** Player struct, just holds the position right now */
    int xPos;
    Shot shots[10];  // Max of 10 as that's the most that can fit on the screen
    int numShots;
} Player;


Player new_player(void);

void move_player(Player* player);

void show_player(Player* player);

Shot new_shot(Player* player);

void player_shoot(Player* player);
