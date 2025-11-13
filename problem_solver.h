//problem_solver.h

#ifndef PROBLEM_SOLVER_H
#define PROBLEM_SOLVER_H

#include <memory>
#include "problem_instance.h"
#include "problem_solution.h"

//metodi virtuali per le classi derivate

class ProblemSolver {
public:
	virtual ~ProblemSolver() = default;
	virtual std::shared_ptr<ProblemSolution> solve(const std::shared_ptr</*const*/ProblemInstance> instance) = 0;

	virtual std::string getSolverName() const = 0;
};

#endif