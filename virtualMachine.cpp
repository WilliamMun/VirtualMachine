#include <iostream>
#include <string>
#include <stdexcept>
using namespace std; 
// ==========================================
// Class Header
// ==========================================

// ==========================================
// EXCEPTION HANDLING
// ==========================================
class VMException {
    private:
        const char* errorMessage;
    public:
        VMException(const char* msg) { errorMessage = msg; }
        const char* getErrorMessage() const { return errorMessage; }
};

// ==========================================
// 1. DATA STRUCTURES
// ==========================================
// ==========================================
// A. CUSTOM VECTOR (Dynamic Array)
// ==========================================
template <typename T>
class CustomVector {
private:
    T* arr;
    int capacity;
    int current_size;

    // Helper function to resize the array when it gets full
    void resize() {
        capacity *= 2; // Double the capacity
        T* temp = new T[capacity]; // Create a new, bigger array
        
        // Copy old elements to the new array
        for (int i = 0; i < current_size; i++) {
            temp[i] = arr[i];
        }
        
        delete[] arr; // Delete the old, small array
        arr = temp;   // Point to the new array
    }

public:
    CustomVector() {
        capacity = 2; // Start with a small capacity
        current_size = 0;
        arr = new T[capacity];
    }
    
    ~CustomVector() {
        delete[] arr; // Prevent memory leaks
    }

    void push_back(T element) {
        // If the array is full, resize it first
        if (current_size == capacity) {
            resize();
        }
        arr[current_size] = element;
        current_size++;
    }
    
    // NOTE: Changed to T& (pass by reference) so the popped value is actually returned to the caller
    void pop_back(T& element) {
        if (current_size == 0) {
            throw underflow_error("Vector is empty!");
        }
        element = arr[current_size - 1]; // Grab the last element
        current_size--; // Logically remove it by shrinking the size
    }

    T at(int index) const {
        if (index < 0 || index >= current_size) {
            throw out_of_range("Index out of bounds!");
        }
        return arr[index];
    }

    T &operator[](int index) { 
        if (index < 0 || index >= current_size) {
            throw out_of_range("Index out of bounds!");
        }
        return arr[index];
    }
    
    const T &operator[](int index) const { 
        if (index < 0 || index >= current_size) {
            throw out_of_range("Index out of bounds!");
        }
        return arr[index];
    }
    
    int size() const {
        return current_size;
    }
};

// ==========================================
// B. CUSTOM STACK (Last-In, First-Out)
// ==========================================
template <typename T>
class CustomStack {
private:
    T* arr;
    int topIndex;
    int maxCapacity;

public:
    CustomStack(int size = 8) {
        maxCapacity = size;
        topIndex = -1; 
        arr = new T[maxCapacity];
    }
    
    ~CustomStack() {
        delete[] arr;
    }

    void push(T element) {
        if (isFull()) {
            throw overflow_error("Stack Overflow! Cannot push.");
        }
        topIndex++;
        arr[topIndex] = element;
    }

    void pop(T& element) {
        if (isEmpty()) {
            throw underflow_error("Stack Underflow! Cannot pop.");
        }
        element = arr[topIndex];
        topIndex--;
    }

    bool isEmpty() const {
        return topIndex == -1;
    }
    
    bool isFull() const {
        return topIndex == maxCapacity - 1;
    }
};

// ==========================================
// C. CUSTOM QUEUE (First-In, First-Out)
// ==========================================
template <typename T>
class CustomQueue {
private:
    T* arr;
    int frontIndex;
    int rearIndex;
    int current_size;
    int maxCapacity;

public:
    CustomQueue(int size = 100) {
        maxCapacity = size;
        arr = new T[maxCapacity];
        frontIndex = 0;
        rearIndex = -1;
        current_size = 0;
    }
    
    ~CustomQueue() {
        delete[] arr;
    }

    void enqueue(T element) {
        if (current_size == maxCapacity) {
            throw overflow_error("Queue is full!");
        }
        rearIndex = (rearIndex + 1) % maxCapacity; 
        arr[rearIndex] = element;
        current_size++;
    }

    void dequeue(T &element) {
        if (isEmpty()) {
            throw underflow_error("Queue is empty!");
        }
        element = arr[frontIndex];
        frontIndex = (frontIndex + 1) % maxCapacity;
        current_size--;
    }

    bool isEmpty() const {
        return current_size == 0;
    }
    
