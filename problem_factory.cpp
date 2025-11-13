//problem_factory.cpp

#include "problem_factory.h"
#include "AP_instance.h"
#include "AP_solver.h"
#include "GAP_instance.h"
#include "GAP_solver.h"
#include "UFLP_instance.h"
#include "UFLP_solver.h"

//includi le derivate

std::shared_ptr<ProblemInstance> CreateProblemInstance(int problem, const std::string& filename) {

    try {
        switch (static_cast<ProblemType>(problem)) {
        case ProblemType::AP: {
            auto instance = std::make_shared<APInstance>();
            instance->loadFromFile(filename);
            if (!instance->isValid()) {
                throw std::runtime_error("AP instance is not valid");
            }
            return instance;
        }
        case ProblemType::GAP: {
            auto instance = std::make_shared<GAPInstance>();
            instance->loadFromFile(filename);
            if (!instance->isValid()) {
                throw std::runtime_error("GAP instance is not valid");
            }
            return instance;
        }
        case ProblemType::UFLP: {
            auto instance = std::make_shared<UFLPInstance>();
            instance->loadFromFile(filename);
            if (!instance->isValid()) {
                throw std::runtime_error("UFLP instance is not valid");
            }
            return instance;
        }
        default:
            throw std::invalid_argument("Unknown problem type");
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error creating problem instance: " << e.what() << std::endl;
        throw;
    }
}


//rende i solver condivisi tramite smart pointer
std::shared_ptr<ProblemSolver> CreateProblemSolver(int problem) {
    switch (static_cast<ProblemType>(problem)) {
    case ProblemType::AP:
        return std::make_shared<APSolver>();
    case ProblemType::GAP:
        return std::make_shared<GAPSolver>();
    case ProblemType::UFLP:
        return std::make_shared<UFLPSolver>();
    default:
        throw std::invalid_argument("Tipo di problema non supportato");
    }
}