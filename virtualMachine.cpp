#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <cctype>
using namespace std;

/**
 * @brief   Base Exception class 
 * @details Handle all types of exception
 * @author  Mun William
 */
class VMException {
    protected:
        // String that stores the error message
        string errorMessage;
    public:
        /**
         * @brief     Parameterized constructor. Constructs VMException object with the respective error message
         * @param msg String that contains error message passed by other function
         * @post      VMException object is created.
         * @author    Mun William
         */
        VMException(const string& msg): errorMessage(msg) {}

        /**
         * @brief  Destructor. Destructs VMException objects and return the memory space to operating system.
         * @note   'virtual' ensures that the derived exception class will be destroyed before base class destroy, prevents dangling pointers of the derived object pointer.
         * @author Mun William
         */
        virtual ~VMException() {}

        /**
         * @brief  Getter function. Retrieve error message of the exception 
         * @note   'const' prevents modification of errorMessage variable 
         * @return Returns error message in constant string reference 
         * @author Mun William
         */
        virtual const string& getErrorMessage() const { return errorMessage; }
};

/**
 * @brief   Derived class from VMException
 * @details Handles file related exception
 * @author  Mun William
 */
class FileException : public VMException {
    private:
        // filename that has error
        string filename;

    public: 
        /**
         * @brief     Parameterized constructor. Constructs FileException object
         * @param fn  Filename that has error
         * @param msg Error message passed by other function
         * @post      FileException object created and constructs VMException with its specialized error header.
         * @author    Mun William
         */
        FileException(const string& fn, const string& msg) : VMException("FILE ERROR- " + msg + "\nFATAL FILE- " + fn), filename(fn) {}

        /**
         * @brief Getter function. Retrieves the filename that contains error.
         * @note   'const' prevents modification of filename variable 
         * @return Returns filename in constant string reference
         * @author Mun William
         */
        const string& getFilename() const { return filename; }
};

/**
 * @brief   Derived class from VMException
 * @details Handles hardware related exception, such as Memory, Register, FlagRegister
 * @author  Mun William
 */
class HardwareException : public VMException {
    public:
        /**
         * @brief     Parameterized constructor. Constructs HardwareException object
         * @param msg Error message passed by other function
         * @post      HardwareException object created and constructs VMException with its specialized error header.
         * @author    Mun William
         */
        HardwareException(const string& msg) : VMException("HARDWARE ERROR- " + msg) {}
};

/**
 * @brief   Derived class from VMException
 * @details Handles syntax related exception. Handles all typos and unintentionally left commas, spaces or additional symbols that is not recognizable by the program
 * @author  Mun William
 */
class SyntaxException : public VMException {
    public:
        /**
         * @brief     Parameterized constructor. Constructs SyntaxException object
         * @param msg Error message passed by other function
         * @post      SyntaxException object created and constructs VMException with its specialized error header.
         * @author    Mun William
         */
        SyntaxException(const string& msg) : VMException("SYNTAX ERROR- " + msg) {}
};

/**
 * @brief   Derived class from VMException
 * @details Handles logic related exception. For example, error of operation in data structures and divides a 0.
 * @author  Mun William
 */
class LogicException : public VMException {
    public:
        /**
         * @brief     Parameterized constructor. Constructs LogicException object
         * @param msg Error message passed by other function
         * @post      LogicException object created and constructs VMException with its specialized error header.
         * @author    Mun William
         */
        LogicException(const string& msg) : VMException("LOGIC ERROR- " + msg) {}
};

/**
 * @brief   Derived class from VMException
 * @details Handles run time crash exception. Catches all the exception class above during run time and format the error header properly with showing PC counter and command that causes crash, and shows the reason (error message).
 * @author  Mun William
 */
class RunTimeCrashException : public VMException {
    public:
        /**
         * @brief        Overloaded parameterized constructor. Constructs RunTimeCrashException object
         * @param errMsg Error message passed by other function
         * @param PC     Integer that storing program counter causes crash
         * @param cmd    Instruction string that causes crash
         * @post         RunTimeCrashException object created and constructs VMException with its specialized error header.
         * @author       Mun William
         */
        RunTimeCrashException(const string& errMsg, int PC, const string& cmd): VMException("CRASH AT PC[" + to_string(PC) + "]\n" + "Command: " + cmd + "\n" + "Reason: " + errMsg) {}
        
        /**
         * @brief        Overloaded parameterized constructor. Constructs RunTimeCrashException object
         * @param errMsg Error message passed by other function
         * @post         RunTimeCrashException object created and constructs VMException with its specialized error header.
         * @author       Mun William
         */
        RunTimeCrashException(const string& errMsg): VMException("COMPILATION ERROR. \nProgram stop compiling. \nReason: " + errMsg) {}
};

/**
 * @brief   A generic dynamic array implementation that automatically manages its own memory and capacity.
 * @details Unlike a standard fixed-size array, this custom vector can dynamically resize itself when full, preventing memory overflow and out-of-bounds errors. It serves as the secure, memory-managed backbone for other higher-level data structures in this project, such as CustomStack and CustomQueue, allowing them to expand infinitely without requiring complex manual memory reallocation or circular indexing logic.
 * @author  Ong Zhong Yik
 */
template <typename T>
class CustomVector {
    private:
        // Pointer to the dynamically allocated array that stores the elements.
        T* arr;

        // The total number of elements the vector can currently hold before needing to allocate more memory.
        int capacity;

        // The actual number of valid elements currently stored in the vector.
        int current_size;

        /**
         * @brief  Helper function to double the capacity of the array when it becomes full.
         * @post   Allocates a new larger array, copies existing elements over, and safely deletes the old array to prevent memory leaks.
         * @author Ong Zhong Yik
         */
        void resize();

    public:
        /**
         * @brief  Default Constructor. Initializes an empty vector.
         * @post   Sets empty vector to initial dynamic memory.
         * @author Ong Zhong Yik
         */
        CustomVector();

        /**
         * @brief  Destructor. Destroys the CustomVector object and frees allocated memory.
         * @post   Uses delete[] to safely release the dynamically allocated array back to the system, preventing memory leaks.
         * @author Ong Zhong Yik
         */
        ~CustomVector() { delete[] arr; }

        /**
         * @brief       Copy Constructor. Creates a new vector as a deep copy of another vector.
         * @param right The constant reference to the existing CustomVector object that is being copied.
         * @post        Allocates a completely new independent memory block to avoid double-free errors and shallow copy issues.
         * @author      Ong Zhong Yik
         */
        CustomVector(const CustomVector<T>& right);

        /**
         * @brief         Adds a new element to the end of the vector.
         * @note          Automatically triggers the resize() function if the current size reaches the maximum capacity.
         * @param element The data value of type T to be added to the vector.
         * @post          The element is successfully appended to the end of the vector, and current_size is incremented by 1.
         * @author        Ong Zhong Yik
         */
        void push_back(T element);

        /**
         * @brief  Removes the last element from the vector.
         * @pre    The vector must not be empty (current_size must be greater than 0).
         * @post   Explicitly calls the destructor of the object being removed (.~T()) to ensure complete memory cleanup, then shrinks the logical size. 
         * @throws LogicException if the vector is already empty (current_size == 0).
         * @author Ong Zhong Yik
         */
        void pop_back();

        /**
         * @brief       Accesses the element at the specified index with strict boundary checking.
         * @note        The 'const' keyword guarantees that calling this function will not modify the internal state of the vector.
         * @pre         The index must be within the valid range of currently stored elements (0 <= index < current_size).
         * @param index The integer position of the element to retrieve.
         * @throws      LogicException if the index is out of bounds (negative or <= current_size).
         * @return      The element of type T located at the specified index.
         * @author      Ong Zhong Yik
         */
        T at(int index) const;

        /**
         * @brief       Overloaded array subscript operator for accessing and modifying elements.
         * @pre         The index must be within the valid range of currently stored elements (0 <= index < current_size).
         * @param index The integer position of the element to access or modify.
         * @throws      VMException if the index is out of bounds (negative or <= current_size).
         * @return      Reference to the element. 
         * @author      Ong Zhong Yik
         */
        T &operator[](int index);
        
        /**
         * @brief       Overloaded array subscript operator for accessing elements (read-only).
         * @note        Used when the vector is passed as a constant reference. The 'const' keyword guarantees that calling this function will not modify the internal state of the vector.
         * @pre         The index must be within the valid range of currently stored elements (0 <= index < current_size).
         * @param index The integer position of the element to access or modify.
         * @throws      LogicException if  the index is out of bounds (negative or <= current_size).
         * @return      A constant reference to the element of type T at the specified index.
         * @author      Ong Zhong Yik
         */
        const T &operator[](int index) const;

        /**
         * @brief  Returns the current number of valid elements inside the vector.
         * @note   The 'const' keyword guarantees that calling this function will not modify the internal state of the vector.
         * @return An integer representing the current number of elements logically stored in the vector.
         * @author Ong Zhong Yik
         */
        int size() const { return current_size; }

