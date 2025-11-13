//GAP_instance.h

#ifndef GAP_INSTANCE_H
#define GAP_INSTANCE_H

#include <string>
#include <vector>
#include "problem_instance.h"

using std::vector; using std::string;

class GAPInstance : public ProblemInstance {
public:
    // Carica l'istanza da file
    void loadFromFile(const string& filename) override;

    // Restituisce il tipo di problema (GAP)
    ProblemType getProblemType() const override { return ProblemType::GAP; }

    // Verifica la validità dell'istanza
    bool isValid() const override;

    // Stampa l'istanza
    void printInstance() const override;

    // Restituisce il nome dell'istanza
    string getInstanceName() const override { return instanceName; }

    // Restituisce la dimensione del problema (numero di agenti e compiti)
    size_t getSize() const override { return m * n; }

    const vector<vector<int>>& getCostMatrix() const { return costMatrix; }
    const vector<vector<int>>& getResourceMatrix() const { return resourceMatrix; }
    const vector<int>& getResourceLimits() const { return resourceLimits; }
    size_t getNumAgents() const { return m; }
    size_t getNumTasks() const { return n; }

private:
    string instanceName;  // Nome dell'istanza
    size_t m = 0;        // Numero di agenti
    size_t n = 0;        // Numero di compiti
    vector<vector<int>> costMatrix;     // Matrice dei costi (m x n)
    vector<vector<int>> resourceMatrix; // Matrice delle risorse (m x n)
    vector<int> resourceLimits;         // Limiti delle risorse per agente (m)

    string getProblemName() const override { return "Generalized Assignment Problem"; }
};

#endif //GAP_INSTANCE_H

