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

#define BLACK      0
#define RED        2
#define WHITE      8
#define DRK_RED    9
#define TRUE       1
#define FALSE      0
#define NONE       0
#define LEFT       1
#define RIGHT      2
#define LOW        0
#define REPLACE    1
#define DOT        2
#define M_OFF      256
#define M_ON       257

/* GEM arrays */
int work_in[11], work_out[57], contrl[12], intin[128],
    ptsin[128], intout[128], ptsout[128];

int desk_palette[16]; /* Desktop color palette. */

/* Our own color palette. */
int my_palette[16] = { 0x000,0x700,0x060,0x770,0x007,0x707,0x333,0x666,
                       0x400,0x444,0x373,0x773,0x337,0x003,0x377,0x400 };
                       
int handle,    /* GEM graphics handle */
    dum;       /* A dummy variable for storage of uneeded values */

/* Data for sprites. */
long alien[] = {0x00000000,0x00000000,0x10000000,0x00000800,
                0x00000000,0x00000000,0x20000000,0x00000400,
                0x00000000,0x00000000,0x60000000,0x00000600,
                0x00000000,0x00000000,0x78000000,0x00000600,
                0x00000000,0x00000000,0x00000000,0x00003C00,
                0x00000000,0x00000000,0x30000000,0x00000C00,
                0x00000000,0x00000000,0x30000000,0x00004E00,
                0x00000000,0x00000000,0x42000000,0x00002400,
                0x00020000,0x00000000,0x83400040,0x00404040,
                0x00030000,0x00000000,0x01C00040,0x00408040,
                0x00030000,0x00000000,0x00800000,0x00000040};
int alien_w = 32, alien_h = 10;


long expl1[] = {0x00000000,0x00000000,0x0C000000,0x00000000,
                0x00000000,0x00000000,0x1E000000,0x00000000,
                0x00000000,0x00000000,0x3F000C00,0x00000C00,
                0x00000000,0x00000000,0x3F000000,0x00000000,
                0x00000000,0x00000000,0x1E000C00,0x00000C00,
                0x00000000,0x00000000,0x1E000C00,0x00000C00,
                0x00000000,0x00000000,0x3F000C00,0x00000000,
                0x00000000,0x00000000,0x3F000C00,0x00000C00,
                0x00000000,0x00000000,0x61800000,0x00000000,
                0x00010000,0x00000000,0xC0E00000,0x00000000,
                0x00010000,0x00000000,0xC0E00000,0x00000000};
int expl_w = 32, expl_h= 10;

long expl2[] = {0x00000000,0x00000000,0x18000000,0x00000000,
                0x00000000,0x00000000,0x3C000000,0x00000000,
                0x00000000,0x00000000,0x7E001800,0x00001800,
                0x00000000,0x00000000,0x7E001800,0x00001800,
                0x00000000,0x00000000,0x3C003800,0x00003800,
                0x00000000,0x00000000,0x3C001800,0x00001800,
                0x00000000,0x00000000,0x7E001800,0x00001000,
                0x00000000,0x00000000,0x3C001800,0x00001800,
                0x00000000,0x00000000,0xFF003C00,0x00003C00,
                0x00030000,0x00000000,0x81C00000,0x00000000,
                0x00030000,0x00000000,0x81C00000,0x00000000};

long expl3[] = {0x003C0018,0x00000018,0x00000000,0x00000000,
                0x007E003C,0x0000003C,0x00000000,0x00000000,
                0x007E003C,0x0000003C,0x00000000,0x00000000,
                0x013D0019,0x00000019,0x00000000,0x00000000,
                0x07BD0799,0x00000799,0x00000000,0x00000000,
                0x03FF0199,0x00000181,0x80000000,0x00000000,
                0x01FF01FF,0x000001FF,0xC000C000,0x0000C000,
                0x00FF006D,0x0000006D,0x00000000,0x00000000,
                0x03B90038,0x00000038,0xC0000000,0x00000000,
                0x03AD002C,0x0000002C,0xC0000000,0x00000000,
                0x00260026,0x00000026,0x00000000,0x00000000};

long expl4[] = {0x01350004,0x00000000,0x00000000,0x00000000,
                0x02CA0282,0x00000082,0x00000000,0x00000000,
                0x01340010,0x00000000,0x80000000,0x00000000,
                0x04B500B1,0x00000031,0x00000000,0x00000000,
                0x0BAA0B00,0x00000A00,0x80008000,0x00000000,
                0x06190210,0x00000210,0x40000000,0x00000000,
                0x00FC00B0,0x00000000,0x80008000,0x00000000,
                0x04A604A0,0x000000A0,0x00000000,0x00000000,
                0x01490009,0x00000000,0x00000000,0x00000000,
                0x07330020,0x00000020,0x80000000,0x00000000,
                0x07030000,0x00000000,0x80000000,0x00000000};


