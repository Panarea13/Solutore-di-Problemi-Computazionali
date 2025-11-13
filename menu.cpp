#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
#include "menu.h"
#include "problem_factory.h"
#include "AP_instance.h"
#include "AP_solution.h"
#include "AP_solver.h"
#include "GAP_instance.h"
#include "GAP_solver.h"
#include "GAP_solution.h"
#include "UFLP_instance.h"
#include "UFLP_solver.h"
#include "UFLP_solution.h"


using std::cin; using std::cout; using std::endl;
using std::string;

//In C++, le variabili statiche dei membri della classe devono essere sia dichiarate che definite
std::shared_ptr<ProblemInstance> Menu::problemaCorrente = nullptr;
std::shared_ptr<ProblemSolution> Menu::soluzioneCorrente = nullptr;
std::shared_ptr<ProblemSolver> Menu::solverCorrente = nullptr;
ProblemType Menu::problemaAttuale = ProblemType::AP; // Valore di default

void Menu::resettaStato() {
    problemaCorrente.reset();
    solverCorrente.reset();
    soluzioneCorrente.reset();
    problemaAttuale = ProblemType::AP;
}

void Menu::clearScreen() {
#if defined(_WIN32)
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = { 0, 0 };
    DWORD count;
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
        FillConsoleOutputCharacter(hStdOut, ' ', csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
        SetConsoleCursorPosition(hStdOut, coord);
    }
#else
    cout << "\033[2J\033[1;1H";
#endif
    cout.flush();
}

void Menu::setColor(int color) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, static_cast<WORD>(color));
#else
    // Codici ANSI per Linux/macOS
    const char* colors[] = {
        "\033[0m",  // Reset
        "\033[31m", // Rosso
        "\033[32m", // Verde
        "\033[33m", // Giallo
        "\033[34m", // Blu
        "\033[35m", // Magenta
        "\033[36m", // Ciano
        "\033[37m"  // Bianco
    };
    cout << colors[color % 8];
#endif
}

void Menu::mostraMenuPrincipale() {
    clearScreen();
    setColor(6);
    cout << "||****************************************************||" << endl;
    cout << "||                                                    ||" << endl;
    cout << "||  SOLUTORI PER PARTICOLARI PROBLEMI COMPUTAZIONALI  ||" << endl;
    cout << "||                                                    ||" << endl;
    cout << "||                  Giugni Gabriele                   ||" << endl;
    cout << "||----------------------------------------------------||" << endl;

    // Opzioni
    setColor(11);
    cout << "||                                                    ||" << endl;
    cout << "||  1. Seleziona problema                             ||" << endl;
    cout << "||  2. Importa e risolvi istanza                      ||" << endl;
    cout << "||  3. Mostra soluzione corrente                      ||" << endl;
    cout << "||  4. Esporta soluzione                              ||" << endl;

    setColor(12);
    cout << "||  5. Esci                                           ||" << endl;

    setColor(6);
    cout << "||                                                    ||" << endl;
    cout << "||****************************************************||" << endl << endl;

    setColor(10);
    cout << "Scelta: ";

    setColor(7);

    int scelta;
    cin >> scelta;

    switch (scelta) {
    case 1:
        selezionaProblemaComputazionale();
        break;
    case 2:
        importaERisolviIstanza();
        break;
    case 3:
        mostraSoluzioneCorrente();
        break;
    case 4:
        esportaSoluzione();
        break;
    case 5:
        clearScreen();
        setColor(6);
        cout << "Grazie per aver utilizzato SOLUTORI PER PARTICOLARI PROBLEMI COMPUTAZIONALI di Giugni Gabriele. Arrivederci!" << endl;
        setColor(7);
        return;
    default:
        setColor(12);
        cout << "Scelta non valida!" << endl;
        setColor(7);
    }
}

