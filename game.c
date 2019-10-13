/**  A basic space fight game. Each player must shoot at the other
 * and dodge the oppenents shots.
 * Jonathon Howe 10/10/2019 */
#include <avr/io.h>
#include "pio.h"
#include "navswitch.h"
#include "player.h"
#include "display.h"
#include "pacer.h"
#include "communication.h"



#define REFRESH_RATE 1000
#define GAME_TICKS (REFRESH_RATE / 6)    // How many loops must pass for the game to update?

int main(void)
{
    system_init();
    initialise_display();
    navswitch_init();
    initialise_ir();

    pacer_init(REFRESH_RATE);
    uint8_t shot_update_tick = 0;

    DDRC |= (1 << 2);

    Player player = new_player();

    while (1) {
        pacer_wait();
        navswitch_update();

        move_player(&player);
        player_shoot(&player);
        show_player(&player);
        show_shots(player.shots, player.numShots);

        if (shot_update_tick++ >= GAME_TICKS) {
            update_shots(player.shots, player.numShots);
            transmit_shot(player.shots, player.numShots);
            receive_shot(&player);
            shot_update_tick = 0;
        }
    }
}
