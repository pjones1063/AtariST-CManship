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


#include <gemdefs.h>
#include <obdefs.h>
#include <osbind.h>

#define TRUE  1
#define FALSE 0
#define PARTS NAME|CLOSER|MOVER|SIZER
#define MIN_WIDTH 64
#define MIN_HEIGHT 64
#define PATTERN 2
#define BELL 7
#define HIGH 2

/* GEM global arrays */
int work_in[11], 
    work_out[57], 
    pxyarray[10],
    contrl[12], 
    intin[128], 
    ptsin[128], 
    intout[128], 
    ptsout[128];

/* Global variables */
int handle, fullx, fully, fullw, fullh, wrkx, wrky,
    wrkw, wrkh, res, char_w, char_h, box_w, box_h, fill;

int msg_buf[8];
int w_h[3];

char *titles[] = {"#1", "#2", "#3"};

main ()
{
   appl_init ();       /* Initialize application.        */
   open_vwork ();      /* Set up workstation.            */
   do_wndw();          /* Go do the window stuff.        */
   v_clsvwk (handle);  /* Close virtual workstation.     */
   appl_exit ();       /* Back to the desktop.           */
}


open_vwork ()
{
   int i;

   /* Get graphics handle, initialize the GEM arrays and open  */
   /* a virtual workstation.                                   */

   handle = graf_handle ( &char_w, &char_h, &box_w, &box_h);
   for ( i=0; i<10; work_in[i++] = 1 );
   work_in[10] = 2;
   v_opnvwk ( work_in, &handle, work_out );
}


do_wndw ()
{
   /* Clear screen. */
   graf_mouse ( M_OFF, 0L );
   v_clrwk ( handle );
   graf_mouse ( M_ON, 0L );

   /* Find screen resolution. */
   res = Getrez();

   /* Initialize and open our windows. */
   open_window();

   /* Change mouse to arrow, and initialize fill style. */
   graf_mouse ( ARROW, 0L );
   fill = 0;

   /* Receive event messages until the window closer is clicked. */
   do {
      evnt_mesag ( msg_buf );
      switch ( msg_buf[0] ) { /* msg_buf[0] is message type. */

         case WM_MOVED:
         case WM_SIZED:
            do_move();
            break;

         case WM_TOPPED:
            wind_set ( msg_buf[3], WF_TOP, 0, 0 );
            break;

         case WM_REDRAW:
            do_redraw ( (GRECT *) &msg_buf[4] );
            break;
      }
   }
   while ( msg_buf[0] != WM_CLOSED );

   /* Close and delete the windows. */
   close_window();
}


do_move()
{
   /* Set window at new location.  Also disallow any      */
   /* window sizes less than our minimum allowable size.  */

   if ( msg_buf[6] < MIN_WIDTH )
      msg_buf[6] = MIN_WIDTH;
   if ( msg_buf[7] < MIN_HEIGHT )
      msg_buf[7] = MIN_HEIGHT;
   wind_set ( msg_buf[3], WF_CURRXYWH,
              msg_buf[4], msg_buf[5], msg_buf[6], msg_buf[7] );
}


draw_interior ( clip )
GRECT clip;
{
   int pxy[4], y, x;

   /* Turn mouse off prior to drawing. */
   graf_mouse ( M_OFF, 0L );

   /* Calculate clip rectangle and turn clipping on. */
   set_clip ( TRUE, clip );

   /* Get coordinates of window's work rectangle. */
   wind_get ( msg_buf[3], WF_WORKXYWH, &wrkx, &wrky, &wrkw, &wrkh );

   /* Set the color and fill style. */
   vsf_interior ( handle, PATTERN );
   fill += 1;
   if ( fill>24 )
      fill = 1;
   vsf_style ( handle, fill );
   vsf_color ( handle, BLACK );

   /* Draw the background in the window's work area. */
   pxy[0] = wrkx;
   pxy[1] = wrky;
   pxy[2] = wrkx + wrkw - 1;
   pxy[3] = wrky + wrkh - 1;
   vr_recfl ( handle, pxy );

   /* Drawing over, so turn the clipping */
   /* off and turn the mouse back on.    */
   set_clip ( FALSE, clip );
   graf_mouse ( M_ON, 0L );
}


do_redraw ( rec1 )
GRECT *rec1;
{
   GRECT rec2;
   int rec_cnt, y;

   /* Init rectangle count, and set y coord for text. */
   rec_cnt = 0;
   if ( res == HIGH )
      y = 15;
   else
      y = 8;

   /* Lock screen for update. */
   wind_update ( BEG_UPDATE );

   /* Get first rectangle from list. */
   wind_get ( msg_buf[3], WF_FIRSTXYWH,
              &rec2.g_x, &rec2.g_y, &rec2.g_w, &rec2.g_h );

   /* Loop through entire rectangle list, */
   /* redrawing where necessary.          */
   while ( rec2.g_w && rec2.g_h ) {
      test_print ( "handle", msg_buf[3], 150, y );
      rec_cnt += 1;
      test_print ( "rec #", rec_cnt, 20, y );
      Cconin();
      if ( rc_intersect ( rec1, &rec2 ) )
         draw_interior ( rec2 );
      wind_get ( msg_buf[3], WF_NEXTXYWH,
                 &rec2.g_x, &rec2.g_y, &rec2.g_w, &rec2.g_h );
   }
   /* Unlock screen after update. */
   wind_update ( END_UPDATE );
   Cconout ( BELL );
}


set_clip ( flag, rec )
int flag;
GRECT rec;
{
   int pxy[4];

   /* Convert rectangle to pxy coords. */
   pxy[0] = rec.g_x;
   pxy[1] = rec.g_y;
   pxy[2] = rec.g_x + rec.g_w - 1;
   pxy[3] = rec.g_y + rec.g_h - 1;

   /* Turn clipping on or off. */
   vs_clip ( handle, flag, pxy );
}


open_window()
{
   int x;

   /* Find the size of the desktop's work area. */
   wind_get ( 0, WF_WORKXYWH, &fullx, &fully, &fullw, &fullh );

   /* Create the windows. */
   for ( x=0; x<3; ++x ) {
      w_h[x] = wind_create ( PARTS, fullx, fully, fullw, fullh );
      wind_set ( w_h[x], WF_NAME, titles[x], 0, 0 );
   }

   /* Draw the windows. */
   wind_open ( w_h[0], fullx, fully, fullw, fullh );
   wind_open ( w_h[1], 50, 65, 100, 100 );
   wind_open ( w_h[2], 100, 90, 100, 100 );
}

   
close_window ()
{
   int x;

   /* Close and delete the windows. */
   for ( x=0; x<3; ++x ) {
      wind_close ( w_h[x] );
      wind_delete ( w_h[x] );
   }
}


test_print ( label, number, x, y )
int number, x, y;
char *label;
{
   char t[100];

   sprintf ( t ,"%s = %d%s", label, number, "     " );
   v_gtext ( handle, x, y, t );
}
