###### General Requirements:

1. Each function cannot exceeds 35 lines. 
2. Always include exception in your function to handle possible error that could appear during program execution.



###### Task Distribution:

Person 1: Register, Memory and CPU Part (William) (\*\*Refer all codes from 2. REGISTER, 3. MEMORY)

Class need to be built:

\-Register 

&#x20;-DataRegister (Inherited from Register)

&#x20;-FlagRegister (Inherited from Register)

\-Memory



Person 2: Data Structure Part (Kelvin) (\*\*Refer all codes from 1. DATA STRUCTURES)

Class need to be built:

\-CustomVector (A template class)

\-CustomQueue (A template class)

\-CustomStack (A template class)



Person 3: Assembly Instructions Logic (Zhun Rui) (\*\*Refer all codes from 5. INSTRUCTION)

Class need to be built:

\-Instruction (Parent class/Base class)

&#x20;-ArithmeticInstruction (Inherited from Instruction)

&#x20;-IOInstruction (Inherited from Instruction)

&#x20;-ShiftInstruction (Inherited from Instruction)



Person 4: Interpreter and Assembly Code Processing (Qian Xian) (\*\*Refer all codes from 4. CPU, 6. INTERPRETER)

Class need to be built:

\-CPU (Composition of DataRegister, FlagRegister, Memory, CustomStack)

\-Runner



###### Example Class \& Function Header Code:



\#include <iostream>

\#include <string>

\#include <stdexcept>



// ==========================================

// 1. DATA STRUCTURES

// ==========================================



// ==========================================

// A. CUSTOM VECTOR 

// ==========================================

template <typename T>

class CustomVector {

private:

&#x20;   T\* arr;

&#x20;   int capacity;

&#x20;   int current\_size;



&#x20;   // Helper function to resize the array when it gets full

&#x20;   void resize();



public:

&#x20;   CustomVector();

&#x20;   \~CustomVector();



&#x20;   // Add an element to the end

&#x20;   void push\_back(T element);

&#x09;

&#x20;   // Remove an element from the end

&#x20;   void pop\_back(T element);



&#x20;   // Access an element at a specific index

&#x20;   T at(int index) const;



&#x20;   // Overloaded \[] operator to perform bound checking and do resizing 

&#x20;   T \&operator\[](int size); // Read and write access

&#x20;   const T \&operator\[](int size); // Read only access

&#x20;   

&#x20;   // Get the current number of elements

&#x20;   int size() const;

};



// ==========================================

// B. CUSTOM STACK

// ==========================================

template <typename T>

class CustomStack {

private:

&#x20;   T\* arr;

&#x20;   int topIndex;

&#x20;   int maxCapacity;



public:

&#x20;   // Constructor with default size of 8 (as per VM specs)

&#x20;   CustomStack(int size = 8);

&#x20;   \~CustomStack();



&#x20;   // Push element to the top of the stack

&#x20;   void push(T element);



&#x20;   // Remove and return the top element

&#x20;   void pop(T element);



&#x20;   // State checks

&#x20;   bool isEmpty() const;

&#x20;   bool isFull() const;

};



// ==========================================

// C. CUSTOM QUEUE 

// ==========================================

template <typename T>

class CustomQueue {

private:

&#x20;   T\* arr;

&#x20;   int frontIndex;

&#x20;   int rearIndex;

&#x20;   int current\_size;

&#x20;   int maxCapacity;



public:

&#x20;   // Constructor with default size

&#x20;   CustomQueue(int size = 100);

&#x20;   \~CustomQueue();



&#x20;   // Add element to the back of the queue

&#x20;   void enqueue(T element);



&#x20;   // Remove and return element from the front

&#x20;   void dequeue(T \&element);



&#x20;   // State checks

&#x20;   bool isEmpty() const;

&#x20;   int size() const;



&#x20;   // Clear queue

&#x20;   void clear(); 

};



// ==========================================

// 2. REGISTER HIERARCHY

// ==========================================



// Base class encapsulating an 8-bit signed value 

class Register {

private:

&#x20;   signed char value; // 1 byte (signed char, -128 to 127) 



public:

&#x20;   Register() : value(0) {}

&#x20;   virtual \~Register() {}



&#x20;   // Encapsulation: getters and setters 

&#x20;   signed char getValue() const { return value; }

&#x20;   void setValue(signed char v) { value = v; }

};



// Represents R0-R7 registers 

class DataRegister : public Register {

public:

&#x20;   DataRegister() : Register() {}

};



// Manages individual flag bits (CF, OF, UF, ZF) 

class FlagRegister {

private:

&#x20;   bool CF, OF, UF, ZF;



public:

&#x20;   FlagRegister() : CF(false), OF(false), UF(false), ZF(false) {}



&#x20;   // Getters and setters for flags 

&#x20;   bool getCF() const { return CF; }

&#x20;   void setCF(bool val) { CF = val; }

&#x20;   

&#x20;   bool getOF() const { return OF; }

&#x20;   void setOF(bool val) { OF = val; }



&#x20;   bool getUF() const { return UF; }

&#x20;   void setUF(bool val) { UF = val; }



&#x20;   bool getZF() const { return ZF; }

&#x20;   void setZF(bool val) { ZF = val; }

&#x20;   

&#x20;   void resetAll() { CF = OF = UF = ZF = false; }

};



