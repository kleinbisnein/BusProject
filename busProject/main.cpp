/**************************
 * Includes and structures
 *
 **************************/

#include <windows.h>
#include <gl/gl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "busProj.h"
FILE *pFileBus;

void processCommandSwitches(int argc, char *argv[], char **ppszBusDataFileName);
void processStopData();


typedef struct NodeT
{
    double longitude;
	double latitude;
} Vertex;

/*
X axis
top longitude is -98.7064313
bottom longitude is -98.228918

Y axis
top latitude is 29.6269373 
bottom latitude is 29.1853647

windowWidth =  longDifference * longitudeScale

windowHeight =  latDifference * latitudeScale

windowWidth and windowHeight are the max window dimensions,
so in this example windowWidth = 1, and windowHeight = 1

latDifference and longDifference is the difference between
maxLongitude and minLongitude, or maxLatitude and minLatitude,
respectively. In this example, latDifference is .4415726, and 
longDifference is -0.4775133 .

Our equations will now look something like this:
2 =  0.4775133 * longitudeScale

2 =   0.4415726 * latitudeScale

Now we divide both sides by the differences,
longitudeScale  =  (2/-0.4775133); which equals -4.188365015173399

latitudeScale  =  (2/0.4415726);  which equals 4.529266535106571

Example:
x = (stop.latitude - minLatitude) * latitude scale
x = ( 29.5059601   - 29.1853647 ) * 4.529266535106571
x = ( 0.3205954) * 4.529266535106571 = 1.452062016529105

then just subtract 1
*/


/**************************
 * Function Declarations
 *
 **************************/

LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
WPARAM wParam, LPARAM lParam);
void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC);
void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC);

/**************
Extra functions
**************/

void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius){
	int i;
	double PI = 3.14159;
	int triangleAmount = 20; //# of triangles used to draw circle
	
	//GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * PI;
	
	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(x, y); // center of circle
		for(i = 0; i <= triangleAmount;i++) { 
			glVertex2f(
		            x + (radius * cos(i *  twicePi / triangleAmount)), 
			    y + (radius * sin(i * twicePi / triangleAmount))
			);
		}
	glEnd();
}

GLfloat rescaleLongitude(double longitude)
{
	double maxLongitude = -98.7064313;
	double minLongitude = -98.228918;
	double longitudeScale = ( 2/(maxLongitude - minLongitude) ); //-4.188365015173399;
	double result;
	
	result = (longitude - minLongitude) * longitudeScale;
	result = result - 1;
	
	return (GLfloat) result;
}

GLfloat rescaleLatitude(double latitude)
{
	double maxLatitude = 29.6269373;
	double minLatitude = 29.1853647;
	double latitudeScale  = ( 2/(maxLatitude - minLatitude) );  //4.529266535106571;
	double result;
	
	result = (latitude - minLatitude) * latitudeScale;
	result = result - 1;
	
	return (GLfloat) result;
}

/**************************
 * WinMain
 *
 **************************/

int WINAPI WinMain (HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine,
                    int iCmdShow)
{
	// Rafael Main Method stuff
		char *pszBusDataFileName = NULL;
	
	// process the command switches
	//processCommandSwitches(argc, argv, &pszBusDataFileName);
	
	//open the bus data file
//	if (pszBusDataFileName == NULL) {
//		exitError(ERR_MISSING_SWITCH, "-c");
//	}
	
	pFileBus = fopen("busData.csv", "r");
	
	// process the reservations
	processStopData();
	
	fclose(pFileBus);
	printf("\n");
	// End Rafael Main Method stuff
    WNDCLASS wc;
    HWND hWnd;
    HDC hDC;
    HGLRC hRC;        
    MSG msg;
    BOOL bQuit = FALSE;
    float theta = 0.0f;

    /* register window class */
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "GLSample";
    RegisterClass (&wc);

    /* create main window */
    hWnd = CreateWindow (
      "GLSample", "OpenGL Sample", 
      WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
      0, 0, 750, 750,
      NULL, NULL, hInstance, NULL);

    /* enable OpenGL for the window */
    EnableOpenGL (hWnd, &hDC, &hRC);
    
    //******************************************************
    //Code for the animation
    Vertex A;
	Vertex B;
	Vertex C;
	Vertex D;

	A.longitude = -98.5896372;
	A.latitude = 29.50749;
	
	B.longitude = -98.5896372;
	B.latitude = 29.55842;
	
	C.longitude = -98.4931211;
	C.latitude = 29.6269373;
	
	D.longitude = -98.585166;
	D.latitude = 29.1853647;
	
	
	//******************************************************

    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage (&msg);
                DispatchMessage (&msg);
            }
        }
        else
        {
            /* OpenGL animation code goes here */
            
            //Setting the background color as black
            glClearColor (1.0f, 1.0f, 1.0f, 1.0f);
            
            //Needed for the program to work
            glClear (GL_COLOR_BUFFER_BIT);
            						
			//Setting the color for the vertices and edges as black
			glColor3f(0.0f, 0.0f, 0.0f);
									
			//Draws the first couple vertices and their connecting edge
			drawFilledCircle(rescaleLongitude(A.longitude), rescaleLatitude(A.latitude), 0.01f);  	//drawing node A
			glBegin(GL_LINES);																		//drawing the edge AB
			glVertex2f(rescaleLongitude(A.longitude), rescaleLatitude(A.latitude));
			glVertex2f(rescaleLongitude(B.longitude), rescaleLatitude(B.latitude));
			glEnd();
			drawFilledCircle(rescaleLongitude(B.longitude), rescaleLatitude(B.latitude), 0.01f);  	//drawing node B
			
			//Draws a node pair going from top latitude to bottom latitude and their edge
			drawFilledCircle(rescaleLongitude(C.longitude), rescaleLatitude(C.latitude), 0.01f);  	//drawing node A
			glBegin(GL_LINES);																		//drawing the edge AB
			glVertex2f(rescaleLongitude(C.longitude), rescaleLatitude(C.latitude));
			glVertex2f(rescaleLongitude(D.longitude), rescaleLatitude(D.latitude));
			glEnd();
			drawFilledCircle(rescaleLongitude(D.longitude), rescaleLatitude(D.latitude), 0.01f);  	//drawing node B
						
			//This goes at the end of your drawings			
			SwapBuffers(hDC);
        }
    }
    
    /* shutdown OpenGL */
    DisableOpenGL (hWnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow (hWnd);

    return msg.wParam;
}


