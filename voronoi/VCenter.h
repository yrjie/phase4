#ifndef VCenter_h
#define VCenter_h

#include "VPoint.h"

/*
	A structure that stores 2D point
*/

struct VCenter
{
public:
    
	double x, y;
	VPoint *p1,*p2,*p3;

	/*
		Constructor for structure; x, y - coordinates
	*/

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