    int size() const {
        return current_size;
    }

    void clear() {
        frontIndex = 0;
        rearIndex = -1;
        current_size = 0;
    } 
};

// ==========================================
// 2. REGISTER HIERARCHY
// ==========================================

// Base class encapsulating an 8-bit signed value 
class Register {
    private:
        signed char value; // 1 byte (signed char, -128 to 127) 

    public:
        Register() : value(0) {}
        virtual ~Register() {}

        // Encapsulation: getters and setters 
        signed char getValue() const { return value; }
        void setValue(signed char v) { value = v; }
};

// Represents R0-R7 registers 
class DataRegister : public Register {
    public:
        DataRegister() : Register() {}
};

// Manages individual flag bits (CF, OF, UF, ZF) 
class FlagRegister {
    private:
        bool CF, OF, UF, ZF;

    public:
        FlagRegister() : CF(false), OF(false), UF(false), ZF(false) {}

        // Getters and setters for flags 
        bool getCF() const { return CF; }
        void setCF(bool val) { CF = val; }
    
        bool getOF() const { return OF; }
        void setOF(bool val) { OF = val; }

        bool getUF() const { return UF; }
        void setUF(bool val) { UF = val; }

        bool getZF() const { return ZF; }
        void setZF(bool val) { ZF = val; }
    
        void resetAll() { CF = OF = UF = ZF = false; }
};

// ==========================================
// 3. MEMORY
// ==========================================

// Handles storage and addressing logic over a vector of bytes 
class Memory {
    private:
        signed char data[64]; // 1-dimensional array of 64 signed bytes

    public: 
        Memory(); // Default constructor
        Memory(const Memory &mem); // Copy constructor
        Memory& operator=(const Memory& other); // Copy assignment operator
        signed char read(int address) const; // 
        void write(int address, signed char value);
        void displayMemory();
};

// ==========================================
// 4. CPU (CENTRAL PROCESSING UNIT)
// ==========================================

// Contains registers, memory, PC, and executes instructions
class CPU {
private:
    DataRegister R[8];     // R0 to R7 
    FlagRegister flags;       // Aggregated flags
    Memory memory;            // Composed memory
 
    unsigned char PC;         // Program Counter (1 byte, starts at 0)
    unsigned char SI;         // Stack Index (1 byte, starts at 0)
 
    CustomStack<signed char> systemStack;

public:
    CPU() : PC(0), SI(0) {}

    // Getters to allow instructions to manipulate CPU state
    DataRegister* getRegister(int index) { return &R[index]; }
    FlagRegister* getFlags() { return &flags; }
    Memory* getMemory() { return &memory; }
 
    unsigned char getPC() const { return PC; }
    void incrementPC() { PC++; }
 
    unsigned char getSI() const { return SI; }
    void incrementSI() { SI++; }
    void decrementSI() { SI--; }

    void updateMathFlags(int result)
    {
        flags.resetAll();
        if (result == 0) {flags.setZF(true);}
        if (result > 127) {flags.setOF(true); flags.setCF(true);}
        if (result < -128) {flags.setUF(true); flags.setCF(true);}
    }
};

// ==========================================
// 5. INSTRUCTION HIERARCHY
// ==========================================

// Abstract base class for all assembly commands 
class Instruction {
public:
    Instruction() = default;
    virtual ~Instruction() {} 
    // Virtual polymorphism 
    virtual void execute(CPU& cpu) = 0; 
};

