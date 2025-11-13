#include "UFLP_solver.h"
#include <algorithm>
#include <random>
#include <limits>
#include <stdexcept>
#include <iostream>

std::shared_ptr<ProblemSolution> UFLPSolver::solve(const std::shared_ptr<ProblemInstance> instance) {
    // Verifica che l'istanza sia del tipo corretto
    auto uflpInstance = std::dynamic_pointer_cast<UFLPInstance>(instance);
    if (!uflpInstance) {
        throw std::invalid_argument("Invalid instance type for UFLP solver");
    }

    // Verifica validità dell'istanza
    if (!uflpInstance->isValid()) {
        throw std::invalid_argument("Invalid UFLP instance");
    }

    return hybridSolution(uflpInstance);
}

std::shared_ptr<UFLPSolution> UFLPSolver::hybridSolution(const std::shared_ptr<UFLPInstance>& instance) {
    // Fase 1: Costruzione greedy della soluzione iniziale
    std::vector<bool> opened = greedyConstruction(instance);

    // Fase 2: Assegnamento clienti ai depositi aperti
    std::vector<size_t> assignments(instance->getNumClients());
    assignClients(opened, assignments, instance);

    // Calcola costo iniziale
    double totalCost = calculateTotalCost(opened, assignments, instance);

    // Fase 3: Ricerca locale per migliorare la soluzione
    localSearch(opened, assignments, totalCost, instance);

    // Crea e restituisce la soluzione
    return std::make_shared<UFLPSolution>(instance, opened, assignments, totalCost);
}

std::vector<bool> UFLPSolver::greedyConstruction(const std::shared_ptr<UFLPInstance>& instance) {
    const size_t m = instance->getNumFacilities();
    const size_t n = instance->getNumClients();
    const auto& transportCosts = instance->getTransportCosts();
    const auto& fixedCosts = instance->getFixedCosts();

    std::vector<bool> opened(m, false);
    std::vector<bool> served(n, false);

    // Continua finché tutti i clienti non sono serviti
    while (std::any_of(served.begin(), served.end(), [](bool s) { return !s; })) {
        double bestSaving = -std::numeric_limits<double>::infinity();
        size_t bestFacility = 0;

        // Per ogni deposito non ancora aperto
        for (size_t i = 0; i < m; ++i) {
            if (opened[i]) continue;

            // Calcola il saving potenziale aprendo questo deposito
            double saving = 0.0;
            size_t clientsServed = 0;

            for (size_t j = 0; j < n; ++j) {
                if (served[j]) continue;

                // Trova il costo corrente per servire questo cliente
                double currentCost = std::numeric_limits<double>::infinity();
                for (size_t k = 0; k < m; ++k) {
                    if (opened[k]) {
                        currentCost = std::min(currentCost, static_cast<double>(transportCosts[k][j]));
                    }
                }

                // Se il cliente non è ancora servito o questo deposito è più conveniente
                if (currentCost == std::numeric_limits<double>::infinity() ||
                    transportCosts[i][j] < currentCost) {
                    saving += (currentCost == std::numeric_limits<double>::infinity() ?
                        0 : currentCost) - transportCosts[i][j];
                    clientsServed++;
                }
            }

            // Il saving netto considera anche il costo fisso
            double netSaving = saving - fixedCosts[i];

            // Preferisci depositi che servono più clienti a parità di saving
            if (clientsServed > 0 && netSaving > bestSaving) {
                bestSaving = netSaving;
                bestFacility = i;
            }
        }

        // Se non troviamo un deposito conveniente, apri quello con costo fisso minimo
        // tra quelli che possono servire clienti non ancora serviti
        if (bestSaving == -std::numeric_limits<double>::infinity()) {
            double minCost = std::numeric_limits<double>::infinity();
            for (size_t i = 0; i < m; ++i) {
                if (opened[i]) continue;

                bool canServeUnsevered = false;
                for (size_t j = 0; j < n; ++j) {
                    if (!served[j]) {
                        canServeUnsevered = true;
                        break;
                    }
                }

                if (canServeUnsevered && fixedCosts[i] < minCost) {
                    minCost = fixedCosts[i];
                    bestFacility = i;
                }
            }
        }

        // Apri il deposito selezionato
        opened[bestFacility] = true;

        // Aggiorna i clienti serviti
        for (size_t j = 0; j < n; ++j) {
            if (served[j]) continue;

            // Trova il miglior deposito per questo cliente tra quelli aperti
            double bestCost = std::numeric_limits<double>::infinity();
            for (size_t k = 0; k < m; ++k) {
                if (opened[k] && transportCosts[k][j] < bestCost) {
                    bestCost = transportCosts[k][j];
                }
            }

            // Se il nuovo deposito è il migliore, marca il cliente come servito
            if (transportCosts[bestFacility][j] == bestCost) {
                served[j] = true;
            }
        }
    }

    return opened;
}

