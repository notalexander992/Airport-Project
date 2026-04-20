#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
 
using namespace std;


// The CSV file has 12 columns so I need a variable for each one.
struct Airport {
    string ident;
    string type;
    string name;
    string elevation_ft;
    string continent;
    string iso_country;
    string iso_region;
    string municipality;
    string gps_code;
    string iata_code;
    string local_code;
    string coordinates;
};

//walks through csv data line by line 
vector<string> splitCSVLine(string line) {
    vector<string> fields;
    string current = "";
    bool insideQuotes = false;
 
    for (int i = 0; i < (int)line.length(); i++) {
        char c = line[i];
        if (c == '"') {
            insideQuotes = !insideQuotes;
        } else if (c == ',' && !insideQuotes) {
            fields.push_back(current);
            current = "";
        } else {
            current += c;
        }
    }
    fields.push_back(current);   // add the last field
    return fields;
}


int main() {
    vector<Airport> airports;
 
    ifstream file("airport-codes.csv");
    if (!file.is_open()) {
        cout << "Could not open airport-codes.csv" << endl;
        return 1;
    }
 
    string line;
    getline(file, line);   // skip the header row (ident,type,name,...)
 
    while (getline(file, line)) {
        vector<string> fields = splitCSVLine(line);
 
        // Sometimes a row is broken or missing columns, skip those
        if (fields.size() < 12) continue;
 
        Airport a;
        a.ident        = fields[0];
        a.type         = fields[1];
        a.name         = fields[2];
        a.elevation_ft = fields[3];
        a.continent    = fields[4];
        a.iso_country  = fields[5];
        a.iso_region   = fields[6];
        a.municipality = fields[7];
        a.gps_code     = fields[8];
        a.iata_code    = fields[9];
        a.local_code   = fields[10];
        a.coordinates  = fields[11];
 
        airports.push_back(a);
    }
    file.close();
 
    cout << "Loaded " << airports.size() << " airports." << endl;
 
    // Just print out the first 3 to check it worked
    cout << "First 3 airports:" << endl;
    for (int i = 0; i < 3 && i < (int)airports.size(); i++) {
        cout << i+1 << ". " << airports[i].name
             << " (" << airports[i].iso_country << ")" << endl;
    }
 
    return 0;
}