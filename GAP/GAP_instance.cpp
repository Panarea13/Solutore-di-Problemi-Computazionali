#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include "GAP_instance.h"
#include "menu.h"


using std::vector; using std::string;
using namespace std;

void GAPInstance::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Impossibile aprire il file: " + filename);
    }

    string line;

    // Leggi il nome dell'istanza
    while (getline(file, line)) {
        if (line.find("NAME:") != string::npos) {
            istringstream iss(line);
            string temp;
            iss >> temp >> instanceName;
            break;
        }
    }

    // Leggi il numero di agenti (m) e compiti (n)
    while (getline(file, line)) {
        if (line.find("m:") != string::npos) {
            istringstream iss(line);
            string temp;
            iss >> temp >> m;
        }
        else if (line.find("n:") != string::npos) {
            istringstream iss(line);
            string temp;
            iss >> temp >> n;
            break;
        }
    }

    // Leggi la matrice dei costi
    costMatrix.resize(m, vector<int>(n));
    getline(file, line); // Salta la riga "c_ij:"
    for (auto i = 0; i < m; ++i) {
        getline(file, line);
        istringstream iss(line);
        for (auto j = 0; j < n; ++j) {
            iss >> costMatrix[i][j];
        }
    }

    // Leggi la matrice delle risorse
    resourceMatrix.resize(m, vector<int>(n));
    getline(file, line); // Salta la riga "r_ij:"
    for (auto i = 0; i < m; ++i) {
        getline(file, line);
        istringstream iss(line);
        for (auto j = 0; j < n; ++j) {
            iss >> resourceMatrix[i][j];
        }
    }

    // Leggi i limiti delle risorse
    resourceLimits.resize(m);
    getline(file, line); // Salta la riga "b_i:"
    getline(file, line);
    istringstream iss(line);
    for (auto i = 0; i < m; ++i) {
        iss >> resourceLimits[i];
    }
}

bool GAPInstance::isValid() const {
    // Verifica che le dimensioni delle matrici siano corrette
    if (costMatrix.size() != m || resourceMatrix.size() != m || resourceLimits.size() != m) {
        return false;
    }

    for (const auto& row : costMatrix) {
        if (row.size() != n) return false;
    }

    for (const auto& row : resourceMatrix) {
        if (row.size() != n) return false;
    }

    // Verifica che i costi e le risorse siano valori positivi
    for (const auto& row : costMatrix) {
        for (int val : row) {
            if (val < 0) return false;
        }
    }

    for (const auto& row : resourceMatrix) {
        for (int val : row) {
            if (val < 0) return false;
        }
    }

    for (int val : resourceLimits) {
        if (val < 0) return false;
    }

    return true;
}

void GAPInstance::printInstance() const {
    Menu menu;
    menu.clearScreen();
    menu.setColor(11);
    cout << "||*****************************************||" << endl;
    cout << "|| Generalized Assignment Problem Instance ||" << endl;
    cout << "||*****************************************||" << endl << endl;
    menu.setColor(6);
    cout << "Problem: " << getProblemName() << endl;
    cout << "Instance: " << instanceName << endl;
    cout << "Agents (m): " << m << ", Tasks (n): " << n << endl;
    if (n <= 10) {
        cout << "\nCost Matrix (c_ij):" << endl;
        menu.setColor(7);
        for (const auto& row : costMatrix) {
            for (int val : row) {
                cout << val << "\t";
            }
            cout << endl;
        }
        menu.setColor(6);
        cout << "\nResource Matrix (r_ij):" << endl;
        menu.setColor(7);
        for (const auto& row : resourceMatrix) {
            for (int val : row) {
                cout << val << "\t";
            }
            cout << endl;
        }
    }
    else {
        menu.setColor(12);
        cout << "\n[Transport cost matrix too large to display - "
            << m << " agents " << n << " tasks]" << endl;
        menu.setColor(6);
    }
    menu.setColor(6);
    cout << "\nResource Limits (b_i):" << endl;
    menu.setColor(7);
    for (int val : resourceLimits) {
        cout << val << " ";
    }
    cout << endl;
}