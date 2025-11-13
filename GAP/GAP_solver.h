#ifndef GAP_SOLVER_H
#define GAP_SOLVER_H

#include "problem_solver.h"
#include "GAP_instance.h"
#include "GAP_solution.h"
#include <memory>
#include <vector>

class GAPSolver : public ProblemSolver {
public:
	//metodo risolutivo del problema GAP
	std::shared_ptr<ProblemSolution> solve(const std::shared_ptr<ProblemInstance> instance) override;

	//restituisce il nome del solver
	std::string getSolverName() const override { return "GAP Solver (MILP/Euristico)"; }  // Nome generico

private:

	//algoritmo di risoluzione euristica
	std::shared_ptr<GAPSolution> solveHeuristic(const std::shared_ptr<GAPInstance> instance);

	// Metodi helper per l'euristica (assegnamento valido e calcolo del costo totale
	bool isAssignmentValid(const std::vector<int>& assignments,
		const std::shared_ptr<GAPInstance> instance) const;
	double calculateTotalCost(const std::vector<int>& assignments,
		const std::shared_ptr<GAPInstance> instance) const;
};

#endif // GAP_SOLVER_H
