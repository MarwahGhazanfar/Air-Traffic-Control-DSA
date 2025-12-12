# Air Traffic Control System – DSA Project

## Overview
This is a **Data Structures & Algorithms (DSA) project** implemented in C++.  
The project simulates a **real-time Air Traffic Control System**, managing flights, runways, and flight history. It demonstrates the use of **vectors, stacks, sorting algorithms, file handling**, and **input validation**.

This project was developed as part of a DSA course to enhance problem-solving and coding skills in **C++**.

---

## Features

- Add new flights (Domestic / International)
- Show all flights with formatted output
- Assign runway and mark flight as departed
- Emergency flight prioritization
- Search flights by flight number or airline
- Cancel flights
- Update flight details
- Maintain flight history using a **stack**
- File handling to **save and load flight data**
- Input validation for **flight number and time format**

---

## Technologies Used

- **Language:** C++  
- **Data Structures:** Vector, Stack, Sorting / Priority Handling
- **File Handling:** `fstream` for persistent data  
- **IDE:** Visual Studio / Any C++ compatible IDE  
- **OS:** Windows / Linux

---

## Project Structure

Air-Traffic-Control-DSA/
├── main.cpp # Core C++ code
├── flights.txt # (Optional) Saved flight data
└── README.md # Project documentation

1. **Clone the repository**
```bash
git clone https://github.com/YourUsername/Air-Traffic-Control-DSA.git

2. **Navigate to the folder**
cd Air-Traffic-Control-DSA

3. **Compile the code**
Linux / macOS:

g++ main.cpp -o AirTrafficControl
./AirTrafficControl


Windows (Visual Studio): Open .cpp file and press Run

4. **Follow on-screen menu to:**

Add flights
Show flights
Assign runway / Depart
Search, Cancel, Update flights
View flight history

Sample Output
--- Air Traffic Control Tower ---
1. Add Flight
2. Show Flights
3. Assign Runway / Depart
4. Flight History
5. Search Flight
6. Cancel Flight
7. Update Flight
8. Exit
Enter choice: 1

Enter flight type (Domestic / International): Domestic
Enter Flight Number (Syntax: ABC-123): PAK-101
Enter Airline Name: Pakistan Airlines
Enter Departure Time (HH:MM): 14:30
Is this an emergency flight? (Yes / No): No
Flight added successfully!

Contributing

You can improve by adding GUI interface, multi-threading, or database support.

Ensure input validation is maintained for all new features.

License

This project is open source. You can use it freely for learning and educational purposes.

Author

Marwah Ghazanfar
