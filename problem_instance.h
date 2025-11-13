//ProblemInstance
#ifndef PROBLEM_INSTANCE_H
#define PROBLEM_INSTANCE_H

#include <string>
#include <memory>

using std::string;

//metodi virtuali per le classi derivate

enum class ProblemType {
	AP,
	GAP,
	UFLP
};

class ProblemInstance {
public:
	// Costruttore
	virtual ~ProblemInstance() = default;

	// Metodo per caricare l'istanza da file
	virtual void loadFromFile(const std::string& filename) = 0;

	virtual ProblemType getProblemType() const = 0;

	// Metodo per verificare la validità dell'istanza
	virtual bool isValid() const = 0;

	// Metodo per stampare l'istanza
	virtual void printInstance() const = 0;
	
	// Metodo per ottenere il nome specifico dell'istanza
	virtual string getInstanceName() const = 0;

	// Metodo per ottenere la dimensione del problema
	virtual size_t getSize() const = 0;

protected:
	// Metodo per ottenere il nome del problema
	virtual string getProblemName() const = 0;
	
	
};
#endif 