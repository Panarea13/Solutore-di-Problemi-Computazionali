//AP_solver.h

#ifndef AP_SOLVER_H
#define AP_SOLVER_H

#include "problem_solver.h"
#include "AP_instance.h"
#include "AP_solution.h"

//Algoritmo greedy per la risoluzione di AP

class APSolver : public ProblemSolver {
public:
	//metodo risolutivo
	std::shared_ptr<ProblemSolution> solve(const std::shared_ptr<ProblemInstance> instance) override;

	//restituisce il nome del solver implementato
	std::string getSolverName() const override { return "Greedy Column Min Solver"; }

private:
	//metodo che applica l'algoritmo greedy
	std::vector<int> APSolver::greedyColumnMin(const std::vector<std::vector<int>>& costMatrix);
};
#endif;