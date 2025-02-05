/*
    GIGIBOX: MERSEMBLY
    qui è dove succede il ben di dio
    
    tutte le operazioni di ciascun istruzione vengono tenute nelle loro funzioni lambda proprie
    queste funzioni lambda vengono tenute dentro l'enorme hashmap assieme alle loro stringhe rappresentantive
*/

#include "GigiAssembly.hpp"

// le funzioni che poi vengono chiamate dall'interprete per eseguire una specifica istruzione
map<string, function<void()>> asmInstructions = { // assegnazione instruzioni asm
    {
        "mov",
        []() {
            int where = getArgFromTop(1); // il dato che viene trasferito
            short* to = Gigi_AssemblyRegisters::getData(getArgFromTop(0)); // dove viene trasferito il dato

            // to = where
            *to = where;
        }
    },
    {
        "add",
        []() {
            *Gigi_AssemblyRegisters::getData(-1) += getArgFromTop(0);
        }
    },
    {
        "sub",
        []() {
            *Gigi_AssemblyRegisters::getData(-1) -= getArgFromTop(0);
        }
    },
    {
        "mul",
        []() {
            *Gigi_AssemblyRegisters::getData(-1) *= getArgFromTop(0);
        }
    },
    {
        "div",
        []() {
            *Gigi_AssemblyRegisters::getData(-1) /= getArgFromTop(0);
        }
    },
    {
        "mod", // modulo
        []() {
            *Gigi_AssemblyRegisters::getData(-1) %= getArgFromTop(0);
        }
    },
    {
        "jmp", // salto incondizionale
        []() {
            Gigi_AssemblyRegisters::programCounter = getArgFromTop(0) - 1; // non so perché cazzo serve il - 1 ma NON TOCCARE per favore :(
            // a volte la mia mente da genio è incomprensibile pure al me del futuro
            // ah ora ho capito perché (dopo quasi una settimana). dopo il compimento di questa istruzione il program counter viene incrementato e senza la negazione dell'incremento salterebbe una riga in più
            // persone a caso su github che leggono come stia parlando con me stesso tutto sto tempo pensando stia impazzendo
        }
    },
    {
        "cmp", // compara il registro A con il primo argomento
        []() {
            Gigi_AssemblyRegisters::Compare(getArgFromTop(0));
        }
    },
    {
        "lda", // carica dati al registro A
        []() {
            *Gigi_AssemblyRegisters::getData(-1) = getArgFromTop(0);
        }
    },
    {
        "sta", // salva il contenuto di A a un indirizzo
        []() {
            *Gigi_AssemblyRegisters::getData(getArgFromTop(0)) = *Gigi_AssemblyRegisters::getData(-1);
        }
    },
    {
        "int", // interrompe l'interpretazione per continuare il ciclo in main
        []() {
            Gigi_AssemblyRegisters::interrupted = true;

            switch (getArgFromTop(0)) {
            default:
                break;
            case 1: { // stampa un messaggio dalla memoria. registro A: indirizzo stringa.
                string buf = formatDataString(*Gigi_AssemblyRegisters::getData(-1));

                cout << buf;
                break;
            }
            case 2: { // crea una texture caricando i dati di un file a percorso dato da una stringa data.
                // registro A: indirizzo stringa nome file. registro B: indirizzo salvataggio texture
                string buf = formatDataString(*Gigi_AssemblyRegisters::getData(-1));

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

                    Gigi_AssemblyRegisters::storedImages.push_back(Gigi_Image(data));

                    int textureIndex = Gigi_AssemblyRegisters::storedImages.size() - 1;
                    Gigi_Image* textureref = &Gigi_AssemblyRegisters::storedImages[textureIndex];

                    // aggiunta dati riconoscimento al vettore
                    int textreDataIndex = *Gigi_AssemblyRegisters::getData(-2);
                    Gigi_AssemblyRegisters::createMemoryIfNone(textreDataIndex);
                    *Gigi_AssemblyRegisters::getData(textreDataIndex) = textureIndex;

                    stream.close();
                }
                catch (int er) {
                    throw 0b11100010; // 111000.. = errore texture, ..10 = errore generale
                }

                break;
            }
            case 3: { // crea uno sprite. registro A: indice texture. B: indirizzo salvataggio dei dati sprite nel vettore. ~~C: visibilità sprite alla creazione~~
                try {
                    int txtind = *Gigi_AssemblyRegisters::getData(*Gigi_AssemblyRegisters::getData(-1));
                    Gigi_AssemblyRegisters::storedSprites.push_back(Gigi_Sprite(Gigi_AssemblyRegisters::storedImages[txtind]));
                    int last = Gigi_AssemblyRegisters::storedSprites.size() - 1;

                    // salvataggio sprite nel vettore
                    Gigi_Sprite* sprRef = &Gigi_AssemblyRegisters::storedSprites[last];
                    Gigi_AssemblyRegisters::createMemoryIfNone(*Gigi_AssemblyRegisters::getData(-2) + 3);
                    short* sprDataIndx = Gigi_AssemblyRegisters::getData(*Gigi_AssemblyRegisters::getData(-2)); // riferenza all'indirizzo in data dove lo sprite verrà salvato

                    *sprDataIndx = *Gigi_AssemblyRegisters::getData(-1); // indice texure
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
            if ((Gigi_AssemblyRegisters::logicalFlag & 0b01000000) == 0b01000000)
                Gigi_AssemblyRegisters::programCounter = getArgFromTop(0) - 1;
        }
    },
    {
        "jne",
        []() {
            if ((Gigi_AssemblyRegisters::logicalFlag & 0b01000000) == 0b00000000)
                Gigi_AssemblyRegisters::programCounter = getArgFromTop(0) - 1;
        }
    },
    {
        "jg",
        []() {
            if ((Gigi_AssemblyRegisters::logicalFlag & 0b00100000) == 0b00100000)
                Gigi_AssemblyRegisters::programCounter = getArgFromTop(0) - 1;
        }
    },
    {
        "jge",
        []() {
            if ((Gigi_AssemblyRegisters::logicalFlag & 0b00100000) == 0b00100000 || (Gigi_AssemblyRegisters::logicalFlag & 0b01000000) == 0b01000000)
                Gigi_AssemblyRegisters::programCounter = getArgFromTop(0) - 1;
        }
    },
    {
        "jl",
        []() {
            if ((Gigi_AssemblyRegisters::logicalFlag & 0b01100000) == 0b00000000)
                Gigi_AssemblyRegisters::programCounter = getArgFromTop(0) - 1;
        }
    },
    {
        "jle",
        []() {
            if ((Gigi_AssemblyRegisters::logicalFlag & 0b01100000) == 0b00000000 || (Gigi_AssemblyRegisters::logicalFlag & 0b01000000) == 0b00000000)
                Gigi_AssemblyRegisters::programCounter = getArgFromTop(0) - 1;
        }
    },
    {
        "lsa", // last address: imposta il valore di A a l'ultimo indirizzo di memoria disponibile nella matrice
        []() {
            *Gigi_AssemblyRegisters::getData(-1) = Gigi_AssemblyRegisters::data.size() - REGISTERS_IN_MEMORY - 1;
        }
    },
    {
        "mal", // memory allocation: alloca memoria nel vettore dati
        []() {
            Gigi_AssemblyRegisters::data.push_back(getArgFromTop(0));
        }
    },
    {
        "bma", // bulk memory allocation: alloca N celle di memoria vuota (0)
        []() {
            int c = getArgFromTop(0);
            for (int i = 0; i < c; i++)
                Gigi_AssemblyRegisters::data.push_back(0);
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
                Gigi_AssemblyRegisters::createMemoryIfNone(getArgFromTop(0) + *Gigi_AssemblyRegisters::getData(getArgFromTop(1)));

                formatStringData(getArgFromTop(0), formatDataString(getArgFromTop(1)));
            }
            else { // valore assoluto
                index++;
                buffer = getArgFromTopStr(index);
                while (buffer != "\"") {
                    if (buffer == "\\\"") buffer = "\"";

                    int toAl = buffer.size();
                    if (index > 2) {
                        *Gigi_AssemblyRegisters::getData(pos + size + 1) = ' ';
                        toAl++;
                    }
                    Gigi_AssemblyRegisters::createMemoryIfNone(toAl);

                    for (int i = 0; i < buffer.size(); i++) {
                        *Gigi_AssemblyRegisters::getData(pos + 1 + i) = buffer[i];
                    }

                    size += buffer.size();
                    index++;
                    buffer = getArgFromTopStr(index);
                }
                *Gigi_AssemblyRegisters::getData(pos) = size;
            }
        }
    },
    {
        "inc", // incrementa A
        []() {
            *Gigi_AssemblyRegisters::getData(-1) += 1;
        }
    },
    {
        "dec", // decrementa A
        []() {
            *Gigi_AssemblyRegisters::getData(-1) -= 1;
        }
    },
    {
        "and", // A = A & primo argomento
        []() {
            *Gigi_AssemblyRegisters::getData(-1) &= getArgFromTop(0);
        }
    },
    {
        "or", // A = A | primo argomento
        []() {
            *Gigi_AssemblyRegisters::getData(-1) |= getArgFromTop(0);
        }
    },
    {
        "xor", // A = A | primo argomento
        []() {
            *Gigi_AssemblyRegisters::getData(-1) ^= getArgFromTop(0);
        }
    },
    {
        "not", // A = ~A
        []() {
            *Gigi_AssemblyRegisters::getData(-1) = ~(*Gigi_AssemblyRegisters::getData(-1));
        }
    },
    {
        "test", // cmp A & primo argomento
        []() {
            Gigi_AssemblyRegisters::Compare(*Gigi_AssemblyRegisters::getData(-1) & getArgFromTop(0));
        }
    },
    {
        "swp", // swap
        []() {
            short* first = Gigi_AssemblyRegisters::getData(getArgFromTop(1));
            short* second = Gigi_AssemblyRegisters::getData(getArgFromTop(0));

            short c = *second;

            *second = *first;
            *first = c;
        }
    },
    {
        "fnc", // function
        []() {
            Gigi_AssemblyRegisters::functions.insert(pair<string, unsigned int>(getArgFromTopStr(0), Gigi_AssemblyRegisters::programCounter));
        }
    },
    {
        "call", // chiama una funzione definita dall'utente
        []() {
            Gigi_AssemblyRegisters::callStack.push(Gigi_AssemblyRegisters::programCounter);
            Gigi_AssemblyRegisters::programCounter = Gigi_AssemblyRegisters::functions.find(getArgFromTopStr(0))->second;
        }
    },
    {
        "ret", // ritorna alla linea di codice dopo l'esecuzione di una funzione
        []() {
            Gigi_AssemblyRegisters::programCounter = Gigi_AssemblyRegisters::callStack.top();
            Gigi_AssemblyRegisters::callStack.pop();
        }
    }
};