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


#include <osbind.h>
#include <gemdefs.h>
#include <obdefs.h>

#define TRUE  1
#define FALSE 0
#define PARTS NAME|CLOSER|FULLER|MOVER|INFO

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
int handle, dum, fullx, fully, fullw, fullh,
    curx, cury, curw, curh, oldx, oldy, oldw, oldh;

int msg_buf[8];

char *title = "C-manship - Issue 16";
char *info = "Learning about windows";


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

   handle = graf_handle ( &dum, &dum, &dum, &dum);
   for ( i=0; i<10; work_in[i++] = 1 );
   work_in[10] = 2;
   v_opnvwk ( work_in, &handle, work_out );
}


do_wndw ()
{
   int w_handle, full_flag;

   /* Find the size of the desktop's (handle 0) work area. */
   wind_get ( 0, WF_WORKXYWH, &fullx, &fully, &fullw, &fullh );

   /* Create window in memory. */
   w_handle = wind_create ( PARTS, fullx, fully, fullw, fullh );

   /* Set the window's title and info text. */
   wind_set ( w_handle, WF_NAME, title, 0, 0 );
   wind_set ( w_handle, WF_INFO, info, 0, 0 );

   /* Draw the window on the screen. */
   graf_growbox ( 10, 10, 10, 10, 50, 50, 250, 200 );
   wind_open ( w_handle, 50, 50, 250, 150 );
   draw_backgrd ( w_handle );

   /* Change mouse to arrow. */
   graf_mouse ( ARROW, 0L );

   /* Receive event messages until the window closer is clicked. */
   do {
      evnt_mesag ( msg_buf );
      switch ( msg_buf[0] ) { /* msg_buf[0] is message type. */

         /* If window is moved, set window at new location found */
         /* in msg_buf[4] through msg_buf[7].  The handle of the */
         /* window moved is in msg_bug[3].                       */

         case WM_MOVED:
            wind_set ( msg_buf[3], WF_CURRXYWH, msg_buf[4], msg_buf[5],
                       msg_buf[6], msg_buf[7] );
            break;

         /* If the fuller button has been clicked, set window to */
         /* appropriate size based on full_flag.                 */

         case WM_FULLED:
            full_flag = full_wind ( w_handle );
            if ( !full_flag ) {
               wind_get ( w_handle, WF_PREVXYWH, 
                          &oldx, &oldy, &oldw, &oldh );
               graf_shrinkbox ( oldx, oldy, oldw, oldh,
                                fullx, fully, fullw, fullh );
               wind_set ( msg_buf[3], WF_CURRXYWH, 
                          oldx, oldy, oldw, oldh );
            }
            else {
               wind_get ( w_handle, WF_CURRXYWH, 
                          &curx, &cury, &curw, &curh );
               graf_growbox ( curx, cury, curw, curh,
                              fullx, fully, fullw, fullh );
               wind_set ( msg_buf[3], WF_CURRXYWH, 
                          fullx, fully, fullw, fullh );
               draw_backgrd ( w_handle );
            }
            break;
      }
   }
   while ( msg_buf[0] != WM_CLOSED );

   /* Get current size of window for use in graf_shrinkbox, */
   /* then close and delete the window.                     */

   wind_get ( w_handle, WF_CURRXYWH, &curx, &cury, &curw, &curh );
   graf_shrinkbox ( 10, 10, 10, 10, curx, cury, curw, curh );
   wind_close ( w_handle );
   wind_delete ( w_handle );
}


/* This function calculates if the window should be drawn to */
/* its maximum size or reset to its previous size.           */

full_wind ( w_h )
int w_h;
{
   int c_x, c_y, c_w, c_h,
       f_x, f_y, f_w, f_h;

   wind_get ( w_h, WF_CURRXYWH, &c_x, &c_y, &c_w, &c_h );
   wind_get ( w_h, WF_FULLXYWH, &f_x, &f_y, &f_w, &f_h );
   if ( c_x != f_x || c_y != f_y || c_w != f_w || c_h != f_h )
      return ( TRUE );
   else
      return ( FALSE );
}


/* This function draws a white background in a window's  */
/* work area.  w_h is the window's handle.               */

draw_backgrd (w_h)
int w_h;
{
   int wrk_x, wrk_y, wrk_w, wrk_h;
   int pxy[4];

   /* Turn off mouse for all drawing operations.  */
   graf_mouse ( M_OFF, 0L );

   /* Get the size of the window's work area. */
   wind_get ( w_h, WF_WORKXYWH, &wrk_x, &wrk_y, &wrk_w, &wrk_h );

   /* Set the color and fill style. */
   vsf_interior ( handle, 1 );
   vsf_color ( handle, WHITE );

   /* Draw the rectangle in the window work area. */
   pxy[0] = wrk_x;
   pxy[1] = wrk_y;
   pxy[2] = wrk_x + wrk_w - 1;
   pxy[3] = wrk_y + wrk_h - 1;
   vr_recfl ( handle, pxy );

   /* Drawing over, so turn mouse back on. */
   graf_mouse ( M_ON, 0L );
}