/********************
 * Window Procedure
 *
 ********************/

LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
                          WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
    case WM_CREATE:
        return 0;
    case WM_CLOSE:
        PostQuitMessage (0);
        return 0;

    case WM_DESTROY:
        return 0;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            return 0;
        }
        return 0;

    default:
        return DefWindowProc (hWnd, message, wParam, lParam);
    }
}


/*******************
 * Enable OpenGL
 *
 *******************/

void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC)
{
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC (hWnd);

    /* set the pixel format for the DC */
    ZeroMemory (&pfd, sizeof (pfd));
    pfd.nSize = sizeof (pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | 
      PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    iFormat = ChoosePixelFormat (*hDC, &pfd);
    SetPixelFormat (*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext( *hDC );
    wglMakeCurrent( *hDC, *hRC );

}


/******************
 * Disable OpenGL
 *
 ******************/

void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent (NULL, NULL);
    wglDeleteContext (hRC);
    ReleaseDC (hWnd, hDC);
}

// Rafael Code to read in the data
void processStopData() {
	
	BusData busData;					// structure for holding the bus data
	char *pszGetsResults;				// result of fgets
	char szInputBuffer[MAX_LENGTH];		// entire input line
	int iScanfCount;
	int count = 0;
	
	//pszGetsResults = fgets(szInputBuffer, MAX_LENGTH, stdin);

	//printf("%s\n", pszGetsResults);
	
	if (pszGetsResults == NULL) {
		printf("ERROR: could not read data file\n");
	}
	
	while (fgets(szInputBuffer, MAX_LENGTH, pFileBus) != NULL) {

		//printf("%s\n", szInputBuffer);

		// scan and get the bus data
		iScanfCount = sscanf(szInputBuffer, "%[^,],%[^,],%lf,%lf\n"
			, busData.stopNumber
			, busData.route
			, &busData.x
			, &busData.y
			);
		
		// error check the scanned data
		if (iScanfCount < 4) {
			printf("ERROR: only read %d values\n"
				, iScanfCount
				);
		}
	
		// print out the bus data
		printf("Stop Number = %s Routes = %s Lat = %lf Lon = %lf\n"
			, busData.stopNumber
			, busData.route
			, busData.x
			, busData.y
			);

		//count++;
	}
}

//void processCommandSwitches(int argc, char *argv[], char **ppszBusFileName)
//{
//    int i;
//    
//    for (i = 1; i < argc; i++)
//    {
//        // check for a switch
//        if (argv[i][0] != '-')
//            exitUsage(i, ERR_EXPECTED_SWITCH, argv[i]);
//        // determine which switch it is
//        switch (argv[i][1])
//        {
//            case 'c':                   
//                if (++i >= argc)
//                    exitUsage(i, ERR_MISSING_ARGUMENT, argv[i - 1]);
//                else
//                    *ppszBusFileName = argv[i];
//                break;
//            case '?':
//                exitUsage(USAGE_ONLY, "", "");
//                break;
//            default:
//                exitUsage(i, ERR_EXPECTED_SWITCH, argv[i]);
//        }
//    }
//}
//
//void exitError(char *pszMessage, char *pszDiagnosticInfo)
//{
//    fprintf(stderr, "Error: %s %s\n"
//        , pszMessage
//        , pszDiagnosticInfo);
//    exit(ERROR_PROCESSING);
//}
//
//void exitUsage(int iArg, char *pszMessage, char *pszDiagnosticInfo)
//{
//    if (iArg >= 0)
//        fprintf(stderr, "Error: bad argument #%d.  %s %s\n"
//            , iArg
//            , pszMessage
//            , pszDiagnosticInfo);
//    fprintf(stderr, "reserve -c busDataFile\n");
//    if (iArg >= 0)
//        exit(-1);
//    else
//        exit(-2);
//}

