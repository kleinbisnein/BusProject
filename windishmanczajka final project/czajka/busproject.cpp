#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "busproject.h"

int main(int argc, char *argv[])
{
    Location    *pHead = NULL;         	// Points to first node in the list
    char        *pszBusFileName = NULL;	//name of bus routes file
    
    
    //make locations
    int i;
	for(i = 1; i <= 6; i++) {
		Location * v = (Location *)malloc(sizeof(Location));
		v->stopNum = i;
		v->distance = 999999;
		v->inGraph = true;
		v->prevLoc = NULL;
		v->x = i*75;
		v->y = i*75;
		insertLoc(&pHead, v);
	}
	
	//make routes
	insertRoute(searchLoc(pHead,1),searchLoc(pHead,2),2);
	insertRoute(searchLoc(pHead,1),searchLoc(pHead,3),4);
	insertRoute(searchLoc(pHead,2),searchLoc(pHead,3),1);
	insertRoute(searchLoc(pHead,2),searchLoc(pHead,4),4);
	insertRoute(searchLoc(pHead,2),searchLoc(pHead,5),2);
	insertRoute(searchLoc(pHead,3),searchLoc(pHead,5),3);
	insertRoute(searchLoc(pHead,5),searchLoc(pHead,4),3);
	insertRoute(searchLoc(pHead,5),searchLoc(pHead,6),2);
	insertRoute(searchLoc(pHead,4),searchLoc(pHead,6),2);
	
	
	//processCommandSwitches(argc, argv, &pszBusFileName);
    // get the flights and print them
    //getStops(&pHead, pszBusFileName);
    return 0;
}
/****************************************************
Reads pszBusFileName and creates locations and routes
based upon the data read in from the file
****************************************************/
Location getStops(Location **ppHead, char *pszBusFileName)
{
    char szInputBuffer[200];		// input buffer for reading data
    int i = 0;                      // subscript in flightM    
    int iScanfCnt;                  // returned by sscanf
    FILE *pBusFile;              // Stream Input for Flights data. 
    Location *fromLoc, *toLoc;                  // One flight
    int fromArr, fromDep, toArr, toDep;
    int thisRoute, prevRoute;
    double minLat = 10000, maxLat = -10000, minLong = 10000, maxLong = -10000;
         
    Location *pHead = NULL;

    /* open the Flights stream data file */
    //if (pszBusFileName == NULL)
    //    exitError(ERR_MISSING_SWITCH, "-b");

    pBusFile = fopen(pszBusFileName, "r");
    //if (pBusFile == NULL)
    //    exitError(ERR_FLIGHT_FILENAME, pszBusFileName);
    /* get flight information until EOF
    ** fgets returns null when EOF is reached.
    */
    while(fgets(szInputBuffer, 200, pBusFile) != NULL)
    {
    	printf("a");
        iScanfCnt = sscanf(szInputBuffer, "%d, %d, %[^,]s, %f, %f, %d, %d"
        	, &thisRoute
            , &toLoc->stopNum
            , toLoc->name
            , &toLoc->y
            , &toLoc->x
			, &toArr
			, &toDep);
		printf("b");
		//Finds min and max long/lat
		if(toLoc->x < minLat)
			minLat = toLoc->x;
		if(toLoc->x > maxLat)
			maxLat = toLoc->x;
		if(toLoc->y < minLong)
			minLong = toLoc->y;
		if(toLoc->y > maxLong)
			maxLong = toLoc->y;
        printf("c");
        // Insert the location into the LL if the location does not already exist
        if(searchLoc(pHead, toLoc->stopNum) == NULL)
        {
        	printf("z");
        	insertLoc(&pHead, toLoc);
		}
        	
        printf("d");
        //Add a route if it is not the first stop in the cycle
        if(thisRoute != prevRoute)
			prevRoute = thisRoute;
        else
        {
        	printf("%s",searchLoc(pHead,toLoc->stopNum)->name);
			insertRoute(searchLoc(pHead,fromLoc->stopNum), searchLoc(pHead,toLoc->stopNum), (toArr - fromDep));
			printf("f");
		}
        
        fromLoc->stopNum = toLoc->stopNum;
        strcpy(fromLoc->name,toLoc->name);
        fromLoc->y = toLoc->y;
        fromLoc->x = toLoc->x;
        fromArr = toArr;
        fromDep = toDep;
        printf("\n");
    }
    int latScale = 2/(maxLat - minLat);
    int longScale = 2/(maxLong-minLong);
    fclose(pBusFile);
    return *pHead;
}

/****************************************************
Finds shortest path from source to target
****************************************************/
Location * Dijikstra(Location * stops, Location * source, Location * target)
{
	//set source vertex distance to zero(0)
	source->distance = 0;


	//while some vertex is a adjacent candidate
	while(locInGraph(stops)) {
		// vertex with min distance from source
		Location * u = findMin(stops);

		//remove vertex 'u' from set Q
		u->inGraph = false;
		
		//for every neighbor
		while(u->adjacent != NULL)
		{
			int alt = u->distance + u->adjacent->weight;
			if(alt  <  u->adjacent->v->distance) {
				u->adjacent->v->distance = alt;
				u->adjacent->v->prevLoc = u;
			}
			u->adjacent = u->adjacent->pNext;
		}  // end inner while
		
	} // end outer while
	
	//push target onto Stack
	//push(target->vertexId);
	
	//construct path from source --> target
	Location * path = target;
	Location * prev = target->prevLoc;

	while(prev != NULL){
		insertLoc(&path, prev);
		prev = prev->prevLoc;
	}

	//push source
	/*push(source->vertexId);

	

	while(!isEmptyStack()) 
	{
		int vertexId = pop();
		Vertex * v =  getVertex(graph.verticesLL_head,  vertexId);

		insertLoc(&path, v);

	}*/

	return path;     //shortest path from source to target
   
} // end function

