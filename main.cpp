#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <fstream>
#include<Windows.h>

using namespace std;

struct Rib {
public:
    int start;
    int end;
};

void initializeGraph(int &picks, int &ribs, vector<Rib> &structRibs);

vector<int> DFSLoop(int &picks, int &ribs, vector<Rib> &structRibs);

void sortRibs(int &picks, int &ribs, vector<Rib> &structRibs);

void transposeGraph(int &picks, int &ribs, vector<Rib> &structRibs);

void reverseSortRibsByTime(int &picks, int &ribs, vector<int> t, vector<Rib> &structRibs);

vector<vector<int>>
reverseDFSLoop(int &picks, int &ribs, vector<int> vertices, vector<Rib> &structRibs);

vector<int> sortPicksByTime(vector<int> f);

int main() {
    SetConsoleOutputCP(CP_UTF8);
    vector<Rib> ribsList;
    int n = 0, m = 0;
    initializeGraph(n, m, ribsList);
    sortRibs(n, m, ribsList);
    vector<int> time = DFSLoop(n, m, ribsList);

    transposeGraph(n, m, ribsList);
    reverseSortRibsByTime(n, m, time, ribsList);
    reverseDFSLoop(n, m, sortPicksByTime(time), ribsList);
    return 0;
}

void initializeGraph(int &picks, int &ribs, vector<Rib> &structRibs) {
    Rib myRib{};
    ifstream inFile;
    inFile.open("myGraph.txt");

    if (!inFile.is_open()) cout << "It is not open" << endl;
    inFile >> picks >> ribs;

    for (int i = 0; i < ribs; i++) {
        inFile >> myRib.start >> myRib.end;

        structRibs.push_back(myRib);
    }
    inFile.close();
}

vector<int> DFSLoop(int &picks, int &ribs, vector<Rib> &structRibs) {

    vector<bool> isDFS(picks);
    stack<int> myStack;
    bool notSorted = true;
    int t = 0;
    vector<int> f(picks);
    while (notSorted) {
        for (int i = 0; i < picks; i++) {
            if (isDFS[i] == false) {
                myStack.push(i + 1);
                isDFS[i] = true;
                break;
            }
            if (i == picks - 1 && isDFS[i] == true) notSorted = false;
        }

        while (!myStack.empty()) {
            for (int i = 0; i < ribs; i++) {
                if (structRibs[i].start == myStack.top()) {
                    if (isDFS[structRibs[i].end - 1] == false) {
                        myStack.push(structRibs[i].end);

                        isDFS[structRibs[i].end - 1] = true;

                    }
                }

            }
            t++;
            f[myStack.top() - 1] = t;
            myStack.pop();
        }


    }
    return f;
}

void sortRibs(int &picks, int &ribs, vector<Rib> &structRibs) {

    Rib tmp{};
    for (int i = 0; i < ribs - 1; i++) {
        for (int j = 0; j < ribs - 1; j++) {
            if ((structRibs[j].start + structRibs[j].end) > (structRibs[j + 1].start + structRibs[j + 1].end)) {

                tmp = structRibs[j];
                structRibs[j] = structRibs[j + 1];
                structRibs[j + 1] = tmp;

            }
        }
    }
}

void transposeGraph(int &picks, int &ribs, vector<Rib> &structRibs) {
    int tmp;
    for (int i = 0; i < ribs; i++) {
        tmp = structRibs[i].start;
        structRibs[i].start = structRibs[i].end;
        structRibs[i].end = tmp;
    }
}

void reverseSortRibsByTime(int &picks, int &ribs, vector<int> t, vector<Rib> &structRibs) {

    Rib tmp{};
    for (int i = 0; i < ribs; i++) {
        for (int j = 0; j < ribs - 1; j++) {
            if (t[structRibs[j].start - 1] + t[structRibs[j].end - 1] <
                t[structRibs[j + 1].start - 1] + t[structRibs[j + 1].end - 1]) {
                tmp = structRibs[j];
                structRibs[j] = structRibs[j + 1];
                structRibs[j + 1] = tmp;
            }
        }
    }
}

vector<vector<int>>
reverseDFSLoop(int &picks, int &ribs, vector<int> vertices, vector<Rib> &structRibs) {

    vector<bool> isDFS(picks);
    stack<int> myStack;
    bool notSorted = true;
    vector<vector<int>> componentsList;
    vector<int> tmpVector;

    while (notSorted) {

        for (int i = 0; i < picks; i++) {
            if (isDFS[vertices[i] - 1] == false) {
                myStack.push(vertices[i]);
                isDFS[vertices[i]-1]=true;
                tmpVector.push_back(vertices[i]);
                break;
            }
        }

        if (myStack.empty()) notSorted = false;

        while (!myStack.empty()) {
            for (int i = 0; i < ribs; i++) {
                if (structRibs[i].start == myStack.top()) {
                    if (isDFS[structRibs[i].end - 1] == false) {
                        myStack.push(structRibs[i].end);
                        tmpVector.push_back(structRibs[i].end);
                        isDFS[structRibs[i].end - 1] = true;

                    }
                }

            }

            myStack.pop();
        }
        if (!tmpVector.empty()) componentsList.push_back(tmpVector);
        tmpVector.clear();


    }
    return componentsList;
}

vector<int> sortPicksByTime(vector<int> f) {
    vector<int> vertices(f.size());
    for (int i = 0; i < f.size(); i++) {
        vertices[f[i] - 1] = i + 1;
    }
    return vertices;
}