long photon1[] = {0x00000000,0x00000000,0x00C00000,0x00000000,
                  0x00000000,0x00000000,0x00C00000,0x00000000,
                  0x00000000,0x00000000,0x00C00000,0x00000000,
                  0x00000000,0x00000000,0x00C00000,0x00000000,
                  0x00000000,0x00000000,0x00800000,0x00000340,
                  0x00000000,0x00000000,0x00800000,0x00000760,
                  0x00000000,0x00000000,0x01800000,0x00000E70,
                  0x00000000,0x00000000,0x03C00080,0x00000C30,
                  0x00000000,0x00000000,0xFFC00100,0x00000030,
                  0x00000000,0x00000000,0x01F00100,0x00000E00,
                  0x00000000,0x00000000,0x023C0000,0x000005C0,
                  0x00000000,0x00000000,0x041F0000,0x000003C0,
                  0x00000000,0x00000000,0x080F0000,0x00000000,
                  0x00000000,0x00000000,0x00070000,0x00000000,
                  0x00000000,0x00000000,0x00020000,0x00000000,
                  0x00000000,0x00000000,0x00000000,0x00000000};
int photon_h = 15, photon_w = 32;

long photon2[] = {0x00000000,0x00000000,0x20000000,0x00000000,
                  0x00000000,0x00000000,0x60000000,0x00000000,
                  0x00000000,0x00000000,0x30000000,0x00000000,
                  0x00000000,0x00000000,0x18040000,0x00000000,
                  0x00000000,0x00000000,0x0C080000,0x000003C0,
                  0x00000000,0x00000000,0x04100000,0x000003E0,
                  0x00000000,0x00000000,0x03E00100,0x00000C10,
                  0x00000000,0x00000000,0x03C00080,0x00000C30,
                  0x00000000,0x00000000,0x03C00100,0x00000C30,
                  0x00000000,0x00000000,0x01800000,0x00000E70,
                  0x00000000,0x00000000,0x01000000,0x000006E0,
                  0x00000000,0x00000000,0x01000000,0x000002C0,
                  0x00000000,0x00000000,0x03000000,0x00000000,
                  0x00000000,0x00000000,0x03000000,0x00000000,
                  0x00000000,0x00000000,0x03000000,0x00000000,
                  0x00000000,0x00000000,0x03000000,0x00000000};

long photon3[] = {0x00000000,0x00000000,0x60000000,0x00000000,
                  0x00000000,0x00000000,0x30200000,0x00000000,
                  0x00000000,0x00000000,0x18200000,0x000003C0,
                  0x00000000,0x00000000,0x0C400000,0x000003A0,
                  0x00000000,0x00000000,0x03800000,0x00000C70,
                  0x00000000,0x00000000,0x03C00100,0x00000C30,
                  0x00000000,0x00000000,0x03FF0040,0x00000C00,
                  0x00000000,0x00000000,0x03800080,0x00000C70,
                  0x00000000,0x00000000,0x06000000,0x000001E0,
                  0x00000000,0x00000000,0x1C000000,0x000003C0,
                  0x00000000,0x00000000,0x78000000,0x00000000,
                  0x00000000,0x00000000,0xF0000000,0x00000000,
                  0x00000000,0x00000000,0x60000000,0x00000000,
                  0x00000000,0x00000000,0x00000000,0x00000000,
                  0x00000000,0x00000000,0x00000000,0x00000000,
                  0x00000000,0x00000000,0x00000000,0x00000000};


typedef struct fdbstr
{
     long      fd_addr;
     int       fd_w;
     int       fd_h;
     int       fd_wdwidth;
     int       fd_stand;
     int       fd_nplanes;
     int       fd_r1;
     int       fd_r2;
     int       fd_r3;
} MFDB;

/************************************************************
* Main program.
************************************************************/
main()
{
   appl_init ();
   open_vwork ();
   if ( Getrez () != LOW )
      form_alert ( 1, "[1][This demo must be run|in low resolution][OK]" );
   else {
      do_animate ();
      Setpalette ( desk_palette );
   }
   v_clsvwk ( handle );
   appl_exit ();
}


/***********************************************************
* do_animate ()
*
* Main program loop. Reads the mouse buttons and goes to
* the appropriate functions based on the button presses.
***********************************************************/
do_animate ()
{
   int repeat, button, x, y;

   set_colors ();
   repeat = TRUE;
   while ( repeat ) {
      button = 0;
      while ( button == NONE )
         vq_mouse ( handle, &button, &x, &y );
      if ( button == LEFT )
         photon ();
      else if ( button == RIGHT )
         repeat = FALSE;
   }
}


