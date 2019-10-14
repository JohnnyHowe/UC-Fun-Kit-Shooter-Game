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
#define RECEIVE_TICKS (GAME_TICKS / 2)


void initialise_everything(void)
{
    system_init();
    initialise_ir();
    initialise_display();
    navswitch_init();

    tinygl_init(REFRESH_RATE);
    tinygl_font_set(&font5x7_1);
    tinygl_text_speed_set(1);
}


void show_player_number(int player_number)
{
    for (uint16_t i = 0; i < REFRESH_RATE * 5; i++) {
        display_character('0' + player_number);
        pacer_wait();
        tinygl_update ();
    }
    clear_display();
}


static void update_game(Player* player)
{
    static uint16_t shot_update_tick = 0;
    if (shot_update_tick++ >= GAME_TICKS) {
        //shot_collision(&player);
        update_shots(player->shots, player->num_shots);
        refresh_shots(player);

        if (is_hit(player)) {
            PORTC |= (1 << 2);
        } else {
            PORTC &= ~(1 << 2);
        };

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


static void receive(Player* player, int player_number)
{
    static uint16_t receive_tick = 0;
    if (receive_tick++ >= RECEIVE_TICKS) {
        receive_shot(player, player_number);
        receive_tick = 0;
    }
}


int main(void)
{
    initialise_everything();
    DDRC |= (1 << 2);

    int player_number = get_player_number();
    pacer_init(REFRESH_RATE);
    show_player_number(player_number);

    Player player = new_player();

    while (1) {
        pacer_wait();
        navswitch_update();

        update_player(&player);
        show_shots(player.shots, player.num_shots);

        transmit(&player, player_number);
        receive(&player, player_number);
        update_game(&player);
    }
}
