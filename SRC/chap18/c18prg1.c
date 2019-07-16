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

#include <gemdefs.h>
#include <obdefs.h>

#define TRUE  1
#define FALSE 0
#define PARTS NAME|CLOSER|MOVER|SIZER
#define MIN_WIDTH 64
#define MIN_HEIGHT 64

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
    wrkw, wrkh, curx, cury, curw, curh, w_handle,
    char_w, char_h, box_w, box_h;

int msg_buf[8];

char *title = "C-MANSHIP #17";

char *text[] = { 
   "This is some sample text",
   "for use in the C-manship",
   "window demonstration found",
   "in Chapter 18."
};
int num_lines = 4; 

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
   /* Initialize and open our window. */
   open_window();

   /* Change mouse to arrow. */
   graf_mouse ( ARROW, 0L );

   /* Receive event messages until the window closer is clicked. */
   do {
      evnt_mesag ( msg_buf );
      switch ( msg_buf[0] ) { /* msg_buf[0] is message type. */

         case WM_MOVED:
         case WM_SIZED:
            do_move();
            break;

         case WM_REDRAW:
            do_redraw ( (GRECT *) &msg_buf[4] );
            break;
      }
   }
   while ( msg_buf[0] != WM_CLOSED );

   /* Close and delete the window. */
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
   wind_get ( w_handle, WF_WORKXYWH, &wrkx, &wrky, &wrkw, &wrkh );

   /* Set the color and fill style. */
   vsf_interior ( handle, 1 );
   vsf_color ( handle, WHITE );

   /* Draw the background in the window's work area. */
   pxy[0] = wrkx;
   pxy[1] = wrky;
   pxy[2] = wrkx + wrkw - 1;
   pxy[3] = wrky + wrkh - 1;
   vr_recfl ( handle, pxy );

   /* Write the text to the window. */
   y = wrky + box_h;
   for ( x=0; x<num_lines; ++x ) {
      v_gtext ( handle, wrkx+8, y, text[x] );
      y += box_h;
   }

   /* Drawing over, so turn the clipping */
   /* off and turn the mouse back on.    */
   set_clip ( FALSE, clip );
   graf_mouse ( M_ON, 0L );
}


do_redraw ( rec1 )
GRECT *rec1;
{
   GRECT rec2;

   /* Lock window for update. */
   wind_update ( BEG_UPDATE );

   /* Get first rectangle from list. */
   wind_get ( msg_buf[3], WF_FIRSTXYWH,
              &rec2.g_x, &rec2.g_y, &rec2.g_w, &rec2.g_h );

   /* Loop through entire rectangle list, */
   /* redrawing where necessary.          */
   while ( rec2.g_w && rec2.g_h ) {
      if ( rc_intersect ( rec1, &rec2 ) )
         draw_interior ( rec2 );
      wind_get ( msg_buf[3], WF_NEXTXYWH,
                 &rec2.g_x, &rec2.g_y, &rec2.g_w, &rec2.g_h );
   }
   /* Unlock window after update. */
   wind_update ( END_UPDATE );
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
   /* Find the size of the desktop's work area. */
   wind_get ( 0, WF_WORKXYWH, &fullx, &fully, &fullw, &fullh );

   /* Create window in memory. */
   w_handle = wind_create ( PARTS, fullx, fully, fullw, fullh );

   /* Set the window's title. */
   wind_set ( w_handle, WF_NAME, title, 0, 0 );

   /* Draw the window on the screen. */
   graf_growbox ( 10, 10, 10, 10, fullx, fully, fullw, fullh );
   wind_open ( w_handle, fullx, fully, fullw, fullh );
}

   
close_window()
{
   /* Get current size of window for use in graf_shrinkbox, */
   /* then close and delete the window.                     */

   wind_get ( w_handle, WF_CURRXYWH, &curx, &cury, &curw, &curh );
   graf_shrinkbox ( 10, 10, 10, 10, curx, cury, curw, curh );
   wind_close ( w_handle );
   wind_delete ( w_handle );
}
