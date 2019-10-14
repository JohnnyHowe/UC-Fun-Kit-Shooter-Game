/**  A basic space fight game. Each player must shoot at the other
 * and dodge the oppenents shots.
 * Jonathon Howe, Tomoya Sakai 10/10/2019 */
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
#define MAX_SCORE 9


void reset(Player* player)
{
    /** Delete all active shots and set the player position to the center
     * player (Player*): Pointer to player to reset */
    player->num_shots = 0;
    player->x_pos = 2;
    clear_display();
}


void display_score(int score)
{
    /** Display score for 1 second
     * score (int): Number to display */
    int i = 0;
    display_character(score + 0 + '0');
    while (i++ < REFRESH_RATE) {
        pacer_wait();
        tinygl_update();
        display_character(score + 0 + '0');
    }
}


void initialise_everything(void)
{
    /** Initialise system, ir sensor, display, navswitch and tiny gl */
    system_init();
    initialise_ir();
    initialise_display();
    navswitch_init();

    tinygl_init(REFRESH_RATE);
    tinygl_font_set(&font5x7_1);
}


static void update_game(Player* player, int player_number, int score)
{
    /** If it's time to update (GAME_TICKS ticks have gone by), update
     * the shots (update and refresh) and if the player is hit, tell
     * the other unit, display the score and reset the player
     * player (Player*): Pointer to player to update.
     * player_number (int): The units player number (0 or 1).
     * score (int): Player score to display (if other player is hit). */
    static uint16_t shot_update_tick = 0;
    update_player(player);
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
    /** If it's time to transmit a shot (GAME_TICKS have gone by). This
     * function only calls the function that decides whether a shot needs
     * to be transmitted once every game loop/tick
     * player (Player*): Pointer to player with shots to transmit.
     * player_number (int): Player/unit number. */
    static uint16_t transmit_tick = 0;
    if (transmit_tick++ >= GAME_TICKS) {
        transmit_shot(player->shots, player->num_shots, player_number);
        transmit_tick = 0;
    }
}


static void receive(Player* player, int player_number, int* score)
{
    /** Receive and process all game IR info (all incoming IR if not
     * deciding player number).
     * player (Player*): Pointer to player to receive shots.
     * player_number (int): Player/unit number.
     * score (int*): Pointer to player score. */
    static uint16_t receive_tick = (RECEIVE_TICKS / 2);
    PORTC &= ~(1 << 2);
    if (receive_tick++ >= RECEIVE_TICKS) {
        int value = receive_value();
        if (value != -1) {
            uint8_t message = value;
            if (check_bit(message, HIT_BIT) == 1) {
                *score = (*score + 1) % (MAX_SCORE + 1);
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
    /** Run it all
     * Initialise everything, run main game loop. */
    initialise_everything();

    int player_number = get_player_number();
    int score = 0;

    pacer_init(REFRESH_RATE);

    Player player = new_player();
    reset(&player);

    while (1) {
        pacer_wait();
        navswitch_update();

        show_shots(player.shots, player.num_shots);
        transmit(&player, player_number);
        receive(&player, player_number, &score);
        update_game(&player, player_number, score);
    }
}
