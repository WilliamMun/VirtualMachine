#include <iostream>
#include <string>

using namespace std;

// ==========================================
// 1. 数据结构要求 (Data Structures)
// 完全手写，不使用 STL 的 <vector>
// ==========================================
template <typename T>
class MyVector {
private:
    T* arr;
    int capacity;
    int currentSize;

public:
    MyVector() {
        capacity = 2;
        currentSize = 0;
        arr = new T[capacity];
    }

    ~MyVector() { delete[] arr; }

    void push_back(T data) {
        if (currentSize == capacity) {
            T* temp = new T[capacity * 2];
            for (int i = 0; i < capacity; i++) {
                temp[i] = arr[i];
            }
            delete[] arr;
            capacity *= 2;
            arr = temp;
        }
        arr[currentSize] = data;
        currentSize++;
    }

    T get(int index) { return arr[index]; }
    int size() { return currentSize; }
};

// ==========================================
// 2. 硬件组件 (Hardware Components)
// 包含 Register, Memory, Flags
// ==========================================
class Register {
protected:
    signed char value; // 要求：1 byte signed char (-128 to 127)[cite: 1, 2]
public:
    Register() : value(0) {}
    virtual ~Register() {}

    void setValue(signed char v) { value = v; }
    signed char getValue() { return value; }
};

// 派生类：通用寄存器 R0-R7
class GeneralRegister : public Register {
public:
    GeneralRegister() : Register() {}
};

// ==========================================
// 3. 中央处理器 (CPU)
// 使用组合 (Composition) 将寄存器和内存包在一起
// ==========================================
class CPU {
private:
    GeneralRegister R[8];  // R0 到 R7[cite: 1, 2]
    signed char PC;        // 程序计数器[cite: 1, 2]
    signed char memory[64];// 64 bytes 内存[cite: 1, 3]

    // Flags (可以用 boolean 或 1 byte)[cite: 1, 2]
    bool OF, UF, CF, ZF;

public:
    CPU() : PC(0), OF(false), UF(false), CF(false), ZF(false) {
        for(int i=0; i<64; i++) memory[i] = 0;
    }

    // 封装：提供外部访问接口
    GeneralRegister& getReg(int index) { return R[index]; }
    void incrementPC() { PC++; }
    signed char getPC() { return PC; }

    // 供后续实现使用的 Flag 设置接口
    void setZF(bool state) { ZF = state; }
    void setOF(bool state) { OF = state; }
};

// ==========================================
// 4. 指令系统 (Instruction Hierarchy & Polymorphism)
// 极其重要：这是体现多态的地方
// ==========================================
class Instruction {
public:
    virtual ~Instruction() {}
    // 纯虚函数：要求所有具体指令必须实现 execute
    virtual void execute(CPU& cpu) = 0;
};

// 具体的指令：ADD <Dest>, <Source>
class AddInstruction : public Instruction {
private:
    int destRegIdx; // 比如 R2，这里存 2
    int srcRegIdx;  // 比如 R0，这里存 0

public:
    AddInstruction(int dest, int src) : destRegIdx(dest), srcRegIdx(src) {}

    void execute(CPU& cpu) override {
        signed char val1 = cpu.getReg(destRegIdx).getValue();
        signed char val2 = cpu.getReg(srcRegIdx).getValue();

        int result = val1 + val2; // 用 int 接住，方便判断溢出

        // --- 这里是你需要补充的 Flags 判断逻辑 ---
        // if (result > 127) { cpu.setOF(true); }
        // if (result == 0) { cpu.setZF(true); }
        // -----------------------------------------

        cpu.getReg(destRegIdx).setValue((signed char)result);
        cpu.incrementPC(); // 执行完，PC + 1[cite: 2]
    }
};

// 具体的指令：MOV <Dest>, <Value>
class MovValInstruction : public Instruction {
private:
    int destRegIdx;
    signed char value;

public:
    MovValInstruction(int dest, signed char val) : destRegIdx(dest), value(val) {}

    void execute(CPU& cpu) override {
        cpu.getReg(destRegIdx).setValue(value);
        cpu.incrementPC();
    }
};

// ==========================================
// 5. 运行器 (Runner)
// 负责协调整个程序的运行
// ==========================================
class Runner {
private:
    CPU myCPU;
    MyVector<Instruction*> program; // 存放指令的多态数组

public:
    ~Runner() {
        for(int i=0; i<program.size(); i++) {
            delete program.get(i);
        }
    }

    void loadHardcodedProgram() {
        // 模拟读取文件：MOV R1, 5
        program.push_back(new MovValInstruction(1, 5));

        // 模拟读取文件：ADD R1, 6 (注意：你需要实现 R1 和常量相加的类，这里用伪代码代替)
        // program.push_back(new AddValInstruction(1, 6));
    }

    void run() {
        cout << "GUGUGAGA" << endl;
        for(int i = 0; i < program.size(); i++) {
            program.get(i)->execute(myCPU); // 多态调用！非常漂亮

            // Debug 输出
            cout << "PC: " << (int)myCPU.getPC()
                 << " | R1: " << (int)myCPU.getReg(1).getValue() << endl;
        }
    }
};

// ==========================================
// 主函数
// ==========================================
int main() {
    Runner vmRunner;
    vmRunner.loadHardcodedProgram();
    vmRunner.run();

    return 0;
}