void Menu::selezionaProblemaComputazionale() {
    clearScreen();
    setColor(6);
    cout << "||****************************************************||" << endl;
    cout << "||                                                    ||" << endl;
    cout << "||  Seleziona il problema computazionale desiderato:  ||" << endl;
    cout << "||                                                    ||" << endl;
    cout << "||----------------------------------------------------||" << endl;
    setColor(11);
    cout << "||                                                    ||" << endl;
    cout << "||  1. Assignment Problem - AP                        ||" << endl;
    cout << "||  2. Generalized Assignment Problem - GAP           ||" << endl;
    cout << "||  3. Uncapacitated Facility Location Problem - UFLP ||" << endl;
    setColor(12);
    cout << "||  4. Torna al menu principale                       ||" << endl;
    setColor(6);
    cout << "||                                                    ||" << endl;
    cout << "||****************************************************||" << endl << endl;
    setColor(10);
    cout << "Scelta: ";
    setColor(7);

    int scelta;

    while (cin >> scelta && scelta != 1 && scelta != 2 && scelta != 3 && scelta != 4) {
        setColor(12);
        cout << "Scelta non valida!" << endl;
        setColor(7);
    }

    switch (scelta) {
    case 1:
        clearScreen();
        setColor(6);
        cout << "Selezionato AP! Premere invio per continuare..." << endl;
        setColor(7);
        problemaAttuale = ProblemType::AP;
        break;
    case 2:
        clearScreen();
        setColor(6);
        cout << "Selezionato GAP! Premere invio per continuare..." << endl;
        setColor(7);
        problemaAttuale = ProblemType::GAP;
        break;
    case 3:
        clearScreen();
        setColor(6);
        cout << "Selezionato UFLP! Premere invio per continuare..." << endl;
        setColor(7);
        problemaAttuale = ProblemType::UFLP;
        break;
    case 4:
        mostraMenuPrincipale();
        return;
    }
    cin.ignore();
    cin.get();
    mostraMenuPrincipale();
}
 
void Menu::importaERisolviIstanza() {
    //verifica il tipo di problema
    setColor(11);
    if (problemaAttuale == ProblemType::AP) {
        cout << "Inserisci il percorso del file AP (con backslash finale!!!): ";
    }
    else if (problemaAttuale == ProblemType::GAP) {
        cout << "Inserisci il percorso del file GAP (con backslash finale!!!): ";
    }
    else {
        cout << "Inserisci il percorso del file UFLP (con backslash finale!!!): ";
    }
    setColor(7);
    string path;
    cin >> path;

    setColor(11);
    cout << endl << endl << "Inserisci il nome del file: ";
    setColor(7);
    string filename;
    cin >> filename;
    string pathCompleto = path + filename;

    try {
        problemaCorrente = CreateProblemInstance(static_cast<int>(problemaAttuale), pathCompleto);

        if (!problemaCorrente) {
            setColor(12);
            cout << "Errore: creazione istanza fallita!" << endl;
            setColor(7);
            return;
        }

        // Debug: stampa il tipo dell'istanza creata
        cout << "Tipo istanza creata: " << static_cast<int>(problemaCorrente->getProblemType()) << endl;

        if (!problemaCorrente->isValid()) {
            setColor(12);
            cout << "Il file non è consistente con le specifiche richieste!";
            return;
        }

        clearScreen();
        problemaCorrente->printInstance();

        solverCorrente = CreateProblemSolver(static_cast<int>(problemaAttuale));
        soluzioneCorrente = solverCorrente->solve(problemaCorrente);

        setColor(10);
        cout << "Istanza importata e risolta con successo!" << endl;
        setColor(6);
        cout << "Premere invio per continuare..." << endl;
        setColor(7);
        cin.ignore();
        cin.get();
        mostraMenuPrincipale();
    }
    catch (const std::exception& e) {
        setColor(12);
        std::cerr << "Errore: " << e.what() << endl;
        setColor(7);
        resettaStato();
    }
}

void Menu::mostraSoluzioneCorrente() {
    if (!soluzioneCorrente) {
        setColor(12);
        std::cout << "Nessuna soluzione disponibile!" << std::endl;
        setColor(7);
        return;
    }

    soluzioneCorrente->print(std::cout);
    setColor(6);
    cout << "Premere invio per continuare..." << endl;
    setColor(7);
    cin.ignore();
    cin.get();
    mostraMenuPrincipale();
}

void Menu::esportaSoluzione() {
    if (!soluzioneCorrente) {
        setColor(12);
        std::cout << "Nessuna soluzione disponibile!" << std::endl;
        setColor(7);
        return;
    }

    std::string filename;
    setColor(11);
    std::cout << "Inserisci il nome del file per esportare la soluzione: ";
    setColor(7);
    std::cin >> filename;

    std::ofstream outFile(filename);
    if (outFile) {
        soluzioneCorrente->print(outFile);
        setColor(10);
        std::cout << "Soluzione esportata con successo!" << std::endl;
    }
    else {
        setColor(12);
        std::cerr << "Errore durante l'esportazione!" << std::endl;
    }
    setColor(6);
    cout << "Premere invio per continuare..." << endl;
    setColor(7);
    cin.ignore();
    cin.get();
    mostraMenuPrincipale();
}