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


#include <osbind.h>

#define S_XOR_D 6
#define TRUE 1
#define FALSE 0
#define LEFT 1
#define RIGHT 2
#define HAND 3
#define OFF 256
#define ON 257

/* The required GEM global arrays */
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

/* Memory Form Definition Block */
typedef struct mfrmblk {
   long f_addr;    /* Addr of form data.                     */
   int f_w;        /* Width of the form in pixels.           */
   int f_h;        /* Height of the form in pixels.          */
   int f_wdwidth;  /* Width of the form divided by 16.       */
   int f_stand;    /* 0 = raster (RC); 1 = normalized (NDC). */
   int f_nplanes;  /* Number of bit planes (1, 2 or 4).      */
   int f_r1;       /* The last three words are reserved.     */
   int f_r2;
   int f_r3;
} MFDB;

/* Data for the ANALOG "A" icon. */
long icon[] =
    {0x00000000,0x00000000,0x1FFF1FFF,0x1FFF1FFF,0xF000F000,0xF000F000,
     0x00000000,0x00000000,0x35552CCB,0x3C3823F8,0x58003800,0xF8000800,
     0x00000000,0x00000000,0x55554CCB,0x7C3843F8,0x58003800,0xF8000800,
     0x00000000,0x00000000,0xD555CCCB,0xBC3883F8,0x58003800,0xF8000800,
     0x00010001,0x00010001,0x57FDCFFF,0x3FFC07FC,0x58003800,0xF8000800,
     0x00030002,0x00020002,0x5803C803,0x38020802,0x58003800,0xF8000800,
     0x00030002,0x00020002,0x5803C803,0x38020802,0x58003800,0xF8000800,
     0x00030002,0x00020002,0x5403CC03,0x3C020402,0x58003800,0xF8000800,
     0x00030002,0x00020002,0x57F3CFF3,0x3FF207F2,0x58003800,0xF8000800,
     0x00030002,0x00020002,0x555BCCCB,0x3C3A03FA,0x58003800,0xF8000800,
     0x00030002,0x00020002,0x555BCCCB,0x3C3A03FA,0x58003800,0xF8000800,
     0x00030002,0x00020002,0x55F3CDF3,0x3DF203F2,0x58003800,0xF8000800,
     0x00030002,0x00020002,0x5503CD03,0x3D020302,0x58003800,0xF8000800,
     0x00030002,0x00020002,0x5583CC83,0x3C820382,0x58003800,0xF8000800,
     0x00030002,0x00020002,0x5583CC83,0x3C820382,0xF800F800,0xF800E800,
     0x00030002,0x00020002,0x5543CCC3,0x3C4203C2,0xF800F800,0xF800E800,
     0x00030002,0x00020002,0x5543CCC3,0x3C4203C2,0xF800F800,0xF800E800,
     0x00030002,0x00020002,0x5563CCE3,0x3C2203E2,0x58003800,0xF8000800,
     0x00010001,0x00010001,0xFFC1FFC1,0xFFC1FFC1,0xF000F000,0xF000F000};
int icn_w = 48,  /* Width of icon.    */
    icn_h = 18;  /* Height-1 of icon. */

/* Main program. */
main ()
{
   appl_init ();          /* Initialize application.     */
   open_vwork ();         /* Set up workstation.         */
   graf_mouse (OFF,&dum); /* Shut off mouse.             */
   v_clrwk (handle);      /* Clear the screen.           */
   graf_mouse (ON,&dum);  /* Bring the critter back.     */
   do_icon ();            /* Go draw icon.               */
   v_clsvwk (handle);     /* Close virtual workstation.  */
   appl_exit ();          /* Back to desktop.            */
}

/* Initialize a virtual workstation. */
open_vwork ()
{
   int i;

   handle = graf_handle (&dum,&dum,&dum,&dum); /* Get graphics handle. */
   for (i=0; i<10; work_in[i++] = 1);     /* Initialize GEM arrays.    */
   work_in[10] = 2;
   v_opnvwk (work_in, &handle, work_out); /* Open virtual workstation. */
}

