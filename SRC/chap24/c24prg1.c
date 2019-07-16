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

#include <obdefs.h>
#include <gemdefs.h>

#define TRUE    1
#define FALSE   0
#define SOLID   1
#define PATTERN 2
#define LEFT    1
#define RIGHT   2

/* GEM arrays */
int work_in[11], work_out[57], contrl[12], intin[128], 
    ptsin[128], intout[128], ptsout[128];

int handle,    /* Application handle. */
    dum;       /* Dummy storage.      */


/******************************************************
* Main program.
******************************************************/
main ()
{
   appl_init ();           /* Init application.         */
   open_vwork ();          /* Open virtual workstation. */
   do_box ();              /* Go do our thing.          */
   v_clsvwk ( handle );    /* Close virtual workstation.*/
   appl_exit ();           /* Back to the desktop.      */
}


/******************************************************
* do_box ()
* 
* Calls screen setup function and handles the mouse,
* calling the appropriate box functions based on the
* mouse's coordinates and button state.
******************************************************/
do_box ()
{
   int mouse_x,    /* Mouse X coordinate.     */
       mouse_y,    /* Mouse Y coordinate.     */
       mouse_but,  /* Mouse button state.     */
       box_x,      /* Selected box X coord.   */
       box_y,      /* Selected box Y coord.   */
       box_width,  /* Selected width of box.  */
       box_height, /* Selected height of box. */ 
       exit,       /* Program exit flag.      */
       coords_ok;  /* Proper coordinates flag.*/

   setup_scrn ();
   coords_ok = FALSE;
   graf_mouse ( ARROW, 0L );

   /* Wait for box coordinates within the boundary. */
   while ( !coords_ok ) {

      /* Poll for left button press. */
      mouse_but = 0;
      while ( mouse_but != LEFT )
         graf_mkstate ( &mouse_x, &mouse_y, &mouse_but, &dum );

      /* Get coordinates for the box. */
      box_x = mouse_x;
      box_y = mouse_y;
      graf_rubberbox ( box_x, box_y, 20, 20, &box_width, &box_height );

      /* Allow only a box whose size fits within the */
      /* boundary to be drawn.                       */
      if ( box_x > 20 && box_x + box_width < work_out[0]-21 &&
           box_y > 20 && box_y + box_height < work_out[1]-21 ) {
         draw_box ( box_x, box_y, box_width, box_height );
         coords_ok = TRUE;
      }
   }
   exit = FALSE;
   while ( !exit ) {
      mouse_but = 0;

      /* Wait for press of left or right mouse button. */
      while ( mouse_but != LEFT && mouse_but != RIGHT )
         graf_mkstate ( &mouse_x, &mouse_y, &mouse_but, &dum );

      if ( mouse_but == LEFT )

         /* If the mouse was on the sizing button, */
         /* allow the user to resize the box.      */
         if ( chose_size ( mouse_x, mouse_y, box_x, box_y, 
                           box_width, box_height ) )
            size_box ( box_x, box_y, &box_width, &box_height );

         /* If the mouse was anywhere else in the */
         /* box, allow the user to move the box.  */
         else if ( chose_move ( mouse_x, mouse_y, box_x, box_y, 
                                  box_width, box_height ) )
            move_box ( &box_x, &box_y, box_width, box_height );

      if ( mouse_but == RIGHT )
         exit = TRUE;
   }
}


/******************************************************
* draw_box ()
* 
* Draws a shaded box with a button in the lower right
* corner.  The input is the X and Y coordinates of
* the box's upper left corner and its width and height.
******************************************************/
draw_box ( x, y, w, h )
int x, y, w, h;
{
   int pxy[4];

   graf_mouse ( M_OFF, 0L );

   /* Draw the main body of the box. */
   vsf_interior ( handle, PATTERN );
   vsf_style ( handle, 5 );
   vsf_color ( handle, BLACK );
   pxy[0] = x;
   pxy[1] = y;
   pxy[2] = x + w - 1;
   pxy[3] = y + h - 1;
   v_bar ( handle, pxy );

   /* Draw the box's sizing button. */
   vsf_interior ( handle, SOLID );
   pxy[0] = x + w - 10;
   pxy[1] = y + h - 10;
   v_bar ( handle, pxy );

   graf_mouse ( M_ON, 0L ); 
}


