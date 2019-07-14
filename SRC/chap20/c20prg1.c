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
#include <gembind.h>
#include <osbind.h>

#define TRUE  1
#define FALSE 0
#define PARTS NAME|CLOSER|SIZER|UPARROW|DNARROW|VSLIDE
#define MAX 50
#define SOLID 1
#define MIN_WIDTH 64
#define MIN_HEIGHT 64

/* GEM arrays. */
int work_in[11],
    work_out[57],
    contrl[12],
    intin[128],
    ptsin[128],
    intout[128],
    ptsout[128];

/* Global variables. */
int handle, w_h, top,
    fullx, fully, fullw, fullh,
    char_w, char_h, box_w, box_h,
    wrkx, wrky, wrkw, wrkh;

/* Message buffer. */
int msg_buf[8];

struct {
   char fnames[MAX][15]; /* Char array for filenames. */
   int  count;           /* Number of filenames read. */
} files;

/* Window title. */
char *title = "C-manship";


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

   handle = graf_handle ( &char_w, &char_h, &box_w, &box_h);
   for ( i=0; i<10; work_in[i++] = 1 );
   work_in[10] = 2;
   v_opnvwk ( work_in, &handle, work_out );
}


do_wndw ()
{
   top = 0;

   get_fnames ();
   wind_get ( 0, WF_WORKXYWH, &fullx, &fully, &fullw, &fullh );
   w_h = wind_create ( PARTS, fullx, fully, fullw, fullh );
   wind_set ( w_h, WF_NAME, title, 0, 0 );
   wind_open ( w_h, 100, 20, 150, 151 );
   calc_slid ( w_h, files.count, 14 );
   graf_mouse ( ARROW, 0L );

   do {
      evnt_mesag ( msg_buf );
      switch ( msg_buf[0] ) { /* msg_buf[0] is message type. */

         case WM_SIZED:
            do_move ();
            break;

         case WM_ARROWED:
            do_arrow ();
            break;

         case WM_VSLID:
            do_vslide ();
            break;

         case WM_REDRAW:
            do_redraw ( (GRECT *) &msg_buf[4] );
            break;
      }
   }
   while ( msg_buf[0] != WM_CLOSED );

   wind_close ( w_h );
   wind_delete ( w_h );
}


do_arrow ()
{
   switch ( msg_buf[4] ) {

   case WA_UPPAGE:
      do_uppage ();
      break;

   case WA_DNPAGE:
      do_dnpage ();
      break;

   case WA_UPLINE:
      do_upline ();
      break;

   case WA_DNLINE:
      do_dnline ();
      break;

   }
}


do_vslide ()
{
   GRECT r;
   int lines_avail;

   wind_get ( w_h, WF_WORKXYWH, &r.g_x, &r.g_y, &r.g_w, &r.g_h );
   lines_avail = r.g_h / char_h;
   top = msg_buf[4] * (files.count - lines_avail) / 1000;
   wind_set ( w_h, WF_VSLIDE, msg_buf[4], 0, 0, 0 );
   draw_interior ( r );
}


do_uppage ()
{
    GRECT r;
    int lines_avail;

    wind_get ( w_h, WF_WORKXYWH, &r.g_x, &r.g_y, &r.g_w, &r.g_h );
    lines_avail = r.g_h / char_h;
    top -= lines_avail;
    if ( top < 0 )
      top = 0;
    draw_interior ( r );
}


do_dnpage ()
{
   GRECT r;
   int lines_avail;

    wind_get ( w_h, WF_WORKXYWH, &r.g_x, &r.g_y, &r.g_w, &r.g_h );
    lines_avail = r.g_h / char_h;
    top += lines_avail;
    if ( top > files.count - lines_avail )
      top = files.count - lines_avail;
    draw_interior ( r );
}

   
do_upline ()
{
   FDB s, d;
   GRECT r;
   int pxy[8];

   if ( top != 0 ) {
      top -= 1;
      wind_get ( w_h, WF_WORKXYWH, &r.g_x, &r.g_y, &r.g_w, &r.g_h );
      set_clip ( TRUE, r );
      graf_mouse ( M_OFF, 0L );
      s.fd_addr = 0L;
      d.fd_addr = 0L;
      pxy[0] = r.g_x; 
      pxy[1] = r.g_y + 1; 
      pxy[2] = r.g_x + r.g_w; 
      pxy[3] = r.g_y + r.g_h - char_h - 1;
      pxy[4] = r.g_x;
      pxy[5] = r.g_y + char_h + 1; 
      pxy[6] = r.g_x + r.g_w; 
      pxy[7] = r.g_y + r.g_h - 1;
      vro_cpyfm ( handle, S_ONLY, pxy, &s, &d );
      v_gtext ( handle, r.g_x+char_w, r.g_y+char_h,
                   &files.fnames[top][0] );
      set_clip ( FALSE, r );
      calc_slid ( w_h, files.count, 14 );
      graf_mouse ( M_ON, 0L );
   }
}


