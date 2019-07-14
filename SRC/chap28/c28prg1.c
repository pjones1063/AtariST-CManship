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

handle_messages ()
{
   switch ( msg_buf[0] ) {

      case MN_SELECTED:
         do_menu ();
         break;

      case WM_REDRAW:
         do_redraw ( (GRECT *) &msg_buf[4] );
         break;

      case WM_FULLED:
         do_full ();
         break;

      case WM_ARROWED:
         do_arrow ();
         break;

      case WM_VSLID:
         do_vslide ();
         break;

      case WM_HSLID:
         do_hslide ();
         break;

      case WM_CLOSED:
         do_wind_close ();
         break;
   }
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


do_full ()
{
   if ( !full )
      wind_set ( w_h2, WF_CURRXYWH, fullx, fully, fullw, fullh );
   else
      wind_set ( w_h2, WF_CURRXYWH, 
                 fullx, fully, fullw, 316 -162*(res==MED) );
   calc_vslid ( cur_count );
   full = !full;
}


do_menu ()
{}

do_arrow ()
{}


do_vslide ()
{}

do_hslide ()
{}

do_wind_close ()
{}


draw_interior ( clip )
GRECT clip;
{
   GRECT r;
   int lines_avail, lines_shown;

   graf_mouse ( M_OFF, 0L );
   vswr_mode ( handle, MD_REPLACE );
   if ( msg_buf[3] == w_h1 ) {
      if ( res == HIGH ) {
         r.g_x = 1;
         r.g_y = 337;
         r.g_w = 638;
         r.g_h = 63;
      }
      else {
         r.g_x = 1;
         r.g_y = 167;
         r.g_w = 638;
         r.g_h = 32;
      }
      draw_rec ( r, 2, 4, GREEN );
      draw_buttons ();
   }
   else {
      if ( !full_draw )
         set_clip ( TRUE, clip );
      wind_get ( w_h2, WF_WORKXYWH, &r.g_x, &r.g_y, &r.g_w, &r.g_h );
      draw_rec ( r, 2, 8, WHITE );
      if ( full )
         lines_avail = 20;
      else
         lines_avail = 15;
      lines_shown = cur_count - cur_top;
      if ( lines_avail > lines_shown ) {
         cur_top = cur_count - lines_avail;
         if ( cur_top < 0 )
            cur_top = 0;
      }
      if ( cur_count == 0 )
         calc_vslid ( 1 );
      else
         calc_vslid ( cur_count );
      calc_hslid ( NUM_COLUMNS );
      updte_chk_wind ();
      full_draw = FALSE;
      set_clip ( FALSE, clip );
   }
   graf_mouse ( M_ON, 0L );
}


draw_rec ( rec, inter, fill, color )
GRECT rec;
int inter, fill, color;
{
   int pxy[4];

   graf_mouse ( M_OFF, 0L );
   vsf_interior ( handle, inter );
   vsf_style ( handle, fill );
   vsf_color ( handle, color );
   pxy[0] = rec.g_x;
   pxy[1] = rec.g_y;
   pxy[2] = rec.g_x + rec.g_w - 1;
   pxy[3] = rec.g_y + rec.g_h - 1;
   vr_recfl ( handle, pxy );
   graf_mouse ( M_ON, 0L );
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


updte_chk_wind ()
{
   int i, y;

   wind_get ( w_h2, WF_WORKXYWH, &wrkx, &wrky, &wrkw, &wrkh );
   i = cur_top;
   y = 0;
   while ( (i < cur_count) && (i < cur_top + wrkh / charh) ) {
      prnt_chk_wnd ( i, wrky + charh + y * charh );
      ++i;
      ++y;
   }
}


prnt_chk_wnd ( index, row )
int index, row;
{
   char a[40], s[10];

   if ( left ) {
      v_gtext ( handle, 6, row, cur_chk_strc[index].cancel );
      v_gtext ( handle, 25, row, cur_chk_strc[index].number );
      sprintf ( a, "$%5ld.%02ld",
         cur_chk_strc[index].amount/100, cur_chk_strc[index].amount%100 );
      v_gtext ( handle, 77, row, a );
      v_gtext ( handle, 169, row, cur_chk_strc[index].payee );
      strcpy ( a, cur_chk_strc[index].memo );
      a[24] = 0;
      v_gtext ( handle, 424, row, a );
   }
   else {
      v_gtext ( handle, 9, row, cur_chk_strc[index].payee );
      v_gtext ( handle, 264, row, cur_chk_strc[index].memo );
      strcpy ( s, cur_chk_strc[index].date );
      format_date ( a, s );
      v_gtext ( handle, 520, row, a );
   }
}


draw_buttons()
{
   set_buttons ();
   button ( "BALANCE", bal_but, 35 );
   button ( "# TRANS", trans_but, 131 );
   button ( "# CHECKS", check_but, 227 );
   button ( "# DEP", dep_but, 323 );
   button ( "MONTH", mnth_but, 419 );
   button ( "DATE", date_but, 515 );
}


set_buttons ()
{
   if ( balance < 0 && balance > (-100) )
      sprintf ( bal_but, "$-%ld.%02ld", balance/100, labs(balance%100) );
   else
      sprintf ( bal_but, "$%ld.%02ld", balance/100, labs(balance%100) );
   sprintf ( trans_but, "%d", num_trans );
   sprintf ( check_but, "%d", num_chks );
   sprintf ( dep_but, "%d", num_deps );
   if ( month == -1 )
      strcpy ( mnth_but, "NONE" );
   else
      strcpy ( mnth_but, months[month] );
}

button ( str1, str2, x1 )
char *str1, *str2;
int x1;
{
      int x2, y1, y2;
      int pxy[10];

      x2 = x1 + 88;
      y1 = 174 * res;
      y2 = 198 * res;
      vswr_mode ( handle, MD_REPLACE );
      vsf_color ( handle, WHITE );
      pxy[0] = x1;
      pxy[1] = y1;
      pxy[2] = x2;
      pxy[3] = y2;
      v_bar ( handle, pxy );
      pxy[3] = y1;
      pxy[4] = x2;
      pxy[5] = y2;
      pxy[6] = x1;
      pxy[7] = y2;
      pxy[8] = x1;
      pxy[9] = y1;
      vsl_width ( handle, 3 );
      vsl_color ( handle, BLACK );
      v_pline ( handle, 5, pxy );
      center_butstring ( str1, x1, 184);
      center_butstring ( str2, x1, 194);
}


center_butstring ( str, x1, y )
char *str;
int x1, y;
{
   int x, x2;

   x2 = x1 + 88;
   x = ((x2-x1)-(strlen(str)*8))/2+x1;
   v_gtext ( handle, x1+5, y * res, "          " );
   v_gtext ( handle, x, y * res, str ); 
}


format_date ( d1, d2 )
char *d1, *d2;
{
   strcpy ( d1, d2 );
   d1[2] = '/';
   strcpy ( &d1[3], &d2[2] );
   d1[5] = '/';
   strcpy ( &d1[6], &d2[4] );
}
