//ProblemFactory

#ifndef PROBLEM_FACTORY_H
#define PROBLEM_FACTORY_H

#include <memory>
#include <string>
#include "problem_instance.h"
#include "problem_solver.h"

//factory per la creazione dell'istanza
std::shared_ptr<ProblemInstance> CreateProblemInstance(int problem, const string& filename);

//factory per la creazione del solver con smart pointer
std::shared_ptr<ProblemSolver> CreateProblemSolver(int problem);

#endif