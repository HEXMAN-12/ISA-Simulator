#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <cstdlib> // For system-specific commands

using namespace std;

// ANSI color codes
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

// Define instruction opcodes
enum Opcode {
    ADD = 0,
    SUB = 1,
    LOAD = 2,
    STORE = 3,
    EXIT = 4
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
    Simulator() : memory(32), registers(8), pc(0) {}

    // Load program into memory
    void loadProgram(const vector<Instruction>& program) {
        for (int i = 0; i < program.size(); ++i) {
            memory[i] = encodeInstruction(program[i]);
        }
    }

    // Execute next instruction
bool executeNext() {
    if (pc >= memory.size()) {
        cerr << RED << "Program execution reached end of memory.\n" << RESET;
        return false;
    }

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
        case EXIT:
            cout << GREEN << "Program execution halted." << RESET << endl;
            return false;
        default:
            cerr << RED << "Invalid opcode\n" << RESET;
            break;
    }
    pc++;
    return true;
}

    // Display registers and memory
    void displayState() {
        system("cls"); // Clear screen
        cout << BLUE << "-----------------------------------\n";
        cout << "|       ISA Simulator Output      |\n";
        cout << "-----------------------------------\n" << RESET;
        cout << "Registers:\n";
        for (int i = 0; i < registers.size(); ++i) {
            cout << CYAN << "R" << i << ": " << RESET << registers[i] << endl;
        }
        cout << "\nMemory:\n";
        for (int i = 0; i < memory.size(); ++i) {
            cout << YELLOW << setw(3) << hex << memory[i] << " ";
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

Instruction parseInstruction(const string& input) {
    stringstream ss(input);
    int opcode, operand1, operand2, operand3;
    ss >> opcode >> operand1 >> operand2 >> operand3;
    return { static_cast<Opcode>(opcode), operand1, operand2, operand3 };
}

void displayIntro() {
  system("cls"); // Clear screen
        cout << BLUE << "-----------------------------------\n";
        cout << "|       ISA Simulator System      |\n";
        cout << "-----------------------------------\n" << RESET;
}

int main() {
    vector<Instruction> program;
    string input;

    displayIntro();

    cout << "Enter program instructions in hexadecimal format (e.g., 'ADD 1 2 0', 'SUB 3 1 0', etc.). Type 'done' to finish:\n";
    while (true) {
        cout << BLUE << "> " << RESET;
        getline(cin, input);

        if (input == "done")
            break;

        program.push_back(parseInstruction(input));
    }

    Simulator simulator;
    simulator.loadProgram(program);

    cout << "\nPress Enter to execute the program...\n";
    cin.ignore(); // Wait for user input

    cout << "Executing program...\n";
    bool running = true;
    while (running) {
        simulator.displayState();
        running = simulator.executeNext();
        cout << "Press Enter to continue...\n";
        cin.ignore(); // Wait for user input
    }

    return 0;
}
