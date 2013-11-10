#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <time.h>
#include <string>
#include <fstream>
#include <strstream>
#include <sstream>
#include "Voronoi.h"
#include "VPoint.h"

using namespace std;
using namespace vor;

#define GLUT_WHEEL_UP 3
#define GLUT_WHEEL_DOWN 4
#define ALPHA_INCREMENTAL 1.0
#define WEIGHT_INCREMENTAL 1000

Voronoi * v;
Vertices * ver;
Centers * cent;
Edges * edg;

double w = 1000, dx=-0.2, dy=-0.5;
const double PI=acos(-1.0);

double alpha = 0.0;

void drawAPoint(double x,double y, bool isCent)
{
	glPointSize(5);
	glBegin(GL_POINTS);
	if (!isCent)
		glColor3f(0,0,0);
	else glColor3f(0,0,1);
		glVertex2d(x,y);
	glEnd();
	glPointSize(1);
}

void drawALine(double x1,double y1, double x2, double y2, bool isAlph)
{
	if (!isAlph){
		glLineWidth(1);
		glColor3f(0,0,1);
	}
	else {
		glLineWidth(2);
		glColor3f(0,0,0);
	}
	glBegin(GL_LINE_LOOP);
		glVertex2d(x1,y1);
		glVertex2d(x2,y2);
	glEnd();
}

void drawACircle(double x, double y,double r, bool isPoly){
	int n=100;
	double angle=2*PI/n;
	if (isPoly){
		glBegin(GL_POLYGON);
		glColor3f(0,0.5,0);
	}
	else {
		glLineWidth(1);
		glBegin(GL_LINE_LOOP);
		glColor3f(0.5,0.5,0.5);
	}
	for (int i=0;i<n;i++){
		glVertex2f(x+r*cos(angle*i), y+r*sin(angle*i));
	}
	glEnd();
}

void drawATriangle(VPoint *p1, VPoint *p2, VPoint *p3){
	glLineWidth(2);
	glBegin(GL_POLYGON);
	glColor3f(1, 0, 0);
	glVertex2f(dx+p1->x/w,dy+p1->y/w);
	glVertex2f(dx+p2->x/w,dy+p2->y/w);
	glVertex2f(dx+p3->x/w,dy+p3->y/w);
	glEnd();
}

void readFile(){

	string line_noStr;

	string line;   // each line of the file
	string command;// the command of each line
	string numberStr; // for single LongInt operation

	ifstream inputFile("input5.txt",ios::in);
	int x,y,r;
	if(inputFile.fail()){
		cerr << "Error: Cannot read input file \"" << "input.txt" << "\"";
		return;
	}
	while(inputFile.good()){
		getline(inputFile,line);
		stringstream linestream(line);
		linestream >> x >> y >>r;
		ver->push_back(new VPoint(10*x , 10*y, 10*r ));
	}
}

void drawAlpha()
{	
	double x,y,r;

	for(Vertices::iterator i = ver->begin(); i!= ver->end(); ++i)
	{
		x=dx+(*i)->x/w;
		y=dy+(*i)->y/w;
		r=(*i)->r/w;
		drawACircle(x,y,r,1);
	}

	for(Vertices::iterator i = ver->begin(); i!= ver->end(); ++i)
	{
		x=dx+(*i)->x/w;
		y=dy+(*i)->y/w;
		r=(*i)->r/w;
		drawACircle(x,y,r,0);
	}

	for(Centers::iterator i = cent->begin(); i!= cent->end(); ++i){
		double xt,yt;
		xt=(*i)->p1->x-(*i)->x;
		yt=(*i)->p1->y-(*i)->y;
		if (sqrt(xt*xt+yt*yt)<(*i)->p1->r)
			drawATriangle((*i)->p1,(*i)->p2,(*i)->p3);
	}

	for(Edges::iterator i = edg->begin(); i!= edg->end(); ++i){
		double f=(*i)->f, g=(*i)->g;
		VPoint *p0=(*i)->left,*p1=(*i)->right;
		double dist=abs((f*p0->x-p0->y+g)/sqrt(f*f+1));
		if (dist<p0->r)
			drawALine(dx+p0->x/w,  dy+p0->y/w, dx+p1->x/w, dy+p1->y/w, 1);
	}

	for(Vertices::iterator i = ver->begin(); i!= ver->end(); ++i)
	{
		x=dx+(*i)->x/w;
		y=dy+(*i)->y/w;
		drawAPoint(x,y,0);
	}

	for(Centers::iterator i = cent->begin(); i!= cent->end(); ++i)
	{
		x=dx+(*i)->x/w;
		y=dy+(*i)->y/w;
		drawAPoint(x,y,1);
	}

	for(Edges::iterator i = edg->begin(); i!= edg->end(); ++i)
		drawALine(dx+(*i)->start->x/w,  dy+(*i)->start->y/w, dx+(*i)->end->x/w, dy+(*i)->end->y/w, 0);
}