/***********************************************************
* photon ()
*
* Performs the photon animation. The function checks for
* a "hit" by getting the color of the screen from the
* current photon position and comparing it to the alien
* ship's color.
***********************************************************/
photon ()
{
   int x, y, pixel, pen, p;
   long *ph[3];

   p = 0;
   ph[0] = photon1;
   ph[1] = photon2;
   ph[2] = photon3;
   graf_mouse ( M_OFF, 0L );
   draw_icon ( alien, 7, 250, 100, alien_w, alien_h );
   x = 20;
   y = 100;
   pen = BLACK;
   while ( pen!=RED && pen!=DRK_RED ) {
      draw_icon ( ph[p], 6, x-6, y-6, photon_w, photon_h );
      evnt_timer ( 10, 0 );
      draw_icon ( ph[p], 6, x-6, y-6, photon_w, photon_h );
      x += 4;
      if ( ( p+=1 ) > 2 )
         p = 0;
      v_get_pixel ( handle, x+19, y+4, &pixel, &pen );
      evnt_timer ( 50, 0 );
   }
   kill_alien ();
   graf_mouse ( M_ON, 0L );
}


/***********************************************************
* kill_alien ()
*
* Performs the exploding ship animation.
***********************************************************/
kill_alien ()
{
   int x, y, rx, ry, i;
   int pxy[2];

   x = 250;
   y = 100;
   vsf_color ( handle, BLACK );

   draw_icon ( expl1, 7, x, y, expl_w, expl_h );
   evnt_timer ( 50, 0 );
   v_circle ( handle, x+20, y+7, 10 );
   draw_icon ( expl2, 7, x+1, y, expl_w, expl_h );
   evnt_timer ( 50, 0 );
   v_circle ( handle, x+20, y+7, 10 );
   draw_icon ( expl3, 7, x+7, y, expl_w, expl_h );
   evnt_timer ( 50, 0 );
   v_circle ( handle, x+20, y+7, 10 );
   draw_icon ( expl4, 7, x+7, y, expl_w, expl_h );
   evnt_timer ( 50, 0 );
   v_circle ( handle, x+20, y+7, 10 );

   x += 13;
   y -= 4;
   vsm_type ( handle, DOT );
   vsm_height ( handle, 1 );
   for ( i=0; i<40; ++i ) {
      rx = rnd ( 16 );
      ry = rnd ( 16 );
      pxy[0] = x + rx;
      pxy[1] = y + ry;
      vsm_color ( handle, WHITE );
      v_pmarker ( handle, 1, pxy );
      evnt_timer ( 10, 0 );
      vsm_color ( handle, BLACK );
      v_pmarker ( handle, 1, pxy );
   }
}


/***********************************************************
* draw_icon ()
*
* A general function for drawing icons. It may be used
* in other programs as long as the header file GEMDEFS.H
* has been included at the top of the program. The input
* to the function is the address of the icon's data, the
* desired drawing mode, the X and Y coords at which the
* icon should be drawn, and the width and height of the
* icon. For low resolution only.
***********************************************************/
draw_icon ( data, mode, dx, dy, width, height )
long data;
int mode, dx, dy, width, height;
{
   MFDB s_m, scr_m;
   int pxy[8];

   s_m.fd_addr = data;
   s_m.fd_w = width;
   s_m.fd_h = height;
   s_m.fd_wdwidth = width / 16; 
   s_m.fd_stand = 0;
   s_m.fd_nplanes = 4;
   scr_m.fd_addr = 0;
   pxy[0] = 0; 
   pxy[1] = 0; 
   pxy[2] = width; 
   pxy[3] = height;
   pxy[4] = dx;
   pxy[5] = dy; 
   pxy[6] = dx + width; 
   pxy[7] = dy + height;
   vro_cpyfm ( handle, mode, pxy, &s_m, &scr_m );
}


/***********************************************************
* set_colors ()
*
* This function stores the original desktop colors, and
* then installs the program's palette.
***********************************************************/
set_colors ()
{
   int x;

   graf_mouse ( M_OFF, 0L );
   for ( x=0; x<16; desk_palette [x++] = Setcolor ( x, -1 ) );
   v_clrwk ( handle );
   Setpalette ( my_palette );
   graf_mouse ( M_ON, 0L );
}


/***********************************************************
* open_vwork ()
*
* This function opens a virtual work station.
***********************************************************/
open_vwork ()
{
    int i;
    handle = graf_handle ( &dum, &dum, &dum, &dum );
    for ( i=0; i<10; work_in[i++] = 1 );
    work_in[10] = 2;
    v_opnvwk ( work_in, &handle, work_out );
}


/***********************************************************
* rnd ()
*
* This function is used to get a random number from 0 to
* n-1. Its input is "n" and its output is the random number.
***********************************************************/
rnd ( n )
int n;
{
   int r;

   r = ( int ) Random ();
   r = abs ( r ) % n;
   return ( r );
}

