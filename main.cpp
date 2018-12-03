#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <stdlib.h>
#include <ctime>


#define SHELLSCRIPT "\
#/bin/bash \n\
influx -import -path=/home/josecarvalho/Documents/PROG/CodeBlocks/importBD/import_data.txt -precision=s \n\
"

using namespace std;

/**
# DDL
CREATE DATABASE pirates
CREATE RETENTION POLICY oneday ON pirates DURATION 1d REPLICATION 1

# DML
# CONTEXT-DATABASE: pirates
# CONTEXT-RETENTION-POLICY: oneday

treasures,captain_id=dread_pirate_roberts value=801 1439856000
treasures,captain_id=flint value=29 1439856000
treasures,captain_id=sparrow value=38 1439856000
treasures,captain_id=tetra value=47 1439856000
treasures,captain_id=crunch value=109 1439858880
**/


/**
struct tm t = {0};  // Initalize to all 0's
t.tm_year = 112;  // This is year-1900, so 112 = 2012
t.tm_mon = 8;
t.tm_mday = 15;
t.tm_hour = 21;
t.tm_min = 54;
t.tm_sec = 13;
time_t timeSinceEpoch = mktime(&t);
// Result: 1347764053

struct tm tm;

// set tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min and tm.tm_sec

tm.tm_year -= 1900; // year start at 1900
tm.tm_mon--;        // months start at january
TIME_STAMP t = timegm(&tm);

**/

int main()
{
    struct tm t = {0};
    time_t ts;
    fstream dataset, format;
    string cell, timestamp;
    char create;
    double open, high, low, close;
    int whichData, volume, lineCount = 1;

    cout << "Choose dataset: " << endl;
    cout << "1. AMZN Min 07 to 16 AM.csv" << endl;
    cout << "2. AMZN Min i.csv" << endl;
    cout << "3. CL #F=1 1 Minute.csv" << endl;
    cout << "4. CL #F Min i.csv" << endl;
    cout << "5. EUR AO-FX 07to16.csv" << endl;
    cout << "6. GBP AO-FX 07to16.csv" << endl;
    cout << "7. JPY AO-FX 07to16.csv" << endl;
    cout << "8. $SPX 07 to 16.csv" << endl;

    cin >> whichData;
    if(whichData < 1 || whichData > 8) {
        cout << "WRONG" << endl;
        return 0;
    }

    switch(whichData) {
        case 1:
            dataset.open("AMZN Min 07 to 16 AM.csv", ios::in);
            break;
        case 2:
            dataset.open("AMZN Min i.csv", ios::in);
            break;
        case 3:
            dataset.open("CL #F=1 1 Minute.csv", ios::in);
            break;
        case 4:
            dataset.open("CL #F Min i.csv", ios::in);
            break;
        case 5:
            dataset.open("EUR AO-FX 07to16.csv", ios::in);
            break;
        case 6:
            dataset.open("GBP AO-FX 07to16.csv", ios::in);
            break;
        case 7:
            dataset.open("JPY AO-FX 07to16.csv", ios::in);
            break;
        case 8:
            dataset.open("$SPX 07 to 16.csv", ios::in);
            break;
    }

    if(!dataset.is_open()) {
        cout << "No data." << endl;
        return 0;
    }

    format.open("import_data.txt", ios::out);
    cout << "Create database (y/n)?";
    cin >> create;
    if(create == 'Y' || create == 'y') {
        format << "# DDL" << endl;
        format << "CREATE DATABASE financial" << endl << endl;
    } else if (create != 'n' && create != 'N'){
        cout << "WRONG" << endl;
        return 0;
    }
    format << "# DML" << endl;
    format << "# CONTEXT-DATABASE: financial" << endl << endl;

    getline(dataset, cell); /// header

    while(getline(dataset, cell, '/')) {
        lineCount++;
        cout << "line " << lineCount << endl;

        if(whichData == 1) {
            t.tm_mon = stoi(cell);

            getline(dataset, cell, '/');
            t.tm_mday = stoi(cell);
        } else {
            t.tm_mday = stoi(cell);

            getline(dataset, cell, '/');
            t.tm_mon = stoi(cell);
        }
        getline(dataset, cell, ',');
        t.tm_year = stoi(cell);

        getline(dataset, cell, ':');
        t.tm_hour = stoi(cell);

        getline(dataset, cell, ':');
        t.tm_min = stoi(cell);

        getline(dataset, cell, ',');
        t.tm_sec = stoi(cell);

        t.tm_year -= 1900; // year start at 1900
        t.tm_mon--;        // months start at january
        ts = timegm(&t);
//        ts = mktime(&t);

        getline(dataset, cell, ',');
        open = stod(cell);
        getline(dataset, cell, ',');
        high = stod(cell);
        getline(dataset, cell, ',');
        low = stod(cell);
        getline(dataset, cell, ',');
        close = stod(cell);
        getline(dataset, cell);
        volume = stoi(cell);

        switch(whichData) {
            case 1:
                format << "amzn_min_07_16";
                break;
            case 2:
                format << "amzn_min_i";
                break;
            case 3:
                format << "cl_f11_minute";
                break;
            case 4:
                format << "cl_f_min_i";
                break;
            case 5:
                format << "eur_ao_fx_07_16";
                break;
            case 6:
                format << "gbp_ao_fx_07_16";
                break;
            case 7:
                format << "jpy_ao_fx_07_16";
                break;
            case 8:
                format << "spx_07_16";
                break;
        }
        format << " open=" << open << ",high=" << high << ",low=" << low << ",close=" << close << ",volume=" << volume << " " << ts << endl;
    }

    system(SHELLSCRIPT);

    return 0;
}






