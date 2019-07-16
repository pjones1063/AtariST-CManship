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

int work_in[11], work_out[57];
int handle;
int contrl[12], intin[128];
int ptsin[128], intout[128], ptsout[128];
int gr_hwchar, gr_hhchar, gr_hwbox, gr_hhbox;

main()
{
  appl_init();
  open_vwork();
  do_pline();
  do_roundrec();
  do_froundrec();
  do_circle();
  do_pmarker();
  do_bar();
  do_ellipse();
  do_arc();
  do_pieslice();
  do_fills();
  v_clsvwk(handle);
  appl_exit();
}

open_vwork()
{
  int i;

  for (i=0; i<10; work_in[i++] = 1);
  work_in[2] = 2;
  handle = graf_handle(&gr_hwchar,&gr_hhchar,&gr_hwbox,&gr_hhbox);
  v_opnvwk(work_in, &handle, work_out);
}

do_pline()
{
  int pxy[4];
  int color, end, type, width;

  pxy[0] = 30;   pxy[1] = 20;
  pxy[2] = 280;  pxy[3] = 20;
  end = 0; width = 1; 
  v_clrwk(handle);
  for (color=1; color<5; ++color) {
    vsl_color(handle,color);
    vsl_ends(handle,end,end);
    vsl_width(handle,width);
    v_pline(handle,2,pxy);
    pxy[1] += 10;  pxy[3] += 10;
    end += 1; width += 2;
  }
  vsl_width(handle,1);
  vsl_ends(handle,0,0);
  vsl_color(handle,1);
  for (type=1; type<7; ++type) {
    vsl_type(handle,type);
    pxy[1] += 10; pxy[3] += 10;
    v_pline(handle,2,pxy);
  }
  Cconin();
}

do_roundrec()
{
  int pxy[4];
  int color, width;

  pxy[0] = 10;   pxy[1] = 10;
  pxy[2] = 300;  pxy[3] = 190;
  width = 1;
  v_clrwk(handle);
  vsl_type(handle,1);
  for (color=1; color<7; ++color) {
    vsl_width(handle,width);
    vsl_color(handle,color);
    v_rbox(handle,pxy);
    width += 2;
    pxy[0] += 20;  pxy[1] += 20;
    pxy[2] -= 10;  pxy[3] -= 10;
  }
  Cconin();
}

do_froundrec()
{
  int pxy[4];
  int color;

  pxy[0] = 10;   pxy[1] = 10;
  pxy[2] = 300;  pxy[3] = 190;
  v_clrwk(handle);
  for (color=1; color<7; ++color) {
    vsf_color(handle,color);
    v_rfbox(handle,pxy);
    pxy[0] += 20;  pxy[1] += 20;
    pxy[2] -= 10;  pxy[3] -= 10;
  }
  Cconin();
}

do_circle()
{
  int color, radius;

  v_clrwk(handle);
  radius = 100;
  for (color=1; color<8; ++color) {
    vsf_color(handle,color);
    v_circle(handle,150,100,radius);
    radius -= 15;
  }
  Cconin();
}

do_pmarker()
{
  int color, height, type;
  int pxy[2];

  v_clrwk(handle);
  pxy[1] = 10;
  for (type=1; type<7; ++type) {
    vsm_type(handle,type);
    height = 2;  pxy[0] = 10;
    for (color=1; color<6; ++color) {
      vsm_color(handle,color);
      vsm_height(handle,height);
      v_pmarker(handle,1,pxy);
      height += 16;  pxy[0] += 60;
    }
    pxy[1] += 35;
  }
  Cconin();
}
    
do_bar()
{
  int pxy[4], color;

  pxy[0] = 10;  pxy[1] = 190;
  pxy[2] = 300;  pxy[3] = 10;
  v_clrwk(handle);
  for (color=1; color<6; ++color) {
    vsf_color(handle,color);
    v_bar(handle,pxy);
    pxy[0] += 25;  pxy[1] -= 20;
    pxy[2] -= 20;  pxy[3] += 10;
  }
  Cconin();
}

do_ellipse()
{
  int color, xradius, yradius;

  v_clrwk(handle);
  xradius = 150;  yradius = 100;
  for (color=1; color<11; ++color) {
    vsf_color(handle,color);
    v_ellipse(handle,150,100,xradius,yradius);
    xradius -= 15;
  }
  Cconin();
}

do_arc()
{
  int color, radius, bang, eang;

  v_clrwk(handle);
  vsl_width(handle,3);
  bang = 900; eang = 0; radius = 10;
  for (color=1; color<6; ++color) {
    vsl_color(handle,color);
    v_arc(handle,150,100,radius,bang,eang);
    bang += 60; eang -= 60; radius += 20;
  }
  Cconin();
}

do_pieslice()
{
  int color, radius, bang, eang;

  v_clrwk(handle);
  radius = 100; bang = 3200; eang = 600;
  for (color=1; color<6; ++color) {
    vsf_color(handle,color);
    v_pieslice(handle,150,100,radius,bang,eang);
    radius -= 15; bang -=200; eang -= 100;
  }
  Cconin();
}

do_fills()
{
  int pxy[4], style, i, num, x;

  pxy[0] = 50; pxy[1] = 30;
  pxy[2] = 250; pxy[3] = 170; num = 25;
  for (i=2; i<4; ++i) {
    vsf_color(handle,i);
    vsf_interior(handle,i);
    for (style=1; style<num; ++style) {
      vsf_style(handle,style);
      v_clrwk(handle);
      v_bar(handle,pxy);
      for (x=0; x<32000; ++x);
    }
    num = 13;
  }
  Cconin();
}
