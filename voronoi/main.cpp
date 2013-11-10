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

int crossProduct(VPoint *p1, VPoint *p2, VPoint *q1, VPoint *q2){
	double x1=p2->x-p1->x,y1=p2->y-p1->y,x2=q2->x-q1->x,y2=q2->y-q1->y,cp;
	cp=x1*y2-x2*y1;
	if (abs(cp)<1e-8) return 0;
	if (cp>1e-8) return 1;
	if (cp<1e-8) return -1;
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
	double x,y,r, xt,yt;

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
		xt=(*i)->p1->x-(*i)->x;
		yt=(*i)->p1->y-(*i)->y;
		if (sqrt(xt*xt+yt*yt)<(*i)->p1->r)
			drawATriangle((*i)->p1,(*i)->p2,(*i)->p3);
	}

	for(Edges::iterator i = edg->begin(); i!= edg->end(); ++i){
		double dist1, dist2;
		VPoint *p1=(*i)->start,*p2=(*i)->end,*lp=(*i)->left,*rp=(*i)->right;
		double f=(*i)->f, g=(*i)->g;
		if (crossProduct(lp, rp, lp, p1)*crossProduct(lp, rp, lp, p2)<=0){
			dist1=abs((f*lp->x-lp->y+g)/sqrt(f*f+1));
			if (dist1<lp->r)
				drawALine(dx+lp->x/w,  dy+lp->y/w, dx+rp->x/w, dy+rp->y/w, 1);
		}
		else {
			xt=p1->x-lp->x;
			yt=p1->y-lp->y;
			dist1=sqrt(xt*xt+yt*yt);
			xt=p2->x-lp->x;
			yt=p2->y-lp->y;
			dist2=sqrt(xt*xt+yt*yt);
			if (dist1<lp->r||dist2<lp->r)
				drawALine(dx+lp->x/w,  dy+lp->y/w, dx+rp->x/w, dy+rp->y/w, 1);
		}
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
	drawAlpha();
	drawText();
	glutSwapBuffers();
}

void reshape (int width, int height) 
{
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(22.5, (GLfloat)width / (GLfloat)height, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
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
	glutInitWindowSize (1000, 750);
	glutInitWindowPosition (50, 50);
	glutCreateWindow ("Alpha shape");
	glClearColor (1.0,1.0,1.0, 1.0);
	glutDisplayFunc(display);

	glutReshapeFunc(reshape);
	glutMouseFunc(MyMouse);

	glutMainLoop(); // Enter GLUT's main loop
	
	return 0;
}