// arithmethic instruction derived class
class ArithmeticInstruction : public Instruction {
private:
    string ar; //"ADD", "SUB", "MUL", "DIV"
    int destRI; //destination register index
    int sourceRI; //source register index
    int compute(int v1, int v2){ // compute the operation and return the value
        if (ar == "ADD") return v1 + v2;
        if (ar == "SUB") return v1 - v2;
        if (ar == "MUL") return v1 * v2;
        if (ar == "DIV") {
            if (v2 == 0) throw VMException("Error: Division by 0."); // throw exception when v1 is divided by 0
            return v1 / v2;
        }
        if (ar != "ADD" && ar != "SUB" && ar != "MUL" && ar != "DIV") throw VMException ("Error: Invalid operation."); // throw exception when operation invalid
        return 0;
    }
public:
    ArithmeticInstruction(string operation, int dest, int source):ar(operation), destRI(dest), sourceRI(source){}; // creating an instruction, example: ADD,R1,R2
    virtual ~ArithmeticInstruction() override = default;
    void execute(CPU& cpu) override { 
        DataRegister* destReg = cpu.getRegister(destRI); //fetch the pointer to destination register
        FlagRegister* flags = cpu.getFlags();  // fetch the pointer to cpu flag register
        int val1 = destReg->getValue(); // read the current int value from destination
        int val2 = cpu.getRegister(sourceRI)->getValue(); //read current int value from source register
        flags->resetAll(); //clear all cpu flags
        int result = compute(val1, val2); // perform math operation
        if (result > 255 || result < -256) flags->setCF(true); //set cf if result out of 9 bit signed boundaries
        if (result > 127) flags->setOF(true); //set of if result exceed 8 bit
        if (result < -128) flags->setUF(true); //set uf if result below 8 bit
        signed char fResult = static_cast<signed char>(result); // force 32 bit result into 8 bit
        destReg->setValue(fResult); // write final result to destination register
        if (fResult == 0) flags->setZF(true); // set zf if final value = 0
    }
};

// increment and decrement instruction derived class
class IncDecInstruction : public Instruction{
private:
    string op; // "INC", "DEC"
    int registerIdx; // variable register index
public:
    IncDecInstruction(string operation, int idx) : op(operation), registerIdx(idx) {} //inc dec constructor, example: INC,R[2]
    void execute(CPU& cpu) override{
        DataRegister* reg = cpu.getRegister(registerIdx); //fetch the pointer to register
        FlagRegister* flags = cpu.getFlags(); // fetch the pointer to cpu flag register
        int result = reg->getValue(); // read the current int value from register
        flags->resetAll(); //clear all cpu flags
        if (op == "INC") {
            result = result + 1; // if operation = increment, result + 1
        } else if (op == "DEC"){
            result = result - 1; // if operation = decrement, result - 1
        } else {
            throw VMException("Error: Invalid operation."); // if not inc or dec, throw exception 
        }
        if (result > 255 || result < -256) flags->setCF(true); //set cf if result out of 9 bit signed boundaries
        if (result > 127) flags->setOF(true); //set of if result exceed 8 bit
        if (result < -128) flags->setUF(true);//set uf if result below 8 bit
        signed char fResult = static_cast<signed char>(result); // force 32 bit result into 8 bit
        reg->setValue(fResult); // write final result to destination register
        if (fResult == 0) flags->setZF(true); // set zf if final value = 0
    }
};

// move instruction derived class
class MoveInstruction : public Instruction{
private:
    int mode; // 1: Immediate, 2: Register-Register, 3: Register-Indirect, 4: Load, 5: Store Address-Register 6:[R?]-R?
    int destI; //destination index
    int sourceI; //source index
    void executeStore(CPU& cpu, Memory* memory){
        if (mode == 5){ //store addres, register
            memory->write(destI, cpu.getRegister(sourceI)->getValue());
        } else if (mode == 6){ //store [register], register
            int address = cpu.getRegister(destI)->getValue();
            memory->write(address, cpu.getRegister(sourceI)->getValue());
        }
    }
public:
    MoveInstruction(int moveMode, int dest, int source): mode(moveMode), destI(dest), sourceI(source){} //move instruction constructor, example: 1, R1, R2
    void execute(CPU& cpu) override{
        Memory* memory = cpu.getMemory(); // fetch the pointer to memory
        if (mode == 1){ //MOV register, intermediate
            cpu.getRegister(destI)->setValue(static_cast<signed char>(sourceI));
        } else if (mode == 2){ // MOV register, register
            cpu.getRegister(destI)->setValue(cpu.getRegister(sourceI)->getValue());
        } else if (mode == 3 || mode == 4){ // MOV register, [register] or LOAD register, [address]
            int address;
            if (mode == 3){ 
                address = cpu.getRegister(sourceI)->getValue(); //get address stored inside the register
            } else {
                address = sourceI; //sourceI is the literal address
            }
            int dataFromMemory = memory->read(address); //fetch data from that memory address
            cpu.getRegister(destI)->setValue(dataFromMemory); // store it in destination register
        } else {
            executeStore(cpu, memory);
        }
    }
};

