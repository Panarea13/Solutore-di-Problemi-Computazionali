//AP_solver.cpp
#include "AP_solver.h"
#include <algorithm>
#include <limits>
#include <vector>
#include <memory>

std::shared_ptr<ProblemSolution> APSolver::solve(const std::shared_ptr<ProblemInstance> instance) {
    auto apInstance = std::dynamic_pointer_cast<APInstance>(instance);
    if (!apInstance) {
        throw std::invalid_argument("Istanza non valida per AP");
    }

    const auto& costMatrix = apInstance->getCostMatrix();
    auto n = costMatrix.size();

    // Usa l'algoritmo greedy
    std::vector<int> assignment = greedyColumnMin(costMatrix);

    // Calcolo del costo totale
    double totalCost = 0.0;
    for (auto i = 0; i < n; ++i) {
        if (assignment[i] != -1) {
            totalCost += costMatrix[i][assignment[i]];
        }
    }

    return std::make_shared<APSolution>(apInstance, assignment, totalCost);
}

std::vector<int> APSolver::greedyColumnMin(const std::vector<std::vector<int>>& costMatrix) {
    //Complessità O(n²)
    //INIZIALIZZAZIONE
    const auto n = costMatrix.size(); 
    std::vector<int> assignment(n, -1);// Inizializza tutti gli assegnamenti a -1 (non assegnato)
    std::vector<bool> assigned_rows(n, false);// Tiene traccia delle righe già assegnate

    //Facciamo una copia della matrice per non modificare l'originale
    auto matrix = costMatrix;

    for (auto col = 0; col < n; ++col) {
        //Trova il minimo nella colonna corrente
        int min_val = std::numeric_limits<int>::max(); //?
        int min_row = -1;

        //Trova il minimo nella colonna corrente tra le righe non ancora assegnate
        for (auto row = 0; row < n; ++row) {
            if (!assigned_rows[row] && matrix[row][col] < min_val) {
                min_val = matrix[row][col];
                min_row = row;
            }
        }

        //Se trovato un minimo valido, assegna
        if (min_row != -1) {
            assignment[min_row] = col; // Assegna il task all'agente
            assigned_rows[min_row] = true; // Marca la riga come assegnata

            //Elimina la riga assegnata (impostando valori alti per evitare riassegnazioni)
            for (auto& val : matrix[min_row]) {
                val = std::numeric_limits<int>::max();
            }
        }
    }

    return assignment;
}