       /**
         * @brief       Removes an element at a specific index.
         * @pre         The index must be within the valid boundaries of the vector (0 <= index < current_size).
         * @param index The integer position of the element to be removed.
         * @throws      LogicException if the index is out of bounds (negative or greater than/equal to current_size).
         * @post        Calls the destructor on the target element and shifts all subsequent elements one step forward to fill the gap. 
         * @author      Ong Zhong Yik
         */
        void erase(int index);

        /**
         * @brief       Copy Assignment Operator. Assigns the contents of one vector to another.
         * @note        The 'const' keyword in the parameter ensures that the source vector remains completely unmodified during the assignment process.
         * @param right The constant reference to the source CustomVector object whose data is being copied.
         * @post        Safely handles self-assignment, deletes the old memory block, and performs a deep copy of the new data.
         * @return      A reference to the newly updated CustomVector object to support chained assignments.
         * @author      Ong Zhong Yik
         */
        CustomVector& operator=(const CustomVector<T> &right);
};

/**
 * @brief   A Last-In, First-Out (LIFO) data structure used for temporary storage and retrieval.
 * @details The stack intentionally restricts access to only the top element (via push, pop, and peek) to enforce strict LIFO behavior, which is essential for operations like expression evaluation or managing CPU state. It does not implement the [] operator or at() function because allowing random access to intermediate elements would violate the foundational principles and security of a stack architecture.
 * @author  Ong Zhong Yik
 */
template <typename T>
class CustomStack {
    private:
        
        // Composition of CustomVector. The underlying dynamic array used to store the stack's elements.
        CustomVector<T> data;

    public:
        /**
         * @brief  Default Constructor. Initializes an empty stack.
         * @note   No manual allocation needed here; the underlying CustomVector handles its own initial setup.
         * @post   A CustomStack object is created.
         * @author Ong Zhong Yik
         */
        CustomStack() {}

        /**
         * @brief  Destructor. Cleans up the stack when it is destroyed.
         * @note   Left empty because the CustomVector's destructor will automatically be triggered to free the memory.
         * @post   CustomStack object is destroyed.
         * @author Ong Zhong Yik
         */
        ~CustomStack() {}

        /**
         * @brief       Copy Constructor. Creates a new stack as a deep copy of an existing stack.
         * @details     Uses an initialization list to directly delegate the deep copy process to CustomVector's copy constructor. 
         * @note        The 'const' keyword ensures that the source stack being copied from cannot be altered during initialization.
         * @param right A constant reference to the source CustomStack object to be duplicated.
         * @post        A new identical copy of CustomStack object is created.
         * @author      Ong Zhong Yik
         */
        CustomStack(const CustomStack<T>& right) : data(right.data) {}

        /**
         * @brief       Copy Assignment Operator. Assigns the data of one stack to another.
         * @details     Delegates the assignment logic and safe memory handling to CustomVector's assignment operator. 
         * @note        The 'const' keyword prevents modification of the source stack during the assignment operation.
         * @param right A constant reference to the source CustomStack object providing the new data.
         * @return      A reference to the updated CustomStack object to allow chained assignment.
         * @author      Ong Zhong Yik
         */
        CustomStack& operator=(const CustomStack<T>& right);

        /**
         * @brief         Pushes a new element onto the top of the stack (Last-In).
         * @details       Directly calls CustomVector's push_back() method, allowing for dynamic resizing if needed.
         * @param element The data value of type T to be added to the top of the stack.
         * @post          The new element is placed at the top of the stack, and the internal logical size increases by 1.
         * @author        Ong Zhong Yik
         */
        void push(T element) { data.push_back(element); }

        /**
         * @brief   Removes the top element from the stack (First-Out).
         * @details Call CustomVector's pop_back() method. 
         * @pre     Stack should not be empty.
         * @throws  LogicException if the stack is empty.
         * @post    The element at the top of the stack is destroyed and removed, decreasing the internal logical size by 1.
         * @author  Ong Zhong Yik
         */
        void pop();

        /**
         * @brief  Checks whether the stack is currently empty.
         * @note   The 'const' keyword guarantees that calling this state-check function will not modify any internal data.
         * @return Returns true if the underlying CustomVector's size is 0, false otherwise.
         * @author Ong Zhong Yik
         */
        bool isEmpty() const { return data.size() == 0; }

        /**
         * @brief  Retrieves the top element of the stack without removing it.
         * @note   The 'const' keyword ensures that the stack's state and data remain unchanged after peeking.
         * @pre    Stack should not be empty.
         * @throws LogicException if the stack is empty.
         * @return Return the last element of the CustomVector. 
         * @author Ong Zhong Yik
         */
        T peek() const;
};

/**
 * @brief   A First-In, First-Out (FIFO) data structure used for sequential data processing and scheduling.
 * @details The queue intentionally restricts access to only the front and back elements (via enqueue, dequeue, and front) to enforce strict FIFO behavior. It does not implement the [] operator or at() function because allowing random access to intermediate elements would violate the fundamental sequential nature of a queue, compromising its predictability and data integrity.
 * @author  Ong Zhong Yik
 */
template <typename T>
class CustomQueue {
    private:
        // Composition of CustomVector. The underlying dynamic array used to store the queue's elements.
        CustomVector<T> data;

    public:
        /**
         * @brief  Default Constructor. Initializes an empty queue.
         * @note   No manual memory allocation needed here; the underlying CustomVector safely handles its own initialization.
         * @post   A CustomQueue object is created.
         * @author Ong Zhong Yik
         */
        CustomQueue() {}

        /**
         * @brief  Destructor. Cleans up the queue when it is destroyed.
         * @note   Left empty because the CustomVector's destructor will automatically be triggered when the queue goes out of scope, safely freeing the memory.
         * @post   CustomQueue object is destroyed.
         * @author Ong Zhong Yik
         */
        ~CustomQueue() {}

        /**
         * @brief       Copy Constructor. Creates a new queue as a deep copy of an existing queue.
         * @details     Uses an initialization list to directly delegate the deep copy process to CustomVector's highly secure copy constructor.
         * @note        The 'const' keyword ensures that the source queue being copied from remains strictly unmodified during the initialization process.
         * @param right A constant reference to the source CustomQueue object that is to be duplicated.
         * @post        A new identical copy of CustomQueue object is created.
         * @author      Ong Zhong Yik
         */
        CustomQueue(const CustomQueue<T>& right) : data(right.data) {}

        /**
         * @brief         Adds a new element to the back of the queue (First-In).
         * @details       Directly calls CustomVector's push_back() method, allowing the queue to expand dynamically without ever getting "full".
         * @param element The data value of type T to be added to the back of the queue.
         * @post          The element is successfully appended to the back of the queue, and the internal logical size increases by 1.
         * @author        Ong Zhong Yik
         */
        void enqueue(T element) { data.push_back(element); }

        /**
         * @brief   Removes the front element from the queue.
         * @details Call CustomVector's erase method so all trailing elements automatically shift forward. 
         * @pre     Queue should not be empty.
         * @throws  LogicException if the queue is empty.
         * @post    The element at the front of the queue is destroyed and removed, and all remaining elements are shifted forward, decreasing the size by 1.
         * @author  Ong Zhong Yik
         */
        void dequeue();

        /**
         * @brief  Checks whether the queue is currently empty.
         * @note   The 'const' keyword guarantees that calling this state-check function will not alter any internal data members of the queue.
         * @return Returns true if the underlying CustomVector's size is 0, false otherwise.
         * @author Ong Zhong Yik
         */
        bool isEmpty() const { return data.size() == 0; }

        /**
         * @brief  Retrieves the front element of the queue without removing it.
         * @note   The 'const' keyword ensures that peeking at the front element does not modify the queue's state or data.
         * @pre    Queue should not be empty.
         * @throws LogicException if the queue is empty.
         * @return Return the first element of the CustomVector. 
         * @author Ong Zhong Yik
         */
        T front() const;

        /**
         * @brief       Copy Assignment Operator. Assigns the data of one queue to another.
         * @details     Safely delegates the assignment logic and deep copy mechanism to CustomVector's assignment operator.
         * @note        The 'const' keyword prevents any accidental modification of the source queue during the assignment operation.
         * @param right A constant reference to the source CustomQueue object providing the new data.
         * @return      A reference to the updated CustomQueue object to support chained assignments.
         * @author      Ong Zhong Yik
         */
        CustomQueue& operator=(const CustomQueue<T>& right);
};

/**
 * @brief    A base class representing a general-purpose register
 * @details  Register class allows operation such as retrieving data from the register and updating the data in register.
 * @author   Mun William
 */
class Register {
    private:
        // Holds 8 bits data loaded into register.
        // 'signed char' restrict the data stores in exactly 1 byte, which is the data will only ranged from -128 to 127.
        signed char value;

    public:
        /**
         * @brief  Default constructor. Constructs a Register object with default value stored in it.
         * @post   The value in register is initialized with value 0.
         * @author Mun William
         */
        Register() : value(0) {}

        /**
         * @brief  Destructor. Destroys the Register object and frees allocated memory.
         * @note   Virtual base destructor ensures derived class destroyed safely when derived class is destroyed via a base pointer, preventing memory leaks.
         * @author Mun William
         */
        virtual ~Register() {}

        /**
         * @brief  Getter function. Returns value stored in Register object.
         * @return The internal value as a signed character.
         * @note   'const' keyword prevents modification of value variable.
         * @author Mun William
         */
        signed char getValue() const { return value; }

