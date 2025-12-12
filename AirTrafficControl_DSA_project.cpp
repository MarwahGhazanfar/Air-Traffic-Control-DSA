#include <iostream>
#include <vector>
#include <stack>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

// ---------------- Flight Structure ----------------
struct Flight {
    string flightNumber;
    string airline;
    string type;       // Domestic / International
    string status;     // Scheduled / Departed / Cancelled
    string runway;
    string departureTime; // HH:MM
    bool isEmergency;
};

// ---------------- Global Lists ----------------
vector<Flight> domesticFlights;
vector<Flight> internationalFlights;
stack<Flight> flightHistory;

// ---------------- Helper Functions ----------------
string toLower(string str) {
    for (int i = 0; i < str.length(); i++) {
        if (str[i] >= 'A' && str[i] <= 'Z')
            str[i] += ('a' - 'A');
    }
    return str;
}

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore();
}

bool isValidFlightNumber(string num) {
    if (num == "") return false;
    for (int i = 0; i < num.length(); i++) {
        char c = num[i];
        if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '-'))
            return false;
    }
    return true;
}

bool isValidTime(string time) {
    if (time.length() != 5 || time[2] != ':') return false;
    if (time[0] < '0' || time[0] > '2') return false;
    if (time[1] < '0' || time[1] > '9') return false;
    if (time[3] < '0' || time[3] > '5') return false;
    if (time[4] < '0' || time[4] > '9') return false;

    int hour = (time[0] - '0') * 10 + (time[1] - '0');
    int min = (time[3] - '0') * 10 + (time[4] - '0');

    if (hour < 0 || hour > 23 || min < 0 || min > 59) return false;
    return true;
}

// ---------------- File Handling ----------------
void saveFlightsToFile() {
    ofstream outFile("flights.txt");
    if (!outFile) return;

    for (int i = 0; i < domesticFlights.size(); i++) {
        Flight f = domesticFlights[i];
        outFile << f.flightNumber << "," << f.airline << "," << f.type << ","
            << f.status << "," << f.runway << "," << f.departureTime << ","
            << f.isEmergency << "\n";
    }
    for (int i = 0; i < internationalFlights.size(); i++) {
        Flight f = internationalFlights[i];
        outFile << f.flightNumber << "," << f.airline << "," << f.type << ","
            << f.status << "," << f.runway << "," << f.departureTime << ","
            << f.isEmergency << "\n";
    }
    outFile.close();
}

void loadFlightsFromFile() {
    ifstream inFile("flights.txt");
    if (!inFile) return;

    string line;
    while (getline(inFile, line)) {
        Flight f;
        int last = 0, count = 0;
        string temp[7];

        for (int i = 0; i <= line.length(); i++) {
            if (i == line.length() || line[i] == ',') {
                temp[count] = line.substr(last, i - last);
                count++;
                last = i + 1;
            }
        }

        f.flightNumber = temp[0];
        f.airline = temp[1];
        f.type = temp[2];
        f.status = temp[3];
        f.runway = temp[4];
        f.departureTime = temp[5];
        f.isEmergency = (temp[6] == "1" || temp[6] == "true");

        if (toLower(f.type) == "domestic") domesticFlights.push_back(f);
        else internationalFlights.push_back(f);

        flightHistory.push(f);
    }
    inFile.close();
}

// ---------------- Core Functions ----------------
vector<Flight>* selectFlightList(string typeChoice) {
    typeChoice = toLower(typeChoice);
    if (typeChoice == "domestic") return &domesticFlights;
    else if (typeChoice == "international") return &internationalFlights;
    else return nullptr;
}

void addFlight() {
    string typeChoice;
    cout << "Enter flight type (Domestic / International): ";
    getline(cin, typeChoice);

    vector<Flight>* flightList = selectFlightList(typeChoice);
    if (flightList == nullptr) {
        cout << "Invalid flight type!\n";
        return;
    }

    Flight f;
    cout << "Enter Flight Number (Syntax: ABC-123): ";
    getline(cin, f.flightNumber);
    if (!isValidFlightNumber(f.flightNumber)) {
        cout << "Invalid flight number!\n";
        return;
    }

    for (int i = 0; i < flightList->size(); i++)
        if (toLower((*flightList)[i].flightNumber) == toLower(f.flightNumber)) {
            cout << "Flight already exists!\n";
            return;
        }

    cout << "Enter Airline Name: ";
    getline(cin, f.airline);

    cout << "Enter Departure Time (HH:MM): ";
    getline(cin, f.departureTime);
    if (!isValidTime(f.departureTime)) {
        cout << "Invalid time format!\n";
        return;
    }

    string emergencyChoice;
    cout << "Is this an emergency flight? (Yes / No): ";
    getline(cin, emergencyChoice);
    f.isEmergency = (toLower(emergencyChoice) == "yes");

    f.type = toLower(typeChoice);
    f.status = "Scheduled";
    f.runway = "Not Assigned";

    flightList->push_back(f);
    flightHistory.push(f);

    // Emergency priority sort
    for (int i = 0; i < flightList->size(); i++) {
        for (int j = 0; j < flightList->size() - i - 1; j++) {
            Flight& a = (*flightList)[j];
            Flight& b = (*flightList)[j + 1];

            if ((a.isEmergency < b.isEmergency) ||
                (a.isEmergency == b.isEmergency && a.departureTime > b.departureTime)) {
                Flight temp = a;
                a = b;
                b = temp;
            }
        }
    }

    cout << "Flight added successfully!\n";
}

