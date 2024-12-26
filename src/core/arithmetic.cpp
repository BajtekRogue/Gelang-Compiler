#include "assembling.hpp"
#include "utility.hpp"


std::vector<AssemblyInstruction> generateMultiplication(){

    std::vector<AssemblyInstruction> code;
    
    code.push_back(AssemblyInstruction(Instruction::LABEL_PROCEDURE, "*"));

    // Store x in p[4] to be able to later determine the sign of the result. If x < 0 then we negate it here. If x == 0 then jump to the end
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Negate x if needed"));
    code.push_back(AssemblyInstruction(Instruction::LOAD, 1));
    code.push_back(AssemblyInstruction(Instruction::STORE, 4));
    code.push_back(AssemblyInstruction(Instruction::JZERO, 54));
    code.push_back(AssemblyInstruction(Instruction::JPOS, 4));
    code.push_back(AssemblyInstruction(Instruction::SUB, 0));
    code.push_back(AssemblyInstruction(Instruction::SUB, 4));
    code.push_back(AssemblyInstruction(Instruction::STORE, 1));

    // Store y in p[5] to be able to later determine the sign of the result. If y < 0 then we negate it here. If y == 0 then jump to the end
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Negate y if needed"));
    code.push_back(AssemblyInstruction(Instruction::LOAD, 2));
    code.push_back(AssemblyInstruction(Instruction::STORE, 5));
    code.push_back(AssemblyInstruction(Instruction::JZERO, 47));
    code.push_back(AssemblyInstruction(Instruction::JPOS, 4));
    code.push_back(AssemblyInstruction(Instruction::SUB, 0));
    code.push_back(AssemblyInstruction(Instruction::SUB, 5));
    code.push_back(AssemblyInstruction(Instruction::STORE, 2));

    // If y > x swap them to make the algorithm more efficient as it is O(log(y))
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Swap x and y if needed"));
    code.push_back(AssemblyInstruction(Instruction::LOAD, 1));
    code.push_back(AssemblyInstruction(Instruction::SUB, 2));
    code.push_back(AssemblyInstruction(Instruction::JPOS, 8));
    code.push_back(AssemblyInstruction(Instruction::JZERO, 7));
    code.push_back(AssemblyInstruction(Instruction::LOAD, 1));
    code.push_back(AssemblyInstruction(Instruction::STORE, 3));
    code.push_back(AssemblyInstruction(Instruction::LOAD, 2));
    code.push_back(AssemblyInstruction(Instruction::STORE, 1));
    code.push_back(AssemblyInstruction(Instruction::LOAD, 3));
    code.push_back(AssemblyInstruction(Instruction::STORE, 2));

    // Result is in p[3], set it to 0
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Set result to 0"));
    code.push_back(AssemblyInstruction(Instruction::SUB, 0));
    code.push_back(AssemblyInstruction(Instruction::STORE, 3));
    code.push_back(AssemblyInstruction(Instruction::LOAD, 2));

    // Start the while loop (WHILE y > 0)
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Start the loop"));
    code.push_back(AssemblyInstruction(Instruction::JZERO, 16));
    code.push_back(AssemblyInstruction(Instruction::JNEG, 15));

    // Compute y % 2
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Compute y % 2"));
    code.push_back(AssemblyInstruction(Instruction::STORE, 2));
    code.push_back(AssemblyInstruction(Instruction::HALF, 0));
    code.push_back(AssemblyInstruction(Instruction::ADD, 0));
    code.push_back(AssemblyInstruction(Instruction::SUB, 2));

    // If y % 2 != 0 then add y to the result
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "If y % 2 != 0 then add x to the result"));
    code.push_back(AssemblyInstruction(Instruction::JZERO, 4));
    code.push_back(AssemblyInstruction(Instruction::LOAD, 3));
    code.push_back(AssemblyInstruction(Instruction::ADD, 1));
    code.push_back(AssemblyInstruction(Instruction::STORE, 3));

    // Double x and half y
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Double x and half y"));
    code.push_back(AssemblyInstruction(Instruction::LOAD, 1));
    code.push_back(AssemblyInstruction(Instruction::ADD, 1));
    code.push_back(AssemblyInstruction(Instruction::STORE, 1));
    code.push_back(AssemblyInstruction(Instruction::LOAD, 2));
    code.push_back(AssemblyInstruction(Instruction::HALF, 0));

    // Continue the loop
    code.push_back(AssemblyInstruction(Instruction::JUMP, -15));

    // If x < 0 then negate the result
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Negate the result if x < 0"));
    code.push_back(AssemblyInstruction(Instruction::LOAD, 4));
    code.push_back(AssemblyInstruction(Instruction::JPOS, 4));
    code.push_back(AssemblyInstruction(Instruction::SUB, 0));
    code.push_back(AssemblyInstruction(Instruction::SUB, 3));
    code.push_back(AssemblyInstruction(Instruction::STORE, 3));

    // If y < 0 then negate the result
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Negate the result if y < 0"));
    code.push_back(AssemblyInstruction(Instruction::LOAD, 5));
    code.push_back(AssemblyInstruction(Instruction::JPOS, 4));
    code.push_back(AssemblyInstruction(Instruction::SUB, 0));
    code.push_back(AssemblyInstruction(Instruction::SUB, 3));
    code.push_back(AssemblyInstruction(Instruction::JUMP, 2));
    code.push_back(AssemblyInstruction(Instruction::LOAD, 3));

    // Return
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Return"));
    code.push_back(AssemblyInstruction(Instruction::JPOS, 3));
    code.push_back(AssemblyInstruction(Instruction::JNEG, 2));
    code.push_back(AssemblyInstruction(Instruction::SUB, 0));
    code.push_back(AssemblyInstruction(Instruction::RTRN, MEMORY_RETURN_MULTIPLICATION));
    code.push_back(AssemblyInstruction(Instruction::LABEL_ENDPROCEDURE, "*\n"));

    return code;
}


std::vector<AssemblyInstruction> generateDivision(){


    return {};
}


std::vector<AssemblyInstruction> generateModulo(){


    return {};
}