        /**
         * @brief   Setter function. Update value in Register object.
         * @param v The new signed character to be stored.
         * @post    Value in register is stored with the new value, v.
         * @author  Mun William
         */
        void setValue(signed char v) { value = v; }
};

/**
 * @brief   A derived class from Register representing data register which use to store data.
 * @details Inherited from Register class. Has public access for public member function in Register class.
 * @author  Mun William
 */
class DataRegister : public Register {
    public:
        /**
         * @brief  Default constructor. Constructs DataRegister object by calling Register class default constructor.
         * @post   The data register will be initialized with value 0.
         * @author Mun William
         */
        DataRegister() : Register() {}
};

/**
 * @brief   A register class that storing status of each flag.
 * @details FlagRegister will only stores 2 values, which are 1 indicates set and 0 indicates reset. The flag register here contains Carry Flag(CF), Underflow Flag(UF), Overflow Flag(OF), Zero Flag(ZF)
 * @author  Mun William
 */
class FlagRegister {
    private:
        // Holds 1 bit data loaded into flag register
        // bool is used since bool only stores 1 bit data, which is true(1) and false(0).
        bool CF, OF, UF, ZF;

        /**
         * @brief       Check whether the result of an arithmetic operation contains carry.
         * @param value The result of the arithmetic operation
         * @note        'int' is used so that the 9th bit which represent the carry bit can be detected.
         * @return      Boolean value which represent whether the result contains carry.
         * @author      Mun William
         */
        bool checkCF(int value) { return ((value & 0x100) != 0); }

        /**
         * @brief        Check whether the result of an arithmetic operation is overflow.
         * @param oper1  Leftside operand of an arithmetic operation
         * @param oper2  Rightside operand of an arithmetic operation
         * @param result Result of the arithmetic operation
         * @note         'unsigned char' guarantee bitwise AND operation works correctly on the raw bits of the 2's complement value.
         * @return       Boolean value which represent whether the result is overflow.
         * @author       Mun William
         */
        bool checkOF(unsigned char oper1, unsigned char oper2, unsigned char result) { return (((~oper1 & ~oper2 & result) & 0x80) != 0); }

        /**
         * @brief        Check whether the result of an arithmetic operation is underflow.
         * @param oper1  Leftside operand of an arithmetic operation
         * @param oper2  Rightside operand of an arithmetic operation
         * @param result Result of the arithmetic operation
         * @note         'unsigned char' guarantee bitwise AND operation works correctly on the raw bits of the 2's complement value.
         * @return       Boolean value which represent whether the result is underflow.
         * @author       Mun William
         */
        bool checkUF(unsigned char oper1, unsigned char oper2, unsigned char result) { return (((oper1 & oper2 & ~result) & 0x80) != 0); }

        /**
         * @brief     Check whether the result of an operation is zero
         * @param val Result of the operation
         * @return    Boolean value which represent whether the result is zero
         * @author    Mun William
         */
        bool checkZF(signed char val) { return (val == 0); }

    public:
        /**
         * @brief  Default constructor. Constructs a flag register object which contains all status flag (carry, underflow, overflow, zero) with default value.
         * @post   Carry flag, overflow flag, underflow flag and zero flag is initialized with 0 (reset).
         * @author Mun William
         */
        FlagRegister() : CF(false), OF(false), UF(false), ZF(false) {}

        /**
         * @brief  Getter function. Return status of carry flag.
         * @return Boolean value which represents whether the result of an arithmetic operation has carry.
         * @note   'const' keyword prevents modification of CF variable.
         * @author Mun William
         */
        bool getCF() const { return CF; }

        /**
         * @brief       Setter function. Update status of carry flag.
         * @param value New boolean value represent new status of carry flag.
         * @post        Carry flag is updated with new value.
         * @author      Mun William
         */
        void setCF(bool value) { CF = value; }

        /**
         * @brief  Getter function. Return status of overflow flag.
         * @return Boolean value which represents whether the result of an arithmetic operation is overflow.
         * @note   'const' keyword prevents modification of OF variable.
         * @author Mun William
         */
        bool getOF() const { return OF; }

        /**
         * @brief       Setter function. Update status of overflow flag.
         * @param value New boolean value represent new status of overflow flag.
         * @post        Overflow flag is updated with new value.
         * @author      Mun William
         */
        void setOF(bool value) { OF = value; }

        /**
         * @brief  Getter function. Return status of underflow flag.
         * @return Boolean value which represents whether the result of an arithmetic operation is underflow.
         * @note   'const' keyword prevents modification of UF variable.
         * @author Mun William
         */
        bool getUF() const { return UF; }

        /**
         * @brief       Setter function. Update status of underflow flag.
         * @param value New boolean value represent new status of underflow flag.
         * @post        Underflow flag is updated with new value.
         * @author      Mun William
         */
        void setUF(bool value) { UF = value; }

        /**
         * @brief  Getter function. Return status of zero flag.
         * @return Boolean value which represents whether the result of an operation is zero.
         * @note   'const' keyword prevents modification of ZF variable.
         * @author Mun William
         */
        bool getZF() const { return ZF; }

        /**
         * @brief       Setter function. Update status of zero flag.
         * @param value New boolean value represent new status of zero flag.
         * @post        Zero flag is updated with new value.
         * @author      Mun William
         */
        void setZF(bool value) { ZF = value; }

        /**
         * @brief  Reset all flag status to 0 (reset status).
         * @post   Carry, overflow, underflow and zero flag is reset.
         * @author Mun William
         */
        void resetAll() { CF = OF = UF = ZF = false; }

        /**
         * @brief        Function that sets carry, underflow, overflow and zero flag after an arithmetic operation.
         * @param oper1  Leftside operand of an arithmetic operation.
         * @param oper2  Rightside operand of an arithmetic operation
         * @param result Result of the arithmetic operation
         * @note         'unsigned char' guarantee bitwise AND operation works correctly on the raw bits of the 2's complement value.
         * @note         'int' is used so that the 9th bit which represent the carry bit can be detected.
         * @post         Carry, underflow, overflow, zero flag sets to its respective status.
         * @author       Mun William
         */
        void flagArithmeticSetter(unsigned char oper1, unsigned char oper2, int result);

        /**
         * @brief       Function that sets carry, underflow, overflow and zero flag after an input operation.
         * @param input Input value by user.
         * @note        'int' is used to preserve the overflow value (value larger than 127) and underflow value (value smaller than -128) input by user.
         * @post        Underflow, overflow and zero flag sets to its respective status.
         * @author      Mun William
         */
        void flagIOSetter(int input);

        /**
         * @brief        Function that sets zero flag after a logical operation (shift, rotate).
         * @param result Result of a logical operation.
         * @note         'unsigned char' guarantee bitwise AND operation works correctly on the raw bits of the 2's complement value.
         * @post         Zero flag sets to its respective status.
         * @author       Mun William
         */
        void flagLogicalSetter(unsigned char result);
};

/**
 * @brief   A memory object that handles storage and addressing logic over an array of bytes.
 * @details Creates a 64-bytes array that act as the memory space in virtual machine.
 * @author  Mun William
 */
class Memory {
    private:
        // 1-dimensional array of 64 signed bytes
        signed char data[64];

    public:
        /**
         * @brief  Default constructor. Construct a new memory object.
         * @post   A new memory object is initialized, with 0 stored inside the 1-dimensional array. (Refer to function implementation)
         * @author Mun William
         */
        Memory();

        /**
         * @brief     Copy constructor. Deep copy the current memory object to a new memory object.
         * @param mem The object to be copied.
         * @note      'const' before parameter mem ensures the parameter will not be modified.
         * @post      A new identical memory object will be created.
         * @author    Mun William
         */
        Memory(const Memory &mem);

        /**
         * @brief       Copy assignment operator. Deep copy the current memory object to a new memory object.
         * @param other The object to be copied.
         * @note        'const' before parameter other ensures the parameter will not be modified.
         * @return      A reference of memory object which is identical to the parameter.
         * @author      Mun William
         */
        Memory& operator=(const Memory& other);

        /**
         * @brief         Retrives value in the specific address (index of array)
         * @param address Index of the 1-dimensional array
         * @pre           address value should between 0 and 64
         * @throws        HardwareException if address value smaller than 0 or larger than 64.
         * @return        Signed character stored inside the specific address (index)
         * @note          'const' keyword at the end prevents modification of value stored in the address.
         * @author        Mun William
         */
        signed char read(int address) const;

        /**
         * @brief         Update value in the specific address (index of array)
         * @param address Index of 1-dimensional array
         * @param value   New value to be stored in the memory address
         * @pre           address value should beteween 0 and 64
         * @throws        HardwareException if address value smaller than 0 or larger than 64.
         * @post          The specific memory address is updated with the value passed in to the function.
         * @author        Mun William
         */
        void write(int address, signed char value);

        /**
         * @brief  Display the value stored in all memory in a square box with 8x8 dimension.
         * @post   A square box with 8x8 dimension will display a 1-dimensional array (representing memory) which storing a group of value.
         * @author Mun William
         */
        void displayMemory();
};

