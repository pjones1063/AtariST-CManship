/***********************************************/
/* CManship Example Source                     */
/*      For the Atari ST 16/32 bit             */
/*       Developed with Megamax C              */
/*       & Laser C (on ST TOS 1.02 only)       */
/*                                             */
/* https://www.gnu.org/licenses/agpl-3.0.txt   */
/*                                             */
/* http://13leader.net                         */
/*                                             */
/***********************************************/


#include <stdio.h>
#include <osbind.h>
#define VOID /**/

/*****************************************************
* main ()
*
* Main Program
*****************************************************/
main()
{
   int first_roll, /* Value of player's first roll. */
       win,        /* Win, loss or no change flag. */
       roll,       /* Value of player's rolls. */
       play,       /* Game play continue flag. */
       first;      /* First roll flag. */
   int num_win = 0;   /* Number of games won. */
   int num_games = 0; /* Number of games played. */
   int ch;            /* Single character storage. */

   play = 1;
   win = 0;
   while (play) {
      first = 1;
      roll = 0;
      first_roll = roll_dice();
      win = check_roll (first, first_roll, roll);
      first = 0;
      while (win == 0) {
         roll = roll_dice();
         win = check_roll (first, first_roll, roll);
      }
      ++num_games;
      if (win == -1) puts("You lose.  ");
      else {
         ++num_win;
         puts("You win!  ");
      }
      percent(num_games, num_win);
      play = play_again();
   }
} 

/*****************************************************
* roll_dice ()
*
* Retrieves a random number from 1 to 6 for each die
* and calculates the total, reporting the sum to
* the player.
*****************************************************/
int roll_dice()
{
   int d1, /* Value of die 1.    */
       d2, /* Value of die 2.    */
       t;  /* Total of dice.     */
   int ch; /* Character storage. */

   puts ("Press RETURN to roll:\n");
   ch = getchar();
   d1 = (int) Random();
   d1 = abs(d1) % 6 + 1;
   d2 = (int) Random();
   d2 = abs(d2) % 6 + 1;
   printf ("Die #1: %d      ", d1);
   printf ("Die #2: %d\n\n", d2);
   t = d1 + d2;
   printf ("Your roll: %d\n\n", t);
   return (t);
}

/*****************************************************
* check_roll ()
*
* Checks to see whether the player has won, lost or
* must roll again.  The input is a flag indicating
* whether this is the first roll of the game, the
* value of the first roll, and the value of the
* current roll if it applies.  The output is a -1
* for a lose condition, 1 for a win condition, and
* a zero if the player must roll again.
*****************************************************/
int check_roll(first, first_roll, roll)
int first, first_roll, roll;
{
   int wn;

   wn = 0;
   if (first == 1) {
      if (first_roll == 7 || first_roll == 11) wn = 1;
      else if (first_roll == 2 || 
               first_roll == 3 || 
               first_roll == 12) wn = -1;
   }
   else if (first_roll == roll) wn = 1;
   else if (roll == 7 || roll == 11) wn = -1;
   return (wn);
}

/******************************************************
* percent ()
*
* Calculates & reports the percentage of games won. The
* input is the # of games played & the # of games won.
*****************************************************/
VOID percent (num_games, num_win)
int num_games, num_win;
{
   float pc;

   pc = ((float) num_win / (float) num_games) * 100.0;
   printf ("You've won %d %% of the games\n", (int) pc);
}

/*****************************************************
* play_again ()
*
* Asks the player if he wants to play again, & returns
* a boolean value based on his answer: yes=1 and no=0.
*****************************************************/
int play_again ()
{
   int p;
   int ch;

   puts ("Play again? ");
   if ((ch = getchar()) == 'Y' || ch == 'y') p = 1;
   else p = 0;
   puts ("\n\n");
   return(p);
}


