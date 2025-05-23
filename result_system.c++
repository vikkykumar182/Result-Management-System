#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <string>
#include <map>

using namespace std;

// Define a structure for Student
struct Student {
    string id;
    string name;
    string class_name;
    map<string, int> marks; // Subject -> Marks
    
    void display() const {
        cout << left << setw(10) << id
             << left << setw(20) << name
             << left << setw(10) << class_name
             << endl;
    }
};

// Function Prototypes
void addStudent(vector<Student>& students);
void displayStudents(const vector<Student>& students);
void addMarks(vector<Student>& students);
void calculateResults(const vector<Student>& students);
void saveDataToFile(const vector<Student>& students);
void loadDataFromFile(vector<Student>& students);

int main() {
    vector<Student> students;
    int choice;

    loadDataFromFile(students);

    do {
        cout << "\n===== Result Management System =====\n";
        cout << "1. Add Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Add Marks\n";
        cout << "4. Calculate Results\n";
        cout << "5. Save and Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addStudent(students);
                break;
            case 2:
                displayStudents(students);
                break;
            case 3:
                addMarks(students);
                break;
            case 4:
                calculateResults(students);
                break;
            case 5:
                saveDataToFile(students);
                cout << "Data saved successfully. Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);

    return 0;
}

void addStudent(vector<Student>& students) {
    Student newStudent;
    cout << "Enter Student ID: ";
    cin >> newStudent.id;
    cout << "Enter Student Name: ";
    cin.ignore();
    getline(cin, newStudent.name);
    cout << "Enter Class: ";
    cin >> newStudent.class_name;
    students.push_back(newStudent);
    cout << "Student added successfully!\n";
}

void displayStudents(const vector<Student>& students) {
    if (students.empty()) {
        cout << "No students to display.\n";
        return;
    }

    cout << left << setw(10) << "ID"
         << left << setw(20) << "Name"
         << left << setw(10) << "Class" << endl;
    cout << string(40, '-') << endl;

    for (const auto& student : students) {
        student.display();
    }
}

void addMarks(vector<Student>& students) {
    string id, subject;
    int mark;

    cout << "Enter Student ID: ";
    cin >> id;

    auto it = find_if(students.begin(), students.end(), [&](const Student& s) { return s.id == id; });
    if (it == students.end()) {
        cout << "Student not found.\n";
        return;
    }

    cout << "Enter Subject: ";
    cin >> subject;
    cout << "Enter Marks: ";
    cin >> mark;

    it->marks[subject] = mark;
    cout << "Marks added successfully!\n";
}

void calculateResults(const vector<Student>& students) {
    if (students.empty()) {
        cout << "No students to calculate results for.\n";
        return;
    }

    for (const auto& student : students) {
        int totalMarks = 0;
        for (const auto& markPair : student.marks) {
            totalMarks += markPair.second;
        }

        double percentage = student.marks.empty() ? 0 : (totalMarks / (double)student.marks.size());

        cout << "\nStudent ID: " << student.id
             << "\nName: " << student.name
             << "\nClass: " << student.class_name
             << "\nTotal Marks: " << totalMarks
             << "\nPercentage: " << fixed << setprecision(2) << percentage << "%\n";

        if (percentage >= 90) cout << "Grade: A\n";
        else if (percentage >= 75) cout << "Grade: B\n";
        else if (percentage >= 50) cout << "Grade: C\n";
        else cout << "Grade: F\n";
    }
}

void saveDataToFile(const vector<Student>& students) {
    ofstream file("students.dat");
    for (const auto& student : students) {
        file << student.id << '|' << student.name << '|' << student.class_name;
        for (const auto& markPair : student.marks) {
            file << '|' << markPair.first << ':' << markPair.second;
        }
        file << '\n';
    }
}

void loadDataFromFile(vector<Student>& students) {
    ifstream file("students.dat");
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        Student student;
        size_t pos = 0;

        // Extract ID
        pos = line.find('|');
        student.id = line.substr(0, pos);
        line.erase(0, pos + 1);

        // Extract Name
        pos = line.find('|');
        student.name = line.substr(0, pos);
        line.erase(0, pos + 1);

        // Extract Class
        pos = line.find('|');
        student.class_name = line.substr(0, pos);
        line.erase(0, pos + 1);

        // Extract Marks
        while ((pos = line.find('|')) != string::npos) {
            string markEntry = line.substr(0, pos);
            size_t sepPos = markEntry.find(':');
            string subject = markEntry.substr(0, sepPos);
            int marks = stoi(markEntry.substr(sepPos + 1));
            student.marks[subject] = marks;
            line.erase(0, pos + 1);
        }

        students.push_back(student);
    }
}