// Contains registers, memory, PC, and executes instructions
// holds data, keep track where the program is, manages temporary storage
class CPU {
    private:
        DataRegister R[8];     // R0 to R7
        FlagRegister flags;       // Aggregated flags (0 or 1 signals)
        Memory memory;            // Composed memory
        unsigned char PC;         // Program Counter, remembers which line of the assembly program is reading (1 byte, starts at 0)
        unsigned char SI;         // Stack Index, count of how many things piled up (1 byte, starts at 0)
        CustomStack<signed char> systemStack; // Temporary store number

    public:
        CPU() : PC(0), SI(0) {} // sets the program counter and stack index to 0 when cpu is first created
        // Getters to allow instructions to manipulate CPU state
        DataRegister* getRegister(int index); // returns pointer to a specific data register, pointer gives the runner the memory address of the pointer
        FlagRegister* getFlags() { return &flags; } // returns pointer to flag registers so the runner can check or update them
        Memory* getMemory() { return &memory; } // returns a pointer to the main memory so the runner can load or store data
        CustomStack<signed char>& getSystemStack() {return systemStack;}
        unsigned char getPC() const { return PC; } // return the current line the Program Counter is on, const prevent changes on PC value
        void incrementPC() { PC++; } // runner calls this after finishing an instruction, move program counter forward by 1, cpu knows to move to next line
        unsigned char getSI() const { return SI; } //return the current number of items piled in the stack
        void incrementSI() { SI++; } // increases stack index by 1 when a new item is added to stack
        void decrementSI() { SI--; } // decreases stack index by 1 when a new item is removed from stack
        void pushToStack(signed char value);
        signed char popFromStack(); // removes the top value from stack and gives it back to caller, & modifies the variable that runner passed into function directly
};

/**
 * @brief    A base class representing a general-purpose instruction
 * @details  Instruction class allows operation such as resetting flag from register, load value of memory address to register and more.
 * @author   Kong Zhun Rui
 */
class Instruction {
public:
    Instruction() = default;
    virtual ~Instruction() {}
    // Virtual polymorphism
    virtual void execute(CPU& cpu) = 0;
    virtual const char* getCommand() const = 0;
};

/**
 * @brief   Derived class from Instruction class that executes arithmetic instruction.
 * @details Perform runtime polymorphism. Executes ArithmeticInstruction::execute() when execute is called via base class pointer, but with derived class object.
 * @author  Kong Zhun Rui
 */
class ArithmeticInstruction : public Instruction {
    private:
        string ar; //"ADD", "SUB", "MUL", "DIV"
        int destRI; //destination register index
        int sourceVal; //source register index or immediate
        bool isImmediate; //true if sourceVal is immediate value, otherwise false
        /**
        * @brief Computes the result of a binary arithmetic operation based on the current operation type.
        * @param v1 Integer value representing the first operand, which is the destination register value.
        * @param v2 Integer value representing the second operand, which is source register value or immediate literal.
        * @throws LogicException if the operation is division and the divisor is zero.
        * @throws SyntaxException if the operation string 'ar' is not match with "ADD", "SUB", "MUL", "DIV".
        * @return Integer representing the computed result of the mathematical operation.
        * @note Double-check the operand order in the division block; it currently performs v2 / v1 instead of v1 / v2.
        * @author Kong Zhun Rui
        */
        int compute(int v1, int v2);
    
    public:
        /**
        * @brief Parameterized constructor. Constructs a binary arithmetic instruction object.
        * @details Initializes a complete arithmetic statement by storing the operation type, the target destination register, and resolving whether the secondary operand is a literal immediate value or an external register index.
        * @param operation String representing the specific mathematical operation. For example, "ADD", "SUB", and "DIV".
        * @param dest Integer value representing the targeted destination data register index.
        * @param source Integer value representing either a raw numeric literal or a source register index.
        * @param immediate Boolean flag indicating if the source parameter is an immediate literal (true) or a register index (false).
        * @post Created a new ArithmeticInstruction object initialized with the provided mathematical parameters.
        * @author Kong Zhun Rui
        */
        ArithmeticInstruction(string operation, int dest, int source, bool immediate):ar(operation), destRI(dest), sourceVal(source), isImmediate(immediate){}; 
        /**
        * @brief Parameterized constructor. Constructs a unary arithmetic instruction object.
        * @details Translates "INC" and "DEC" into "ADD" and "SUB" with a literal factor of 1.
        * @param operation String representing the syntax keyword ("INC" or "DEC") of the operation.
        * @param dest Integer value representing the targeted data register index.
        * @throws SyntaxException if the operation string does not match "INC" or "DEC".
        * @post Created a new ArithmeticInstruction object initialized as a mapped binary operation with an immediate value of 1.
        * @author Kong Zhun Rui
        */
        ArithmeticInstruction(string operation, int dest); 
        /**
         * @brief Execute the arithmetic instruction.
         * @param cpu Reference to CPU object, that containing the memory, register.
         * @note This is a polymorphic function. 'override' means ArithmeticInstruction::execute() function will override pure virtual function, Instruction::execute() in base class.
         * @post Execute the respective instruction, resetting flag status, retrieving 2 operand, computing the result, update CPU flag based on outcome, and save the final truncated 8-bit result back into the destination register.
         * @author Kong Zhun Rui
         */
        void execute(CPU& cpu) override;

        /**
         * @brief  Getter function. Get the command string of the instruction
         * @note   'const' prevents modification on the command string.
         * @return C-string that storing arithmetic command
         * @author Mun William
         */
        const char* getCommand() const override { return ar.c_str(); }
};

/**
 * @brief Derived class from Instruction class that executes move instruction.
 * @details Perform runtime polymorphism. Executes MoveInstruction::execute() when execute is called via base class pointer, but with derived class object.
 * @author Kong Zhun Rui
 */
class MoveInstruction : public Instruction{
    private:
        int mode; // 1: Immediate, 2: Register-Register, 3: Register-Indirect
        int destI; //destination index
        int sourceI; //source index
    
    public:
        /**
        * @brief Parameterized constructor. Constructs a new move instruction object.
        * @param moveMode Integer value representing the data transfer mode (1: Immediate, 2: Register-Register, 3: Register-Indirect, 4: Register, [address]).
        * @param dest Integer value representing the destination register index.
        * @param source Integer value representing the source register index, memory address, or immediate literal.
        * @pre moveMode should be in the range 1-4, and dest should be a valid register index (0-7).
        * @post Created a new MoveInstruction object with initialized mode, destination, and source attributes.
        * @author Kong Zhun Rui
        */
        MoveInstruction(int moveMode, int dest, int source): mode(moveMode), destI(dest), sourceI(source){} 
        /**
         * @brief Execute the move instruction.
         * @param cpu Reference to CPU object, that containing the memory, register.
         * @note This is a polymorphic function. 'override' means MoveInstruction::execute() function will override pure virtual function, Instruction::execute() in base class.
         * @post Execute the respective instruction, either loading a literal value to destination register, copy another value from register, or read data from memory address stored in register.
         * @author Kong Zhun Rui
         */
        void execute(CPU& cpu) override;

        /**
         * @brief  Getter function. Get the command string of the instruction
         * @note   'const' prevents modification on the command string.
         * @return C-string that storing move command
         * @author Mun William
         */
        const char* getCommand() const override { return (mode == 4) ? "LOAD" : "MOV"; }
};

/**
 * @brief Derived class from Instruction class that executes input or output instruction.
 * @details Perform runtime polymorphism. Executes IOInstruction::execute() when execute is called via base class pointer, but with derived class object.
 * @author Kong Zhun Rui
 */
class IOInstruction : public Instruction {
    private:
        string op; //"INPUT" and "DISPLAY"
        int regI; //register array index
    public:
        /**
        * @brief Parameterized constructor. Constructs a new Input/Output instruction object.
        * @param operation String representing the specific I/O command type ("INPUT" or "DISPLAY").
        * @param idx Integer value representing the targeted data register index.
        * @post Created a new IOInstruction object with the operation type and register index initialized.
        * @author Kong Zhun Rui
        */
        IOInstruction(string operation, int idx) : op(operation), regI(idx) {}
        /**
        * @brief Executes the instruction for input and output.
        * @param cpu Reference to CPU object, that containing the memory, register.
        * @note This is a polymorphic function. 'override' means IOInstruction::execute() function will override pure virtual function, Instruction::execute() in base class.
        * @throws SyntaxException if the operation string 'op' matches neither "INPUT" nor "DISPLAY".
        * @post If op is "INPUT", clears all CPU flags, prompts the user for an integer, stores the truncated 8-bit result in the designated register, and updates relevant status flags. If op is "DISPLAY", prints the current register value.
        * @author Kong Zhun Rui
        */
        void execute(CPU& cpu) override;
        
        /**
         * @brief  Getter function. Get the command string of the instruction
         * @note   'const' prevents modification on the command string.
         * @return C-string that storing I/O command
         * @author Mun William
         */
        const char* getCommand() const override { return op.c_str(); }
};

/**
 * @brief Derived class from Instruction class that executes shift and rotate instruction.
 * @details Perform runtime polymorphism. Executes ShiftInstruction::execute() when execute is called via base class pointer, but with derived class object.
 * @author Kong Zhun Rui
 */