/* Main program loop */
do_icon ()
{
   int button,  /* Mouse button pressed.   */
       x,       /* Mouse X coordinate.     */  
       y,       /* Mouse Y coordinate.     */
       ox,      /* Old Mouse X coordinate. */
       oy,      /* Old Mouse Y coordinate. */
       repeat;  /* Loop flag.              */

   graf_mouse (HAND,&dum);                /* Switch mouse forms.      */
   x = 50 ; y = 50;                       /* Initial location of icon.*/
   repeat = TRUE;                         /* Get into WHILE loop.     */
   while (repeat) {                       /* Begin WHILE loop.        */
      x -= 30; y -= 20;                   /* Adjust for mouse coords. */
      ox = x; oy = y;                     /* Save old coordinates.    */
      graf_mouse (OFF,&dum);              /* Turn off mouse.          */
      draw_icon (icon,S_XOR_D,icn_w,      /* Go draw icon.            */
           icn_h,x,y,x+icn_w,y+icn_h);
      graf_mouse (ON,&dum);               /* Turn on mouse.           */
      button = 0;                         /* Get into WHILE loop.     */
      while (button == 0)                 /* Begin WHILE loop.        */
         vq_mouse (handle,&button,&x,&y); /* Get mouse status.        */
      if (button == LEFT) {               /* If left button pushed... */
         graf_mouse (OFF,&dum);           /* Turn off mouse.          */
         draw_icon (icon,S_XOR_D,icn_w,   /* Erase old icon.          */
          icn_h,ox,oy,ox+icn_w,oy+icn_h);
         graf_mouse (ON,&dum);            /* Turn mouse back on.      */
      }
      if (button == RIGHT)                /* If right button pushed...*/
         repeat = FALSE;                  /* get out of loop.         */
   }
}

/* Perform raster operation. */
draw_icon (data,mode,width,height,dx1,dy1,dx2,dy2)
long data[];
int mode,   /* Raster writing mode.                               */
    width,  /* Icon width.                                        */
    height, /* Icon height.                                       */
    dx1,    /* Upper left X coordinate of destination rectangle.  */
    dy1,    /* Upper left Y coordinate of destination rectangle.  */
    dx2,    /* Lower right X coordinate of destination rectangle. */
    dy2;    /* Lower right Y corrdinate of destination rectangle. */
{
   MFDB s_m,   /* Form definition block for source.                  */
        scr_m; /* Form definition block for screen.                  */
   int pxy[8]; /* Coordinates for source and destination rectangles. */

   s_m.f_addr = (long) data;          /* Store addr of icon data in MFDB. */
   s_m.f_w = width;                   /* Store width of icon in MFDB.     */
   s_m.f_h = height;                  /* Store height of icon in MFDB.    */
   s_m.f_wdwidth = width/16;          /* Store icon width/16 in MFDB.     */
   s_m.f_stand = 0;                   /* Raster coordinates.              */
   s_m.f_nplanes = 4;                 /* Low resolution (4 bit planes).   */
   s_m.f_r1 = s_m.f_r2 = s_m.f_r3 = 0;/* Zero out reserved words.         */
   scr_m.f_addr = 0;                  /* Set up screen MFDB.              */
   pxy[0] = 0;       /* Upper left X coordinate of source block.       */
   pxy[1] = 0;       /* Upper left Y coordinate of source block.       */
   pxy[2] = width;   /* Lower right X coordinate of source block.      */
   pxy[3] = height;  /* Lower right Y coordinate of source block.      */
   pxy[4] = dx1;     /* Upper left X coordinate of destination block.  */
   pxy[5] = dy1;     /* Upper left Y coordinate of destination block.  */
   pxy[6] = dx2;     /* Lower right X coordinate of destination block. */
   pxy[7] = dy2;     /* Lower right Y coordinate of destination block. */
   vro_cpyfm(handle,mode,pxy,&s_m,&scr_m); /* Do the raster operation. */
}
                                             
