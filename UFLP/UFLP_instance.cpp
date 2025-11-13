#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include "UFLP_instance.h"
#include "menu.h"

using std::vector;
using std::string;
using std::cout;
using std::endl;

void UFLPInstance::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::string line;

    // Leggi NAME
    std::getline(file, line);
    size_t pos = line.find("NAME:");
    if (pos != std::string::npos) {
        instanceName = line.substr(pos + 5);
        // Rimuovi spazi bianchi all'inizio e alla fine
        instanceName.erase(0, instanceName.find_first_not_of(" \t"));
        instanceName.erase(instanceName.find_last_not_of(" \t") + 1);
    }

    // Leggi m e n
    std::getline(file, line);
    pos = line.find("m:");
    if (pos != std::string::npos) {
        m = std::stoi(line.substr(pos + 2));
    }

    std::getline(file, line);
    pos = line.find("n:");
    if (pos != std::string::npos) {
        n = std::stoi(line.substr(pos + 2));
    }

    // Leggi c_ij
    transportCosts.resize(m, std::vector<int>(n));
    std::getline(file, line); // Leggi la riga "c_jj:"

    for (auto i = 0; i < m; ++i) {
        std::getline(file, line);
        std::istringstream iss(line);
        for (size_t j = 0; j < n; ++j) {
            if (!(iss >> transportCosts[i][j])) {
                throw std::runtime_error("Invalid transport cost data");
            }
        }
    }

    // Leggi f_j
    fixedCosts.resize(m);
    std::getline(file, line); // Leggi la riga "f_j:"
    std::getline(file, line);
    std::istringstream iss(line);

    for (auto i = 0; i < m; ++i) {
        if (!(iss >> fixedCosts[i])) {
            throw std::runtime_error("Invalid fixed cost data");
        }
    }
}

bool UFLPInstance::isValid() const {
    if (m == 0 || n == 0) return false;
    if (transportCosts.size() != m) return false;
    for (const auto& row : transportCosts) {
        if (row.size() != n) return false;
    }
    if (fixedCosts.size() != m) return false;
    return true;
}

void UFLPInstance::printInstance() const {
    Menu menu;
    menu.clearScreen();
    menu.setColor(11);
    cout << "||*******************************************||" << endl;
    cout << "||  Uncapacitated Facility Location Problem  ||" << endl;
    cout << "||*******************************************||" << endl << endl;

    menu.setColor(6);
    cout << "Problem: " << getProblemName() << endl;
    cout << "Instance: " << instanceName << endl;
    cout << "Facilities (m): " << m << ", Clients (n): " << n << endl << endl;

    // Stampa matrice dei costi di trasporto
    if (n <= 10 && m <= 10) {  // Stampa completa solo per istanze piccole
        menu.setColor(6);
        cout << "Transport Costs Matrix (c_ij):" << endl;
        menu.setColor(7);

        // Intestazione colonne (clienti)
        cout << "        ";
        for (int j = 0; j < n; ++j) {
            printf("C%-6d ", j + 1);  // Formattazione allineata
        }
        cout << endl;

        // Righe della matrice
        for (int i = 0; i < m; ++i) {
            menu.setColor(6);
            printf("F%-4d  ", i + 1);  // Formattazione allineata
            menu.setColor(7);
            for (int j = 0; j < n; ++j) {
                printf("%-7d ", transportCosts[i][j]);  // Formattazione allineata
            }
            cout << endl;
        }
    }
    else {
        menu.setColor(12);
        cout << "\n[Transport cost matrix too large to display - "
            << m << " facilities x " << n << " clients]" << endl;
        menu.setColor(6);
    }

    // Stampa costi fissi
    cout << "\nFixed Costs (f_j):" << endl;
    menu.setColor(7);
    for (int i = 0; i < m; ++i) {
        menu.setColor(6);
        printf("Facility %-4d: ", i + 1);
        menu.setColor(7);
        printf("%-7d\n", fixedCosts[i]);
    }
}