#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include "Functions.h"
#include <cstdlib>
#include <sstream>

using namespace std;
using namespace std::chrono;

// Global variables
auto global_start = high_resolution_clock::now();
auto global_end = high_resolution_clock::now();
duration<double> global_elapsed;
string global_line;
int global_count;
double comp = 0;

void PythonScript(const vector<vector<double>> &timeData, const string &graphTitle)
{
    // Convert vector<vector<double>> into a JSON-style string representation
    ostringstream jsonString;
    jsonString << "[";
    for (size_t rowIndex = 0; rowIndex < timeData.size(); ++rowIndex)
    {
        const auto &row = timeData[rowIndex];
        jsonString << "[";
        for (size_t colIndex = 0; colIndex < row.size(); ++colIndex)
        {
            jsonString << row[colIndex];
            if (colIndex < row.size() - 1)
            {
                jsonString << ", ";
            }
        }
        jsonString << "]";
        if (rowIndex < timeData.size() - 1)
        {
            jsonString << ", ";
        }
    }
    jsonString << "]";
    string jsonStringOutput = jsonString.str();

    // Create the command string to execute the Python script
    string pythonCommand = "python graph.py \"" + jsonStringOutput + "\" " + graphTitle + "";

    cout << "Running command: " << pythonCommand << endl;

    // Execute the command
    int executionStatus = system(pythonCommand.c_str());
    if (executionStatus == 0)
    {
        cout << "Python script executed successfully!" << endl;
    }
    else
    {
        cerr << "Failed to execute the Python script!" << endl;
    }
}

void measureSortingTimes(ifstream& inputFile, vector<vector<double>>& timeRecords, vector<vector<double>>& compRecords) {
    while (getline(inputFile, global_line)) {
        if (global_line.find("Test cases for n=") != string::npos) {
            int n;
            sscanf(global_line.c_str(), "Test cases for n=%d:", &n);
            vector<int> test_cases(n);
            vector<int> test_copy;

            for (int i = 0; i < n; ++i) {
                inputFile >> test_cases[i];
            }
            global_count = 0;

            // Bubble Sort
            test_copy = test_cases;
            global_start = high_resolution_clock::now();
            comp = bubble(test_copy);
            global_end = high_resolution_clock::now();
            global_elapsed = global_end - global_start;
            timeRecords[global_count].push_back(global_elapsed.count());
            compRecords[global_count].push_back(comp);
            global_count += 1;

            // Insertion Sort
            test_copy = test_cases;
            global_start = high_resolution_clock::now();
            comp = insertion(test_copy);
            global_end = high_resolution_clock::now();
            global_elapsed = global_end - global_start;
            timeRecords[global_count].push_back(global_elapsed.count());
            compRecords[global_count].push_back(comp);
            global_count += 1;

            // Merge Sort
            test_copy = test_cases;
            global_start = high_resolution_clock::now();
            comp = merge_sort(test_copy, 0, n - 1);
            global_end = high_resolution_clock::now();
            global_elapsed = global_end - global_start;
            timeRecords[global_count].push_back(global_elapsed.count());
            compRecords[global_count].push_back(comp);
            global_count += 1;

            // Quick Sort for median pivot element 
            test_copy = test_cases;
            global_start = high_resolution_clock::now();
            comp = quick(test_copy, 0, n - 1, 1);
            global_end = high_resolution_clock::now();
            global_elapsed = global_end - global_start;
            timeRecords[global_count].push_back(global_elapsed.count());
            compRecords[global_count].push_back(comp);
            global_count++;

            // Heap Sort
            test_copy = test_cases;
            global_start = high_resolution_clock::now();
            comp = heap(test_copy);
            global_end = high_resolution_clock::now();
            global_elapsed = global_end - global_start;
            timeRecords[global_count].push_back(global_elapsed.count());
            compRecords[global_count].push_back(comp);
            global_count += 1;

            // Radix Sort
            test_copy = test_cases;
            global_start = high_resolution_clock::now();
            radix(test_copy);
            global_end = high_resolution_clock::now();
            global_elapsed = global_end - global_start;
            timeRecords[global_count].push_back(global_elapsed.count());
            global_count += 1;

            inputFile.ignore(); // Ignore the blank line after each section
        }
    }
}

int main()
{
    ifstream Bestfile("Input_files/Increasing.txt");
    ifstream Worstfile("Input_files/Decreasing.txt");
    ifstream Randomfile("Input_files/random.txt");

    vector<vector<double>> best_time(6);
    vector<vector<double>> worst_time(6);
    vector<vector<double>> random_time(6);

    vector<vector<double>> best_comp(5);
    vector<vector<double>> worst_comp(5);
    vector<vector<double>> random_comp(5);


    if (Bestfile.is_open()){
        measureSortingTimes(Bestfile, best_time, best_comp);
        Bestfile.close();
    } 
    else{
        cerr << "Error opening Bestfile.txt" << endl;
    }

    if(Worstfile.is_open()){
        measureSortingTimes(Worstfile, worst_time, worst_comp);
        Worstfile.close();
    }
    else{
        cerr << "Error opening Worstfile.txt" << endl;
    }

    if(Randomfile.is_open()){
        measureSortingTimes(Randomfile, random_time, random_comp);
        Randomfile.close();
    }
    else{
        cerr << "Error opening Randomfile.txt" << endl;
    }

    PythonScript(best_time, "IncreasingOrderInputFile");
    PythonScript(worst_time, "DecreasingOrderInputFile");
    PythonScript(random_time, "RandomOrderElementsInputFile");

    PythonScript(best_comp, "IncreasingOrderInputFileComparisions");
    PythonScript(worst_comp, "DecreasingOrderInputFileComparisions");
    PythonScript(random_comp, "RandomOrderElementsInputFileComparisions");


    
    return 0;
}