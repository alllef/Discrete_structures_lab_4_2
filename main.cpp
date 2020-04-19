#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <fstream>
#include <iomanip>
#include<Windows.h>

using namespace std;

void initializeGraph(int &picks, int &ribs, vector<int> &start, vector<int> &end);

stack<int> topologicalSort(int &picks, int &ribs, vector<int> &start, vector<int> &end);

void sortRibs(int &picks, int &ribs, vector<int> &start, vector<int> &end);

void printTopologicalSort(stack<int> topologicalStack);

int main() {
    SetConsoleOutputCP(CP_UTF8);
    vector<int> startVector = {};
    vector<int> endVector = {};
    vector<int> vectorOfPower = {};
    int n = 0, m = 0;

    initializeGraph(n, m, startVector, endVector);
    sortRibs(n, m, startVector, endVector);
    printTopologicalSort(topologicalSort(n, m, startVector, endVector));

    return 0;
}

void initializeGraph(int &picks, int &ribs, vector<int> &start, vector<int> &end) {
    int startNumber, endNumber;

    ifstream inFile;
    inFile.open("myGraph.txt");

    if (!inFile.is_open()) cout << "It is not open" << endl;
    inFile >> picks >> ribs;

    for (int i = 0; i < ribs; i++) {
        inFile >> startNumber >> endNumber;
        start.push_back(startNumber);
        end.push_back(endNumber);
    }
    inFile.close();
}

stack<int> topologicalSort(int &picks, int &ribs, vector<int> &start, vector<int> &end) {

    int startPick = 1;
    vector<bool> isDFS(picks);
    vector<int> DFSNumbers(picks);
    int DFSNumber = 0;
    stack<int> myStack;
    stack<int> topologicalStack;
    bool notSorted = true;

    while (notSorted) {

        myStack.push(startPick);
        isDFS[startPick - 1] = true;
        DFSNumbers[startPick - 1] = ++DFSNumber;

        while (!myStack.empty()) {
            for (int i = 0; i < ribs; i++) {
                if (start[i] == myStack.top()) {
                    if (isDFS[end[i] - 1] == false) {
                        myStack.push(end[i]);

                        DFSNumbers[end[i] - 1] = ++DFSNumber;
                        isDFS[end[i] - 1] = true;

                    }
                }

            }
            topologicalStack.push(myStack.top());

            myStack.pop();
        }

        for (int i = 0; i < picks; i++) {
            if (isDFS[i] == false) {
                startPick = i + 1;
                break;
            }
            if (i == picks - 1 && isDFS[i] == true) notSorted = false;
        }

    }
    return topologicalStack;
}

void sortRibs(int &picks, int &ribs, vector<int> &start, vector<int> &end) {

    int endTmp;
    int startTmp;

    for (int i = 0; i < ribs - 1; i++) {
        for (int j = 0; j < ribs - 1; j++) {
            if ((start[j] + end[j]) > (start[j + 1] + end[j + 1])) {

                startTmp = start[j];
                start[j] = start[j + 1];
                start[j + 1] = startTmp;

                endTmp = end[j];
                end[j] = end[j + 1];
                end[j + 1] = endTmp;
            }
        }
    }
}

void printTopologicalSort(stack<int> topologicalStack) {
    int topologicalNumber = 0;
    cout <<setw(22)<< "Топологічний номер " <<setw(11)<< "Вершина\n";
    while (!topologicalStack.empty()) {
        cout <<setw(10)<< ++topologicalNumber << setw(15) << topologicalStack.top() << endl;
        topologicalStack.pop();
    }
}
