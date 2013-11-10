#ifndef VCenter_h
#define VCenter_h

#include "VPoint.h"

/*
	A structure that stores the center of circle
*/

struct VCenter
{
public:
    
	double x, y;
	VPoint *p1,*p2,*p3;

    VCenter (double nx, double ny, VPoint *np1, VPoint *np2, VPoint *np3) 
	{
		x = nx; 
		y = ny;
		p1=np1;
		p2=np2;
		p3=np3;
	}
};

#endif