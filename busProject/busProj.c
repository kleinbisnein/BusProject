#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "busProj.h"
FILE *pFileBus;

void processCommandSwitches(int argc, char *argv[], char **ppszBusDataFileName);
void processStopData();

int notmain() {
	
	char *pszBusDataFileName = NULL;
	
	// process the command switches
	//processCommandSwitches(argc, argv, &pszBusDataFileName);
	
	//open the bus data file
//	if (pszBusDataFileName == NULL) {
//		exitError(ERR_MISSING_SWITCH, "-c");
//	}
	
	pFileBus = fopen("busData.csv", "r");
	if (pFileBus == NULL) {
		exitError(ERR_BUS_DATA_FILENAME, pszBusDataFileName);
	}
	
	// process the reservations
	processStopData();
	
	fclose(pFileBus);
	printf("\n");
	return 0;
}

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

void processCommandSwitches(int argc, char *argv[], char **ppszBusFileName)
{
    int i;
    
    for (i = 1; i < argc; i++)
    {
        // check for a switch
        if (argv[i][0] != '-')
            exitUsage(i, ERR_EXPECTED_SWITCH, argv[i]);
        // determine which switch it is
        switch (argv[i][1])
        {
            case 'c':                   
                if (++i >= argc)
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
    fprintf(stderr, "reserve -c busDataFile\n");
    if (iArg >= 0)
        exit(-1);
    else
        exit(-2);
}
