/*********************************************************************
 * main.cpp
 * Authored by Kris Hauser 2002-2003
 *
 * Interfaces with glut, reads in some parameters.  Nothing special.
 * Copyright 2003, Regents of the University of California 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *********************************************************************/




#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctype.h>

#include <gL/glut.h>

#include <math.h>

#include "simulation/UI.h"

//#define ARRAYSIZE(x) sizeof(x)/sizeof(x[0])

//#define TIMER_DELAY 12
#define TIMER_DELAY 0

void timer(int);
void update();
void key(unsigned char k, int x, int y);
void mouse(int button, int state, int x, int y);



DeformMain* deform;


const char* OPTIONS_STRING = "Options:\n\
\t-scene [scene]: sets the scene file.  Default is \"scene.setup\".\n\
\t-v: verbose.  v[n] (where [n] is a digit) or vv...v (n times) increases the verbosity.\n\
\t-drawconst: toggle draw constraints (default false).\n\
\t-drawpen: toggle draw penetration depths (default false).\n\
\t-drawanim: toggle draw animations (default true). \n\
\t-help: show help.\n\
";

void showhelp() 
{
	std::printf("------Arguments-----\n");
	std::printf(OPTIONS_STRING);
	std::printf("\n");
	std::printf("------Runtime commands-----\n");
	std::printf("Navigation:\n");
	std::printf("Right mouse button rotates the camera.\n");
	std::printf("Left mouse button translates the camera.\n");
	std::printf("Middle mouse button zooms the camera.\n");
	std::printf("\n");
	std::printf("Space key starts/stops the animation\n");

}

int verbose = 0;
const char* scene_file = "scene.setup";
bool draw_penetration = false;
bool draw_animation = true;
bool draw_constraints = false;

//#include "ps2vector.h"
//#include "ps2matrix.h"


int main(int argc, char **argv)
{
  int i;
  for(i=1; i<argc; i++) {
    if(argv[i][0] == '-') {
      if(0 == strcmp(argv[i], "-scene")) {
	scene_file = argv[i+1];
	i++;
      }
      else if(0 == strcmp(argv[i], "-drawpen")) {
	draw_penetration = true;
      }
      else if(0 == strcmp(argv[i], "-drawconst")) {
	draw_constraints = true;
      }
      else if(0 == strcmp(argv[i], "-drawanim")) {
	draw_animation = true;
      }
      else if(0 == strcmp(argv[i], "-help")) {
	showhelp();
	exit(0);
      }                  
      else if(argv[i][1] == 'v') {
	if(isdigit(argv[i][2]))
	  verbose = argv[i][2] - '0';
	else {
	  for(int j=1; j<strlen(argv[i]); j++)
	    if(argv[i][j] == 'v')
	      verbose++;
	}
      }
      else {
	std::printf("Invalid option: %s\n", argv[i]);
	exit(-1);
      }
    }
    else
      break;
  }

   //ps2vector::self_test();
   //ps2matrix::self_test();

   

#if TIMER_DELAY
   glutTimerFunc(TIMER_DELAY, timer, 0);
#else
   glutIdleFunc(update);
#endif
   


}



#define MODELPREFIX "models/"

const char* models  [] = {
  "cube3x3",
  /*"cube3x3h",
  "tube",
  "torus",
  "tetra",
  "sphere",
  "mushroom",
  "mushroom2",
  "dodo",
  //"bimbojubblies",
  "sheet",
  //"bat3",
  //"sheep",*/
};





bool nav [4] = { 0,0,0,0 };

void timer(int)
{
  update();
  glutTimerFunc(TIMER_DELAY, timer, 0);
}

void update()
{
  //ps2matrix::speed_test();
  deform->Update();
  deform->Nav(nav[0], nav[1], nav[2], nav[3]);
  nav[0] = nav[1] = nav[2] = nav[3] = false;
  glutPostRedisplay();
}




void
key(unsigned char k, int x, int y)
{
  deform->Key(k);
}