class ShiftInstruction : public Instruction {
    private:
        string op; //"SHL" "SHR" "ROR" "ROL"
        int regI; //register array index
        int count; //raw number of bit positions to shift/rotate
    public:
        /**
        * @brief Parameterized constructor. Constructs a new bitwise shift instruction object.
        * @param operation String representing the specific shift and rotate direction ("SHL" for shift left, "SHR" for shift right, "ROL" for rotate left or "ROR" for rotate right).
        * @param idx Integer value representing the targeted data register index.
        * @param shiftCount Integer value representing how many bit positions to shift.
        * @pre idx should be a valid data register index (typically 0-7), and shiftCount should be non-negative.
        * @post Created a new ShiftInstruction object with the operation type, register index, and shift count initialized.
        * @author Kong Zhun Rui
        */
        ShiftInstruction(string operation, int idx, int shiftCount): op(operation), regI(idx), count(shiftCount) {} 
        /**
        * @brief Executes shift and rotate instruction.
        * @param cpu Reference to CPU object, that containing the memory, register.
        * @note This is a polymorphic function. 'override' means ShiftInstruction::execute() function will override pure virtual function, Instruction::execute() in base class.
        * @throws LogicException if the shift count is negative.
        * @post Clears all CPU flags, shifts or rotates the 8-bit register value by the specified count, saves the updated 8-bit signed value back to the register, and sets the logical status flags.
        * @author Kong Zhun Rui
        */
        void execute(CPU& cpu) override;

        /**
         * @brief  Getter function. Get the command string of the instruction
         * @note   'const' prevents modification on the command string.
         * @return C-string that storing logical command (shift and rotate operation)
         * @author Mun William
         */
        const char* getCommand() const override { return op.c_str(); }
};
/**
 * @brief Derived class from Instruction class that executes reset target flag instruction.
 * @details Perform runtime polymorphism. Executes ResetFlagsInstruction::execute() when execute is called via base class pointer, but with derived class object.
 * @author Kong Zhun Rui
 */
class ResetFlagsInstruction : public Instruction {
    private:
        string targetFlag; //cf, of, uf, zf
    public:
        /**
        * @brief Parameterized constructor. Constructs a new clear-flag instruction object.
        * @param flagName String representing the specific CPU condition flag to reset. For example, "ZF", "CF", or "UF".
        * @post Created a new ResetFlagsInstruction object with the targeted status flag name initialized.
        * @author Kong Zhun Rui
        */
        ResetFlagsInstruction(string flagName) : targetFlag(flagName){}
        /**
        * @brief Executes the reset flag instruction to clear the flag status.
        * @param cpu Reference to CPU object, that containing the memory, register.
        * @post Clears the specific status flag, which is"CF", "ZF", "OF", or "UF" to match the targetFlag property to false.
        * @author Kong Zhun Rui
        */
        void execute(CPU& cpu) override;

        /**
         * @brief  Getter function. Get the command string of the instruction
         * @note   'const' prevents modification on the command string.
         * @return C-string that storing reset flag command
         * @author Mun William
         */
        const char* getCommand() const override { return "RESET"; }
};

/**
 * @brief   Derived class from Instruction class that executes load or store instruction.
 * @details Perform runtime polymorphism. Executes LoadStoreInstruction::execute() when execute is called via base class pointer, but with derived class object.
 * @author  Mun William
 */
class LoadStoreInstruction : public Instruction {
    private:
        /**
         * Identifies the instruction based on:
         * mode 1: LOAD <Register>, [<Address>] Example: LOAD R1, [20]
         * mode 2: STORE <Register>, <Address> Note that arrangement of operand is NOT IMPORTANT. Example: STORE R1, 43 or STORE 43, R1
         * mode 3: STORE <Register>, [<Register>] Example: STORE R1, [R2]
         */
        int mode;

        // Stores the data register index (0-7)
        int dataRegisterIndex;

        // Stores the index for data register which storing address (for mode 3) (0-7)
        int addressRegisterIndex;

        // Stores the memory address (index of array with 64 element)
        int memoryAddress;

    public:
        /**
         * @brief        Parameterized constructor. Constructs a new load or store instruction object.
         * @details      Use when handling instruction with mode 1: LOAD <Register>, [<Address>] and mode 2: STORE <Register>, <Address>
         * @param m      Integer value represent mode of load or store instruction.
         * @param dRI    Integer value represent data register index
         * @param memAdd Signed char value represent memory address (index of array with 64 elements)
         * @pre          m should be in range 1-3, dRI should be in range 0-7, memAdd should be in range 0-63
         * @throws       SyntaxException if m smaller than 1 or larger than 3
         * @post         Created a new LoadStoreInstruction object.
         * @author       Mun William
         */
        LoadStoreInstruction(int m, int dRI, signed char memAdd);

        /**
         * @brief     Parameterized constructor. Constructs a new load or store instruction object.
         * @details   Use when handling instruction with mode 3: STORE <Register>, [<Register>]
         * @param m   Integer value represent mode of load or store instruction.
         * @param dRI Integer value represent data register index
         * @param aRI Integer value represent index of data register which storing an address
         * @pre       m should be in range 1-3, dRI should be in range 0-7, aRI should be in range 0-7
         * @throws    SyntaxException if m smaller than 1 or larger than 3.
         * @post      Created a new LoadStoreInstruction object.
         * @author    Mun William
         */
        LoadStoreInstruction(int m, int dRI, int aRI);

        /**
         * @brief     Executes the LOAD or STORE instruction based on its mode.
         * @param cpu Reference to CPU object, that containing the memory, register.
         * @note      This is a polymorphic function. 'override' means LoadStoreInstruction::execute() function will override pure virtual function, Instruction::execute() in base class.
         * @throws    SyntaxException if mode is not 1, 2, or 3.
         * @post      Execute the respective instruction, load value found in specified memory address into specified register, or store value found in specified register into specified memory address.
         * @author    Mun William
         */
        void execute(CPU& cpu) override;

        /**
         * @brief  Getter function. Get the command string of the instruction
         * @note   'const' prevents modification on the command string.
         * @return C-string that storing load or store command
         * @author Mun William
         */
        const char* getCommand() const override { return (mode == 1) ? "LOAD" : "STORE"; }
};

/**
 * @brief   Derived class from Instruction class that executes stack instruction.
 * @details Perform runtime polymorphism. Executes StackInstruction::execute() when execute is called via base class pointer, but with derived class object.
 * @author  Mun William
 */
class StackInstruction : public Instruction {
    private:
        // Stores the type of stack operation, either "PUSH" or "POP"
        string operation;

        // Stores the data register index (0-7)
        int dataRegisterIndex;

        // Stores the reference of stack in the virtual machine
        CustomStack<signed char>& systemStack;

    public:
        /**
         * @brief       Parameterized constructor. Constructs a new stack instruction object.
         * @param op    String that represent the type of stack operation.
         * @param dRI   Integer value represent data register index
         * @param sysSk CustomStack reference that point to the stack in CPU.
         * @throws      SyntaxException if op is not "PUSH" or "POP".
         * @post        Created a new StackInstruction object.
         * @author      Mun William
         */
        StackInstruction(string op, int dRI, CustomStack<signed char>& sysSk);

        /**
         * @brief     Executes the PUSH or POP instruction.
         * @param cpu Reference to CPU object, that containing the memory, register.
         * @note      This is a polymorphic function. 'override' means StackInstruction::execute() function will override pure virtual function, Instruction::execute() in base class.
         * @throws    SyntaxException if operation is not "PUSH" or "POP".
         * @post      Execute the PUSH instruction, push value in register into program stack, or POP instruction, pop value in the program stack into the register.
         * @author    Mun William
         */
        void execute(CPU& cpu) override;

        /**
         * @brief  Getter function. Get the command string of the instruction
         * @note   'const' prevents modification on the command string.
         * @return C-string that storing stack command
         * @author Mun William
         */
        const char* getCommand() const override { return operation.c_str(); }
};

// Loads programs, decodes instructions, delegates execution to CPU
class Runner {
    private:
        CPU virtualMachine; // Composition, actual virtual machine that will do math and store data
        CustomVector<Instruction*> program; // dynamic array vector that hold pointers to instructions
        // uses polymorphism, holds generic instruction pointers, but they will point to specific types

        bool isBlankLine(string dummy); // helper function, checks if a line is empty or just spaces
        string format4(int num); // Helper function to pad numbers with leading zeroes (eg. 5 into 0005)
        int numberReg(string dummy); // helper function, extracts the number from a register (eg. R1 becomes 1)
        Instruction* handleMove(int reg, string value);

        // acts as translator, the read text from file and figure whih instruction object to create
        Instruction* MathAndLogic(const string& first, stringstream& rest);
        Instruction* parseIOAndStack(const string& first, stringstream& rest);
        Instruction* parseLoadStore(const string& first, stringstream& rest);
        Instruction* ShiftAndReset(const string& first, stringstream& rest);
        string buildCpuStateString(); // build the shared cpu state to prevent repetition
        void decodeAndStore(string currentline); 

