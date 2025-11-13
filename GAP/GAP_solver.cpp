// GAP_solver.cpp
#include <algorithm>
#include <random>
#include <limits>
#include <vector>
#include <stdexcept>
#include <stack>
#include "GAP_solver.h"

using namespace std;

std::shared_ptr<ProblemSolution> GAPSolver::solve(const std::shared_ptr<ProblemInstance> instance) {
	auto gapInstance = std::dynamic_pointer_cast<GAPInstance>(instance);
	if (!gapInstance) {
		throw std::runtime_error("Tipo di istanza non valido per il solver GAP");
	}
	try {
		const size_t m = gapInstance->getNumAgents();
		const size_t n = gapInstance->getNumTasks();
		const auto& costMatrix = gapInstance->getCostMatrix();
		const auto& resourceMatrix = gapInstance->getResourceMatrix();
		const auto& resourceLimits = gapInstance->getResourceLimits();

		// Struttura per tenere traccia della soluzione migliore
		std::vector<int> bestAssignment(n, -1);
		double bestCost = std::numeric_limits<double>::max();

		// Stack per simulare la ricorsione
		struct Node {
			size_t task;
			int agent;
			bool isBacktrack;
		};
		std::stack<Node> stack;
		stack.push({ 0, -1, false });

		// Variabili per lo stato corrente
		std::vector<int> currentAssignment(n, -1);
		std::vector<int> usedResources(m, 0);

		while (!stack.empty()) {
			Node node = stack.top();
			stack.pop();

			if (node.task == n) {
				// Soluzione completa trovata
				double totalCost = 0.0;
				for (auto j = 0; j < n; ++j) {
					totalCost += costMatrix[currentAssignment[j]][j];
				}
				if (totalCost < bestCost) {
					bestCost = totalCost;
					bestAssignment = currentAssignment;
				}
				continue;
			}

			if (node.isBacktrack) {
				// Ripristina lo stato precedente
				if (node.agent >= 0) {
					usedResources[node.agent] -= resourceMatrix[node.agent][node.task];
					currentAssignment[node.task] = -1;
				}
				continue;
			}

			// Prova il prossimo agente valido
			for (int agent = static_cast<int>(m) - 1; agent >= 0; --agent) {
				if (agent <= node.agent) continue;

				// Verifica se l'agente può prendere il task
				if (usedResources[agent] + resourceMatrix[agent][node.task] <= resourceLimits[agent]) {
					// Prepara il backtrack
					stack.push({ node.task, agent, true });

					// Fai l'assegnamento
					currentAssignment[node.task] = agent;
					usedResources[agent] += resourceMatrix[agent][node.task];

					// Passa al prossimo task
					stack.push({ node.task + 1, -1, false });
					break;
				}
			}
		}

		if (bestCost == std::numeric_limits<double>::max()) {
			throw std::runtime_error("Nessuna soluzione esatta trovata");
		}
		return std::make_shared<GAPSolution>(gapInstance, bestAssignment, bestCost);
	}

	catch (const std::runtime_error&) {
		// Se la soluzione esatta fallisce, prova l'euristica
		return solveHeuristic(gapInstance);
	}
}

std::shared_ptr<GAPSolution> GAPSolver::solveHeuristic(const std::shared_ptr<GAPInstance> instance) {
	const int m = static_cast<int>(instance->getNumAgents());  // Conversione esplicita
	const int n = static_cast<int>(instance->getNumTasks());   // Conversione esplicita
	const auto& costMatrix = instance->getCostMatrix();
	const auto& resourceMatrix = instance->getResourceMatrix();
	const auto& resourceLimits = instance->getResourceLimits();

	std::vector<int> miglioriAssegnamenti(n);
	double migliorCosto = std::numeric_limits<double>::max();

	// Euristica semplice: prova assegnamenti casuali e sceglie il migliore valido
	const int NUM_TENTATIVI = 1000;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> agentDist(0, m - 1);

	for (int tentativo = 0; tentativo < NUM_TENTATIVI; ++tentativo) {
		std::vector<int> assegnamentiCorrenti(n);
		std::vector<int> risorseUsate(m, 0);

		// Assegna ogni task a un agente casuale
		for (auto j = 0; j < n; ++j) {
			assegnamentiCorrenti[j] = agentDist(gen);
		}

		// Verifica se l'assegnamento è valido
		if (isAssignmentValid(assegnamentiCorrenti, instance)) {
			double costoCorrente = calculateTotalCost(assegnamentiCorrenti, instance);
			if (costoCorrente < migliorCosto) {
				migliorCosto = costoCorrente;
				miglioriAssegnamenti = assegnamentiCorrenti;
			}
		}
	}

	// Se non trova assegnamenti validi, usa un approccio greedy
	if (migliorCosto == std::numeric_limits<double>::max()) {
		miglioriAssegnamenti.assign(n, -1);
		std::vector<int> risorseUsate(m, 0);

		for (auto j = 0; j < n; ++j) {
			int migliorAgente = -1;
			double costoMinimo = std::numeric_limits<double>::max();

			for (auto i = 0; i < m; ++i) {
				if (risorseUsate[i] + resourceMatrix[i][j] <= resourceLimits[i]) {
					if (costMatrix[i][j] < costoMinimo) {
						costoMinimo = costMatrix[i][j];
						migliorAgente = i;
					}
				}
			}

			if (migliorAgente == -1) {
				// Se nessun agente può prendere il task, assegna a quello con minore violazione
				int violazioneMinima = std::numeric_limits<int>::max();
				for (auto i = 0; i < m; ++i) {
					int violazione = risorseUsate[i] + resourceMatrix[i][j] - resourceLimits[i];
					if (violazione < violazioneMinima) {
						violazioneMinima = violazione;
						migliorAgente = i;
					}
				}
			}

			miglioriAssegnamenti[j] = migliorAgente;
			risorseUsate[migliorAgente] += resourceMatrix[migliorAgente][j];
		}

		migliorCosto = calculateTotalCost(miglioriAssegnamenti, instance);
	}

	return std::make_shared<GAPSolution>(instance, miglioriAssegnamenti, migliorCosto);
}

bool GAPSolver::isAssignmentValid(const std::vector<int>& assignments,
	const std::shared_ptr<GAPInstance> instance) const {
	const size_t m = instance->getNumAgents();
	const auto& resourceMatrix = instance->getResourceMatrix();
	const auto& resourceLimits = instance->getResourceLimits();

	std::vector<int> risorseUsate(m, 0);

	for (auto j = 0; j < assignments.size(); ++j) {
		int agente = assignments[j];
		if (agente < 0 || agente >= static_cast<int>(m)) {
			return false;
		}
		risorseUsate[agente] += resourceMatrix[agente][j];
	}

	for (auto i = 0; i < m; ++i) {
		if (risorseUsate[i] > resourceLimits[i]) {
			return false;
		}
	}

	return true;
}

double GAPSolver::calculateTotalCost(const std::vector<int>& assignments,
	const std::shared_ptr<GAPInstance> instance) const {
	const auto& costMatrix = instance->getCostMatrix();
	double totale = 0.0;

	for (auto j = 0; j < assignments.size(); ++j) {
		int agente = assignments[j];
		totale += costMatrix[agente][j];
	}

	return totale;
}