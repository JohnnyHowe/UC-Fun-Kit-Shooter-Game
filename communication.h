/** Functions to help with communications between the two kits.
 * Full docstrings for the functions are in the communication.c file.
 * Jonathon Howe, Tomoya Sakai 12/10/2019.
*/


void initialise_ir(void);

int receive_value(void);

int check_bit(int message, int bit);

int get_player_number(void);