/****************************************************
Checks for unvisited locations.
Returns true if there are locations that have not
been visited.
****************************************************/
bool locInGraph(Location * head) 
{

	Location * ptr = head;

	while(ptr != NULL) {
		if(ptr->inGraph == true)
			return true;
		else
			ptr = ptr->pNext;
	}
	return false;
}

/****************************************************
Finds location with min distance from source
****************************************************/
Location * findMin(Location * head)
{

	Location * ptr = head;
	int min = 999999;
	Location * minVertex = NULL;

	while(ptr != NULL) {
		if(ptr->distance < min && ptr->inGraph != false) {
			min = ptr->distance;
			minVertex = ptr;
		}
		ptr = ptr->pNext;
	}
	return minVertex;
}

/****************************************************
Adds a location to the beginning ofthe LL
****************************************************/
void insertLoc(Location **pHead, Location *location)
{
	Location * pNew = (Location *)malloc(sizeof(Location));  //(Vertex_LL *) casts memory from malloc
	pNew = location;
	pNew->pNext = *pHead;
	*pHead = pNew;
}

/****************************************************
Makes toLoc and fromLoc point to a new route with
weight of weight. The routes also point to toLoc and
fromLoc
****************************************************/
Route *insertRoute(Location *fromLoc, Location *toLoc, int weight)
{
	Route * pNew;
    Route * tempFrom;
    Route * tempTo;

	pNew = new Route;
	pNew->u = fromLoc;
	pNew->v = toLoc;
	pNew->weight = weight;

    tempFrom = fromLoc->adjacent;
    tempTo = toLoc->adjacent;
   
    if(fromLoc->adjacent != NULL)
    {
    	pNew->pNext = fromLoc->adjacent;
    	fromLoc->adjacent = pNew;
	}
	else
		fromLoc->adjacent = pNew;
		
	if(toLoc->adjacent != NULL)
    {
    	pNew->pNext = toLoc->adjacent;
    	toLoc->adjacent = pNew;
	}
	else
		toLoc->adjacent = pNew;
	
	return pNew;
}

/****************************************************
Searches the linked list of locations for the one 
with the same name
****************************************************/
Location *searchLoc(Location *pHead, int stopNum)
{
	Location *p = pHead;
    while(p != NULL)
    {
        if(p->stopNum == stopNum)
        	return p;
        p = p->pNext;
    }
    return NULL;
}
/****************************************************
Makes a memory space for a new location
****************************************************/
Location *allocateLoc(Location location)
{
    Location *pNew = new Location;
    if (pNew == NULL)
    {
    	printf("Not enough space for a new location");
    	return pNew;
	}
	strcpy(pNew->name,location.name);
    pNew->adjacent = NULL;
    pNew->x = location.x;
	pNew->y = location.y;
	pNew->pNext = NULL;
	pNew->distance = 999999;
	pNew->prevLoc = NULL;
	pNew->inGraph = true;
    return pNew;
}

//Print
void printStops(Location * head)
{
	Location *p = head;
	while(p != NULL)
	{
		printf("Location Name: %d\n",p->stopNum);
		p = p->pNext;
	}
}



















/****************************************************
-
****************************************************/
void processCommandSwitches(int argc, char *argv[], char **ppszBusFileName)
{
    int i;
    int rc = 0;
    int bShowCommandHelp = FALSE;

    for (i = 1; i < argc; i++)
    {
        // check for a switch
        if (argv[i][0] != '-')
            exitUsage(i, ERR_EXPECTED_SWITCH, argv[i]);
        // determine which switch it is
        switch (argv[i][1])
        {
        case 'b':                   // Flight File Name
            if (++i >= argc)        // There must be an additional argument
                exitUsage(i, ERR_MISSING_ARGUMENT, argv[i - 1]);
            else
                *ppszBusFileName = argv[i];
            break;
        case '?':
            exitUsage(USAGE_ONLY, "", "");
            break;
        default:
            exitUsage(i, ERR_EXPECTED_SWITCH, argv[i]);
        }
    }
}

void exitError(char *pszMessage, char *pszDiagnosticInfo)
{
    fprintf(stderr, "Error: %s %s\n"
        , pszMessage
        , pszDiagnosticInfo);
    exit(ERROR_PROCESSING);
}

void exitUsage(int iArg, char *pszMessage, char *pszDiagnosticInfo)
{
    if (iArg >= 0)
        fprintf(stderr, "Error: bad argument #%d.  %s %s\n"
        , iArg
        , pszMessage
        , pszDiagnosticInfo);
    fprintf(stderr, "p5 -f flightFileName -c commandFileName\n");
    if (iArg >= 0)
        exit(-1);
    else
        exit(-2);
}