// input or display instruction derived class
class IOInstruction : public Instruction {
private:
    string op; //"INPUT" and "DISPLAY"
    int regI; //register array index
public:
    IOInstruction(string operation, int idx) : op(operation), regI(idx) {} // ioi instruction constructor
    void execute(CPU& cpu) override {
        DataRegister* reg = cpu.getRegister(regI); //fetch the pointer to register
        FlagRegister* flags = cpu.getFlags(); // fetch the pointer to cpu flag register
        if (op == "INPUT"){ //check instruction is input command
            cout << "Please enter input value:" << endl;
            int rawInput; // to store user value
            cin >> rawInput; //read user value
            flags->resetAll(); //clear all cpu flags
            if (rawInput > 127) flags->setOF(true); //set of if value > 127
            if (rawInput < -128) flags->setUF(true); // set uf if value < 128
            if (rawInput == 0) flags->setZF(true); // set zf if value = 0
            reg->setValue(static_cast<signed char>(rawInput)); //convert 32-bit integer to 8-bit signed byte
        } else if (op == "DISPLAY") { //check instruction is display command
            cout << static_cast<int>(reg->getValue()) << endl;
        } else if (op != "INPUT" && op != "DISPLAY") { //if not, throw an exception
            throw VMException("Error: Invalid operation.");
        }
    }
};

// shift and rotate instruction derived class
class ShiftInstruction : public Instruction {
private:
    string op; //"SHL" "SHR" "ROR" "ROL"
    int regI; //register array index
    int count; //raw number of bit positions to shift/rotate
public:
    ShiftInstruction(string operation, int idx, int shiftCount): op(operation), regI(idx), count(shiftCount) {} //shift and rotate instruction constructor
    void execute(CPU& cpu) override {
        if (count < 0) return; //immediate execution halt if a negative shift value is provided
        DataRegister* reg = cpu.getRegister(regI); //fetch pointer to register
        FlagRegister* flags = cpu.getFlags(); //fetch pointer to cpu flags
        flags->resetAll(); //clear all flags
        unsigned char val = static_cast<unsigned char>(reg->getValue()); //cast the register's signed value to an unsigned byte
        int dCount = count % 8; // calculate the shift index count within the boundaries of an 8-bit block
        if (count >= 8 && (op == "SHL" || op == "SHR")) val = 0; //shifting left or right by 8 or more positions completely zeroes out the byte
        else if (dCount > 0) {
            if (op == "SHL"){ //shift left
                flags->setCF((val >> (8 - dCount)) & 1); //isolate and capture the last bit pushed out from the left into cf
                val = val << dCount; // push bit to the left
            }else if (op == "SHR"){ //shift right
                flags->setCF((val >> (dCount - 1)) & 1); // isolate and capture the last bit pushed out from the right into cf
                val = val >> dCount; //push bit to the right
            }else if (op == "ROL") { //rotate left
                flags->setCF((val >> (8 - dCount)) & 1);  // cf mirrors the last bit moving out from the left that loops around to the right         
                val = (val << dCount) | (val >> (8 - dCount));      
            }else if (op == "ROR"){ //rotate right
                flags->setCF((val >> (dCount - 1)) & 1); // cf mirrors the last bit moving out from the right that loops around to the left
                val = (val >> dCount) | (val << (8 - dCount));
            }
        }
        signed char fResult = static_cast<signed char>(val); // convert the unsigned byte container back to a signed char format 
        reg->setValue(fResult);
        if (fResult == 0) flags->setZF(true); //set zf to true if the final register value = zero
    }
};

// ==========================================
// 6. RUNNER (INTERPRETER)
// ==========================================

// Loads programs, decodes instructions, delegates execution to CPU 
class Runner {
private:
    CPU virtualMachine; // Composition 
    CustomVector<Instruction*> program; // Polymorphic storage 

    bool isBlankLine(std::string dummy)
    {
        if (dummy.empty()) return true;
        for(int i=0; i < dummy.length(); i++)
        {
            if(dummy[i] != ' ' && dummy[i]!= '\t' && dummy[i]!= '\r' && dummy[i]!= '\n')
            return false;
        }
        return true;
    }

    // Helper function to pad numbers with leading zeroes
    string format4(int num) {
        stringstream belt;
        // setfill('0') tells it to use zeroes. 
        // setw(4) tells it to make sure the string is exactly 4 characters wide.
        belt << setfill('0') << setw(4) << num;
        return belt.str();
    }

    int numberReg(std::string dummy)
    {
        if(dummy.empty()) return 0;
        if(dummy[0] == 'R' || dummy[0] == 'r')
        {
            std::string justNumber = dummy.substr(1);
            return stoi(justNumber);
        }
        return 0;
    }

