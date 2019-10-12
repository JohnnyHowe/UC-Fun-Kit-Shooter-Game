/**  A basic space fight game. Each player must shoot at the other
 * and dodge the oppenents shots.
 * Jonathon Howe 10/10/2019
*/
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

    Player player = {3};

    while (1) {
        navswitch_update();
        move_player(&player);
        show_player(&player);
    }
}