void UFLPSolver::localSearch(std::vector<bool>& opened,
    std::vector<size_t>& assignments,
    double& totalCost,
    const std::shared_ptr<UFLPInstance>& instance) {
    const size_t m = instance->getNumFacilities();
    bool improved = true;
    const int maxIterations = 100;
    int iterations = 0;

    while (improved && iterations < maxIterations) {
        improved = false;
        iterations++;

        // Tentativo 1: Chiusura di depositi
        for (size_t i = 0; i < m; ++i) {
            if (!opened[i]) continue;

            // Prova a chiudere il deposito i
            opened[i] = false;

            // Riassegna i clienti
            std::vector<size_t> newAssignments(instance->getNumClients());
            bool feasible = true;

            for (size_t j = 0; j < instance->getNumClients(); ++j) {
                double bestCost = std::numeric_limits<double>::infinity();
                size_t bestFacility = 0;

                for (size_t k = 0; k < m; ++k) {
                    if (opened[k] && instance->getTransportCosts()[k][j] < bestCost) {
                        bestCost = instance->getTransportCosts()[k][j];
                        bestFacility = k;
                    }
                }

                if (bestCost == std::numeric_limits<double>::infinity()) {
                    feasible = false;
                    break;
                }

                newAssignments[j] = bestFacility;
            }

            if (feasible) {
                double newCost = calculateTotalCost(opened, newAssignments, instance);
                if (newCost < totalCost) {
                    assignments = newAssignments;
                    totalCost = newCost;
                    improved = true;
                    continue;
                }
            }

            // Ripristina lo stato se non migliora
            opened[i] = true;
        }

        // Tentativo 2: Apertura di nuovi depositi
        for (size_t i = 0; i < m; ++i) {
            if (opened[i]) continue;

            // Prova ad aprire il deposito i
            opened[i] = true;

            // Riassegna i clienti
            std::vector<size_t> newAssignments(instance->getNumClients());
            assignClients(opened, newAssignments, instance);

            double newCost = calculateTotalCost(opened, newAssignments, instance);
            if (newCost < totalCost) {
                assignments = newAssignments;
                totalCost = newCost;
                improved = true;
                continue;
            }

            // Ripristina lo stato se non migliora
            opened[i] = false;
        }

        // Tentativo 3: Scambio di depositi (chiudi uno, apri un altro)
        if (!improved) {
            for (size_t i = 0; i < m; ++i) {
                if (!opened[i]) continue;

                for (size_t k = 0; k < m; ++k) {
                    if (opened[k] || k == i) continue;

                    // Scambia: chiudi i, apri k
                    opened[i] = false;
                    opened[k] = true;

                    // Verifica fattibilità e calcola nuovo costo
                    std::vector<size_t> newAssignments(instance->getNumClients());
                    bool feasible = true;

                    for (size_t j = 0; j < instance->getNumClients(); ++j) {
                        double bestCost = std::numeric_limits<double>::infinity();
                        size_t bestFacility = 0;

                        for (size_t f = 0; f < m; ++f) {
                            if (opened[f] && instance->getTransportCosts()[f][j] < bestCost) {
                                bestCost = instance->getTransportCosts()[f][j];
                                bestFacility = f;
                            }
                        }

                        if (bestCost == std::numeric_limits<double>::infinity()) {
                            feasible = false;
                            break;
                        }

                        newAssignments[j] = bestFacility;
                    }

                    if (feasible) {
                        double newCost = calculateTotalCost(opened, newAssignments, instance);
                        if (newCost < totalCost) {
                            assignments = newAssignments;
                            totalCost = newCost;
                            improved = true;
                            break;
                        }
                    }

                    // Ripristina lo stato se non migliora
                    opened[i] = true;
                    opened[k] = false;
                }

                if (improved) break;
            }
        }
    }
}

void UFLPSolver::assignClients(const std::vector<bool>& opened,
    std::vector<size_t>& assignments,
    const std::shared_ptr<UFLPInstance>& instance) {
    const size_t m = instance->getNumFacilities();
    const size_t n = instance->getNumClients();
    const auto& transportCosts = instance->getTransportCosts();

    for (size_t j = 0; j < n; ++j) {
        double bestCost = std::numeric_limits<double>::infinity();
        size_t bestFacility = 0;

        for (size_t i = 0; i < m; ++i) {
            if (opened[i] && transportCosts[i][j] < bestCost) {
                bestCost = transportCosts[i][j];
                bestFacility = i;
            }
        }

        assignments[j] = bestFacility;
    }
}

double UFLPSolver::calculateTotalCost(const std::vector<bool>& opened,
    const std::vector<size_t>& assignments,
    const std::shared_ptr<UFLPInstance>& instance) {
    double totalCost = 0.0;
    const auto& transportCosts = instance->getTransportCosts();
    const auto& fixedCosts = instance->getFixedCosts();

    // Costi fissi per i depositi aperti
    for (size_t i = 0; i < opened.size(); ++i) {
        if (opened[i]) {
            totalCost += fixedCosts[i];
        }
    }

    // Costi di trasporto
    for (size_t j = 0; j < assignments.size(); ++j) {
        totalCost += transportCosts[assignments[j]][j];
    }

    return totalCost;
}