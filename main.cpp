#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

//each field in csv data links to my struct names
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

// this converts all strings to lowercase , allowing to make searches case-insensistive
vector<Airport> airports;
string toLower(string s) {
    for (int i = 0; i < (int)s.length(); i++) {
        s[i] = tolower(s[i]);
    }
    return s;
}

//splits lines in the csv into seperate fields
vector<string> splitCSVLine(string line) {
    vector<string> fields;
    string current = "";
    bool insideQuotes = false; // if parser is inside quotation marks

    for (int i = 0; i < (int)line.length(); i++) {
        char c = line[i];
        if (c == '"') {
            insideQuotes = !insideQuotes;
        } else if (c == ',' && !insideQuotes) { // if a comma is outside the quotes it will mark it 
            fields.push_back(current);
            current = ""; // reset sequence 
        } else {
            current += c;
        }
    }
    fields.push_back(current);
    return fields;
}

// loads my airport data from csv into the airports vector
void loadAirports() {
    ifstream file("airport-codes.csv");
    if (!file.is_open()) {
        cout << "Could not open airport-codes.csv" << endl;
        return;
    }

    string line;
    getline(file, line);

    while (getline(file, line)) {
        vector<string> fields = splitCSVLine(line);
        if (fields.size() < 12) continue;

        Airport a;
        a.ident = fields[0];
        a.type = fields[1];
        a.name = fields[2];
        a.elevation_ft = fields[3];
        a.continent = fields[4];
        a.iso_country = fields[5];
        a.iso_region = fields[6];
        a.municipality = fields[7];
        a.gps_code = fields[8];
        a.iata_code = fields[9];
        a.local_code = fields[10];
        a.coordinates = fields[11];

        airports.push_back(a);
    }
    file.close();

    cout << "Loaded " << airports.size() << " airports." << endl;
}

// saves all data from the vector into a new csv file
void saveAirports() {
    ofstream file("airport-codes-updated.csv");
    if (!file.is_open()) {
        cout << "Could not save file." << endl;
        return;
    }

    file << "ident,type,name,elevation_ft,continent,iso_country,iso_region,municipality,gps_code,iata_code,local_code,coordinates\n";

    for (int i = 0; i < (int)airports.size(); i++) {
        Airport a = airports[i];
        file << a.ident << ","
             << a.type << ","
             << a.name << ","
             << a.elevation_ft << ","
             << a.continent << ","
             << a.iso_country << ","
             << a.iso_region << ","
             << a.municipality << ","
             << a.gps_code << ","
             << a.iata_code << ","
             << a.local_code << ","
             << a.coordinates << "\n";
    }
    file.close();
    cout << "Saved " << airports.size() << " airports to airport-codes-updated.csv." << endl;
}


// print all airport , used for the simple menu navigation menu
void printAirport(Airport a) {
    cout << "-------------------------------------" << endl;
    cout << "Name:         " << a.name << endl;
    cout << "Ident:        " << a.ident << endl;
    cout << "Type:         " << a.type << endl;
    cout << "IATA:         " << (a.iata_code.empty() ? "(none)" : a.iata_code) << endl;
    cout << "Country:      " << a.iso_country << endl;
    cout << "Municipality: " << a.municipality << endl;
    cout << "Elevation:    " << a.elevation_ft << " ft" << endl;
    cout << "Coordinates:  " << a.coordinates << endl;
}

// searches for airport based on a two letter code like the examples provided
void searchByCountry() {
    string code;
    cout << "Enter 2-letter country code (e.g. GB, US): ";
    cin >> code;

    for (int i = 0; i < (int)code.length(); i++) {
        code[i] = toupper(code[i]);
    }

    int found = 0;
    for (int i = 0; i < (int)airports.size(); i++) {
        if (airports[i].iso_country == code) {
            printAirport(airports[i]);
            found++;
        }
    }
    cout << "-------------------------------------" << endl;
    cout << "Found " << found << " airport(s) in " << code << "." << endl;
}