    public:
        Runner() {}  // default constructor
        ~Runner(); // destructor to clean up dynamic allocated memory, prevent memory leak
        void loadProgram(const string& filename); // loads asm file, read it, translate into instructions
        void executeProgram(const string& outputFilename = "output.txt"); // loops through the saved instructions and tells the CPU to perform them
        void dumpStateToScreen(); // screen output
        void dumpStateToFile(ofstream& outFile); // file output
};

// ==========================================
// Class Implementation
// ==========================================
template <typename T>
CustomVector<T>::CustomVector()
{
    capacity = 2; // Start with a small capacity
    current_size = 0;
    arr = new T[capacity];
}

template <typename T>
CustomVector<T>::CustomVector(const CustomVector<T>& right)
{
    capacity = right.capacity;
    current_size = right.current_size;
    arr = new T[capacity];
    for (int i = 0; i < current_size; i++) {
        arr[i] = right.arr[i];
    }
}

template <typename T>
void CustomVector<T>::push_back(T element)
{
    // If the array is full, resize it first
    if (current_size == capacity) {
        resize();
    }
    arr[current_size] = element;
    current_size++;
}

template <typename T>
void CustomVector<T>::pop_back()
{
    if (current_size == 0) {
        throw LogicException("Trying pop_back on empty CustomVector."); ///< @note Modified by Mun William: Exception Handling
    }
    arr[current_size - 1].~T();
    // Logically remove it by shrinking the size
    current_size--;
}

template <typename T>
T CustomVector<T>::at(int index) const
{
    if (index < 0 || index >= current_size) {
        throw LogicException("Index out of bounds in CustomVector."); ///< @note Modified by Mun William: Exception Handling
    }
    return arr[index];
}

template <typename T>
const T& CustomVector<T>::operator[](int index) const
{
    if (index < 0 || index >= current_size) {
        throw LogicException("Index out of bounds in CustomVector."); ///< @note Modified by Mun William: Exception Handling
    }
    return arr[index];
}

template <typename T>
void CustomVector<T>::erase(int index)
{
    if (index < 0 || index >= current_size) {
        throw LogicException("Index out of bounds in CustomVector."); ///< @note Modified by Mun William: Exception Handling
    }

    arr[index].~T();

    for (int i = index; i < current_size - 1; i++) {
        arr[i] = arr[i + 1];
    }
    current_size--;
}

template <typename T>
T& CustomVector<T>::operator[](int index)
{
    if (index < 0 || index >= current_size) {
        throw LogicException("Index out of bounds in CustomVector."); ///< @note Modified by Mun William: Exception Handling
    }
    return arr[index];
}

template<typename T>
CustomVector<T> &CustomVector<T>::operator=(const CustomVector<T> &right)
{
    if (this == &right) return *this;

    delete[] arr;

    capacity = right.capacity;
    current_size = right.current_size;
    arr = new T[capacity];

    for (int i = 0; i < current_size; i++) {
        arr[i] = right.arr[i];
    }
    return *this;
}

template <typename T>
void CustomVector<T>::resize()
{
    capacity *= 2; // Double the capacity
    T* temp = new T[capacity]; // Create a new, bigger array

    // Copy old elements to the new array
    for (int i = 0; i < current_size; i++) {
        temp[i] = arr[i];
    }

    delete[] arr; // Delete the old, small array
    arr = temp;   // Point to the new array
}

template <typename T>
CustomStack<T> &CustomStack<T>::operator=(const CustomStack<T> &right)
{
    if (this == &right)
        return *this;
    data = right.data;
    return *this;
}

template <typename T>
void CustomStack<T>::pop()
{
    if (isEmpty()) {
        throw LogicException("Stack underflow. Trying to pop empty CustomStack."); ///< @note Modified by Mun William: Exception Handling
    }
    data.pop_back();
}

template <typename T>
T CustomStack<T>::peek() const
{
    if (isEmpty()) {
        throw LogicException("Stack underflow. Trying to peek empty CustomStack."); ///< @note Modified by Mun William: Exception Handling
    }
    return data[data.size() - 1];
}

template <typename T>
void CustomQueue<T>::dequeue()
{
    if (isEmpty()) {
        throw LogicException("Queue underflow. Trying to dequeue empty CustomQueue."); ///< @note Modified by Mun William: Exception Handling
    }
    data.erase(0);
}

template <typename T>
T CustomQueue<T>::front() const
{
    if (isEmpty()) {
        throw LogicException("Queue underflow. Trying to front empty CustomQueue."); ///< @note Modified by Mun William: Exception Handling
    }
    // directly see the front
    return data[0];
}

template <typename T>
CustomQueue<T> &CustomQueue<T>::operator=(const CustomQueue<T> &right)
{
    if (this == &right)
        return *this;
    data = right.data;
    return *this;
}

void FlagRegister::flagArithmeticSetter(unsigned char oper1, unsigned char oper2, int result)
{
    setCF(checkCF(result));
    setOF(checkOF(oper1, oper2, static_cast<unsigned char>(result)));
    setUF(checkUF(oper1, oper2, static_cast<unsigned char>(result)));
    setZF(checkZF(static_cast<signed char>(result)));
}

void FlagRegister::flagIOSetter(int input)
{
    setOF(input > 127);
    setUF(input < -128);
    setZF(input == 0);
}

void FlagRegister::flagLogicalSetter(unsigned char result)
{
    setZF(checkZF(static_cast<signed char>(result)));
}

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

    for(int i=0;i<64;i++){
        this->data[i] = other.data[i];
    }

    return *this;
}

signed char Memory::read(int address) const
{
    if(address < 0 || address > 63){
        throw HardwareException("Address " + to_string(address) + " is invalid to read.");
    } else {
        return data[address];  
    }
}

void Memory::write(int address, signed char value)
{
    if(address < 0 || address > 63){
        throw HardwareException("Address " + to_string(address) + " is invalid to read.");
    } else {
        data[address] = value;  
    }
}

void Memory::displayMemory()
{
    cout << "#Memory#" << endl;
    cout << "#";
    for(int i = 0; i < 64; i++){
        if(i == 8 || i == 16 || i == 24 || i == 32 || i == 40 || i == 48 || i == 56){
            cout << endl;
            cout << "#" << static_cast<int>(data[i]) << "#";
        } else {
            cout << static_cast<int>(data[i]) << "#";
        }
    }
    cout << endl;
}

DataRegister *CPU::getRegister(int index)
{   
    // Checks register index at here to prevent out of bound register index accessed by user. Otherwise it throws error if index is out of bound.
    if(index < 0 || index > 7) ///< @note Added by Mun William: Exception Handling
        throw HardwareException("Register index " + to_string(index) + " is not valid."); ///< @note Added by Mun William: Exception Handling

    return &R[index]; 
} 

void CPU::pushToStack(signed char value)
{
    systemStack.push(value); // puts the data into the customstack
    incrementSI(); //updates counter so the cpu knows
}

signed char CPU::popFromStack()
{
    try { ///< @note Added by Mun William: Exception Handling
        signed char peek = systemStack.peek();
        systemStack.pop();
        decrementSI();
        return peek;
    } catch (LogicException& e) { ///< @note Added by Mun William: Exception Handling
        throw HardwareException("Stack underflow. Trying to pop empty stack."); ///< @note Added by Mun William: Exception Handling
    }
}

int ArithmeticInstruction::compute(int v1, int v2){
    if (ar == "ADD") return v1 + v2;
    if (ar == "SUB") return v1 - v2;
    if (ar == "MUL") return v1 * v2;
    if (ar == "DIV") {
        if (v1 == 0) throw LogicException("Division by 0."); // throw exception when v1 is divided by 0
        return v2 / v1;
    }
    if (ar != "ADD" && ar != "SUB" && ar != "MUL" && ar != "DIV") throw SyntaxException ("Error: Invalid operation."); // throw exception when operation invalid
    return 0;
}

ArithmeticInstruction::ArithmeticInstruction(string operation, int dest): destRI(dest)
{
    if (operation == "INC"){
        ar = "ADD";
        sourceVal = 1;
        isImmediate = true;
    } else if (operation == "DEC"){
        ar = "SUB";
        sourceVal = 1;
        isImmediate = true;
    } else {
        throw SyntaxException("Invalid arithmetic operation syntax"); ///< @note Modified by Mun William: Exception Handling
    }
}

void ArithmeticInstruction::execute(CPU& cpu)
{
    DataRegister* destReg = cpu.getRegister(destRI); //fetch the pointer to destination register
    FlagRegister* flags = cpu.getFlags();  // fetch the pointer to cpu flag register
    flags->resetAll(); //clear all cpu flags

    int val1 = destReg->getValue(); // read the current int value from destination
    int val2 = 0; 

    //route the operand lookup based on the flag type
    if (isImmediate){
        val2 = sourceVal; //use the raw int directly
    } else {
        val2 = cpu.getRegister(sourceVal)->getValue(); //fetch from register index
    }

    int result = compute(val1, val2); // perform math operation

    flags->flagArithmeticSetter(static_cast<unsigned char>(val1), static_cast<unsigned char>(val2), result);

    destReg->setValue(static_cast<signed char>(result)); // write final result to destination register
}

