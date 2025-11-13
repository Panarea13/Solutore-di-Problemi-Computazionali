// GAP_solution.cpp
#include <iomanip>
#include <iostream>
#include "GAP_solution.h"
#include "menu.h"

using std::cout; using std::endl;

GAPSolution::GAPSolution(const std::shared_ptr<GAPInstance> instance,
    const std::vector<int>& assignments,
    double totalCost)
    : nomeIstanza(instance->getInstanceName()),
    assegnamenti(assignments),
    costoTotale(totalCost),
    m(instance->getNumAgents()),
    n(instance->getNumTasks()) {}

std::ostream& GAPSolution::print(std::ostream& os) const {
    Menu menu;
    menu.setColor(11);
    cout << endl;
    os << "||*******************************************||" << endl;
    os << "||  Generalized Assignment Problem Solution  ||" << endl;
    os << "||*******************************************||" << endl << endl;
    menu.setColor(6);
    os << "Istanza: " << nomeIstanza << endl;
    os << "Numero agenti: " << m << endl;
    os << "Numero task: " << n << endl << endl;

    os << "Assegnamento task:" << endl;

     // Calcola la larghezza massima per i numeri
    const size_t task_width = std::to_string(n).length();
    const size_t agent_width = std::to_string(m).length();
    const size_t columns = 5; // 5 elementi per riga

    menu.setColor(7);
    for (auto j = 0; j < n; ++j) {
        menu.setColor(11);
        os << "  Task ";
        menu.setColor(7); 
    /*TASK VIENE STAMPATO PRIMA DI AGENT DIVERSAMENTE DA AP
    PERCHE' OGNI AGENTE PUO' ESSERE ASSOCIATO A n >= 0 TASK NEL GAP
    IN QUESTO MODO MANTENIAMO UNA FORMATTAZIONE ORDINATA IN MODO CRESCENTE*/
        os << std::setw(task_width) << std::right << j + 1
            << " -> Agente "
            << std::setw(agent_width) << std::right << assegnamenti[j] + 1;

        // A capo ogni 'columns' elementi o alla fine
        if ((j + 1) % columns == 0 || j == n - 1) {
            os << endl;
        }
        else {
            os << "  "; // Spaziatura tra le colonne
        }
    }

    return os;
}