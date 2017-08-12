//DEFINE CONSTANTS
// boolean
#define FALSE           0
#define TRUE            1

// error Messages 
#define ERR_MISSING_SWITCH          "missing switch"
#define ERR_EXPECTED_SWITCH         "expected switch, found "
#define ERR_MISSING_ARGUMENT        "missing argument for "
#define ERR_COMMAND_FILENAME        "invalid command file name "
#define ERR_CUSTOMER_ID_DATA        "invalid customer id data "
#define ERR_CUSTOMER_ADDRESS_DATA   "invalid customer address data "
#define ERR_RESERVATION_DATA        "invalid customer reservation data "
#define ERR_FLIGHT_FILENAME         "invalid flight definition file name "
#define ERR_FLIGHT_DEF_DATA         "invalid flight definition data "
#define ERR_TOO_MANY_FLIGHTS        "attempted to put more than MAX_FLIGHTS in the flight array, found "
#define ERR_FLIGHT_NOT_FOUND        "flight ID not found "
#define ERR_TOO_FEW_SEATS           "not enough available seats to satisfy request "
#define ERR_CUSTOMER_SUB_COMMAND    "invalid CUSTOMER subcommand "
#define ERR_FLIGHT_SUB_COMMAND      "invalid FLIGHTsubcommand "
#define ERR_INVALID_COMMAND         "invalid command "
#define ERR_INCREASE_SUB_COMMAND    "invalid INCREASE subcommand "
#define ERR_SHOW_SUB_COMMAND        "invalid SHOW subcommand "
#define ERR_FLIGHT_EXISTS           "flight already exists"

// program return codes
#define ERR_COMMAND_LINE_SYNTAX     -1      // invalid command line syntax
#define USAGE_ONLY                  -2      // show usage only
#define ERROR_PROCESSING            -3

//STRUCTS
typedef struct Location
{
	char name[50];
	int stopNum;
	int distance;
	double x, y;
	bool inGraph;
	struct Route *adjacent;
	struct Location *pNext;
	struct Location *prevLoc;
} Location;

typedef struct Route
{
	int weight;
	struct Route * pNext;
	struct Location *u;
	struct Location *v;
} Route;

typedef struct Graph
{
	struct Location *lHead;
	struct Route *rHead;
} Graph;

typedef struct
{
	char fromLoc[50], toLoc[50];
} Request;

//PROTOTYPES
//stop and route functions
Location getStops(Location **ppHead,char * buslist);
Location *searchLoc(Location *p, int stopNum);
Location *insertLoc(Location *p, Location location);
Route *insertRoute(Location *fromLoc, Location *toLoc, int weight);
Location *allocateLoc(Location location);
void printNames(Location *p);
void printWeights(Location *p);

//dijikstra
Location * Dijikstra(Graph graph, Location * source, Location * target);
bool locInGraph(Location *head);
Location * findMin(Location * head);

void processCommandSwitches(int argc, char *argv[], char **ppszBusFileName);

//graphics
void drawGraph();

//printing
void printStops(Location * head);


// Error handling functions
void exitError(char *pszMessage, char *pszDiagnosticInfo);
void exitUsage(int iArg, char *pszMessage, char *pszDiagnosticInfo);