    Instruction* MathAndLogic(const std::string& first, std::stringstream& rest)
    {
        std::string dest,value;

        if (first == "INC" || first == "DEC") {
            rest >> dest;
            if (first == "INC") return new IncInstruction(numberReg(dest));
            return new DecInstruction(numberReg(dest));
        }
        
        if (first != "ADD" && first != "SUB" && first != "MUL" && first != "DIV" && first != "MOV") return nullptr;
        
        rest >> dest >> value;
        
        // Clean variable 'dest' (Remove the trailing comma)
        if (dest.back() == ',') {
            dest.pop_back(); 
        }
        
        int reg = numberReg(dest);
        
        if (first == "MOV") {
            if (value[0] == 'R' || value[0] == 'r') return new MovRegInstruction(reg, numberReg(value));
            return new MovImmInstruction(reg, stoi(value));
        }
        if (first == "ADD") return new AddImmInstruction(reg, stoi(value));
        if (first == "SUB") return new SubImmInstruction(reg, stoi(value));
        if (first == "MUL") return new MulImmInstruction(reg, stoi(value));
        if (first == "DIV") return new DivImmInstruction(reg, stoi(value));
        
        return nullptr;
    }

    Instruction* MemAndIO(const string& first, stringstream& rest) {
        string a, b;
        if (first == "INPUT") { rest >> a; return new InputInstruction(numberReg(a)); }
        if (first == "DISPLAY") { rest >> a; return new DisplayInstruction(numberReg(a)); }
        if (first == "PUSH") { rest >> a; return new PushInstruction(numberReg(a)); }
        if (first == "POP") { rest >> a; return new PopInstruction(numberReg(a)); }
        
        if (first == "LOAD") {
            rest >> a >> b;
            
            // Clean 'a' (Remove the comma)
            if (a.back() == ',') a.pop_back();
            
            // Clean 'b' (Remove both square brackets for memory addressing)
            if (b.front() == '[') b.erase(0, 1);
            if (b.back() == ']') b.pop_back();
            
            if (b[0] == 'R' || b[0] == 'r') return new LoadRegAddrInstruction(numberReg(a), numberReg(b));
            return new LoadImmAddrInstruction(numberReg(a), stoi(b));
        }
        
        if (first == "STORE") {
            rest >> a >> b;
            
            // Clean 'a' (Remove the comma)
            if (a.back() == ',') a.pop_back();
            
            // Check if 'a' is an indirect address (e.g., "[R2]")
            if (a.front() == '[') {
                a.erase(0, 1); 
                if (a.back() == ']') a.pop_back();
                return new StoreRegAddrInstruction(numberReg(a), numberReg(b));
            }
            return new StoreInstruction(stoi(a), numberReg(b));
        }
        return nullptr;
    }

    Instruction* ShiftAndReset(const string& first, stringstream& rest) {
        string a, b;
        if (first == "RESET") { rest >> a; return new ResetInstruction(a); }
        if (first != "SHL" && first != "SHR" && first != "ROL" && first != "ROR") return nullptr;
        
        rest >> a >> b;
        
        // Clean 'a' (Remove the comma)
        if (a.back() == ',') a.pop_back();
        
        int reg = numberReg(a), count = stoi(b);
        
        if (first == "SHL") return new ShlInstruction(reg, count);
        if (first == "SHR") return new ShrInstruction(reg, count);
        if (first == "ROL") return new RolInstruction(reg, count);
        if (first == "ROR") return new RorInstruction(reg, count);
        return nullptr;
    }

public:
    Runner() {}

    ~Runner() 
    {
        for (int i = 0; i < program.size(); i++)
        {delete program.at(i);}
    }

    void loadProgram(const std::string& filename) {
        // Read .asm file line by line 
        // Decode strings into Instruction objects
        // Store in CustomVector
        ifstream file(filename);
        if(!file.is_open()){
            cout << "Error: Could not open file" << filename << "\n";
            exit(1);
        }

        //store into queue
        CustomQueue<string> lineQueue;
        string line;

        while(getline(file,line))
        {
            if(isBlankLine(line)) continue;
            lineQueue.enqueue(line);
        }
        file.close();

        //dequeue and put into vector
        while(!lineQueue.isEmpty())
        {
            string currentLine;
            lineQueue.dequeue(currentLine);

            stringstream line(currentLine);
            string first;
            line >> first;

            Instruction* inst = MathAndLogic(first, line);
            if (!inst) inst = MemAndIO(first, line);
            if (!inst) inst = ShiftAndReset(first, line);
        
            if (inst) program.push_back(inst); 
            else cout << "Warning: Unrecognized command -> " << first << "\n";
        }
    }

