#ifndef MENU_H
#define MENU_H

#include <memory>
#include <string>
#include "problem_instance.h"
#include "problem_solution.h"
#include "problem_solver.h"

class Menu {
public:
    //mostra il menu principlae
    static void mostraMenuPrincipale();

    //metodo per la selezione del problema computazionale
    static void selezionaProblemaComputazionale();

    //meotdo per l'importazione e la risoluzione dell'istanza
    static void importaERisolviIstanza();

    //metodo per mostrare la soluzione del problema
    static void mostraSoluzioneCorrente();

    //metodo dedito all'esportazione su file di testo
    static void esportaSoluzione();

    //metodo per ripulire il terminale
    static void clearScreen();

    //metodo per selezionare i colori di stampa su terminale
    static void setColor(int color);

protected:

    //metodo per resettare l'istanza del problema
    static void resettaStato();

private:
    static std::shared_ptr<ProblemInstance> problemaCorrente;
    static std::shared_ptr<ProblemSolution> soluzioneCorrente;
    static std::shared_ptr<ProblemSolver> solverCorrente;
    static ProblemType problemaAttuale;

    
};

#endif