/*
    GIGIBOX: MERDSEMBLY
    (un interprete per) una lingua di programmazione a basso livello ispirata ad assembly
    viola 90% delle convenzioni delle diverse versioni di assembly ma almeno è a basso livello ;)
    e più importante funziona per l'obbrobrio che sto creando

    (c) TM copyright trademark sìmo dal 2025 fino all'oltretomba
    altra roba legale innecessaria blah blah blah fanculo la legge piratate i programmi adobe

    ascoltate yakui the maid su spotify/bandcamp
*/

#pragma once

#include <map>
#include <string>
#include <functional>
#include <vector>
#include <iostream>
#include <fstream>
#include <stack>

#include "../Game/Sprite.hpp"
#include "../Game/Texture.hpp"

// TURN_BITS_TO_ZERO(0b11010011, 0b01100001) = 0b10010010
#define TURN_BITS_TO_ZERO(VAR, NUMBER) VAR = (VAR | NUMBER) ^ NUMBER

#define REGISTERS_IN_MEMORY 4

using std::map;
using std::string;
using std::function;
using std::vector;
using std::pair;
using std::fstream;
using std::stack;

using std::cout;
using std::stoi;

using Gigi::Sprite;

// GigiRegisters

namespace Gigi::Assembly {

    struct Registers {
        static unsigned long programCounter;

        static string registerNames[REGISTERS_IN_MEMORY];

        static bool interrupted;

        static vector<Sprite> storedSprites;
        static vector<Image> storedImages;

        /*
        usa i bit del numero 8 bit per significare quali flag sono alzate

        primo bit = sign flag; dipendente dal primo bit del numero
        secondo bit = zero bit; se il numero è zero
        terzo bit = overflow; se avviene un overflow (es. 0 - 1 = 255(?), 255(?) + 1 = 0)

        */
        static uint8_t logicalFlag;

        /*
        una matrice con tutti i dati disponibili al programma.
        i registri sono conservati qua dentro, con posti riservati alle prime posizioni (o come si dici in termini tecnici "davanti la matrice") del vettore.
        i registri non possono essere accessi tramite l'indirizzo/indice del vettore poiché un offset è hardcoded nell'accesso dei dati
        */
        static vector<short> data;

        // lo stack di dove vengono tenute posizioni nel codice da cui riprendere quando si finisce una funzione
        static stack<unsigned int> callStack;

        // tiene nomi di funzioni e le linee a cui iniziano
        static map<string, unsigned int> functions;

        // crea memoria finché la dimensione dei dati non equivale index
        inline static void createMemoryIfNone(int index);

        // una funzione per ottenere i dati del vettore data in modo più pulito, aggiunge un offset all'indice/indirizzo
        inline static short* getData(int address);

        // ottiene la logical flag della variabile inserita e la passa a logicalFlag
        static void Compare(int a);

        // inizializzazione registri nella memoria
        static void AddRegistersToData();
    };

    // GigiAssemblyInstructions

    extern vector<string> argStack;

    string getArgFromTopStr(int index);
    int getArgFromTop(int index);
    string formatDataString(int index);
    void formatStringData(int index, string str);

    extern map<string, function<void()>> asmInstructions;

    // GigiAssemblyInterpreter

    struct Interpreter {
        static bool running;

        // dove viene tenuto il codice del programma. l'indice è la linea/posizione dell'istruzione
        static vector<string> programInstructions;

        static void InterpretLine(string str);

        static void InterpretLines(vector<string> lines);

        static void StepProgram();
    };
}