void showFlights() {
    string typeChoice;
    cout << "Enter flight type to display (Domestic / International): ";
    getline(cin, typeChoice);

    vector<Flight>* flightList = selectFlightList(typeChoice);
    if (flightList == nullptr || flightList->size() == 0) {
        cout << "No flights found!\n";
        return;
    }

    cout << "\n" << left << setw(12) << "FlightNo"
        << setw(15) << "Airline"
        << setw(10) << "Departure"
        << setw(12) << "Status"
        << setw(10) << "Runway"
        << setw(10) << "Emergency" << "\n";
    cout << "-------------------------------------------------------------\n";

    for (int i = 0; i < flightList->size(); i++) {
        Flight f = (*flightList)[i];
        cout << left << setw(12) << f.flightNumber
            << setw(15) << f.airline
            << setw(10) << f.departureTime
            << setw(12) << f.status
            << setw(10) << f.runway
            << setw(10) << (f.isEmergency ? "Yes" : "No") << "\n";
    }
}

void assignRunway() {
    string typeChoice, flightNum;
    cout << "Enter flight type (Domestic / International): ";
    getline(cin, typeChoice);

    vector<Flight>* flightList = selectFlightList(typeChoice);
    if (flightList == nullptr) {
        cout << "Invalid flight type!\n";
        return;
    }

    cout << "Enter Flight Number to assign runway: ";
    getline(cin, flightNum);

    for (int i = 0; i < flightList->size(); i++) {
        if (toLower((*flightList)[i].flightNumber) == toLower(flightNum)) {
            cout << "Enter Runway Number (e.g., RW-1): ";
            string runway; getline(cin, runway);
            (*flightList)[i].runway = runway;
            (*flightList)[i].status = "Departed";

            flightHistory.push((*flightList)[i]);
            cout << "\nFlight Departed:\n";
            cout << left << setw(12) << "FlightNo"
                << setw(15) << "Airline"
                << setw(10) << "Departure"
                << setw(12) << "Status"
                << setw(10) << "Runway"
                << setw(10) << "Emergency" << "\n";
            cout << "-------------------------------------------------------------\n";
            Flight f = (*flightList)[i];
            cout << left << setw(12) << f.flightNumber
                << setw(15) << f.airline
                << setw(10) << f.departureTime
                << setw(12) << f.status
                << setw(10) << f.runway
                << setw(10) << (f.isEmergency ? "Yes" : "No") << "\n";

            return;
        }
    }
    cout << "Flight not found!\n";
}

// ---------------- New Features ----------------
void searchFlight() {
    string typeChoice, searchTerm;
    cout << "Enter flight type (Domestic / International): ";
    getline(cin, typeChoice);

    vector<Flight>* flightList = selectFlightList(typeChoice);
    if (flightList == nullptr || flightList->size() == 0) {
        cout << "No flights found!\n";
        return;
    }

    cout << "Enter Flight Number or Airline to search: ";
    getline(cin, searchTerm);
    searchTerm = toLower(searchTerm);

    cout << "\nSearch Results:\n";
    cout << left << setw(12) << "FlightNo"
        << setw(15) << "Airline"
        << setw(10) << "Departure"
        << setw(12) << "Status"
        << setw(10) << "Runway"
        << setw(10) << "Emergency" << "\n";
    cout << "-------------------------------------------------------------\n";

    bool found = false;
    for (int i = 0; i < flightList->size(); i++) {
        Flight f = (*flightList)[i];
        if (toLower(f.flightNumber) == searchTerm || toLower(f.airline) == searchTerm) {
            cout << left << setw(12) << f.flightNumber
                << setw(15) << f.airline
                << setw(10) << f.departureTime
                << setw(12) << f.status
                << setw(10) << f.runway
                << setw(10) << (f.isEmergency ? "Yes" : "No") << "\n";
            found = true;
        }
    }
    if (!found) cout << "No matching flights found.\n";
}

