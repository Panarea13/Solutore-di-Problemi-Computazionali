// GAP_solution.h
#ifndef GAP_SOLUTION_H
#define GAP_SOLUTION_H

#include "problem_solution.h"
#include "GAP_instance.h"
#include <vector>

class GAPSolution : public ProblemSolution {
public:
    GAPSolution(const std::shared_ptr<GAPInstance> instance,
        const std::vector<int>& assignments,
        double totalCost);

    //metodo di stampa
    std::ostream& print(std::ostream& os) const override;

    //restituisce il costo totale
    double getCost() const override { return costoTotale; }

    //restituisce il nome dell'istanza
    const std::string& getInstanceName() const override { return nomeIstanza; }

    //restituisce il tipo di problema 
    ProblemType getProblemType() const override { return ProblemType::GAP; }

    //restituisce gli assegnamenti
    const std::vector<int>& getAssignments() const { return assegnamenti; }

private:
    std::string nomeIstanza;
    std::vector<int> assegnamenti; // assegnamenti[j] = agente assegnato al task j
    double costoTotale;
    size_t m; // numero di agenti
    size_t n; // numero di task
};

#endif // GAP_SOLUTION_H
