/*
    GIGIBOX: MERDSEMBLY
    (un interprete per) una lingua di programmazione a basso livello ispirata ad assembly
    viola 90% delle convenzioni delle diverse versioni di assembly ma almeno è a basso livello ;)
    e più importante funziona per l'obbrobrio che sto creando

    (c) TM copyright trademark sìmo dal 2025 fino all'oltretomba
    altra roba legale innecessaria blah blah blah fanculo la legge piratate i programmi adobe

    ascoltate yakui the maid su spotify/bandcamp
*/

#include "GigiAssembly.hpp"

// GigiRegisters


unsigned long Gigi_AssemblyRegisters::programCounter = 0;

string Gigi_AssemblyRegisters::registerNames[]{
        "a",
        "b",
        "c"
};

vector<Gigi_Sprite> Gigi_AssemblyRegisters::storedSprites;
vector<Gigi_Image> Gigi_AssemblyRegisters::storedImages;

// in main, while(!interrupted) continua interpretazione assembly. viene reso false dall'istruzione int
bool Gigi_AssemblyRegisters::interrupted = false;

/*
    una matrice con tutti i dati disponibili al programma.

    i registri sono conservati qua dentro, con posti riservati alle prime posizioni (o come si dice in termini tecnici "davanti la matrice") del vettore.
    i registri non possono essere accessi tramite l'indirizzo/indice del vettore poiché un offset positivo è hardcoded nell'accesso dei dati

    se si vuole accedere al registro A durante l'implementazione di funzionalità delle istruzioni, A si troverebbe realmente a indice 0
    se A è l'unico registro/dato riservato implementato nel vettore, per ottenere il primo dato
*/
vector<short> Gigi_AssemblyRegisters::data;

// lo stack di dove vengono tenute posizioni nel codice da cui riprendere quando si finisce una funzione
stack<unsigned int> Gigi_AssemblyRegisters::callStack;

// tiene nomi di funzioni e le linee a cui iniziano
map<string, unsigned int> Gigi_AssemblyRegisters::functions;

void Gigi_AssemblyRegisters::createMemoryIfNone(int index) {
    while (Gigi_AssemblyRegisters::data.size() <= index + REGISTERS_IN_MEMORY)
        Gigi_AssemblyRegisters::data.push_back(0);
}

void Gigi_AssemblyRegisters::AddRegistersToData() {
    for (int i = Gigi_AssemblyRegisters::data.size(); i < REGISTERS_IN_MEMORY; i++)
        Gigi_AssemblyRegisters::data.push_back(0);
}

uint8_t Gigi_AssemblyRegisters::logicalFlag;

/*
    una funzione per ottenere i dati del vettore data in modo più pulito, aggiunge un offset all'indice/indirizzo quando viene utilizzato
    usare un numero negativo per negare l'offset e accedere alla memoria dei registri
*/
short* Gigi_AssemblyRegisters::getData(int address) {
    Gigi_AssemblyRegisters::createMemoryIfNone(address);
    return &Gigi_AssemblyRegisters::data[address + REGISTERS_IN_MEMORY];
}

void Gigi_AssemblyRegisters::Compare(int a) {
    short b = *getData(-1);
    unsigned short c = b - a;
    uint8_t bufferflag = 0;

    if (c > b) bufferflag += 0b00100000; // se il numero è diventato più grande (è avvenuto un overflow) ovvero a > b
    if (c == 0) bufferflag += 0b01000000;
    bufferflag += (c | 0b0111111111111111 ^ 0b0111111111111111) >> 8; // aggiunge il primo bit di c al primo bit di bufferflag
    // ^ probabilmente sto facendo roba innecessaria ma non ne so sicuro

    logicalFlag = bufferflag;
}

// GigiAssemblyInstructions

vector<string> argStack;

string getArgFromTopStr(int index) {
    return argStack[argStack.size() - 1 - index]; 
}

int getArgFromTop(int index) {
    string buffer = getArgFromTopStr(index);

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
        if (buffer == Gigi_AssemblyRegisters::registerNames[i]) {
            buffern = -(i + 1);
            isRegister = true;
            break;
        }
        if(i == REGISTERS_IN_MEMORY - 1)
            buffern = stoi(buffer);
    }
    
    // if mov a 4, $a = 4 and a = -1 ($4 = 4 and 4 = data[4])
    if (value ^ isRegister) return buffern; // return if (value && !isRegister)$5 || (!value && isRegister)a
    if(pointing && value) return *Gigi_AssemblyRegisters::getData(*Gigi_AssemblyRegisters::getData(buffern));
    return *Gigi_AssemblyRegisters::getData(buffern); // retur if (value && isRegister) || (!value && !register)
}

string formatDataString(int index) {
    string buffer;

    for (int i = 0; i < *Gigi_AssemblyRegisters::getData(index); i++) {
        buffer += *Gigi_AssemblyRegisters::getData(index + i + 1);
    }

    return buffer;
}

void formatStringData(int index, string str) {
    int dataLastCharacter = index + str.size() + 1;
    Gigi_AssemblyRegisters::createMemoryIfNone(dataLastCharacter);

    for (int i = index; i < dataLastCharacter; i++) {
        *Gigi_AssemblyRegisters::getData(1 + i) = str[i];
    }
}

// GigiAssemblyInterpreter

bool Gigi_AssemblyInterpreter::running = true;

// dove viene tenuto il codice del programma. l'indice è la linea/posizione dell'istruzione
vector<string> Gigi_AssemblyInterpreter::programInstructions;

void Gigi_AssemblyInterpreter::InterpretLine(string str) {
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

void Gigi_AssemblyInterpreter::InterpretLines(vector<string> lines) {
    while (Gigi_AssemblyRegisters::programCounter >= 0 && Gigi_AssemblyRegisters::programCounter < lines.size() && running) {
        Gigi_AssemblyInterpreter::InterpretLine(lines[Gigi_AssemblyRegisters::programCounter]);
        Gigi_AssemblyRegisters::programCounter++;
    }
}

void Gigi_AssemblyInterpreter::StepProgram() {
    if (!running) {
        Gigi_AssemblyRegisters::interrupted = true;
        return;
    }
    if (Gigi_AssemblyRegisters::programCounter >= Gigi_AssemblyInterpreter::programInstructions.size()) {
        running = false;
        return;
    }
    try {
        Gigi_AssemblyInterpreter::InterpretLine(Gigi_AssemblyInterpreter::programInstructions[Gigi_AssemblyRegisters::programCounter]);
        Gigi_AssemblyRegisters::programCounter++;
    }
    catch(int e){
        cout << "GIGIBOX ERROR:\n\tIN:\tGigi_AssemblyInterpreter::StepProgram()\n\tCODICE ERRORE:\t" << e << "\n\tDESCRIZIONE ERRORE (potrebbe non essere accurato):\tE' stato interpretato codice invalido che ha causato errori fatali al programma\n\tPROCEDURE:\tL'interprete e' stato interrotto indefinitivamente. Si prega di riavviare GigiBox\n\n";
        Gigi_AssemblyInterpreter::running = false;
    }
}