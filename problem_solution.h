//ProbemSolution

#ifndef PROBLEM_SOLUTION_H
#define PROBLEM_SOLUTION_H

#include <iostream>
#include <iosfwd> 
#include <memory>
#include "problem_instance.h"

//metodi virtuali per le classi derivate

class ProblemSolution {
public:
    virtual ~ProblemSolution() = default;
    virtual std::ostream& print(std::ostream& os) const = 0;
    virtual double getCost() const = 0;
    virtual const std::string& getInstanceName() const = 0;
    const std::string& getInstanceName() const { return instanceName; };
    virtual ProblemType getProblemType() const = 0;

    ProblemSolution() = default;
    ProblemSolution(std::string& iname) : instanceName(name) {};

protected:
    std::string instanceName;
};

#endif 