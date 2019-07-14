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

do_menu ()
{
   int button;

   switch ( msg_buf[3] ) {

      case DESK:
         form_alert(1,"[0][  MicroCheck ST  |by Clayton Walnum  | | Copy\
right 1989   |by Clayton Walnum  ][CONTINUE]");
         break;

      case FILEBAR:
         switch ( msg_buf[4] ) {

            case NEWACCNT:
               do_newacct ();
               break;

            case OPENMBR:
               button = get_acct ();
               if ( button )
                  open_acct ( filename );
               break;

            case CLOSEMBR:
               do_wind_close ();
               break;

            case NEWMNTH:
               do_new_mnth ();
               break;

            case QUIT:
               do_quit ();
               break;
        }
        break;

     case CHECKS:
        switch ( msg_buf[4] ) {

           case ENTER:
              do_enter ();
              break;

           case SEARCH:
              do_search ();
              break;

           case CHKCAN:
              do_check_canc ();
              break;

           case RECONCIL:
              do_reconcil ();
              break;

           case CHKAUTO:
              do_auto ();
              break;
        }
        break;

     case PRINT:
        switch ( msg_buf[4] ) {

           case PRNTWIND:
              print_wind ();
              break;

           case PRNTREG:
              print_reg ();
              break;
        }

     case UTILITY:
        switch ( msg_buf[4] ) {

           case NEWYEAR:
              do_new_year ();
              break;

           case NEWDATE:
              get_new_date ();
              break;

           case IMPORT:
              do_import ();
              break;

        }
        break;
   }
   menu_tnormal ( menu_addr, msg_buf[3], TRUE );
}


do_wind_close ()
{
   int button;
   GRECT r;

   wind_get ( w_h2, WF_WORKXYWH, &r.g_x, &r.g_y, &r.g_w, &r.g_h );
   wind_update ( BEG_UPDATE );
   if ( search ) {
      search = FALSE;
      cur_top = edit_top;
      cur_count = num_trans;
      cur_chk_strc = checks;
      srch_trans = 0;
      set_menu_entries ();
      strcpy ( windname, acct_name );
      strcpy ( &windname[strlen(windname)], ": Edit mode" );
      wind_set ( w_h2, WF_NAME, windname, 0, 0 );
      draw_interior ( r );
   }
   else if ( canceling ) {
      if ( !saved )
         save_month ( monthfile );
      canceling = FALSE;
      strcpy ( windname, acct_name );
      strcpy ( &windname[strlen(windname)], ": Edit mode" );
      wind_set ( w_h2, WF_NAME, windname, 0, 0 );
      draw_interior ( r );
      set_menu_entries ();
   }
   else if ( loaded ) {
      button = form_alert ( 1, "[2][Do you want to close|this account?]\
[YES|NO]");
      if ( button == YES ) {
         do_save ();
         draw_rec ( r, 2, 8, WHITE );
         set_menu_entries ();
         wind_set ( w_h2, WF_NAME, noacct, 0, 0 );
      }
   }
   wind_update ( END_UPDATE );
}


handle_button ()
{
   wind_get ( w_h2, WF_WORKXYWH, &wrkx, &wrky, &wrkw, &wrkh );
   if ( mouse_y > wrky && mouse_y < wrky + cur_count * charh + 4 
           && mouse_y < wrky + wrkh && mouse_x > wrkx 
           && mouse_x < wrkx+wrkw && num_clicks == 1 )
      if ( !search && !canceling )
         edit ();
      else if ( canceling )
         canc_chk ();
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

   case WA_LFPAGE:
   case WA_RTPAGE:
      do_hslide ();
      break;

   }
}


do_vslide ()
{
   GRECT r;
   int lines_avail;

   wind_get ( w_h2, WF_VSLIDE, &r.g_x, &r.g_y, &r.g_w, &r.g_h );
   if ( r.g_x != msg_buf[4] ) {
      wind_update ( BEG_UPDATE );
      wind_get ( w_h2, WF_WORKXYWH, &r.g_x, &r.g_y, &r.g_w, &r.g_h );
      lines_avail = r.g_h / charh;
      cur_top = (long)msg_buf[4]*((long)cur_count-(long)lines_avail)/1000L;
      wind_set ( w_h2, WF_VSLIDE, msg_buf[4], 0, 0, 0 );
      draw_interior ( r );
      wind_update ( END_UPDATE );
   }
}


