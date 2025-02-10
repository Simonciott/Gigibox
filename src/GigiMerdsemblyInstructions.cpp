/*
    GIGIBOX: MERSEMBLY
    qui è dove succede il ben di dio
    
    tutte le operazioni di ciascun istruzione vengono tenute nelle loro funzioni lambda proprie
    queste funzioni lambda vengono tenute dentro l'enorme hashmap assieme alle loro stringhe rappresentantive
*/

#include "GigiAssembly.hpp"

using namespace Gigi::Assembly;

// le funzioni che poi vengono chiamate dall'interprete per eseguire una specifica istruzione
map<string, function<void()>> Gigi::Assembly::asmInstructions = { // assegnazione instruzioni asm
    {
        "mov",
        []() {
            int where = getArgFromTop(1); // il dato che viene trasferito
            short* to = Registers::getData(getArgFromTop(0)); // dove viene trasferito il dato

            // to = where
            *to = where;
        }
    },
    {
        "add",
        []() {
            *Registers::getData(-1) += getArgFromTop(0);
        }
    },
    {
        "sub",
        []() {
            *Registers::getData(-1) -= getArgFromTop(0);
        }
    },
    {
        "mul",
        []() {
            *Registers::getData(-1) *= getArgFromTop(0);
        }
    },
    {
        "div",
        []() {
            *Registers::getData(-1) /= getArgFromTop(0);
        }
    },
    {
        "mod", // modulo
        []() {
            *Registers::getData(-1) %= getArgFromTop(0);
        }
    },
    {
        "jmp", // salto incondizionale
        []() {
            Registers::programCounter = getArgFromTop(0) - 1; // non so perché cazzo serve il - 1 ma NON TOCCARE per favore :(
            // a volte la mia mente da genio è incomprensibile pure al me del futuro
            // ah ora ho capito perché (dopo quasi una settimana). dopo il compimento di questa istruzione il program counter viene incrementato e senza la negazione dell'incremento salterebbe una riga in più
            // persone a caso su github che leggono come stia parlando con me stesso tutto sto tempo pensando stia impazzendo
        }
    },
    {
        "cmp", // compara il registro A con il primo argomento
        []() {
            Registers::Compare(getArgFromTop(0));
        }
    },
    {
        "lda", // carica dati al registro A
        []() {
            *Registers::getData(-1) = getArgFromTop(0);
        }
    },
    {
        "sta", // salva il contenuto di A a un indirizzo
        []() {
            *Registers::getData(getArgFromTop(0)) = *Registers::getData(-1);
        }
    },
    {
        "int", // interrompe l'interpretazione per continuare il ciclo in main
        []() {
            Registers::interrupted = true;

            switch (getArgFromTop(0)) {
            default:
                break;
            case 1: { // stampa un messaggio dalla memoria. registro A: indirizzo stringa.
                string buf = formatDataString(*Registers::getData(-1));

                cout << buf;
                break;
            }
            case 2: { // crea una texture caricando i dati di un file a percorso dato da una stringa data.
                // registro A: indirizzo stringa nome file. registro B: indirizzo salvataggio texture
                string buf = formatDataString(*Registers::getData(-1));

                try {
                    fstream stream;
                    stream.open("./" + buf + ".gimg"); // DID: RISOLVERE QUESTO (quando si usa un nome file dinamico (a variabile), il file dà errori e non si apre)
                    // RISOLTO! NOTA: NON DARE ALLE STRINGHE NULL ADDIZIONALI. INTERFERIRANNO CON TUTTO

                    if (!stream.is_open() || stream.fail()) throw 0b11100000;

                    //cout << stream.is_open() << endl << stream.bad() << endl << stream.fail() << endl;

                    string filedata;

                    unsigned char charbuffer;
                    int datasize = 0;

                    int metadata[3];

                    for (int i = 0; i < 3; i++) {
                        charbuffer = stream.get();
                        filedata += charbuffer;
                        datasize++;
                        metadata[i] = charbuffer + 1;
                    }

                    while (datasize <= 3 * metadata[2] + 2 + metadata[0] * metadata[1] + 2) { // non so perché serve il + 2 alla fine ma non toccare manco quello
                        charbuffer = stream.get();
                        filedata += charbuffer;
                        datasize++;
                    }

                    uint8_t* data = (uint8_t*)malloc(datasize);
                    for (int i = 0; i < filedata.size(); i++) {
                        data[i] = filedata[i];
                    }

                    Registers::storedImages.push_back(Gigi::Image(data));

                    int textureIndex = Registers::storedImages.size() - 1;
                    Gigi::Image* textureref = &Registers::storedImages[textureIndex];

                    // aggiunta dati riconoscimento al vettore
                    int textreDataIndex = *Registers::getData(-2);
                    Registers::createMemoryIfNone(textreDataIndex);
                    *Registers::getData(textreDataIndex) = textureIndex;

                    stream.close();
                }
                catch (int er) {
                    throw 0b11100010; // 111000.. = errore texture, ..10 = errore generale
                }

                break;
            }
            case 3: { // crea uno sprite. registro A: indice texture. B: indirizzo salvataggio dei dati sprite nel vettore. ~~C: visibilità sprite alla creazione~~
                try {
                    int txtind = *Registers::getData(*Registers::getData(-1));
                    Registers::storedSprites.push_back(Sprite(Registers::storedImages[txtind]));
                    int last = Registers::storedSprites.size() - 1;

                    // salvataggio sprite nel vettore
                    Sprite* sprRef = &Registers::storedSprites[last];
                    Registers::createMemoryIfNone(*Registers::getData(-2) + 3);
                    short* sprDataIndx = Registers::getData(*Registers::getData(-2)); // riferenza all'indirizzo in data dove lo sprite verrà salvato

                    *sprDataIndx = *Registers::getData(-1); // indice texure
                    sprRef->xHook = (sprDataIndx + 1);
                    sprRef->yHook = (sprDataIndx + 2);
                    sprRef->visibleHook = (bool*)(sprDataIndx + 3);

                    *sprRef->visibleHook = true;
                }
                catch (int er) {
                    throw 0b11010010; // 11010... = errore sprite
                }
            }
            }
        }
    },

    // salti condizionali
    {
        "je",
        []() {
            if ((Registers::logicalFlag & 0b01000000) == 0b01000000)
                Registers::programCounter = getArgFromTop(0) - 1;
        }
    },
    {
        "jne",
        []() {
            if ((Registers::logicalFlag & 0b01000000) == 0b00000000)
                Registers::programCounter = getArgFromTop(0) - 1;
        }
    },
    {
        "jg",
        []() {
            if ((Registers::logicalFlag & 0b00100000) == 0b00100000)
                Registers::programCounter = getArgFromTop(0) - 1;
        }
    },
    {
        "jge",
        []() {
            if ((Registers::logicalFlag & 0b00100000) == 0b00100000 || (Registers::logicalFlag & 0b01000000) == 0b01000000)
                Registers::programCounter = getArgFromTop(0) - 1;
        }
    },
    {
        "jl",
        []() {
            if ((Registers::logicalFlag & 0b01100000) == 0b00000000)
                Registers::programCounter = getArgFromTop(0) - 1;
        }
    },
    {
        "jle",
        []() {
            if ((Registers::logicalFlag & 0b01100000) == 0b00000000 || (Registers::logicalFlag & 0b01000000) == 0b00000000)
                Registers::programCounter = getArgFromTop(0) - 1;
        }
    },
    {
        "lsa", // last address: imposta il valore di A a l'ultimo indirizzo di memoria disponibile nella matrice
        []() {
            *Registers::getData(-1) = Registers::data.size() - REGISTERS_IN_MEMORY - 1;
        }
    },
    {
        "mal", // memory allocation: alloca memoria nel vettore dati
        []() {
            Registers::data.push_back(getArgFromTop(0));
        }
    },
    {
        "bma", // bulk memory allocation: alloca N celle di memoria vuota (0)
        []() {
            int c = getArgFromTop(0);
            for (int i = 0; i < c; i++)
                Registers::data.push_back(0);
        }
    },
    {
        "str", // string: alloca una stringa a una certa posizione nella memoria. da incapsulare la stringa in virgolette spaziate (" esempio "). usare backslash per avere virgolette nella stringa
        // se il secondo parametro è un numero/indirizzo, si supporrà che il secondo parametro si riferisca a un'altra stringa da duplicare alla posizione data (primo parametro)
        []() {
            int pos = getArgFromTop(0);
            //string str;
            int index = 1, size = 0;
            string buffer = getArgFromTopStr(index);

            if (buffer != "\"") { // duplicazione stringa
                Registers::createMemoryIfNone(getArgFromTop(0) + *Registers::getData(getArgFromTop(1)));

                formatStringData(getArgFromTop(0), formatDataString(getArgFromTop(1)));
            }
            else { // valore assoluto
                index++;
                buffer = getArgFromTopStr(index);
                while (buffer != "\"") {
                    if (buffer == "\\\"") buffer = "\"";

                    int toAl = buffer.size();
                    if (index > 2) {
                        *Registers::getData(pos + size + 1) = ' ';
                        toAl++;
                    }
                    Registers::createMemoryIfNone(toAl);

                    for (int i = 0; i < buffer.size(); i++) {
                        *Registers::getData(pos + 1 + i) = buffer[i];
                    }

                    size += buffer.size();
                    index++;
                    buffer = getArgFromTopStr(index);
                }
                *Registers::getData(pos) = size;
            }
        }
    },
    {
        "inc", // incrementa A
        []() {
            *Registers::getData(-1) += 1;
        }
    },
    {
        "dec", // decrementa A
        []() {
            *Registers::getData(-1) -= 1;
        }
    },
    {
        "and", // A = A & primo argomento
        []() {
            *Registers::getData(-1) &= getArgFromTop(0);
        }
    },
    {
        "or", // A = A | primo argomento
        []() {
            *Registers::getData(-1) |= getArgFromTop(0);
        }
    },
    {
        "xor", // A = A | primo argomento
        []() {
            *Registers::getData(-1) ^= getArgFromTop(0);
        }
    },
    {
        "not", // A = ~A
        []() {
            *Registers::getData(-1) = ~(*Registers::getData(-1));
        }
    },
    {
        "test", // cmp A & primo argomento
        []() {
            Registers::Compare(*Registers::getData(-1) & getArgFromTop(0));
        }
    },
    {
        "swp", // swap
        []() {
            short* first = Registers::getData(getArgFromTop(1));
            short* second = Registers::getData(getArgFromTop(0));

            short c = *second;

            *second = *first;
            *first = c;
        }
    },
    {
        "fnc", // function
        []() {
            Registers::functions.insert(pair<string, unsigned int>(getArgFromTopStr(0), Registers::programCounter));
            while (Interpreter::programInstructions[Registers::programCounter] != "ret") {
                if (Registers::programCounter >= Interpreter::programInstructions.size())
                    throw 0b10000100; // 100001.. = errore ret. ..00 = errore assenza
                Registers::programCounter++;
            }
        }
    },
    {
        "call", // chiama una funzione definita dall'utente
        []() {
            Registers::callStack.push(Registers::programCounter);
            Registers::programCounter = Registers::functions.find(getArgFromTopStr(0))->second;
        }
    },
    {
        "ret", // ritorna alla linea di codice dopo l'esecuzione di una funzione
        []() {
            Registers::programCounter = Registers::callStack.top();
            Registers::callStack.pop();
        }
    }
};