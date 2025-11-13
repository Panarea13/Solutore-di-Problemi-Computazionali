//AP_instance.h

#ifndef AP_INSTANCE_H
#define AP_INSTANCE_H

#include <vector>
#include <string>
#include "problem_instance.h"

class APInstance : public ProblemInstance {
public:
    //carica l'istanza da file
    void loadFromFile(const std::string& filename) override;

    //verifica che sia valida
    bool isValid() const override;

    //stampa dell'istanza su terminale
    void printInstance() const override;

    //restituisce il tipo di problema (AP)
    ProblemType getProblemType() const override { return ProblemType::AP; }

    //restituisce il nome dell'istanza
    std::string getInstanceName() const override { return instanceName; }

    //restituisce la dimensione
    size_t getSize() const override { return m; }

    //restituisce la matrice dei costi
    const std::vector<std::vector<int>>& getCostMatrix() const { return costMatrix; }

protected:
    //restituisce il nome del problema
    std::string getProblemName() const override { return "Assignment Problem (AP)"; }

private:
    std::string instanceName;
    size_t m;
    std::vector<std::vector<int>> costMatrix;
};

#endif