    void executeProgram() {
        // Iterate through CustomVector of instructions
        // Call instruction->execute(virtualMachine)
        // Ensure virtualMachine.incrementPC() is called
        try {
            for (int i = 0; i < program.size(); i++)
            {
                program.at(i) ->execute(virtualMachine);
                virtualMachine.incrementPC();
                dumpState();
            }
        }
        catch(const VMException& e)
        {
            cout << "\n Error: " << e.getErrorMessage() << "\n Stopping";
        }
    }

    void dumpState() {
        // Printing Format After Executing Each Line of Assembly Code: 
	    // #ProgramCounter#0000#
	    // #Flags#Overflow#0#Underflow#0#Carry#0#Zero#0#
        // #Register#R1#0000#R2#0000#R3#0000#R4#0000#R5#0000#R6#0000#R7#0000#R8#0000#
        // #Memory#
	    // #0000#0000#0000#0000#0000#0000#0000#0000#  
	    // #0000#0000#0000#0000#0000#0000#0000#0000# 
	    // #0000#0000#0000#0000#0000#0000#0000#0000# 
	    // #0000#0000#0000#0000#0000#0000#0000#0000# 
	    // #0000#0000#0000#0000#0000#0000#0000#0000# 
	    // #0000#0000#0000#0000#0000#0000#0000#0000# 
	    // #0000#0000#0000#0000#0000#0000#0000#0000# 
	    // #0000#0000#0000#0000#0000#0000#0000#0000# 
	    // Note: All outputs print number in decimal format.

        cout << "#Begin#\n";
        
        cout << "#Registers#";
        for (int i = 0; i < 8; i++) {
            cout << format4((int)virtualMachine.getRegister(i).getValue()) << "#";
        }
        cout << "\n";

        FlagRegister& f = virtualMachine.getFlags();
        cout << "#Flags#" << f.getOF() << "#" << f.getUF() << "#" << f.getCF() << "#" << f.getZF() << "#\n";

        cout << "#PC#" << format4((int)virtualMachine.getPC()) << "#\n";

        cout << "#Memory#\n";
        Memory& mem = virtualMachine.getMemory();
        for (int row = 0; row < 8; row++) {
            cout << "#";
            for (int col = 0; col < 8; col++) {
                int address = (row * 8) + col;
                cout << format4((int)mem.read(address)) << "#";
            }
            cout << "\n";
        }
        
        cout << "#End#\n";
    }
};

// ==========================================
// Class Implementation
// ==========================================
Memory::Memory() // Default constructor
{
    for (int i = 0; i < 64; ++i) {
        data[i] = 0;
    }
}

Memory::Memory(const Memory &mem) // Copy constructor
{
    for(int i = 0; i < 64; i++){
        data[i] = mem.data[i];
    }
}

Memory& Memory::operator=(const Memory& other)
{
    if(this == &other)
        return *this;

    for(int i=0;i<6;i++){
        this->data[i] = other.data[i];
    }

    return *this;
}

signed char Memory::read(int address) const
{
    if (address >= 0 && address < 64) 
        return data[address];
    else 
        throw VMException("Data cannot be displayed due to address out of bound.");
}

void Memory::write(int address, signed char value)
{
    if (address >= 0 && address < 64) 
        this->data[address] = value;
    else
        throw VMException("Data cannot be written due to address out of bound.");
}

void Memory::displayMemory()
{
    cout << "#Memory#" << endl;
    cout << "#";
    for(int i = 0; i < 64; i++){
        if(i == 8 || i == 16 || i == 24 || i == 32 || i == 40 || i == 48 || i == 56){
            cout << endl;
            cout << "#" << data[i] << "#";
        } else {
            cout << data[i] << "#";
        }
    }
    cout << endl;
}

// ==========================================
// ENTRY POINT
// ==========================================
int main() {
    Runner interpreter;
    // Load the program from file / Ask user to enter file name to be compiled
    // Get file and compile the assembly code 
    // Print the VM's state after executed each line of assembly code
    return 0;
}