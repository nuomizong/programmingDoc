

/*****************************************************
Explanation:
	When drag-drop a file onto an application in Explorer 
	it does nothing more than to pass the file names as 
	arguments to the program. Likewise for multiple files.

	In order to get a list of file names, then just 
	iterate over all arguments, do whatever you want with 
	them and be done. This will work for zero to almost 
	arbitrary many arguments.

nuomizong 16/06/2015.
******************************************************/


#include <cstdio>
#include <cstring>

void FindMirrorPointAboutLine(double x1, double y1, double k, double b, double & x2, double & y2)
{
	/*********************
	Horizontal line
	**********************/
	if ( k == 0.0 )
	{
		x2 = x1;
		y2 = 2.0*b - y1;
		return;
	}

	x2 = ((1.0- k*k)*x1+2.0*k*y1-2.0*k*b)/(k*k+1.0);
	y2 = y1 + k * (x1-x2);
}

int main(int argc, char *argv[])
{
	std::printf("Good Morning, My Princess!\n");
	

	double x1 = 0.0, y1 = 2.0, x2, y2;
	double k = 1.0, b= 3.0;

	FindMirrorPointAboutLine(x1, y1, k, b, x2, y2);

	std::printf("x2=%lf, y2=%lf\n", x2, y2);

	std::getchar();

	return true;
}