// ==========================================

// 3. MEMORY

// ==========================================



// Handles storage and addressing logic over a vector of bytes 

class Memory {

private:

&#x20;   signed char data\[64]; // 1-dimensional array of 64 signed bytes



public:

&#x20;   Memory() {

&#x20;       for (int i = 0; i < 64; ++i) {

&#x20;           data\[i] = 0;

&#x20;       }

&#x20;   }



&#x20;   signed char read(int address) const {

&#x20;       if (address >= 0 \&\& address < 64) return data\[address];

&#x20;       return 0; // Handle error appropriately

&#x20;   }



&#x20;   void write(int address, signed char value) {

&#x20;       if (address >= 0 \&\& address < 64) data\[address] = value;

&#x20;   }

};



// ==========================================

// 4. CPU (CENTRAL PROCESSING UNIT)

// ==========================================



// Contains registers, memory, PC, and executes instructions

class CPU {

private:

&#x20;   DataRegister R\[8];     // R0 to R7 

&#x20;   FlagRegister flags;       // Aggregated flags

&#x20;   Memory memory;            // Composed memory

&#x20;

&#x20;   unsigned char PC;         // Program Counter (1 byte, starts at 0)

&#x20;   unsigned char SI;         // Stack Index (1 byte, starts at 0)

&#x20;

&#x20;   CustomStack<signed char> systemStack;



public:

&#x20;   CPU() : PC(0), SI(0) {}



&#x20;   // Getters to allow instructions to manipulate CPU state

&#x20;   DataRegister\& getRegister(int index) { return R\[index]; }

&#x20;   FlagRegister\& getFlags() { return flags; }

&#x20;   Memory\& getMemory() { return memory; }

&#x20;

&#x20;   unsigned char getPC() const { return PC; }

&#x20;   void incrementPC() { PC++; }

&#x20;

&#x20;   unsigned char getSI() const { return SI; }

&#x20;   void incrementSI() { SI++; }

&#x20;   void decrementSI() { SI--; }

};



// ==========================================

// 5. INSTRUCTION HIERARCHY

// ==========================================



// Abstract base class for all assembly commands 

class Instruction {

public:

&#x20;   virtual \~Instruction() {}

&#x20;   // Virtual polymorphism 

&#x20;   virtual void execute(CPU\& cpu) = 0; 

};



// Derived instruction classes 

class ArithmeticInstruction : public Instruction {

public:

&#x20;   void execute(CPU\& cpu) override {

&#x20;       // Implement ADD, SUB, MUL, DIV logic here

&#x20;   }

};



class IOInstruction : public Instruction {

public:

&#x20;   void execute(CPU\& cpu) override {

&#x20;       // Implement INPUT, DISPLAY logic here

&#x20;   }

};



class ShiftInstruction : public Instruction {

public:

&#x20;   void execute(CPU\& cpu) override {

&#x20;       // Implement SHL, SHR, ROL, ROR logic here

&#x20;   }

};



// ==========================================

// 6. RUNNER (INTERPRETER)

// ==========================================



// Loads programs, decodes instructions, delegates execution to CPU 

class Runner {

private:

&#x20;   CPU virtualMachine; // Composition 

&#x20;   CustomVector<Instruction\*> program; // Polymorphic storage 



public:

&#x20;   Runner() {}



&#x20;   void loadProgram(const std::string\& filename) {

&#x20;       // Read .asm file line by line 

&#x20;       // Decode strings into Instruction objects

&#x20;       // Store in CustomVector

&#x20;   }



&#x20;   void executeProgram() {

&#x20;       // Iterate through CustomVector of instructions

&#x20;       // Call instruction->execute(virtualMachine)

&#x20;       // Ensure virtualMachine.incrementPC() is called

&#x20;   }



&#x20;   void dumpState() {

&#x20;       // Printing Format After Executing Each Line of Assembly Code: 

&#x09;// #ProgramCounter#0000#

&#x09;// #Flags#Overflow#0#Underflow#0#Carry#0#Zero#0#

&#x20;       // #Register#R1#0000#R2#0000#R3#0000#R4#0000#R5#0000#R6#0000#R7#0000#R8#0000#

&#x20;       // #Memory#

&#x09;// #0000#0000#0000#0000#0000#0000#0000#0000#  

&#x09;// #0000#0000#0000#0000#0000#0000#0000#0000# 

&#x09;// #0000#0000#0000#0000#0000#0000#0000#0000# 

&#x09;// #0000#0000#0000#0000#0000#0000#0000#0000# 

&#x09;// #0000#0000#0000#0000#0000#0000#0000#0000# 

&#x09;// #0000#0000#0000#0000#0000#0000#0000#0000# 

&#x09;// #0000#0000#0000#0000#0000#0000#0000#0000# 

&#x09;// #0000#0000#0000#0000#0000#0000#0000#0000# 

&#x09;// Note: All outputs print number in decimal format.

&#x20;   }

};



// ==========================================

// ENTRY POINT

// ==========================================

int main() {

&#x20;   Runner interpreter;

&#x20;   // Load the program from file / Ask user to enter file name to be compiled

&#x20;   // Get file and compile the assembly code 

&#x20;   // Print the VM's state after executed each line of assembly code

&#x20;   return 0;

}

