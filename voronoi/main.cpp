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

Voronoi * v;
Vertices * ver;
Vertices * dir;
Edges * edg;

double w = 1000;
const double PI=acos(-1.0);

void drawAPoint(double x,double y)
{
	glPointSize(5);
	glBegin(GL_POINTS);
	glColor3f(0,0,0);
		glVertex2d(x,y);
	glEnd();
	glPointSize(1);
}

void drawALine(double x1,double y1, double x2, double y2)
{
	glPointSize(1);
	glBegin(GL_LINE_LOOP);
	glColor3f(0,0,1);
		glVertex2d(x1,y1);
		glVertex2d(x2,y2);
	glEnd();
}

void drawACircle(double x, double y,double r){
	int n=100;
	double angle=2*PI/n;
	glBegin(GL_POLYGON);
	glColor3f(0,0.5,0);
	for (int i=0;i<n;i++){
		glVertex2f(x+r*cos(angle*i), y+r*sin(angle*i));
	}
	glEnd();
}

void readFile(){

	string line_noStr;

	string line;   // each line of the file
	string command;// the command of each line
	string numberStr; // for single LongInt operation

	ifstream inputFile("input.txt",ios::in);
	int x,y,r;
	if(inputFile.fail()){
		cerr << "Error: Cannot read input file \"" << "input.txt" << "\"";
		return;
	}
	while(inputFile.good()){
		getline(inputFile,line);
		stringstream linestream(line);
		linestream >> x >> y >>r;
		ver->push_back(new VPoint(10*x , 10*y, r ));
	}
}

void drawVoronoi()
{	
	double x,y,r;
	edg = v->GetEdges(ver, w, w);

	for(Vertices::iterator i = ver->begin(); i!= ver->end(); ++i)
	{
		x=-1+2*(*i)->x/w;
		y=-1+2*(*i)->y/w;
		r=(*i)->r;
		drawACircle(x,y,r/10);
	}

	for(Vertices::iterator i = ver->begin(); i!= ver->end(); ++i)
	{
		x=-1+2*(*i)->x/w;
		y=-1+2*(*i)->y/w;
		r=(*i)->r;
		drawAPoint(x,y);
		//drawAPoint((*i)->x, (*i)->y);
	}

	for(Edges::iterator i = edg->begin(); i!= edg->end(); ++i)
		drawALine(-1+2*(*i)->start->x/w,  -1+2*(*i)->start->y/w, -1+2*(*i)->end->x/w, -1+2*(*i)->end->y/w);
}

void display (void) 
{
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -5.0f); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//drawAPoint(650,450);
	drawVoronoi();
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

int main (int argc, char **argv) 
{
	v = new Voronoi();
	ver = new Vertices();
	dir = new Vertices();

	readFile();

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (1000, 750); // Set the width and height of the window
	glutInitWindowPosition (50, 50); // Set the position of the window
	glutCreateWindow ("Alpha shape"); // Set the title for the window
	glClearColor (1.0,1.0,1.0, 1.0);
	glutDisplayFunc(display); // Tell GLUT to use the method "display" for rendering

	glutReshapeFunc(reshape); // Tell GLUT to use the method "reshape" for reshaping

	//glutKeyboardFunc(keyPressed); // Tell GLUT to use the method "keyPressed" for key presses
	//glutKeyboardUpFunc(keyUp); // Tell GLUT to use the method "keyUp" for key up events

	glutMainLoop(); // Enter GLUT's main loop
	
	return 0;
}