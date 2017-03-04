///* nuomizong
//   20/07/2015 */
//
#include <cstdio>
#include <GL\glut.h>
#include "Vector3d.h"
#include "viewport2d.h"
#include "sphere.h"
#include "spaceGeometry.h"
#include <ctime>
#include <random>

// Two circles and drop another one.
double xa = 1.0, ya = 1.0, ra = 1.0;

int nVerts = 20;

double xi[20], yi[20], xii[20], yii[20];

Viewport2d viewport;

void OnRender()
{
	double theta, x, y;
	// viewport functions.
	viewport.RenderMode();

	// obj draw functions.
	//m_Obj.Draw(viewport.showVertices, viewport.showEdges, viewport.showFaces);
	// draw circles.
	glColor3f( 1.0f, 0.0f, 0.0f );
	
	glBegin( GL_LINE_LOOP );
	
	for( int j=0; j<20; j++)
		glVertex2f( xi[j], yi[j]);
	glEnd();

	glColor3f( 0.0f, 0.0f, 1.0f );
	
	glBegin( GL_LINE_LOOP );
	
	for( int j=0; j<20; j++)
		glVertex2f( xii[j], yii[j]);
	glEnd();


	glutSwapBuffers();
}
void OnReshape(int nw, int nh)
{
	viewport.Reshape(nw, nh);
}
void OnMouseDown(int button, int state, int x, int y)
{
	viewport.OnMouseClick(button, state, x, y);
}
/* only responses when moving with holding mouse button. */
void OnMouseMove(int x, int y)
{
	viewport.MouseMoveFunction(x, y);

	switch (viewport.selObj)
	{
	case -1:
		break;
	case 0:
	{
		/*if (viewport.selMode == SELECT_MOVE) snowman1.TranslateObj(viewport.oldX, viewport.oldY, x, y);
		else if (viewport.selMode == SELECT_ROTATE) snowman1.RotateObj(viewport.oldX, viewport.oldY, x, y);
		else if (viewport.selMode == SELECT_SCALE) snowman1.ScaleObj(viewport.oldX, viewport.oldY, x, y);*/
		break;
	}
	case 1:
	{
		/*if (viewport.selMode == SELECT_MOVE) snowman2.TranslateObj(viewport.oldX, viewport.oldY, x, y);
		else if (viewport.selMode == SELECT_ROTATE) snowman2.RotateObj(viewport.oldX, viewport.oldY, x, y);
		else if (viewport.selMode == SELECT_SCALE) snowman2.ScaleObj(viewport.oldX, viewport.oldY, x, y);*/
		break;
	}
	}

	viewport.oldX = x;
	viewport.oldY = y;

	glutPostRedisplay();
}
void OnIdle()
{
	viewport.Idle();
}

void OnSpecialKey( int key, int x, int y )
{
	viewport.OnSpecialKey(key, x, y);
}

void OnNormalKey( unsigned char key, int x, int y)
{
	viewport.OnNormalKey(key, x, y);
}

int main(int argc, char *argv[])
{
	std::printf("Good Morning, My Princess!\n");
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(viewport.width, viewport.height);
	glutCreateWindow(viewport.winTitle);

	glutDisplayFunc(OnRender);
	glutReshapeFunc(OnReshape);
	glutMotionFunc(OnMouseMove);
	glutMouseFunc(OnMouseDown);
	glutKeyboardFunc(OnNormalKey);
	glutSpecialFunc(OnSpecialKey);
	glutIdleFunc(OnIdle);
	
	// Processing function.
	//SpaceGeometry::ComputeInscribedCircle_ofThreeCircles( xa, ya, ra, xb, yb, rb, xc, yc, rc, xt, yt, rt );

	for ( int i=0; i<nVerts; i++)
	{
		double theta = i * 2*3.1415926 / nVerts;
		xi[i] = xa + ra * cos( theta );
		yi[i] = ya + ra * sin( theta );
	}

	std::default_random_engine generator;
	generator.seed( 400 );
	std::uniform_real_distribution<double> distribution(0.0, 0.3);

	double e[20];

	e[0] = 0.0; e[7] = 0.0; e[14] = 0.0;

	for ( int i=1; i<7; i++)
		e[i] = distribution( generator );
	for ( int i=8; i<14; i++)
		e[i] = distribution( generator );
	for ( int i=15; i<20; i++)
		e[i] = distribution( generator );

	// generate 7 random values.
	for ( int i=0; i<nVerts; i++)
	{
		double theta = i * 2*3.1415926 / nVerts;
		xii[i] = xa + ( ra-e[i] ) * cos( theta );
		yii[i] = ya + ( ra-e[i] ) * sin( theta );
	}

	viewport.setClipArea(-1, 10, -1, 10);
	// Processing done.

	viewport.Init(1);
	viewport.ShowHelp();

	glutMainLoop();
	
	return true;
}