/******************************************************
* size_box ()
* 
* Resizes a box.  The input is the X and Y coordinates
* of the box and pointers to its width and height.  The
* function returns the new width and height by way
* of the pointers, thus replacing the old values of 
* the width and height.
******************************************************/
size_box ( x, y, w, h )
int x, y, *w, *h;
{
   int old_w, old_h;
   int pxy[4];

   old_w = *w;
   old_h = *h;

   /* Get the new box size. */
   graf_rubberbox ( x, y, 20, 20, w, h );

   /* Don't allow the new box to exceed the boundary. */
   if ( x + *w > work_out[0]-20 | y + *h > work_out[1]-20 ) {
      *w = old_w;
      *h = old_h;
   }

   /* If the size is okay, draw the box. */
   else {
      draw_box ( x, y, *w, *h );

      /* Erase the leftover portions (if */
      /* any) of the old box.            */
      graf_mouse ( M_OFF, 0L );
      vsf_interior ( handle, SOLID );
      vsf_color ( handle, WHITE );
      if ( *w < old_w ) {
         pxy[0] = x + *w;
         pxy[1] = y;
         pxy[2] = x + old_w - 1;
         pxy[3] = y + old_h - 1;
         v_bar ( handle, pxy );
      }
      if ( *h < old_h ) {
         pxy[0] = x;
         pxy[1] = y + *h;
         pxy[2] = x + old_w - 1;
         pxy[3] = y + old_h - 1;
         v_bar ( handle, pxy );
      }
      graf_mouse ( M_ON, 0L );
   }
}


/******************************************************
* move_box ()
* 
* Repositions a box.  The input is a pointer to the
* box's X coord., a pointer to the box's Y coord., and 
* the box's width and height.  The new X and Y 
* coordinates are returned from the function by way of 
* the pointers, thus replacing the old X and Y values.
******************************************************/
move_box ( x, y, w, h )
int *x, *y, w, h;
{
   int old_x, old_y;
   int pxy[4];

   old_x = *x;
   old_y = *y;
   graf_mouse ( FLAT_HAND, 0L );

   /* Get new location for the box. */
   graf_dragbox ( w, h, *x, *y, 21, 21, 
                  work_out[0]-41, work_out[1]-41, x, y );

   /* Erase the old box. */
   graf_mouse ( M_OFF, 0L );
   vsf_color ( handle, WHITE );
   vsf_interior ( handle, SOLID );
   pxy[0] = old_x;
   pxy[1] = old_y;
   pxy[2] = old_x + w - 1;
   pxy[3] = old_y + h - 1;
   v_bar ( handle, pxy );

   /* Draw the new box. */
   draw_box ( *x, *y, w, h );

   graf_mouse ( M_ON, 0L );
   graf_mouse ( ARROW, 0L );
}


/******************************************************
* chose_size ()
* 
* Returns a boolean value based on whether the mouse
* button was pressed while over the box's sizing
* button.  The input is the X and Y coordinates of
* the mouse, the X and Y coordinates of the box, and
* the width and height of the box.
******************************************************/
chose_size ( mx, my, bx, by, bw, bh )
int mx, my, bx, by, bw, bh;
{
   if ( mx>bx+bw-10 && mx<bx+bw && my>by+bh-10 && my<by+bh )
      return ( TRUE );
   else
      return ( FALSE );
}


/******************************************************
* chose_move ()
* 
* Returns a boolean value based on whether the mouse
* button was pressed while over an area of the box
* other than the sizing button.  The input is the X 
* and Y coordinates of the mouse, the X and Y 
* coordinates of the box, and the width and height of 
* the box.
******************************************************/
chose_move ( mx, my, bx, by, bw, bh )
int mx, my, bx, by, bw, bh;
{
   if ( mx>bx && mx<bx+bw && my>by && my<by+bh )
      return ( TRUE );
   else
      return ( FALSE );
}


/******************************************************
* setup_scrn ()
* 
* Prepares the screen by clearing the workstation and
* drawing a border.
******************************************************/
setup_scrn ()
{
   int pxy[10];

   graf_mouse ( M_OFF, 0L );

   /* Erase the screen. */
   v_clrwk ( handle );

   /* Draw the border. */
   pxy[0] = 20;
   pxy[1] = 20;
   pxy[2] = work_out[0] - 20;
   pxy[3] = 20;
   pxy[4] = work_out[0] - 20;
   pxy[5] = work_out[1] - 20;
   pxy[6] = 20;
   pxy[7] = work_out[1] - 20;
   pxy[8] = 20;
   pxy[9] = 20;
   v_pline ( handle, 5, pxy );

   graf_mouse ( M_ON, 0L );
}


/*****************************************************
* open_vwork ()
* Opens a virtual workstation.
*****************************************************/
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

