#ifndef UFLP_SOLUTION_H
#define UFLP_SOLUTION_H

#include "problem_solution.h"
#include <vector>
#include <memory>
#include <string>

class UFLPInstance;

class UFLPSolution : public ProblemSolution {
public:
    UFLPSolution(std::shared_ptr<ProblemInstance> inst,
        const std::vector<bool>& opened,
        const std::vector<size_t>& assignments,
        double cost);

    //stampa la soluzione
    std::ostream& print(std::ostream& os) const override;

    //restituisce il costo totale
    double getCost() const override { return totalCost; }

    //restituisce il nome dell'istanza
    const std::string& getInstanceName() const override { return instanceName; } // Implementazione inline

    //restituisce il tipo di problema
    ProblemType getProblemType() const override { return ProblemType::UFLP; }

    // Getter specifici
    const std::vector<bool>& getOpenedFacilities() const { return openedFacilities; }
    const std::vector<size_t>& getClientAssignments() const { return clientAssignments; }

private:
    std::shared_ptr<ProblemInstance> instance;
    std::vector<bool> openedFacilities;
    std::vector<size_t> clientAssignments;
    std::string instanceName; // Membro aggiunto per memorizzare il nome
    double totalCost;
};

#endif