void cancelFlight() {
    string typeChoice, flightNum;
    cout << "Enter flight type (Domestic / International): ";
    getline(cin, typeChoice);

    vector<Flight>* flightList = selectFlightList(typeChoice);
    if (flightList == nullptr || flightList->size() == 0) {
        cout << "No flights found!\n";
        return;
    }

    cout << "Enter Flight Number to cancel: ";
    getline(cin, flightNum);

    for (int i = 0; i < flightList->size(); i++) {
        if (toLower((*flightList)[i].flightNumber) == toLower(flightNum)) {
            (*flightList)[i].status = "Cancelled";
            flightHistory.push((*flightList)[i]);
            cout << "\nFlight Cancelled:\n";
            cout << left << setw(12) << "FlightNo"
                << setw(15) << "Airline"
                << setw(10) << "Departure"
                << setw(12) << "Status"
                << setw(10) << "Runway"
                << setw(10) << "Emergency" << "\n";
            cout << "-------------------------------------------------------------\n";
            Flight f = (*flightList)[i];
            cout << left << setw(12) << f.flightNumber
                << setw(15) << f.airline
                << setw(10) << f.departureTime
                << setw(12) << f.status
                << setw(10) << f.runway
                << setw(10) << (f.isEmergency ? "Yes" : "No") << "\n";
            return;
        }
    }
    cout << "Flight not found!\n";
}

void updateFlight() {
    string typeChoice, flightNum;
    cout << "Enter flight type (Domestic / International): ";
    getline(cin, typeChoice);

    vector<Flight>* flightList = selectFlightList(typeChoice);
    if (flightList == nullptr || flightList->size() == 0) {
        cout << "No flights found!\n";
        return;
    }

    cout << "Enter Flight Number to update: ";
    getline(cin, flightNum);

    for (int i = 0; i < flightList->size(); i++) {
        if (toLower((*flightList)[i].flightNumber) == toLower(flightNum)) {
            Flight& f = (*flightList)[i];

            cout << "Enter new Airline Name (or press Enter to skip): ";
            string airline; getline(cin, airline);
            if (!airline.empty()) f.airline = airline;

            cout << "Enter new Departure Time HH:MM (or press Enter to skip): ";
            string time; getline(cin, time);
            if (!time.empty()) {
                if (isValidTime(time)) f.departureTime = time;
                else cout << "Invalid time format, skipped.\n";
            }

            cout << "Enter new Status (Scheduled / Departed / Cancelled) or press Enter: ";
            string status; getline(cin, status);
            if (!status.empty()) f.status = status;

            flightHistory.push(f);
            cout << "\nUpdated Flight Details:\n";
            cout << left << setw(12) << "FlightNo"
                << setw(15) << "Airline"
                << setw(10) << "Departure"
                << setw(12) << "Status"
                << setw(10) << "Runway"
                << setw(10) << "Emergency" << "\n";
            cout << "-------------------------------------------------------------\n";
            cout << left << setw(12) << f.flightNumber
                << setw(15) << f.airline
                << setw(10) << f.departureTime
                << setw(12) << f.status
                << setw(10) << f.runway
                << setw(10) << (f.isEmergency ? "Yes" : "No") << "\n";
            return;
        }
    }
    cout << "Flight not found!\n";
}

// ---------------- Flight History ----------------
void showFlightHistory() {
    if (flightHistory.empty()) {
        cout << "No flight history available.\n";
        return;
    }

    stack<Flight> temp = flightHistory;
    vector<Flight> reversed;
    while (!temp.empty()) {
        reversed.push_back(temp.top());
        temp.pop();
    }

    cout << "\n--- Flight History ---\n";
    cout << left << setw(12) << "FlightNo"
        << setw(15) << "Airline"
        << setw(10) << "Type"
        << setw(12) << "Status"
        << setw(10) << "Runway"
        << setw(10) << "Emergency" << "\n";
    cout << "-------------------------------------------------------------\n";

    for (int i = reversed.size() - 1; i >= 0; i--) {
        Flight f = reversed[i];
        cout << left << setw(12) << f.flightNumber
            << setw(15) << f.airline
            << setw(10) << f.type
            << setw(12) << f.status
            << setw(10) << f.runway
            << setw(10) << (f.isEmergency ? "Yes" : "No") << "\n";
    }
}

// ---------------- Main Menu ----------------
void mainMenu() {
    loadFlightsFromFile();

    while (true) {
        cout << "\n--- Air Traffic Control Tower ---\n";
        cout << "1. Add Flight\n2. Show Flights\n3. Assign Runway / Depart\n";
        cout << "4. Flight History\n5. Search Flight\n6. Cancel Flight\n7. Update Flight\n8. Exit\n";
        cout << "Enter choice: ";

        string choice; getline(cin, choice);

        if (choice == "1") addFlight();
        else if (choice == "2") showFlights();
        else if (choice == "3") assignRunway();
        else if (choice == "4") showFlightHistory();
        else if (choice == "5") searchFlight();
        else if (choice == "6") cancelFlight();
        else if (choice == "7") updateFlight();
        else if (choice == "8") { saveFlightsToFile(); break; }
        else cout << "Invalid choice!\n";

        pauseScreen();
    }
}

// ---------------- Main ----------------
int main() {
    mainMenu();
    return 0;
}