void MoveInstruction::execute(CPU& cpu)
{
    Memory* memory = cpu.getMemory(); // fetch the pointer to memory
    FlagRegister* flags = cpu.getFlags();
    if (mode == 1){ //MOV register, intermediate
        cpu.getRegister(destI)->setValue(static_cast<signed char>(sourceI));
    } else if (mode == 2){ // MOV register, register
        cpu.getRegister(destI)->setValue(cpu.getRegister(sourceI)->getValue());
    } else if (mode == 3 || mode == 4){ // MOV register, [register] or LOAD register, [address] 
        int address = cpu.getRegister(sourceI)->getValue(); //get address stored inside the register
        int dataFromMemory = memory->read(address); //fetch data from that memory address
        cpu.getRegister(destI)->setValue(dataFromMemory); // store it in destination register
    }

    flags->flagIOSetter(sourceI);
}

void IOInstruction::execute(CPU& cpu)
{
    DataRegister* reg = cpu.getRegister(regI); //fetch the pointer to register
    FlagRegister* flags = cpu.getFlags(); // fetch the pointer to cpu flag register

    if (op == "INPUT"){ //check instruction is input command
        flags->resetAll(); //clear all cpu flags

        cout << "?" << endl;
        int rawInput; // to store user value
        cin >> rawInput; //read user value

        reg->setValue(static_cast<signed char>(rawInput)); //convert 32-bit integer to 8-bit signed byte
        flags->flagIOSetter(rawInput);

    } else if (op == "DISPLAY") { //check instruction is display command
        cout << "R" << regI << "=" << static_cast<int>(reg->getValue()) << endl;
    } else if (op != "INPUT" && op != "DISPLAY") { //if not, throw an exception
        throw SyntaxException("Invalid IO operation."); ///< @note Modified by Mun William: Exception Handling
    }
}

void ShiftInstruction::execute(CPU& cpu)
{
    if (count < 0) 
        throw LogicException("Cannot shift by a negative number."); //immediate execution halt if a negative shift value is provided
    DataRegister* reg = cpu.getRegister(regI); //fetch pointer to register
    FlagRegister* flags = cpu.getFlags(); //fetch pointer to cpu flags
    flags->resetAll(); //clear all flags
    unsigned char val = static_cast<unsigned char>(reg->getValue()); //cast the register's signed value to an unsigned byte
    int dCount = count % 8; // calculate the shift index count within the boundaries of an 8-bit block
    if (count >= 8 && (op == "SHL" || op == "SHR")) val = 0; //shifting left or right by 8 or more positions completely zeroes out the byte
    else if (dCount > 0) {
        if (op == "SHL"){ //shift left
            val = val << dCount; // push bit to the left
        }else if (op == "SHR"){ //shift right
            val = val >> dCount; //push bit to the right
        }else if (op == "ROL") { //rotate left
            val = (val << dCount) | (val >> (8 - dCount));
        }else if (op == "ROR"){ //rotate right
            val = (val >> dCount) | (val << (8 - dCount));
        }
    }
    reg->setValue(static_cast<signed char>(val));
    flags->flagLogicalSetter(static_cast<unsigned char>(val));
}

void ResetFlagsInstruction::execute(CPU& cpu)
{
    FlagRegister* flags = cpu.getFlags();
    if (targetFlag == "CF") flags->setCF(false);
    else if (targetFlag == "ZF") flags->setZF(false);
    else if (targetFlag == "OF") flags->setOF(false);
    else if (targetFlag == "UF") flags->setUF(false);
}

LoadStoreInstruction::LoadStoreInstruction(int m, int dRI, signed char memAdd)
{
    addressRegisterIndex = -1;
    dataRegisterIndex = dRI;
    memoryAddress = static_cast<int>(memAdd); // Address out of bound will be handled by Memory class, so here we don't do checking first.

    if(m >= 1 && m <= 3){
        mode = m;
    } else {
        throw SyntaxException("Invalid LOAD or STORE instruction format.");
    }
}

LoadStoreInstruction::LoadStoreInstruction(int m, int dRI, int aRI)
{
    memoryAddress = -1;
    dataRegisterIndex = dRI;
    addressRegisterIndex = aRI;

    if(m >= 1 && m <= 3){
        mode = m;
    } else {
        throw SyntaxException("Invalid load or store operation.");
    }
}

void LoadStoreInstruction::execute(CPU &cpu)
{
    DataRegister* datReg = cpu.getRegister(dataRegisterIndex);

    Memory* mem = cpu.getMemory();

    if(mode == 1){ // LOAD <Register>, [<Address>]
        datReg->setValue(mem->read(memoryAddress));
    } else if (mode == 2){ // STORE <Register>, <Address>
        mem->write(memoryAddress, datReg->getValue());
    } else if (mode == 3){ // STORE <Register>, [<Register>]
        DataRegister* addReg = cpu.getRegister(addressRegisterIndex);
        mem->write(static_cast<int>(addReg->getValue()), datReg->getValue());
    } else {
        throw SyntaxException("Invalid load or store operation."); // Prevent unexpected value passing into mode
    }
}

StackInstruction::StackInstruction(string op, int dRI, CustomStack<signed char> &sysSk): systemStack(sysSk)
{
    dataRegisterIndex = dRI;

    if(op == "PUSH" || op == "POP"){
        operation = op;
    } else {
        throw SyntaxException("Invalid stack operation.");
    }
}

void StackInstruction::execute(CPU& cpu)
{
    DataRegister* datReg = cpu.getRegister(dataRegisterIndex);

    if(operation == "PUSH"){
        cpu.pushToStack(datReg->getValue());
    } else if (operation == "POP") {
        datReg->setValue(cpu.popFromStack());
    } else {
        throw SyntaxException("Invalid stack operation."); // Prevent unexpected value passing into operation
    }
}

bool Runner::isBlankLine(string dummy)
{
    if (dummy.empty()) return true; // if there is zero character, return blank
    for(int i=0; i < dummy.length(); i++) // look at every character in the string
    {
        if(dummy[i] != ' ' && dummy[i]!= '\t' && dummy[i]!= '\r' && dummy[i]!= '\n') // find anything that is not a space, tab or enter key, then not blank
            return false;
    }
    return true; // if only found spaces or tabs, it is blank
}

string Runner::format4(int num)
{
    stringstream belt;
    // setfill('0') tells it to use zeroes.
    // setw(4) tells it to make sure the string is exactly 4 characters wide.
    belt << setfill('0') << setw(4) << num;
    return belt.str(); //  convert the stream back into a normal string
}

int Runner::numberReg(string dummy)
{
    if(dummy.empty()) return 0; // if zero character, returns 0
    if(dummy[0] == 'R' || dummy[0] == 'r') // check if the first letter is an R or r
    {
        string justNumber = dummy.substr(1); // extract everything after the R (e.g. grab the 1 from R1)
        return stoi(justNumber); // convert the string 1 into integer 1
    }
    return 0;
}

Instruction* Runner::handleMove(int reg, string value) {
        if (value.front() == '[') {
            // Register indirect [R1], we are moving based on a memory address stored in a register
            string inner = value.substr(1, value.length() - 2); // strip the brackets to get R1
            return new MoveInstruction(3, reg, numberReg(inner));
        }
        else if (value[0] == 'R' || value[0] == 'r'){
            // Register to register (eg. MOV R1, R2)
            return new MoveInstruction(2, reg, numberReg(value));
        }
        // immediate to register (eg. MOV R1, 5)
        return new MoveInstruction(1, reg, stoi(value));
    }

Instruction* Runner::MathAndLogic(const string& first, stringstream& rest) {
    // filter out commands this function doesn't handle
    if (first != "INC" && first != "DEC" && first != "ADD" && first != "SUB" && first != "MUL" && first != "DIV" && first != "MOV") return nullptr;

    string dest,value;
    
    // if the command is increment or decrement, only uses 1 register
    if (first == "INC" || first == "DEC") {
        rest >> dest; // read the next word (eg. R1)
        if (dest.empty()) throw SyntaxException("Missing register for: ");
        if (dest.back() == ',') throw SyntaxException("Unexpected comma in: " + dest);
        return new ArithmeticInstruction(first, numberReg(dest));
    }

    // for math and mov, read the next two words (destination and value)
    rest >> dest >> value;

    if (dest.empty()) throw SyntaxException("Missing operands for command: ");
    
    if (value.empty()) {
        if (dest.find(',') == string::npos) throw SyntaxException("Missing comma and value operand in: " + dest);
        if (dest.back() == ',') throw SyntaxException("Missing value: " + dest);
        throw SyntaxException("Missing space after comma: " + rest.str());
    }
    if (dest.back() != ',') throw SyntaxException("Missing comma: " + dest + " " + value);

    // clean variable 'dest' (remove the trailing comma)
    dest.pop_back();

    int reg = numberReg(dest); // convert R1 to 1

    // handle move instructions which have diff modes
    if (first == "MOV") {
        return handleMove(reg,value); }
        
    // if wasnt a MOV, it must be basic math operating
    // check if the second value is a register (starting with R or r)
    if (value[0] == 'R' || value[0] == 'r'){
        // if its a register (eg. add r1, r2)
        return new ArithmeticInstruction(first, reg, numberReg(value), false); } // means not immediate)
    // it is an immediate number (eg. add r1, 6)
    else {
        return new ArithmeticInstruction(first, reg, stoi(value), true); } // means its immediate
}

