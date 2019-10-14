/** Functions to help with processing shots.
 * Also includes the Shot struct definition
 * Jonathon Howe, Tomoya Sakai 12/10/2019 */
#define MAX_SHOTS 14

/** Shot struct definition which holds but the x and y position of each shot,
 * as well as the direction that the shot is going, 1 for going away from player, -1 for coming towards.
*/
typedef struct {
    int x_pos;
    int y_pos;
    int direction;
} Shot;

void transmit_shot(Shot* shots, int num_shots, int player_number);

void show_shot(Shot* shot);

void show_shots(Shot* shots, int num_shots);

void move_shot(Shot* shot);

void update_shots(Shot* shots, int num_shots);

int pos_to_transmit(Shot* shots, int num_shots);

Shot process_shot(int player_number, uint8_t message);

void set_null_shot(Shot* shot);
