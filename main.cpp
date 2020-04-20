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

void transposeGraph(int &picks, int &ribs, vector<int> &start, vector<int> &end);

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

vector<int> DFSLoop(int &picks, int &ribs, vector<int> &start, vector<int> &end, int startPick) {

    vector<bool> isDFS(picks);
    stack<int> myStack;
    bool notSorted = true;
    int t = 0;
    vector<int> f(picks);
    while (notSorted) {

        myStack.push(startPick);
        isDFS[startPick - 1] = true;
        while (!myStack.empty()) {
            for (int i = 0; i < ribs; i++) {
                if (start[i] == myStack.top()) {
                    if (isDFS[end[i] - 1] == false) {
                        myStack.push(end[i]);

                        isDFS[end[i] - 1] = true;

                    }
                }

            }
            t++;
            f[startPick - 1] = t;
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
    return f;
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

void transposeGraph(int &picks, int &ribs, vector<int> &start, vector<int> &end) {
    int tmp;
    for (int i = 0; i < ribs; i++) {
        tmp = start[i];
        start[i] = end[i];
        end[i] = tmp;
    }
}

void reverseSortRibsByTime(int &picks, int &ribs, vector<int> &start, vector<int> &end, vector<int> &t) {
    int endTmp;
    int startTmp;
    for (int i = 0; i < ribs; i++) {
        for (int j = 0; j < ribs; j++) {
            if (t[start[j] - 1] + t[end[j] - 1] < t[start[j + 1] - 1] + t[end[j + 1] - 1]) {
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

vector<int> reverseDFSLoop(int &picks, int &ribs, vector<int> &start, vector<int> &end, vector<int> f) {

    vector<bool> isDFS(picks);
    stack<int> myStack;
    bool notSorted = true;
    vector<vector<int>> componentsList;
    vector<int> tmpVector;

    while (notSorted) {

        for (int i = f.size(); i > 0; i--) {
            for (int j = 0; j < picks; j++) {
                if (f[j] == i && isDFS[j] == false) {
                    myStack.push(i + 1);
                    tmpVector.push_back(i + 1);
                    isDFS[i] = true;
                    break;
                }
            }
            if (!myStack.empty()) break;

        }
        if (myStack.empty()) notSorted = false;

        while (!myStack.empty()) {
            for (int i = 0; i < ribs; i++) {
                if (start[i] == myStack.top()) {
                    if (isDFS[end[i] - 1] == false) {
                        myStack.push(end[i]);
                        tmpVector.push_back(end[i]);
                        isDFS[end[i] - 1] = true;

                    }
                }

            }

            myStack.pop();
        }
        componentsList.push_back(tmpVector);
        tmpVector.clear();
    }
    return tmpVector;
}

struct Rib {
public:
    int start;
    int end;
};