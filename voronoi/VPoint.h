#ifndef VPoint_h
#define VPoint_h

/*
	A structure that stores 2D point
*/

struct VPoint
{
public:
    
	double x, y, r;

	/*
		Constructor for structure; x, y - coordinates
	*/

    VPoint(double nx, double ny, double nr) 
	{
		x = nx; 
		y = ny;
		r = nr;
	}
};

#endif