do_dnline ()
{
   FDB s, d;
   GRECT r;
   int pxy[8];
   int lines_avail, index;

   wind_get ( w_h, WF_WORKXYWH, &r.g_x, &r.g_y, &r.g_w, &r.g_h );
   lines_avail = r.g_h / char_h;
   if ( (top + lines_avail) < files.count ) {
      top += 1;
      index = top + lines_avail - 1;
      set_clip ( TRUE, r );
      graf_mouse ( M_OFF, 0L );
      s.fd_addr = 0L;
      d.fd_addr = 0L;
      pxy[0] = r.g_x; 
      pxy[1] = r.g_y + char_h + 1; 
      pxy[2] = r.g_x + r.g_w; 
      pxy[3] = r.g_y + r.g_h - 1;
      pxy[4] = r.g_x;
      pxy[5] = r.g_y + 1; 
      pxy[6] = r.g_x + r.g_w; 
      pxy[7] = r.g_y + r.g_h - char_h - 1;
      vro_cpyfm ( handle, S_ONLY, pxy, &s, &d );
      v_gtext ( handle, r.g_x+char_w, r.g_y+(lines_avail)*char_h,
                     &files.fnames[index][0] );
      if ( index != files.count-1 ) 
         v_gtext ( handle, r.g_x+char_w, r.g_y+(lines_avail)*char_h+char_h,
                        &files.fnames[index+1][0] );
      else
         v_gtext ( handle, r.g_x+char_w, r.g_y+(lines_avail)*char_h+char_h,
                        "              " );
      set_clip ( FALSE, r );
      calc_slid ( w_h, files.count, 14 );
      graf_mouse ( M_ON, 0L );
   }
}


get_fnames ()
{
   char dta[44];
   int end, p, x, null_found;

   p = 0;
   files.count = 0;
   Fsetdta ( dta );
   end = Fsfirst ( "*.*", 17 );

   while ( (end > -1) && (files.count <= MAX) ) {
      null_found = FALSE;
      files.count += 1;
      for ( x=0; x<14; ++x ) {
         if ( dta[30+x] == 0 )
            null_found = TRUE;
         if ( null_found )
            dta[30+x] = ' ';
         files.fnames[p][x] = dta[30+x];
      }
      files.fnames[p][14] = 0;
      p += 1;
      end = Fsnext ();
   }
}


calc_slid ( w_h, line_cnt, col_cnt )
int w_h, line_cnt, col_cnt;
{
   int lines_avail, cols_avail, vslid_siz, pos;

   wind_get ( w_h, WF_WORKXYWH, &wrkx, &wrky, &wrkw, &wrkh );
   lines_avail = wrkh / char_h;
   cols_avail = wrkw / char_w;
   vslid_siz = 1000 * lines_avail / line_cnt;
   wind_set ( w_h, WF_VSLSIZE, vslid_siz, 0, 0, 0 );
   pos = (int) ( (float)(top) ) / ( (float)(files.count - lines_avail) ) * 1000;
   wind_set ( w_h, WF_VSLIDE, pos, 0, 0, 0 );
}


do_move()
{
   if ( msg_buf[6] < MIN_WIDTH )
      msg_buf[6] = MIN_WIDTH;
   if ( msg_buf[7] < MIN_HEIGHT )
      msg_buf[7] = MIN_HEIGHT;
   wind_set ( msg_buf[3], WF_CURRXYWH,
              msg_buf[4], msg_buf[5], msg_buf[6], msg_buf[7] );
   calc_slid ( w_h, files.count, 14 );
}


draw_interior ( clip )
GRECT clip;
{
   int pxy[4];
   int x, lines_avail, lines_shown;

   graf_mouse ( M_OFF, 0L );
   set_clip ( TRUE, clip );
   wind_get ( msg_buf[3], WF_WORKXYWH, &wrkx, &wrky, &wrkw, &wrkh );

   vsf_interior ( handle, SOLID );
   vsf_color ( handle, WHITE );
   pxy[0] = wrkx;
   pxy[1] = wrky;
   pxy[2] = wrkx + wrkw - 1;
   pxy[3] = wrky + wrkh - 1;
   vr_recfl ( handle, pxy );

   lines_avail = wrkh / char_h;
   lines_shown = files.count - top;
   if ( lines_avail > lines_shown ) {
      top = files.count - lines_avail;
      if ( top < 0 )
         top = 0;
   }

   for ( x=top; x<files.count; ++x )
      v_gtext ( handle, wrkx+8, wrky+(x+1-top)*char_h, 
                   &files.fnames[x][0] );

   set_clip ( FALSE, clip );
   calc_slid ( w_h, files.count, 14 );
   graf_mouse ( M_ON, 0L );
}


do_redraw ( rec1 )
GRECT *rec1;
{

   GRECT rec2;

   wind_update ( BEG_UPDATE );
   wind_get ( msg_buf[3], WF_FIRSTXYWH,
              &rec2.g_x, &rec2.g_y, &rec2.g_w, &rec2.g_h );

   while ( rec2.g_w && rec2.g_h ) {
      if ( rc_intersect ( rec1, &rec2 ) )
         draw_interior ( rec2 );
      wind_get ( msg_buf[3], WF_NEXTXYWH,
                 &rec2.g_x, &rec2.g_y, &rec2.g_w, &rec2.g_h );
   }

   wind_update ( END_UPDATE );

}


set_clip ( flag, rec )
int flag;
GRECT rec;
{
   int pxy[4];

   pxy[0] = rec.g_x;
   pxy[1] = rec.g_y;
   pxy[2] = rec.g_x + rec.g_w - 1;
   pxy[3] = rec.g_y + rec.g_h - 1;
   vs_clip ( handle, flag, pxy );
}

