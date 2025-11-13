#include "AP_solution.h"
#include "menu.h"
#include <iostream>
#include <iomanip>  // Per std::fixed e std::setprecision

using std::cout; using std::endl; 

// Implementazione CORRETTA del costruttore
APSolution::APSolution(const std::shared_ptr<APInstance>& instance,
    const std::vector<int>& assignment,
    double totalCost)
    : ProblemSolution(instanceName),
    //instanceName(instance ? instance->getInstanceName() : "undefined"),
    assignment(assignment),
    totalCost(totalCost) {

    // Verifica che l'istanza sia valida
    if (!instance) {
        throw std::invalid_argument("APInstance non può essere nullptr");
    }

    // Verifica la dimensione dell'assegnamento
    if (assignment.size() != instance->getSize()) {
        throw std::invalid_argument("Dimensione dell'assegnamento non corrisponde alla dimensione dell'istanza");
    }

    // Verifica che i valori di assegnamento siano validi
    for (int task : assignment) {
        if (task < 0 || task >= static_cast<int>(instance->getSize())) {
            throw std::invalid_argument("Assegnamento contiene valori non validi");
        }
    }
}

std::ostream& APSolution::print(std::ostream& os) const {
    Menu menu;
    menu.setColor(11);
    cout << endl;
    os << "||*******************************||" << endl;
    os << "||  Assignment Problem Solution  ||" << endl;
    os << "||*******************************||" << endl << endl;
    menu.setColor(6);
    os << "Instance: " << instanceName << endl;;
    os << "Problem Type: AP" << endl;
    os << "Total Cost: " << std::fixed << std::setprecision(2);
    menu.setColor(7);
    os << totalCost << endl;
    menu.setColor(6);
    os << "Assignment: " << endl;

    menu.setColor(7);

    // Calcola la larghezza massima per i numeri degli agenti e dei task
    // Correzione: usa size_t invece di int per evitare i warning
    const size_t agent_width = std::to_string(assignment.size()).length();
    const size_t task_width = std::to_string(assignment.size()).length();
    const size_t columns = 5; // Numero di colonne desiderato

    for (auto i = 0; i < assignment.size(); ++i) {
        // Formatta ogni voce con spaziatura fissa
        menu.setColor(11);
        os << "  Agent ";
        menu.setColor(7);
        os << std::setw(agent_width) << std::right << i + 1
            << " -> Task "
            << std::setw(task_width) << std::right << assignment[i] + 1;

        // A capo ogni 'columns' elementi o alla fine
        if ((i + 1) % columns == 0 || i == assignment.size() - 1) {
            os << endl;
        }
        else {
            os << "  "; // Spaziatura tra le colonne
        }
    }
    return os;
}