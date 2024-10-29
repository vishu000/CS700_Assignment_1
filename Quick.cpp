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

void pythonScript(const vector<vector<double>> &allTimes, string title)
{
    // Convert vector<vector<double>> to JSON-like string
    ostringstream list;
    list << "[";
    for (size_t i = 0; i < allTimes.size(); ++i)
    {
        const auto &executionTimes = allTimes[i];
        list << "[";
        for (size_t j = 0; j < executionTimes.size(); ++j)
        {
            list << executionTimes[j];
            if (j < executionTimes.size() - 1)
            {
                list << ", ";
            }
        }
        list << "]";
        if (i < allTimes.size() - 1)
        {
            list << ", ";
        }
    }
    list << "]";
    string execList = list.str();

    // Construct the command to call the Python script
    string command = "python graph_quick.py \"" + execList + "\" " + title + "";

    cout << "Executing: " << command << endl;

    // Execute the command
    int success = system(command.c_str());
    if (success == 0)
    {
        cout << "Execution successful!" << endl;
    }
    else
    {
        cerr << "Error executing command!" << endl;
    }
}

void measureSortingTimes(ifstream& inputFile, vector<vector<double>>& timeRecords) {
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

            // Quick Sort for normal pivot element
            test_copy = test_cases;
            global_start = high_resolution_clock::now();
            quick(test_copy, 0, n - 1, 0);
            global_end = high_resolution_clock::now();
            global_elapsed = global_end - global_start;
            timeRecords[global_count].push_back(global_elapsed.count());
            global_count++;

            // Quick Sort for median pivot element
            test_copy = test_cases;
            global_start = high_resolution_clock::now();
            quick(test_copy, 0, n - 1, 1);
            global_end = high_resolution_clock::now();
            global_elapsed = global_end - global_start;
            timeRecords[global_count].push_back(global_elapsed.count());
            global_count++;

            // Quick Sort for random pivot element
            test_copy = test_cases;
            global_start = high_resolution_clock::now();
            quick(test_copy, 0, n - 1, 2);
            global_end = high_resolution_clock::now();
            global_elapsed = global_end - global_start;
            timeRecords[global_count].push_back(global_elapsed.count());
            global_count++;

            inputFile.ignore(); // Ignore the blank line after each section
        }
    }
}

int main()
{
    ifstream Bestfile("Input_files/random.txt");
    ifstream Worstfile("Input_files/Decreasing.txt");
    ifstream Randomfile("Input_files/random.txt");

    vector<vector<double>> best_quick(3);
    vector<vector<double>> worst_quick(3);
    vector<vector<double>> random_quick(3);

    if (Bestfile.is_open()){
        measureSortingTimes(Bestfile, best_quick); // quick_normal
        Bestfile.close();
    } 
    else{
        cerr << "Error opening Increasing.txt" << endl;
    }

    if(Worstfile.is_open()){
        measureSortingTimes(Worstfile, worst_quick); // quick_median
        Worstfile.close();
    }
    else{
        cerr << "Error opening Decreasing.txt" << endl;
    }

    if(Randomfile.is_open()){
        measureSortingTimes(Randomfile, random_quick); // quick_random
        Randomfile.close();
    }
    else{
        cerr << "Error opening random.txt" << endl;
    }

    pythonScript(best_quick, "BestCase");
    pythonScript(worst_quick, "WorstCase");
    pythonScript(random_quick, "AverageCase");

    return 0;
}












