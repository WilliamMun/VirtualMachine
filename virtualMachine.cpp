#include <iostream>
#include <string>
#include <stdexcept>

// ==========================================
// 1. DATA STRUCTURES
// ==========================================

// ==========================================
// A. CUSTOM VECTOR 
// ==========================================
template <typename T>
class CustomVector {
private:
    T* arr;
    int capacity;
    int current_size;

    // Helper function to resize the array when it gets full
    void resize();

public:
    CustomVector();
    ~CustomVector();

    // Add an element to the end
    void push_back(T element);
	
    // Remove an element from the end
    void pop_back(T element);

    // Access an element at a specific index
    T at(int index) const;

    // Overloaded [] operator to perform bound checking and do resizing 
    T &operator[](int size); // Read and write access
    const T &operator[](int size); // Read only access
    
    // Get the current number of elements
    int size() const;
};

// ==========================================
// B. CUSTOM STACK
// ==========================================
template <typename T>
class CustomStack {
private:
    T* arr;
    int topIndex;
    int maxCapacity;

public:
    // Constructor with default size of 8 (as per VM specs)
    CustomStack(int size = 8);
    ~CustomStack();

    // Push element to the top of the stack
    void push(T element);

    // Remove and return the top element
    void pop(T element);

    // State checks
    bool isEmpty() const;
    bool isFull() const;
};

// ==========================================
// C. CUSTOM QUEUE 
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
    // Constructor with default size
    CustomQueue(int size = 100);
    ~CustomQueue();

    // Add element to the back of the queue
    void enqueue(T element);

    // Remove and return element from the front
    void dequeue(T &element);

    // State checks
    bool isEmpty() const;
    int size() const;

    // Clear queue
    void clear(); 
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
    Memory() {
        for (int i = 0; i < 64; ++i) {
            data[i] = 0;
        }
    }

    signed char read(int address) const {
        if (address >= 0 && address < 64) return data[address];
        return 0; // Handle error appropriately
    }

    void write(int address, signed char value) {
        if (address >= 0 && address < 64) data[address] = value;
    }
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
    DataRegister& getRegister(int index) { return R[index]; }
    FlagRegister& getFlags() { return flags; }
    Memory& getMemory() { return memory; }
 
    unsigned char getPC() const { return PC; }
    void incrementPC() { PC++; }
 
    unsigned char getSI() const { return SI; }
    void incrementSI() { SI++; }
    void decrementSI() { SI--; }
};

// ==========================================
// 5. INSTRUCTION HIERARCHY
// ==========================================

// Abstract base class for all assembly commands 
class Instruction {
public:
    virtual ~Instruction() {}
    // Virtual polymorphism 
    virtual void execute(CPU& cpu) = 0; 
};

// Derived instruction classes 
class ArithmeticInstruction : public Instruction {
public:
    void execute(CPU& cpu) override {
        // Implement ADD, SUB, MUL, DIV logic here
    }
};

class IOInstruction : public Instruction {
public:
    void execute(CPU& cpu) override {
        // Implement INPUT, DISPLAY logic here
    }
};

class ShiftInstruction : public Instruction {
public:
    void execute(CPU& cpu) override {
        // Implement SHL, SHR, ROL, ROR logic here
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

public:
    Runner() {}

    void loadProgram(const std::string& filename) {
        // Read .asm file line by line [cite: 80]
        // Decode strings into Instruction objects
        // Store in CustomVector
    }

    void executeProgram() {
        // Iterate through CustomVector of instructions
        // Call instruction->execute(virtualMachine)
        // Ensure virtualMachine.incrementPC() is called
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
    }
};

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