// allows the user to search for a specific airprot name
void searchByName() {
    string query;
    cout << "Enter part of airport name: ";
    cin.ignore();
    getline(cin, query);

    string queryLower = toLower(query);
    int found = 0;

    for (int i = 0; i < (int)airports.size(); i++) {
        string nameLower = toLower(airports[i].name);
        if (nameLower.find(queryLower) != string::npos) {
            printAirport(airports[i]);
            found++;
            if (found >= 20) {
                cout << "(showing first 20 results only)" << endl;
                break;
            }
        }
    }
    cout << "-------------------------------------" << endl;
    cout << "Found " << found << " airport(s) matching \"" << query << "\"." << endl;
}


// allows the users to search by IATA code
void searchByIATA() {
    string code;
    cout << "Enter 3-letter IATA code (e.g. LHR, JFK): ";
    cin >> code;

    for (int i = 0; i < (int)code.length(); i++) {
        code[i] = toupper(code[i]);
    }

    int found = 0;
    for (int i = 0; i < (int)airports.size(); i++) {
        if (airports[i].iata_code == code) {
            printAirport(airports[i]);
            found++;
        }
    }
    if (found == 0) {
        cout << "No airport found with IATA code " << code << "." << endl;
    }
}


// counts how many airports exist per airport type 
void countByType() {
    vector<string> types;
    vector<int> counts;

    for (int i = 0; i < (int)airports.size(); i++) {
        string t = airports[i].type; // gets current airport 

        bool found = false; // if false it means already exitsts (USE FOR DEBUGGING)
        for (int j = 0; j < (int)types.size(); j++) {
            if (types[j] == t) {
                counts[j]++;
                found = true;
                break;
            }
        }
        if (!found) { // if its new adds it to the list
            types.push_back(t);
            counts.push_back(1);
        }
    }

    cout << "-------------------------------------" << endl;
    cout << "Airport count by type:" << endl;
    for (int i = 0; i < (int)types.size(); i++) {
        cout << "  " << types[i] << ": " << counts[i] << endl;
    }
}


// function to allow the user to manually input new airports into the system 
void addAirport() {
    Airport a;
    cin.ignore(); 

    cout << "Enter ident: ";        getline(cin, a.ident);
    cout << "Enter type: ";         getline(cin, a.type);
    cout << "Enter name: ";         getline(cin, a.name);
    cout << "Enter elevation ft: "; getline(cin, a.elevation_ft);
    cout << "Enter continent: ";    getline(cin, a.continent);
    cout << "Enter country code: "; getline(cin, a.iso_country);
    cout << "Enter region: ";       getline(cin, a.iso_region);
    cout << "Enter municipality: "; getline(cin, a.municipality);
    cout << "Enter GPS code: ";     getline(cin, a.gps_code);
    cout << "Enter IATA code: ";    getline(cin, a.iata_code);
    cout << "Enter local code: ";   getline(cin, a.local_code);
    cout << "Enter coordinates (lon, lat): "; getline(cin, a.coordinates);

    airports.push_back(a);
    cout << "Airport added! Total now: " << airports.size() << endl;
}


// simple data output into terminal console 
void showMenu() {
    cout << endl;
    cout << "===== NSE Airport Search =====" << endl;
    cout << "1. Search by country code" << endl;
    cout << "2. Search by partial name" << endl;
    cout << "3. Search by IATA code" << endl;
    cout << "4. Count airports by type" << endl;
    cout << "5. Add a new airport" << endl;
    cout << "6. Save and exit" << endl;
    cout << "Choose: ";
}

int main() {
    loadAirports();

    int choice;
    while (true) {
        showMenu(); // loads airport data 
        cin >> choice;

        // handles users choices
        if (choice == 1)      searchByCountry();
        else if (choice == 2) searchByName();
        else if (choice == 3) searchByIATA();
        else if (choice == 4) countByType();
        else if (choice == 5) addAirport();
        else if (choice == 6) {
            saveAirports();
            cout << "Goodbye!" << endl;
            break;
        }
        else cout << "Invalid choice." << endl;
    }
    return 0;
}