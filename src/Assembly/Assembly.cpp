/*
    GIGIBOX: MERDSEMBLY
    (un interprete per) una lingua di programmazione a basso livello ispirata ad assembly
    viola 90% delle convenzioni delle diverse versioni di assembly ma almeno è a basso livello ;)
    e più importante funziona per l'obbrobrio che sto creando

    (c) TM copyright trademark sìmo dal 2025 fino all'oltretomba
    altra roba legale innecessaria blah blah blah fanculo la legge piratate i programmi adobe

    ascoltate yakui the maid su spotify/bandcamp
*/

#include "Assembly.hpp"

typedef Gigi::LanguageManager lm;

// GigiRegisters

unsigned long Gigi::Assembly::Registers::programCounter = 0;

string Gigi::Assembly::Registers::registerNames[]{
        "a",
        "b",
        "c",
        "in" // registro per contenere gli input/controlli della tastiera/controller
};

vector<Sprite> Gigi::Assembly::Registers::storedSprites;
vector<Gigi::Image> Gigi::Assembly::Registers::storedImages;

// in main, while(!interrupted) continua interpretazione assembly. viene reso false dall'istruzione int
bool Gigi::Assembly::Registers::interrupted = false;

/*
    una matrice con tutti i dati disponibili al programma.

    i registri sono conservati qua dentro, con posti riservati alle prime posizioni (o come si dice in termini tecnici "davanti la matrice") del vettore.
    i registri non possono essere accessi tramite l'indirizzo/indice del vettore poiché un offset positivo è hardcoded nell'accesso dei dati

    se si vuole accedere al registro A durante l'implementazione di funzionalità delle istruzioni, A si troverebbe realmente a indice 0
    se A è l'unico registro/dato riservato implementato nel vettore, per ottenere il primo dato
*/
vector<short> Gigi::Assembly::Registers::data;

// lo stack di dove vengono tenute posizioni nel codice da cui riprendere quando si finisce una funzione
stack<unsigned int> Gigi::Assembly::Registers::callStack;

// tiene nomi di funzioni e le linee a cui iniziano
map<string, unsigned int> Gigi::Assembly::Registers::functions;

void Gigi::Assembly::Registers::createMemoryIfNone(int index) {
    while (Gigi::Assembly::Registers::data.size() <= index + REGISTERS_IN_MEMORY)
        Gigi::Assembly::Registers::data.push_back(0);
}

void Gigi::Assembly::Registers::AddRegistersToData() {
    for (int i = Gigi::Assembly::Registers::data.size(); i < REGISTERS_IN_MEMORY; i++)
        Gigi::Assembly::Registers::data.push_back(0);
    }

uint8_t Gigi::Assembly::Registers::logicalFlag;

/*
    una funzione per ottenere i dati del vettore data in modo più pulito, aggiunge un offset all'indice/indirizzo quando viene utilizzato
    usare un numero negativo per negare l'offset e accedere alla memoria dei registri
*/
short* Gigi::Assembly::Registers::getData(int address) {
    Gigi::Assembly::Registers::createMemoryIfNone(address);
    return &Gigi::Assembly::Registers::data[address + REGISTERS_IN_MEMORY];
}

void Gigi::Assembly::Registers::Compare(int a) {
    short b = *Gigi::Assembly::Registers::getData(-1);
    unsigned short c = b - a;
    uint8_t bufferflag = 0;

    if (c > b) bufferflag += 0b00100000; // se il numero è diventato più grande (è avvenuto un overflow) ovvero a > b
    if (c == 0) bufferflag += 0b01000000;
    bufferflag += (c | 0b0111111111111111 ^ 0b0111111111111111) >> 8; // aggiunge il primo bit di c al primo bit di bufferflag
    // ^ probabilmente sto facendo roba innecessaria ma non ne so sicuro

    logicalFlag = bufferflag;
}

// GigiAssemblyInstructions

vector<string> Gigi::Assembly::argStack;

string Gigi::Assembly::getArgFromTopStr(int index) {
    return argStack[argStack.size() - 1 - index];
}

