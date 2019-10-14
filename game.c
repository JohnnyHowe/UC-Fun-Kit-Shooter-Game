/**  A basic space fight game. Each player must shoot at the other
 * and dodge the oppenents shots.
 * Jonathon Howe 10/10/2019 */

#include <avr/io.h>
#include "pio.h"
#include "navswitch.h"
#include "../fonts/font5x7_1.h"
#include "../../utils/tinygl.h"
#include "../../utils/pacer.h"

#include "communication.h"
#include "player.h"
#include "game_display.h"


#define REFRESH_RATE 500
#define GAME_TICKS 100
#define RECEIVE_TICKS (GAME_TICKS / 4)


void reset(Player* player)
{
    player->num_shots = 0;
    player->x_pos = 2;
    clear_display();
}


void display_score(int score)
{
    int i = 0;
    display_character(score + 0 + '0');
    while (i++ < 500) {
        pacer_wait();
        tinygl_update();
        display_character(score + 0 + '0');
    }
}


void initialise_everything(void)
{
    system_init();
    initialise_ir();
    initialise_display();
    navswitch_init();

    tinygl_init(REFRESH_RATE);
    tinygl_font_set(&font5x7_1);
}


static void update_game(Player* player, int player_number, int score)
{
    static uint16_t shot_update_tick = 0;
    if (shot_update_tick++ >= GAME_TICKS) {
        update_shots(player->shots, player->num_shots);
        refresh_shots(player);

        if (is_hit(player)) {
            transmit_hit(player_number);
            display_score(score);
            reset(player);
        }
        shot_update_tick = 0;
    }
}


static void transmit(Player* player, int player_number)
{
    static uint16_t transmit_tick = 0;
    if (transmit_tick++ >= GAME_TICKS) {
        transmit_shot(player->shots, player->num_shots, player_number);
        transmit_tick = 0;
    }
}


static void receive(Player* player, int player_number, int* score)
{
    static uint16_t receive_tick = (RECEIVE_TICKS / 2);
    PORTC &= ~(1 << 2);
    if (receive_tick++ >= RECEIVE_TICKS) {

        int value = receive_value();
        if (value != -1) {
            uint8_t message = value;
            if (check_bit(message, 5) == 1) {
                *score = (*score + 1) % 10;
                display_score(*score);
                reset(player);
            } else {
                receive_shot(player, player_number, message);
            }
            receive_tick = 0;
        }
    }
}


int main(void)
{
    initialise_everything();

    int player_number = get_player_number();
    int score = 0;

    pacer_init(REFRESH_RATE);
    // show_player_number(player_number);

    Player player = new_player();
    reset(&player);

    while (1) {
        pacer_wait();
        navswitch_update();

        update_player(&player);
        show_shots(player.shots, player.num_shots);

        transmit(&player, player_number);
        receive(&player, player_number, &score);
        update_game(&player, player_number, score);
    }
}
