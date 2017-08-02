/**** typedefs ****/

// Header information
typedef struct {
	char stopNumber[6];
	char route[4];
	double x;
	double y;
} BusData;

// Constants
#define MAX_LENGTH 100

// Boolean
#define FALSE 0
#define TRUE 1

// Error Messages
#define ERR_MISSING_SWITCH          "missing switch"
#define ERR_EXPECTED_SWITCH         "expected switch, found"
#define ERR_MISSING_ARGUMENT        "missing argument for"
#define ERR_BUS_DATA_FILENAME       "invalid bus data file name"

// Program Return Codes
#define ERR_COMMAND_LINE_SYNTAX     -1      // invalid command line syntax
#define USAGE_ONLY                  -2      // show usage only
#define ERROR_PROCESSING            -3

// Prototypes
void exitError(char *pszMessage, char *pszDiagnosticInfo);
void exitUsage(int iArg, char *pszMessage, char *pszDiagnosticInfo);