int Gigi::Assembly::getArgFromTop(int index) {
    string buffer = Gigi::Assembly::getArgFromTopStr(index);

    bool pointing = false; // se il valore si sta riferendo a un indirizzo usando in numero (4 == %$4) (se a = 4, mov c %$a == c = data[a])
    int isTherePointer = buffer.find("%");
    if (isTherePointer != string::npos) {
        pointing = true;
        buffer.erase(isTherePointer, 1);
    }

    bool value = false; // dichiara che il parametro si sta riferendo al valore dell'indirizzo/registo

    int isThereValue = buffer.find("$");
    if (isThereValue != string::npos) { // controlla se esiste un $ nella stringa
        value = true;
        buffer.erase(isThereValue, 1);
    }

    int buffern = 0;
    bool isRegister = false;
    for (int i = 0; i < REGISTERS_IN_MEMORY; i++) {
        if (buffer == Registers::registerNames[i]) {
            buffern = -(i + 1);
            isRegister = true;
            break;
        }
        if (i == REGISTERS_IN_MEMORY - 1)
            buffern = stoi(buffer);
    }

    // if mov a 4, $a = 4 and a = -1 ($4 = 4 and 4 = data[4])
    if (value ^ isRegister) return buffern; // return if (value && !isRegister)$5 || (!value && isRegister)a
    if (pointing && value) return *Registers::getData(*Registers::getData(buffern));
    return *Registers::getData(buffern); // retur if (value && isRegister) || (!value && !register)
}

string Gigi::Assembly::formatDataString(int index) {
    string buffer;

    for (int i = 0; i < *Registers::getData(index); i++) {
        buffer += *Registers::getData(index + i + 1);
    }

    return buffer;
}

void Gigi::Assembly::formatStringData(int index, string str) {
    int dataLastCharacter = index + str.size() + 1;
    Gigi::Assembly::Registers::createMemoryIfNone(dataLastCharacter);

    for (int i = index; i < dataLastCharacter; i++) {
        *Gigi::Assembly::Registers::getData(1 + i) = str[i];
    }
}

// GigiAssemblyInterpreter

bool Gigi::Assembly::Interpreter::running = true;

// dove viene tenuto il codice del programma. l'indice è la linea/posizione dell'istruzione
vector<string> Gigi::Assembly::Interpreter::programInstructions;

void Gigi::Assembly::Interpreter::InterpretLine(string str) {
    transform(str.begin(), str.end(), str.begin(), tolower); // rende la stringa minuscolo

    // rimuove commenti
    int commenti = str.find(";");
    if (commenti != string::npos) str.erase(commenti, str.size());

    vector<string> segments;

    // separa str in pezzetti
    while (!str.empty()) {
        int substrSize = str.find(" ");
        if (substrSize == string::npos) substrSize = str.size();

        string strbuffer = str.substr(0, substrSize);

        if (!strbuffer.empty()) segments.push_back(strbuffer);
        str.erase(0, substrSize + 1);
    }

    if (!segments.size()) return;

    // processa l'istruzione separata
    for (int i = segments.size() - 1; i > 0; i--) {
        argStack.push_back(segments[i]);
    }
    asmInstructions[segments[0]]();
    for (int i = 1; i < segments.size(); i++) {
        argStack.pop_back();
    }
}

void Gigi::Assembly::Interpreter::InterpretLines(vector<string> lines) {
    while (Registers::programCounter >= 0 && Registers::programCounter < lines.size() && running) {
        Interpreter::InterpretLine(lines[Registers::programCounter]);
        Registers::programCounter++;
    }
}

void Gigi::Assembly::Interpreter::StepProgram() {
    if (!running) {
        Registers::interrupted = true;
        return;
    }
    if (Registers::programCounter >= Interpreter::programInstructions.size()) {
        running = false;
        return;
    }
    try {
    Interpreter::InterpretLine(Interpreter::programInstructions[Registers::programCounter]);
        Registers::programCounter++;
    }
    catch (int e) {
        Logger::logError("Gigi_AssemblyInterpreter::StepProgram()", "logger.error.assembly.illegalcode", "logger.error.assembly.haltproc", e, LOGGER_ALL);
        Interpreter::running = false;
    }
}