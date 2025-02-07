#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>

using namespace std;

// Define instruction opcodes
enum Opcode {
    ADD = 0,
    SUB = 1,
    LOAD = 2,
    STORE = 3
};

// Instruction structure
struct Instruction {
    Opcode opcode;
    int operand1;
    int operand2;
    int operand3;
};

class Simulator {
private:
    vector<int> memory; // Memory
    vector<int> registers; // Registers
    int pc; // Program counter

public:
    Simulator() : memory(256), registers(8), pc(0) {}

    // Load program into memory
    void loadProgram(const vector<Instruction>& program) {
        for (int i = 0; i < program.size(); ++i) {
            memory[i] = encodeInstruction(program[i]);
        }
    }

    // Execute next instruction
    void executeNext() {
        Instruction instr = decodeInstruction(memory[pc]);
        switch (instr.opcode) {
            case ADD:
                registers[instr.operand1] += registers[instr.operand2];
                break;
            case SUB:
                registers[instr.operand1] -= registers[instr.operand2];
                break;
            case LOAD:
                registers[instr.operand1] = memory[instr.operand2];
                break;
            case STORE:
                memory[instr.operand2] = registers[instr.operand1];
                break;
            default:
                cerr << "Invalid opcode\n";
                break;
        }
        pc++;
    }

    // Display registers and memory
    void displayState() {
        cout << "Registers:\n";
        for (int i = 0; i < registers.size(); ++i) {
            cout << "R" << i << ": " << registers[i] << endl;
        }
        cout << "\nMemory:\n";
        for (int i = 0; i < memory.size(); ++i) {
            cout << setw(5) << hex << memory[i] << " ";
            if ((i + 1) % 8 == 0) cout << endl;
        }
        cout << endl;
    }

private:
    // Encode instruction into a single integer
    int encodeInstruction(const Instruction& instr) {
        return (instr.opcode << 12) | (instr.operand1 << 8) | (instr.operand2 << 4) | instr.operand3;
    }

    // Decode instruction from a single integer
    Instruction decodeInstruction(int instr) {
        return { static_cast<Opcode>((instr >> 12) & 0xF),
                 (instr >> 8) & 0xF,
                 (instr >> 4) & 0xF,
                 instr & 0xF };
    }
};

int main() {
    // Example program
    vector<Instruction> program = {
        {ADD, 1, 2, 0}, // R1 = R1 + R2
        {SUB, 3, 1, 0}, // R3 = R3 - R1
        {LOAD, 2, 10, 0}, // R2 = Mem[10]
        {STORE, 3, 20, 0} // Mem[20] = R3
    };


    Simulator simulator;
    simulator.loadProgram(program);

    cout << "Initial State:\n";
    simulator.displayState();

    cout << "Executing program...\n";
    for (int i = 0; i < program.size(); ++i) {
        cout << "Executing Instruction " << i+1 << endl;
        simulator.executeNext();
        simulator.displayState();
    }

    return 0;
}
