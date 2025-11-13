//AP_solution.h

#ifndef AP_SOLUTION_H
#define AP_SOLUTION_H

#include "problem_solution.h"
#include "AP_instance.h"
#include <memory>  // Aggiunto per std::shared_ptr
#include <vector>  // Aggiunto per std::vector
#include <string>  // Aggiunto per std::string

class APSolution : public ProblemSolution {
public:
    // Dichiarazione corretta del costruttore
    APSolution(const std::shared_ptr<APInstance>& instance,
        const std::vector<int>& assignment,
        double totalCost);

    //metodo di stampa
    std::ostream& print(std::ostream& os) const override;

    //metodo che restituisce il costo totale
    double getCost() const override { return totalCost; }

    //restituisce il nome dell'istanza
    //const std::string& getInstanceName() const override { return instanceName; }

    //restituisce il tipo di problema
    ProblemType getProblemType() const override { return ProblemType::AP; }

private:
    std::vector<int> assignment;
    double totalCost;
};

#endif
