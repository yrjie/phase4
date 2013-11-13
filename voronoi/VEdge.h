#ifndef VEdge_h
#define VEdge_h

#include "Vpoint.h"
#include <iostream>
#include <cmath>

/*
	A class that stores an edge in Voronoi diagram

	start		: pointer to start point
	end			: pointer to end point
	left		: pointer to Voronoi place on the left side of edge
	right		: pointer to Voronoi place on the right side of edge

	neighbour	: some edges consist of two parts, so we add the pointer to another part to connect them at the end of an algorithm

	direction	: directional vector, from "start", points to "end", normal of |left, right|
	f, g		: directional coeffitients satisfying equation y = f*x + g (edge lies on this line)
*/

class VEdge
{
public:

	VPoint *	start;
	VPoint *	end;
	VPoint *	direction;
	VPoint *	left;
	VPoint *	right;
	
	double		f;
	double		g;
	int cy;

	VEdge * neighbour;

	/*
		Constructor of the class

		s		: pointer to start
		a		: pointer to left place
		b		: pointer to right place
	*/

	VEdge(VPoint * s, VPoint * a, VPoint * b)
	{
		start		= s;
		left		= a;
		right		= b;
		neighbour	= 0;
		end			= 0;

		if (abs(a->y - b->y)<1e-8){
			cy=0;
			f=1;
			g=-(b->x + a->x)/2+(a->r*a->r-b->r*b->r)/(a->x-b->x)/2;
		}
		else {
			cy=1;
			f = (b->x - a->x) / (a->y - b->y) ;
			g = s->y - f * s->x ;
		}
		direction = new VPoint(b->y - a->y, -(b->x - a->x),0);
	}

	/*
		Destructor of the class
		direction belongs only to the current edge, other pointers can be shared by other edges
	*/

	~VEdge()
	{
		delete direction ;
	}
	
};

#endif