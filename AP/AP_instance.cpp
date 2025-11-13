#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include "AP_instance.h"
#include "menu.h"

using std::cout; using std::endl;
using std::string;
using std::vector;
using namespace std;

//Implementazione di loadFromFile
void APInstance::loadFromFile(const string& filename) {
	ifstream file(filename);
	if (!file.is_open()) {
		throw runtime_error("Impossibile aprire il file: " + filename);
	}
	
	string line;
	while (getline(file, line)) {
		if (line.empty()) continue;

		if (line.find("NAME:") == 0) { //se lo trova
			instanceName = line.substr(5); 
		} 
		else if (line.find("n:") == 0) {
			m = stoul(line.substr(2)); 
			costMatrix.resize(m, vector<int>(m));
		}
		else if (line.find("c_ij:") == 0) {
			for (size_t i = 0; i < m && getline(file, line); ++i) {
				istringstream iss(line); 
				for (size_t j = 0; j < m; ++j) {
					if (!(iss >> costMatrix[i][j])) { 
						throw runtime_error("Formato file non valido: matrice dei costi incompleta");
					}
				}
			}
		}
	}

	if (costMatrix.empty()) {
		throw runtime_error("Il file non contiene dati validi per eseguire AP");
	}
}

bool APInstance::isValid() const {
	//verifica che la matrice dei costi sia quadrata e abbia le dimensioni corrette
	if (costMatrix.size() != m) return false;
	for (const auto& row : costMatrix) {
		if (row.size() != m) return false;
	}
	return true;
}

//implementazione di printInstance
void APInstance::printInstance() const {
		Menu menu;
		menu.clearScreen();
		menu.setColor(11);
		cout << "||*******************************||" << endl;
		cout << "||  Assignment Problem Instance  ||" << endl;
		cout << "||*******************************||" << endl << endl;
		menu.setColor(6);
		cout << "Name: " << instanceName << endl;
		cout << "Size: " << m << " agents/tasks" << endl;
		if (m <= 10) {
		cout << "Cost Matrix:" << endl;
		menu.setColor(7);
		for (const auto& row : costMatrix) {
			for (int cost : row) {
				cout << cost << "\t";
			}
			cout << endl;
		}
	}
}