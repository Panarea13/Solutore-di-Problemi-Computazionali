#include <iomanip>
#include <iostream>
#include "UFLP_solution.h"
#include "menu.h"

using std::cout;
using std::endl;

UFLPSolution::UFLPSolution(std::shared_ptr<ProblemInstance> inst,
    const std::vector<bool>& opened,
    const std::vector<size_t>& assignments,
    double cost)
    : instance(inst),
    openedFacilities(opened),
    clientAssignments(assignments),
    instanceName(inst->getInstanceName()), // Inizializza il membro instanceName
    totalCost(cost) {}

std::ostream& UFLPSolution::print(std::ostream& os) const {
    Menu menu;
    menu.setColor(11);
    os << endl;
    os << "||**************************************************||" << endl;
    os << "|| Uncapacitated Facility Location Problem Solution ||" << endl;
    os << "||**************************************************||" << endl << endl;

    menu.setColor(6);
    os << "Instance: " << instanceName << endl; // Usa il membro locale
    os << "Total Cost: " << totalCost << endl << endl;

    // Stampa depositi aperti
    menu.setColor(11);
    os << "Opened Facilities:" << endl;
    menu.setColor(7);

    size_t openedCount = 0;
    const size_t columns = 5;
    for (size_t i = 0; i < openedFacilities.size(); ++i) {
        if (openedFacilities[i]) {
            os << "  Facility " << std::setw(2) << std::right << i + 1;
            openedCount++;

            if (openedCount % columns == 0 || i == openedFacilities.size() - 1) {
                os << endl;
            }
            else {
                os << "   ";
            }
        }
    }

    if (openedCount == 0) {
        os << "  No facilities opened" << endl;
    }
    os << endl;

    // Stampa assegnamenti clienti
    menu.setColor(11);
    os << "Client Assignments:" << endl;
    menu.setColor(7);

    const size_t client_width = std::to_string(clientAssignments.size()).length();
    const size_t facility_width = std::to_string(openedFacilities.size()).length();

    for (size_t j = 0; j < clientAssignments.size(); ++j) {
        menu.setColor(11);
        os << "  Client ";
        menu.setColor(7);
        os << std::setw(client_width) << std::right << j + 1
            << " -> Facility "
            << std::setw(facility_width) << std::left << clientAssignments[j] + 1;

        if ((j + 1) % columns == 0 || j == clientAssignments.size() - 1) {
            os << endl;
        }
        else {
            os << "  ";
        }
    }

    return os;
}