void drawText(){
	stringstream ss;
	ss << "Alpha = " << alpha << endl;
	string s = ss.str();

	glColor3f(0.0, 0.0, 0.0);
	glRasterPos2f(-1000/w, 750/w);
	for(int i=0; i < s.length(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s.at(i));
}

void display (void) 
{
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -5.0f); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//drawAPoint(650,450);
	drawAlpha();
	drawText();
	glutSwapBuffers();
}

void reshape (int width, int height) 
{
	glViewport(0, 0, (GLsizei)width, (GLsizei)height); // Set our viewport to the size of our window
	glMatrixMode(GL_PROJECTION); // Switch to the projection matrix so that we can manipulate how our scene is viewed
	glLoadIdentity(); // Reset the projection matrix to the identity matrix so that we don't get any artifacts (cleaning up)
	//gluOrtho2D(0,width,height,0);
	gluPerspective(22.5, (GLfloat)width / (GLfloat)height, 1.0, 100.0); // Set the Field of view angle (in degrees), the aspect ratio of our window, and the new and far planes
	glMatrixMode(GL_MODELVIEW); // Switch back to the model view matrix, so that we can start drawing shapes correctly
}

void MyMouse(int button, int state, int x, int y){
	double addedWeight = 0.0;
	if(state == GLUT_UP){
		if(button == GLUT_WHEEL_UP){
			alpha += ALPHA_INCREMENTAL;
			addedWeight = WEIGHT_INCREMENTAL;
		}
		else if (button == GLUT_WHEEL_DOWN && alpha >= 1.0){
			alpha -= ALPHA_INCREMENTAL;
			addedWeight = -WEIGHT_INCREMENTAL;
		}

		for(Vertices::iterator i = ver->begin(); i != ver->end(); i++)
			(*i)->r = sqrt( (*i)->r * (*i)->r + addedWeight );
	}

	glutPostRedisplay();
}

int main (int argc, char **argv) 
{
	v = new Voronoi();
	ver = new Vertices();
	cent=new Centers();

	readFile();
	
	edg = v->GetEdges(ver, w, w, cent);

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (1000, 750); // Set the width and height of the window
	glutInitWindowPosition (50, 50); // Set the position of the window
	glutCreateWindow ("Alpha shape"); // Set the title for the window
	glClearColor (1.0,1.0,1.0, 1.0);
	glutDisplayFunc(display); // Tell GLUT to use the method "display" for rendering

	glutReshapeFunc(reshape); // Tell GLUT to use the method "reshape" for reshaping
	glutMouseFunc(MyMouse);
	//glutKeyboardFunc(keyPressed); // Tell GLUT to use the method "keyPressed" for key presses
	//glutKeyboardUpFunc(keyUp); // Tell GLUT to use the method "keyUp" for key up events

	glutMainLoop(); // Enter GLUT's main loop
	
	return 0;
}