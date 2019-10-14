/** Functions to help with player movement and shooting.
 * Also includes the Player struct definition.
 * Full docstrings for the functions are in the player.c file.
 * Jonathon Howe, Tomoya Sakai 12/10/2019
*/

#include "shot.h"

/** Player struct definition which holds the x-axis position, the array of shots that the player has made,
 * and how many shots the player has made.
 */
typedef struct {
    int x_pos;
    Shot shots[MAX_SHOTS];  
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

void update_player(Player* player);

int is_hit(Player* player);

void transmit_hit(int player_number);
