/***********************************************/
/* CManship Example Source                     */
/*      For the Atari ST 16/32 bit             */
/*       Developed with Megamax C              */
/*       & Lattice C for the Atari ST          */
/*                                             */
/* https://www.gnu.org/licenses/agpl-3.0.txt   */
/*                                             */
/* http://13leader.net                         */
/*                                             */
/***********************************************/

#include "MENU.H"

#define MU_MESAG 0x0010
#define ARROW    0
#define R_TREE   0
#define TRUE     1
#define FALSE    0

/* The usual required GEM global arrays */
int work_in[11], 
    work_out[57], 
    pxyarray[10],
    contrl[12], 
    intin[128], 
    ptsin[128], 
    intout[128], 
    ptsout[128];

/* Global variables */
int handle, dum;
int msg_buf[8], op1, op2, op3, on;

char *alrt = "[1][C-manship, Chapter 16|by Clayton Walnum][Okay]";
char *on_str = "     On     ";
char *off_str = "    Off     ";

main ()
{
   appl_init ();       /* Initialize application.        */
   open_vwork ();      /* Set up workstation.            */
   do_menu();          /* Go do the MENU.                */
   v_clsvwk (handle);  /* Close virtual workstation.     */
   appl_exit ();       /* Back to the desktop.           */
}

open_vwork ()
{
   int i;

   /* Get graphics handle, initialize the GEM arrays and open  */
   /* a virtual workstation.                                   */

   handle = graf_handle ( &dum, &dum, &dum, &dum);
   for ( i=0; i<10; work_in[i++] = 1 );
   work_in[10] = 2;
   v_opnvwk ( work_in, &handle, work_out );
}

do_menu ()
{
   long menu_adr; /* Address of the tree containing our menu.   */

   /* First, we initialize our option flags, so we can keep     */
   /* track of which ones are active.  Also, we change the      */
   /* mouse pointer to an arrow.                                */

   op1 = TRUE;
   op2 = FALSE;
   op3 = FALSE;
   on  = TRUE;

   graf_mouse ( ARROW, &dum );

   /* Here we load the resource file.  If the file is missing,  */
   /* we warn the user with an alert box then terminate the     */
   /* program by skipping the code following the ELSE.          */
 
   if ( ! rsrc_load ("\MENU.RSC") )
      form_alert ( 1, "[1][MENU.RSC missing!][Okay]" );

   /* If the resource file loads OK, we get the address of the  */
   /* tree, then handle menu messages from evnt_multi().        */

   else {
      rsrc_gaddr ( R_TREE, TREE00, &menu_adr );
      menu_bar ( menu_adr, TRUE );
      do {
         evnt_multi ( MU_MESAG,0,0,0,0,0,0,0,0,0,0,0,0,0,msg_buf,
                      0,0,&dum,&dum,&dum,&dum,&dum,&dum );

         switch ( msg_buf[3] ) {

            case DESK:
               switch ( msg_buf[4] ) {
                  case INFO:
                     form_alert ( 1, alrt );
                     break;
               }

            case FILE:
               switch ( msg_buf[4] ) {
                  case LOAD:
                     v_gtext ( handle, 20, 120, "Load file " );
                     break;
                  case SAVE:
                     v_gtext ( handle, 20, 120, "Save file " );
                     break;
               }

            case OPTIONS:
               switch ( msg_buf[4] ) {
                  case OPTION1:
                     menu_icheck ( menu_adr, OPTION1, op1=!op1 );
                     break;
                  case OPTION2:
                     menu_icheck ( menu_adr, OPTION2, op2=!op2 );
                     break;
                  case OPTION3:
                     menu_icheck ( menu_adr, OPTION3, op3=!op3 );
                     break;
               }

            case SELECTS:
               switch ( msg_buf[4] ) {
                  case ONOFF:
                     if ( on )
                        menu_text ( menu_adr, ONOFF, off_str );
                     else
                        menu_text ( menu_adr, ONOFF, on_str );
                     menu_ienable ( menu_adr, SELECT1, on=!on );
                     menu_ienable ( menu_adr, SELECT2, on );
                     menu_ienable ( menu_adr, SELECT3, on );
                     break;
                  case SELECT1:
                     v_gtext ( handle, 20, 120, "Select 1 " );
                     break;
                  case SELECT2:
                     v_gtext ( handle, 20, 120, "Select 2 " );
                     break;
                  case SELECT3:
                     v_gtext ( handle, 20, 120, "Select 3 " );
                     break;
               }
            menu_tnormal ( menu_adr, msg_buf[3], TRUE );
         }
      }           
      while ( msg_buf[4] != QUIT );
      menu_bar ( menu_adr, FALSE );
   }
}