do_hslide ()
{
   GRECT r;

   wind_get ( w_h2, WF_HSLIDE, &r.g_x, &r.g_y, &r.g_w, &r.g_h );
   if ( r.g_x != msg_buf[4] ) {
      wind_update ( BEG_UPDATE );
      wind_get ( w_h2, WF_WORKXYWH, &r.g_x, &r.g_y, &r.g_w, &r.g_h );
      left = !left;
      if ( left ) {
         wind_set ( w_h2, WF_INFO, infotext, 0, 0 );
         wind_set ( w_h2, WF_HSLIDE, 0, 0, 0, 0 );
      }
      else {
         wind_set ( w_h2, WF_INFO, &infotext[20], 0, 0 );
         wind_set ( w_h2, WF_HSLIDE, 166, 0, 0, 0 );
      }
      draw_interior ( r );
      wind_update ( END_UPDATE );
   }
}


do_uppage ()
{
    GRECT r;
    int lines_avail;

   wind_update ( BEG_UPDATE );
    wind_get ( w_h2, WF_WORKXYWH, &r.g_x, &r.g_y, &r.g_w, &r.g_h );
    lines_avail = r.g_h / charh;
    cur_top -= lines_avail;
    if ( cur_top < 0 )
      cur_top = 0;
   wind_update ( END_UPDATE );
    draw_interior ( r );
}


do_dnpage ()
{
   GRECT r;
   int lines_avail;

   wind_update ( BEG_UPDATE );
   wind_get ( w_h2, WF_WORKXYWH, &r.g_x, &r.g_y, &r.g_w, &r.g_h );
   lines_avail = r.g_h / charh;
   cur_top += lines_avail;
   if ( cur_top > cur_count - lines_avail )
      cur_top = cur_count - lines_avail;
   draw_interior ( r );
   wind_update ( END_UPDATE );
}

   
do_upline ()
{
   MFDB s, d;
   GRECT r;
   int pxy[8];

   if ( cur_top != 0 ) {
      wind_update ( BEG_UPDATE );
      cur_top -= 1;
      wind_get ( w_h2, WF_WORKXYWH, &r.g_x, &r.g_y, &r.g_w, &r.g_h );
      set_clip ( TRUE, r );
      graf_mouse ( M_OFF, 0L );
      s.fd_addr = 0L;
      d.fd_addr = 0L;
      pxy[0] = r.g_x; 
      pxy[1] = r.g_y + 2; 
      pxy[2] = r.g_x + r.g_w; 
      pxy[3] = r.g_y + r.g_h - charh - 2;
      pxy[4] = r.g_x;
      pxy[5] = r.g_y + charh + 2; 
      pxy[6] = r.g_x + r.g_w; 
      pxy[7] = r.g_y + r.g_h - 2;
      vro_cpyfm ( handle, S_ONLY, pxy, &s, &d );
      prnt_chk_wnd ( cur_top, r.g_y + charh );
      set_clip ( FALSE, r );
      calc_vslid ( cur_count );
      wind_update ( END_UPDATE );
      graf_mouse ( M_ON, 0L );
   }
}


do_dnline ()
{
   MFDB s, d;
   GRECT r;
   int pxy[8];
   int lines_avail, index;

   wind_get ( w_h2, WF_WORKXYWH, &r.g_x, &r.g_y, &r.g_w, &r.g_h );
   lines_avail = r.g_h / charh;
   if ( (cur_top + lines_avail) < cur_count ) {
      wind_update ( BEG_UPDATE );
      cur_top += 1;
      index = cur_top + lines_avail - 1;
      set_clip ( TRUE, r );
      graf_mouse ( M_OFF, 0L );
      s.fd_addr = 0L;
      d.fd_addr = 0L;
      pxy[0] = r.g_x; 
      pxy[1] = r.g_y + charh + 3 - 1*(res==MED); 
      pxy[2] = r.g_x + r.g_w; 
      pxy[3] = r.g_y + r.g_h - 1;
      pxy[4] = r.g_x;
      pxy[5] = r.g_y + 3 -1*(res==MED); 
      pxy[6] = r.g_x + r.g_w; 
      pxy[7] = r.g_y + r.g_h - charh - 1;
      vro_cpyfm ( handle, S_ONLY, pxy, &s, &d );
      prnt_chk_wnd ( index, r.g_y + lines_avail * charh );
      set_clip ( FALSE, r );
      calc_vslid ( cur_count );
      wind_update ( END_UPDATE );
      graf_mouse ( M_ON, 0L );
   }
}


do_quit ()
{
   int button;

   button = form_alert(1,"[2][Are you sure you|want to quit?][YES|NO]");
   if ( button == YES ) {
      search = FALSE;
      all_done = TRUE;
      if ( !saved )
         do_save ();
   }
}

save_month()
{}

do_new_year()
{}

do_new_mnth()
{}

do_newacct()
{}

get_acct()
{}

open_acct()
{}

get_new_date()
{}

do_save()
{}

do_import()
{}

do_reconcil()
{}

canc_chk()
{}

do_check_canc()
{}

print_reg()
{}

do_search()
{}

print_wind()
{}

do_auto()
{}

do_enter()
{}

edit()
{}


