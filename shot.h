/** Shot struct and helper function definitions.
 * Jonathon Howe 12/10/2019 */
#define MAX_SHOTS 14


typedef struct {
    /** Player struct, just holds the position right now */
    int x_pos;
    int y_pos;
    int direction;  // 1 = Towards player, -1 = away from player
} Shot;

// Shot NULL_SHOT = {-1, -1, 0};


void show_shot(Shot* shot);

void show_shots(Shot* shots, int num_shots);

void move_shot(Shot* shot);

void update_shots(Shot* shots, int num_shots);

int pos_to_transmit(Shot* shots, int num_shots);

Shot process_shot(void);

void set_null_shot(Shot* shot);
