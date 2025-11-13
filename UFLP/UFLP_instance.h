#ifndef UFLP_INSTANCE_H
#define UFLP_INSTANCE_H

#include "problem_instance.h"
#include <vector>
#include <string>

class UFLPInstance : public ProblemInstance {
private:
    std::string instanceName;
    size_t m; // Numero di depositi
    size_t n; // Numero di clienti
    std::vector<std::vector<int>> transportCosts; // Matrice c_ij (m x n)
    std::vector<int> fixedCosts; // Vettore f_i (m)

public:
    UFLPInstance() = default;
    ~UFLPInstance() override = default;

    //metodo di caricamento da file
    void loadFromFile(const std::string& filename) override;

    //verifica la validità dell'istanza
    bool isValid() const override;

    //stampa l'istanza
    void printInstance() const override;

    //restituisce il tipo di problema
    ProblemType getProblemType() const override { return ProblemType::UFLP; }

    //restituisce il nome dell'istanza
    std::string getInstanceName() const override { return instanceName; }

    //restituisce la dimensione
    size_t getSize() const override { return m * n; }

    // Getter specifici per UFLP
    const std::vector<std::vector<int>>& getTransportCosts() const { return transportCosts; }
    const std::vector<int>& getFixedCosts() const { return fixedCosts; }
    size_t getNumFacilities() const { return m; }
    size_t getNumClients() const { return n; }

protected:
    std::string getProblemName() const override { return "Uncapacitated Facility Location Problem"; }
};

#endif
