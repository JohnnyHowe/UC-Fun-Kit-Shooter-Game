/** Shot struct and helper function definitions.
 * Jonathon Howe 12/10/2019 */
#define MAX_SHOTS 10


typedef struct {
    /** Player struct, just holds the position right now */
    int x_pos;
    int y_pos;
    int direction;  // 1 = Towards player, -1 = away from player
} Shot;


void show_shot(Shot* shot);

void show_shots(Shot* shots, int num_shots);

void move_shot(Shot* shot);

void update_shots(Shot* shots, int num_shots);

int pos_to_transmit(Shot* shots, int num_shots);

Shot process_shot(void);
