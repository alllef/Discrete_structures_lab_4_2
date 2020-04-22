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

int PickExists(int pick, vector<Rib> &structRibs, vector<bool> &isDFS);

vector<vector<int>> stronglyConnectedComponents(int &picks, int &ribs, vector<Rib> &structRibs);

void printStronglyConnectedComponents(vector<vector<int>> componentsList);

int main() {
    SetConsoleOutputCP(CP_UTF8);
    vector<Rib> ribsList;
    int n = 0, m = 0;
    initializeGraph(n, m, ribsList);
    sortRibs(n, m, ribsList);
    printStronglyConnectedComponents(stronglyConnectedComponents(n,m,ribsList));
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
        }
        if (myStack.empty()) notSorted = false;

        while (!myStack.empty()) {
            if (PickExists(myStack.top(), structRibs, isDFS)) {
                int currentPick = PickExists(myStack.top(), structRibs, isDFS);
                isDFS[currentPick - 1] = true;
                myStack.push(currentPick);
            } else {
                t++;
                f[myStack.top() - 1] = t;
                myStack.pop();
            }
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
                isDFS[vertices[i] - 1] = true;
                tmpVector.push_back(vertices[i]);
                break;
            }
        }

        if (myStack.empty()) notSorted = false;

        while (!myStack.empty()) {
            if (PickExists(myStack.top(), structRibs, isDFS)) {
                int currentPick = PickExists(myStack.top(), structRibs, isDFS);
                isDFS[currentPick - 1] = true;
                myStack.push(currentPick);
                tmpVector.push_back(currentPick);
            } else myStack.pop();


        }
        if (!tmpVector.empty()) componentsList.push_back(tmpVector);
        tmpVector.clear();


    }
    return componentsList;
}

vector<int> sortPicksByTime(vector<int> f) {
    vector<int> vertices;
    for (int i = f.size(); i > 0; i--) {
        for(int j=1; j<=f.size(); j++){
            if(f[j-1]==i) vertices.push_back(j);
        }
    }
    return vertices;
}

int PickExists(int pick, vector<Rib> &structRibs, vector<bool> &isDFS) {
    for (auto & structRib : structRibs) {
        if (structRib.start == pick && isDFS[structRib.end - 1] == false) return structRib.end;

    }
    return 0;
}

vector<vector<int>> stronglyConnectedComponents(int &picks, int &ribs, vector<Rib> &structRibs){
    vector<int> time = DFSLoop(picks, ribs, structRibs);
    transposeGraph(picks, ribs, structRibs);
    reverseSortRibsByTime(picks, ribs, time, structRibs);
    return    reverseDFSLoop(picks, ribs, sortPicksByTime(time), structRibs);
}

void printStronglyConnectedComponents(vector<vector<int>> componentsList){
cout << " В графі є "<<componentsList.size()<< " Сильнозв'язаних компоненти\n";
for(int i=0; i<componentsList.size(); i++){
    cout<< "В компоненті "<<i+1<< " знаходяться вершини ";
    for(int j : componentsList[i]){
       cout<< j<< " ";
    }
    cout<<endl;
}
}