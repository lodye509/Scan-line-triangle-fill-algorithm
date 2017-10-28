
#include <GL/glut.h>
#include <GL/glu.h>
//#include <OpenGL/gl.h> // for Xcode
//#include <OpenGL/glu.h>
//#include <GLUT/glut.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>

float *pixel;
void display();
const int width = 200, length = 200;

typedef struct Edge
{
	int yMax;
	float x, dx;
	struct Edge * pNext;
} Edge, *lEdge;

void init (void)
{
    glClearColor (1.0, 1.0, 0.0, 1.0);  // Set display-window color to white.
    glMatrixMode (GL_PROJECTION);       // Set projection parameters.
    gluOrtho2D (0.0, 200.0, 0.0, 150.0);
}

int getMin(int a, int b, int c)
{
	int temp;
	if (a > b)
	{
		temp = b;
		if(b > c)
		{
			temp = c;
		}
	}
	else if(a > c)
	{
		temp = c;
		if(c > b)
		{
			temp = b;
		}
	}
	else temp = a;

	return temp;
}

int getMax(int a, int b, int c)
{
	int temp;
	if (a > b)
	{
		temp = a;
		if(c > a)
			temp = c;
	}
	else if(b > c)
	{
		temp = b;
		if(a > b)
			temp = a;
	}
	else if(c > a)
	{
		temp = c;
		if(b > c)
			temp = b;
	}

	return temp;
}

void setPixel(int x, int y, float r, float g, float b)
{
	pixel[3*y*width+x*3+0] = r;
	pixel[3*y*width+x*3+1] = g;
    pixel[3*y*width+x*3+2] = b;
}

void pixelDraw(int x0, int y0, int xEnd, int yEnd, float r, float g, float b){
    int dx = abs(xEnd - x0);
    int dy = abs(yEnd - y0);
    int x,y;
    int twoDy = 2 * dy;
    int twoDx = 2 * dx;
    int twoDyMiDx = 2 * (dy - dx);
    int twoDxMiDy = 2 * (dx - dy);
    int p = twoDy - dx;
    int iDy = (yEnd - y0) > 0 ? 1:-1;
        
    if(x0 > xEnd)
    {
        x = xEnd;
        y = yEnd;
    }
    else{
        x = x0;
        y = y0;
    }
        
    for(x=x0;x<=xEnd;x++)
        setPixel(x,y,r,g,b);
}

void fillTri(int x1, int y1, int x2, int y2, int x3, int y3)
{
	int ymin, ymax;
	ymin = getMin(y1, y2, y3);
	ymax = getMax(y1, y2, y3);
	int a0 = 0, a1 = 0;
	lEdge pAET = NULL;
	lEdge *pNET = NULL;

	pAET = new Edge();
	pAET->pNext = NULL;

	//a0 = a1 = y0;
	int lengthNET = ymax - ymin;
	pNET = new Edge*[lengthNET + 1];

	for(int i=0; i < lengthNET+1 ; i++)
	{
		pNET[i] = new Edge();
		pNET[i]->pNext = NULL;
	}

	for(int i = ymin; i < ymax; i+=1)
	{
		Edge *pEdgeFirst = pNET[i - ymin];

		pAET = new Edge; 
		pAET->pNext = NULL;
	}
}

void triangleDraw(double x0, double y0, double x1, double y1, double x2, double y2, float r, float g, float b)
{
    typedef struct Edge
    {
        int yMax;
        float dx;
        float x;
        struct Edge *eNext;
    }Edge, *lEdge;
    
    int yMin = getMin(y0, y1, y2);
    int yMax = getMax(y0, y1, y2);
    double y = y0;
    double xLeft = x0, xRight = x0;
    double m1 = (y1-y0)/(x1-x0);
    double m2 = (y2-y1)/(x2-x1);
    double m3 = (y2-y0)/(x2-x0);
    
    lEdge pAET = NULL;
    lEdge *pET = NULL;
    
    pAET = new Edge;
    pAET->eNext = NULL;
    /*
    pET = new lEdge[y2 - y0 + 1];
    for(int i = 0; i <= y2-y0;i++)
    {
        pET[i] = new Edge;
        pET[i]->eNext = NULL;
    }
	*/
    //Draw vertex
    setPixel(x0, y0, r, g, b);
    setPixel(x1, y1, r, g, b);
    setPixel(x2, y2, r, g, b);
    
    if(y0 != y1)
    {
        lEdge pE;
        
        pE = new Edge;
        pE->yMax = y1;
        pE->x = x0;
        pE->dx = (x1 - x0)/(y1 - y0);
        pE->eNext = NULL;
        
        while(y < y2)
        {
            y++;
            if(y != y1)
            {
                xLeft = xLeft+ 1/m1;
                xRight = xRight + 1/m3;
                pixelDraw(xLeft, y, xRight, y, r, g, b);
            }
            else if(y == y1)
            {
                xLeft = xLeft+ 1/m1;
                xRight = xRight + 1/m3;
                pixelDraw(xLeft, y, xRight, y, r, g, b);
                m1 = m2;
            }
        }
    }
    else if(y0 == y1)
    {
        xRight = x1;
        pixelDraw(x0, y0, x1, y1, r, g, b);
        while(y < y2)
        {
            y++;
            xLeft = xLeft+ 1/m1;
            xRight = xRight + 1/m3;
            pixelDraw(xLeft, y, xRight, y, r, g, b);
        }
    }
}

void display (void)
{
	 glClear (GL_COLOR_BUFFER_BIT);  // Clear display window.
	 glLoadIdentity();

     glDrawPixels(width, length, GL_RGB, GL_FLOAT, pixel);
	 glFlush ( );
}
int main(int argc,char** argv)
{
	pixel = new float[3*width*length];
    glutInit (&argc, argv);                         // Initialize GLUT.
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);   // Set display mode.
    glutInitWindowPosition (400, 200);   // Set top-left display-window position.
    glutInitWindowSize (width, length);      // Set display-window width and height.
    glutCreateWindow ("ECS 175 Assigtment 2"); // Create display window.

	//           x0  y0  x1   y1   x2    y2   r  g    b
    triangleDraw(45, 30, 30,  120, 195,  130, 1, 0.8, 0);
	triangleDraw(30, 10, 20,  50,  50,   75,  0, 0.3, 1);

    init ( );                            // Execute initialization procedure.
    glutDisplayFunc (display);       // Send graphics to display window.
    glutMainLoop ( );                    // Display everything and wait.
    return 0;
}