/** Player struct and helper function definitions.
 * Jonathon Howe 12/10/2019
*/
#include "shot.h"


typedef struct {
    /** Player struct, just holds the position right now */
    int x_pos;
    Shot shots[MAX_SHOTS];  // Max of 10 as that's the most that can fit on the screen
    int num_shots;
} Player;


Player new_player(void);

void move_player(Player* player);

void show_player(Player* player);

Shot new_shot(Player* player);

void player_shoot(Player* player);

int can_shoot(Shot* shots, int num_shots);

void receive_shot(Player* player, int player_number, uint8_t message);

void refresh_shots(Player* player);

void shot_collision(Player* player);

int shots_collided(Shot* shot1, Shot* shot2);

void update_player(Player* player);

int is_hit(Player* player);

void transmit_hit(int player_number);
