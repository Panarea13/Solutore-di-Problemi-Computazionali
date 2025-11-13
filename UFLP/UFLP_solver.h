#ifndef UFLP_SOLVER_H
#define UFLP_SOLVER_H

#include "problem_solver.h"
#include "UFLP_instance.h"
#include "UFLP_solution.h"
#include <memory>
#include <vector>

/*
Solver ibrido per il problema UFLP che combina euristica greedy e ricerca locale
 *
 * Implementa un algoritmo a due fasi:
 * 1. Costruzione di una soluzione iniziale tramite euristica greedy
 * 2. Miglioramento della soluzione attraverso ricerca locale
 */
class UFLPSolver : public ProblemSolver {
public:
    UFLPSolver() = default;
    ~UFLPSolver() override = default;

    //risolve un'istanza del problema UFLP
    std::shared_ptr<ProblemSolution> solve(const std::shared_ptr<ProblemInstance> instance) override;

    //restituisce il nome del solver
    std::string getSolverName() const override {
        return "UFLP Hybrid Solver (Greedy + Local Search)";
    }

private:
    //algoritmo ibrido principale

    std::shared_ptr<UFLPSolution> hybridSolution(const std::shared_ptr<UFLPInstance>& instance);

    //fase di costruzione greedy della soluzione iniziale
    std::vector<bool> greedyConstruction(const std::shared_ptr<UFLPInstance>& instance);

    //fase di ricerca locale per migliorare la soluzione
    void localSearch(std::vector<bool>& opened,
        std::vector<size_t>& assignments,
        double& totalCost,
        const std::shared_ptr<UFLPInstance>& instance);

    //assegna i clienti ai depositi aperti pi� convenienti
    void assignClients(const std::vector<bool>& opened,
        std::vector<size_t>& assignments,
        const std::shared_ptr<UFLPInstance>& instance);

    //calcola il costo totale di una soluzione
    double calculateTotalCost(const std::vector<bool>& opened,
        const std::vector<size_t>& assignments,
        const std::shared_ptr<UFLPInstance>& instance);
};

#endif // UFLP_SOLVER_H