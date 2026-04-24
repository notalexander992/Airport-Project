#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
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

vector<Airport> airports;
string toLower(string s) {
    for (int i = 0; i < (int)s.length(); i++) {
        s[i] = tolower(s[i]);
    }
    return s;
}

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
    fields.push_back(current);
    return fields;
}

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

void showMenu() {
    cout << endl;
    cout << "===== NSE Airport Search =====" << endl;
    cout << "1. Search by country code" << endl;
    cout << "2. Search by partial name" << endl;
    cout << "3. Search by IATA code" << endl;
    cout << "4. Exit" << endl;
    cout << "Choose: ";
}

int main() {
    loadAirports();

    int choice;
    while (true) {
        showMenu();
        cin >> choice;

        if (choice == 1)      searchByCountry();
        else if (choice == 2) searchByName();
        else if (choice == 3) searchByIATA();
        else if (choice == 4) { cout << "Goodbye!" << endl; break; }
        else cout << "Invalid choice." << endl;
    }

    return 0;
}