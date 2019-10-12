/**  A basic space fight game. Each player must shoot at the other
 * and dodge the oppenents shots.
 * Jonathon Howe 10/10/2019 */
#include <avr/io.h>
#include "pio.h"
#include "navswitch.h"
#include "player.h"
#include "display.h"


int main(void)
{
    system_init();
    initialise_display();
    navswitch_init();

    DDRC |= (1 << 2);

    Player player = new_player();

    while (1) {
        navswitch_update();

        move_player(&player);
        player_shoot(&player);
        show_player(&player);

        update_shots(player.shots, player.numShots);
        show_shots(player.shots, player.numShots);
    }
}