Instruction* Runner::parseIOAndStack(const string& first, stringstream& rest) {
    if (first != "INPUT" && first != "DISPLAY" && first != "PUSH" && first != "POP") return nullptr; 

    string a;

    rest >> a;

    if (a.empty()) throw SyntaxException("Missing register operand for command: ");

    if (a.back() == ',') throw SyntaxException("Unexpected comma after register in: " + a);

    // input from keyboard or display to screen
    if (first == "INPUT" || first == "DISPLAY"){
        return new IOInstruction(first, numberReg(a));}
    else {
        return new StackInstruction(first, numberReg(a), virtualMachine.getSystemStack());}
}

Instruction* Runner::parseLoadStore(const string& first, stringstream& rest){
    if (first != "LOAD" && first != "STORE") return nullptr; // exit early if not memory

    string a, b;
    rest >> a >> b;

    if (a.empty()) throw SyntaxException("Missing operands for command: ");
    if (b.empty()) {
        if (a.find(',') == string::npos) throw SyntaxException("Missing comma and value operand in: " + a);
        if (a.back() == ',') throw SyntaxException("Missing value operand for command: ");
        else throw SyntaxException("Missing space after comma in: " + rest.str()); }
    if (a.back() != ',') throw SyntaxException("Missing comma after first operand in: " + a + " " + b); 
     
    a.pop_back();

    // loading from memory into a register
    if (first == "LOAD"){
        if (b.front() == '[') {
            b = b.substr(1, b.length() -2); // clean bracket
            // if loading from an address stored inside a register, eg. Load R1, [R2]
            if (b[0] == 'R' || b[0] == 'r') return new LoadStoreInstruction(1, numberReg(a), numberReg(b));

            // loading direct from a direct memory number, (eg. load R1, [20])
            return new LoadStoreInstruction(1, numberReg(a), static_cast<signed char>(stoi(b)));
        }
    }

    // storing from a register into a memory
    if (first == "STORE"){
        // if storing into an address pointed to by a register, eg. store R1, [R2]
        if (b.front() == '['){
            b = b.substr(1, b.length() - 2); // clean brackets
            return new LoadStoreInstruction(3, numberReg(a), numberReg(b)); }
        else if (a[0] == 'R' || a[0]== 'r') {
            // storing directly into a specific memory slot (eg. store R3, 20), 20 is the memory address R3 is the register that holds the value to be stored
            return new LoadStoreInstruction(2, numberReg(a), static_cast<signed char>(stoi(b))); }
        // stores into memory slot (eg. store 20, R3), this also stores the value in register 3 to memory 20
        else  {
            return new LoadStoreInstruction(2, numberReg(b), static_cast<signed char>(stoi(a))); }
    }
    return nullptr;
}

Instruction* Runner::ShiftAndReset(const string& first, stringstream& rest) {
    if (first != "RESET" && first != "SHL" && first != "SHR" && first != "ROL" && first != "ROR") return nullptr;

    string a,b;
    rest >> a;
    if (a.empty()) throw SyntaxException("Missing operands for command: ");

    // clearing the flags
    if (first == "RESET"){
    if (a.back() == ',') throw SyntaxException("Unexpected comma in RESET: " + a);
        return new ResetFlagsInstruction(a);
    }

    rest >> b;

    if (b.empty()) {
        if (a.find(',') == string::npos) throw SyntaxException("Missing comma and value operand in: " + a);
        if (a.back() == ',') throw SyntaxException("Missing value: " + a);
        throw SyntaxException("Missing space after comma in: " + rest.str());}
    if (a.back() != ',') throw SyntaxException("Missing comma: " + a + " " + b);

    a.pop_back();

    return new ShiftInstruction(first, numberReg(a), stoi(b));
}

string Runner::buildCpuStateString()
{
    stringstream out;
    out << "#Begin#\n";
        
    out << "#Registers#";
    for (int i = 0; i < 8; i++) {
        out << format4((int)virtualMachine.getRegister(i)->getValue()) << "#";
    }
    out << "\n";

    FlagRegister* f = virtualMachine.getFlags();
    out << "#Flags#OF#" << f->getOF() << "#UF#" << f->getUF() << "#CF#" << f->getCF() << "#ZF#" << f->getZF() << "#\n";

    out << "#PC#" << format4((int)virtualMachine.getPC()) << "#\n";
        
    return out.str();
}

Runner::~Runner()
{
    for (int i = 0; i < program.size(); i++)
        {delete program.at(i);}
}

void Runner::decodeAndStore(string currentLine){
    stringstream lineStream(currentLine); // turn the string into a stream to read word by word
        string first;
        lineStream >> first; // read the first word (eg. ADD)

        // try to translate the instruction by passing it into our 3 parser functions, if the first cant handle it, then returns nullptr, so we try MemAndIO
        Instruction* inst = MathAndLogic(first, lineStream);
        if (!inst) inst = parseIOAndStack(first, lineStream);
        if (!inst) inst = parseLoadStore(first, lineStream);
        if (!inst) inst = ShiftAndReset(first, lineStream);
        
        // if one of the parsers successfully created an instruction, save it
        if (inst) {
            // --- NEW STRICT CHECK: Prevent multiple commands on one line ---
            string extraGarbage;
            if (lineStream >> extraGarbage) {
            throw SyntaxException("Trailing characters or multiple instructions on one line: " + extraGarbage); }
            program.push_back(inst); }
        else throw SyntaxException("Invalid syntax found: " + first);
}

void Runner::loadProgram(const string& filename)
{
    // Read .asm file line by line
    // Decode strings into Instruction objects
    // Store in CustomVector
    ifstream file(filename);
    if(!file.is_open()){
        throw FileException(filename, "File is not found or cannot be opened."); 
    }

    //store into queue
    CustomQueue<string> lineQueue;
    string line;

    // read every line from the file, and put it in a queue
    while(getline(file,line))
    {
        if(isBlankLine(line)) continue; // skip empty lines
        lineQueue.enqueue(line); // put the line back at the queue
    }
    file.close(); // close the file when done

    // take lines out the queue one by one, translate them and put them into a vector
    try {
        while(!lineQueue.isEmpty())
        {
            string currentLine = lineQueue.front();

            // cast all uppercase
            for (char &c : currentLine) c = toupper(c);

            decodeAndStore(currentLine);
            lineQueue.dequeue();
        }
    } catch (VMException& e){
        throw RunTimeCrashException(e.getErrorMessage());
    }
}

void Runner::executeProgram(const string& outputFilename)
{
    ofstream outFile;
    outFile.open(outputFilename);

    // try-catch blocks protect the program from crashing
    // try {
    // loop through our vector of instructions from top to bottom
    for (int i = 0; i < program.size(); i++)
    {
        try{
            // tell the specific instruction to execute itself on our virtual machine
            program.at(i) ->execute(virtualMachine); // move the program counter forward by 1
            dumpStateToScreen(); // @debug
            virtualMachine.incrementPC(); // move the program counter forward by 1
        } catch (VMException& e){
            if(outFile.is_open()) {
                outFile << "\n Stopping...";
                outFile << "\n Command Caused Error: " << program.at(i)->getCommand();
                outFile << "\n Reason: " << e.getErrorMessage() << endl;
            }
            throw RunTimeCrashException(e.getErrorMessage(), virtualMachine.getPC()+1, program.at(i)->getCommand());
        }
    }
    dumpStateToScreen();
    dumpStateToFile(outFile);
    outFile.close();
}

void Runner::dumpStateToScreen()
{
    cout << buildCpuStateString();
    virtualMachine.getMemory()-> displayMemory();
    cout << "#End#\n";
}

void Runner::dumpStateToFile(ofstream& outFile)
{
    outFile << buildCpuStateString();

    outFile << "#Memory#\n";
    Memory* mem = virtualMachine.getMemory();
    for (int row = 0; row < 8; row++) {
        outFile << "#";
        for (int col = 0; col < 8; col++) {
            // Integer cast prevents ASCII symbols from ruining the file
            outFile << format4((int)mem->read((row * 8) + col)) << "#";
        }
        outFile << "\n"; 
    }

    outFile << "#End#\n";
}

int main() {
    Runner interpreter;
    string filename;

    try{
        cout << "Enter the name of the assembly file you want to run (eg., test.asm): ";
        cin >> filename;
        
        if(filename.length() < 4 || filename.substr(filename.length() - 4) != ".asm")
            throw FileException(filename, "Invalid input file type- " + filename + ".\nMust be a .asm file.");
    } catch (VMException& e){
        cerr << e.getErrorMessage() << endl;
        return 1;
    } 
    
    string baseName = filename.substr(0, filename.length() - 4); 
    
    // build the new string
    string outName = "output - " + baseName + ".txt";

    try{
        interpreter.loadProgram(filename);
        interpreter.executeProgram(outName);
    } catch (VMException& e){
        cerr << e.getErrorMessage() << endl;
        return 1;
    }

    cout << "\n Program finished! Check output.txt for the full record \n";
    return 0;
}