#include "assembling.hpp"
#include "utility.hpp"


std::vector<AssemblyInstruction> generateMultiplication(){

    std::vector<AssemblyInstruction> code;
    
    const int64_t X_ADDRESS = 20;
    const int64_t Y_ADDRESS = 40;
    const int64_t PRODUCT_ADDRESS = 80;

    code.push_back(AssemblyInstruction(Instruction::LABEL_PROCEDURE, Arithmetic::multiplicationProcedureName));

    // Store x in p[X_ADDRESS] to be able to later determine the sign of the result. If x < 0 then we negate it here. If x == 0 then jump to the end
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Negate x if needed"));
    code.push_back(AssemblyInstruction(Instruction::LOAD, Memory::multiplicationLeft));
    code.push_back(AssemblyInstruction(Instruction::STORE, X_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::JZERO, 54));
    code.push_back(AssemblyInstruction(Instruction::JPOS, 4));
    code.push_back(AssemblyInstruction(Instruction::SUB, 0));
    code.push_back(AssemblyInstruction(Instruction::SUB, X_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::STORE, Memory::multiplicationLeft));

    // Store y in p[Y_ADDRESS] to be able to later determine the sign of the result. If y < 0 then we negate it here. If y == 0 then jump to the end
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Negate y if needed"));
    code.push_back(AssemblyInstruction(Instruction::LOAD, Memory::multiplicationRight));
    code.push_back(AssemblyInstruction(Instruction::STORE, Y_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::JZERO, 47));
    code.push_back(AssemblyInstruction(Instruction::JPOS, 4));
    code.push_back(AssemblyInstruction(Instruction::SUB, 0));
    code.push_back(AssemblyInstruction(Instruction::SUB, Y_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::STORE, Memory::multiplicationRight));

    // If y > x swap them to make the algorithm more efficient as it is O(log(y))
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Swap x and y if needed"));
    code.push_back(AssemblyInstruction(Instruction::LOAD, Memory::multiplicationLeft));
    code.push_back(AssemblyInstruction(Instruction::SUB, Memory::multiplicationRight));
    code.push_back(AssemblyInstruction(Instruction::JPOS, 8));
    code.push_back(AssemblyInstruction(Instruction::JZERO, 7));
    code.push_back(AssemblyInstruction(Instruction::LOAD, Memory::multiplicationLeft));
    code.push_back(AssemblyInstruction(Instruction::STORE, PRODUCT_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::LOAD, Memory::multiplicationRight));
    code.push_back(AssemblyInstruction(Instruction::STORE, Memory::multiplicationLeft));
    code.push_back(AssemblyInstruction(Instruction::LOAD, PRODUCT_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::STORE, Memory::multiplicationRight));

    // Result is in p[PRODUCT_ADDRESS], set it to 0
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Set result to 0"));
    code.push_back(AssemblyInstruction(Instruction::SUB, 0));
    code.push_back(AssemblyInstruction(Instruction::STORE, PRODUCT_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::LOAD, Memory::multiplicationRight));

    // Start the while loop (WHILE y > 0)
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Start the loop"));
    code.push_back(AssemblyInstruction(Instruction::JZERO, 16));
    code.push_back(AssemblyInstruction(Instruction::JNEG, 15));

    // Compute y % 2
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Compute y % 2"));
    code.push_back(AssemblyInstruction(Instruction::STORE, Memory::multiplicationRight));
    code.push_back(AssemblyInstruction(Instruction::HALF, 0));
    code.push_back(AssemblyInstruction(Instruction::ADD, 0));
    code.push_back(AssemblyInstruction(Instruction::SUB, Memory::multiplicationRight));

    // If y % 2 != 0 then add y to the result
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "If y % 2 != 0 then add x to the result"));
    code.push_back(AssemblyInstruction(Instruction::JZERO, 4));
    code.push_back(AssemblyInstruction(Instruction::LOAD, PRODUCT_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::ADD, Memory::multiplicationLeft));
    code.push_back(AssemblyInstruction(Instruction::STORE, PRODUCT_ADDRESS));

    // Double x and half y
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Double x and half y"));
    code.push_back(AssemblyInstruction(Instruction::LOAD, Memory::multiplicationLeft));
    code.push_back(AssemblyInstruction(Instruction::ADD, Memory::multiplicationLeft));
    code.push_back(AssemblyInstruction(Instruction::STORE, Memory::multiplicationLeft));
    code.push_back(AssemblyInstruction(Instruction::LOAD, Memory::multiplicationRight));
    code.push_back(AssemblyInstruction(Instruction::HALF, 0));

    // Continue the loop
    code.push_back(AssemblyInstruction(Instruction::JUMP, -15));

    // If x < 0 then negate the result
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Negate the result if x < 0"));
    code.push_back(AssemblyInstruction(Instruction::LOAD, X_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::JPOS, 4));
    code.push_back(AssemblyInstruction(Instruction::SUB, 0));
    code.push_back(AssemblyInstruction(Instruction::SUB, PRODUCT_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::STORE, PRODUCT_ADDRESS));

    // If y < 0 then negate the result
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Negate the result if y < 0"));
    code.push_back(AssemblyInstruction(Instruction::LOAD, Y_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::JPOS, 4));
    code.push_back(AssemblyInstruction(Instruction::SUB, 0));
    code.push_back(AssemblyInstruction(Instruction::SUB, PRODUCT_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::JUMP, 2));
    code.push_back(AssemblyInstruction(Instruction::LOAD, PRODUCT_ADDRESS));

    // Return
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Return"));
    code.push_back(AssemblyInstruction(Instruction::JPOS, 3));
    code.push_back(AssemblyInstruction(Instruction::JNEG, 2));
    code.push_back(AssemblyInstruction(Instruction::SUB, 0));
    code.push_back(AssemblyInstruction(Instruction::RTRN, Memory::returnMultiplication));
    code.push_back(AssemblyInstruction(Instruction::LABEL_ENDPROCEDURE, Arithmetic::multiplicationProcedureName + "\n"));

    return code;
}


std::vector<AssemblyInstruction> generateDivision(){

    std::vector<AssemblyInstruction> code;

    const int64_t X_ADDRESS = 30;
    const int64_t Y_ADDRESS = 60;
    const int64_t QUOTIENT_ADDRESS = 90;
    const int64_t REMAINDER_ADDRESS = 120;
    const int64_t CURRENT_ADDRESS = 150;
    const int64_t POWER_ADDRESS = 180;
    const int64_t TEMP_ADDRESS = 210;

    code.push_back(AssemblyInstruction(Instruction::LABEL_PROCEDURE, Arithmetic::divisionProcedureName));

    // Store x in p[X_ADDRESS] to be able to later determine the sign of the result. If x < 0 then we negate it here. If x == 0 then jump to the end
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Negate x if needed"));
    code.push_back(AssemblyInstruction(Instruction::LOAD, Memory::divisionLeft));
    code.push_back(AssemblyInstruction(Instruction::STORE, X_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::JZERO, 80));
    code.push_back(AssemblyInstruction(Instruction::JPOS, 4));
    code.push_back(AssemblyInstruction(Instruction::SUB, 0));
    code.push_back(AssemblyInstruction(Instruction::SUB, X_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::STORE, Memory::divisionLeft));

    // Store y in p[Y_ADDRESS] to be able to later determine the sign of the result. If y < 0 then we negate it here. If y == 0 then jump to the end
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Negate y if needed"));
    code.push_back(AssemblyInstruction(Instruction::LOAD, Memory::divisionRight));
    code.push_back(AssemblyInstruction(Instruction::STORE, Y_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::JZERO, 73));
    code.push_back(AssemblyInstruction(Instruction::JPOS, 4));
    code.push_back(AssemblyInstruction(Instruction::SUB, 0));
    code.push_back(AssemblyInstruction(Instruction::SUB, Y_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::STORE, Memory::divisionRight));

    // Set quotient to 0 and remainder to x
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Set quotient to 0, remainder to x, current to y and powet to 1"));
    code.push_back(AssemblyInstruction(Instruction::LOAD, Memory::divisionLeft));
    code.push_back(AssemblyInstruction(Instruction::STORE, REMAINDER_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::SUB, 0));
    code.push_back(AssemblyInstruction(Instruction::STORE, QUOTIENT_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::LOAD, Memory::divisionRight));
    code.push_back(AssemblyInstruction(Instruction::STORE, CURRENT_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::LOAD, Memory::one));
    code.push_back(AssemblyInstruction(Instruction::STORE, POWER_ADDRESS));

    // Shift y left until just before it exceeds x
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Shift y left until just before it exceeds x"));
    code.push_back(AssemblyInstruction(Instruction::LOAD, REMAINDER_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::HALF, 0));
    code.push_back(AssemblyInstruction(Instruction::STORE, TEMP_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::LOAD, CURRENT_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::SUB, TEMP_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::JPOS, 8));
    code.push_back(AssemblyInstruction(Instruction::LOAD, CURRENT_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::ADD, 0));
    code.push_back(AssemblyInstruction(Instruction::STORE, CURRENT_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::LOAD, POWER_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::ADD, 0));
    code.push_back(AssemblyInstruction(Instruction::STORE, POWER_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::JUMP, -9)); 

    // Now shift back down, subtracting when possible
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "While power > 0"));
    code.push_back(AssemblyInstruction(Instruction::LOAD, POWER_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::JZERO, 15));

    // If remainder >= current then subtract current from remainder and add power to quotient
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "If remainder >= current"));
    code.push_back(AssemblyInstruction(Instruction::LOAD, REMAINDER_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::SUB, CURRENT_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::JNEG, 5));
    code.push_back(AssemblyInstruction(Instruction::STORE, REMAINDER_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::LOAD, QUOTIENT_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::ADD, POWER_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::STORE, QUOTIENT_ADDRESS));

    // Left shift current and power
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Left shift current and power"));
    code.push_back(AssemblyInstruction(Instruction::LOAD, CURRENT_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::HALF, 0));
    code.push_back(AssemblyInstruction(Instruction::STORE, CURRENT_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::LOAD, POWER_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::HALF, 0));
    code.push_back(AssemblyInstruction(Instruction::STORE, POWER_ADDRESS));

    // Continue the loop
    code.push_back(AssemblyInstruction(Instruction::JUMP, -15));

    // If x < 0 then check y
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "If x < 0 then check y"));
    code.push_back(AssemblyInstruction(Instruction::LOAD, X_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::JPOS, 14));

    // If y < 0 then do:  r = -r
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Negate the result if y < 0"));
    code.push_back(AssemblyInstruction(Instruction::LOAD, Y_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::JPOS, 5));
    code.push_back(AssemblyInstruction(Instruction::SUB, 0));
    code.push_back(AssemblyInstruction(Instruction::SUB, REMAINDER_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::STORE, REMAINDER_ADDRESS)); 
    code.push_back(AssemblyInstruction(Instruction::JUMP, 17));

    // Else (y > 0) do: q = -1 - q, r = y - r
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Shft the result if y > 0"));
    code.push_back(AssemblyInstruction(Instruction::SUB, 0));
    code.push_back(AssemblyInstruction(Instruction::SUB, QUOTIENT_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::SUB, Memory::one));
    code.push_back(AssemblyInstruction(Instruction::STORE, QUOTIENT_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::LOAD, Memory::divisionRight));
    code.push_back(AssemblyInstruction(Instruction::SUB, REMAINDER_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::STORE, REMAINDER_ADDRESS));

    // But if x > 0 and y < 0 do: q = -1 - q, r = r - y
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Shft the result if y < 0"));
    code.push_back(AssemblyInstruction(Instruction::LOAD, Y_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::JPOS, 8));
    code.push_back(AssemblyInstruction(Instruction::SUB, 0));
    code.push_back(AssemblyInstruction(Instruction::SUB, QUOTIENT_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::SUB, Memory::one));
    code.push_back(AssemblyInstruction(Instruction::STORE, QUOTIENT_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::LOAD, REMAINDER_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::SUB, Memory::divisionRight));
    code.push_back(AssemblyInstruction(Instruction::STORE, REMAINDER_ADDRESS));

    // If p[Memory::divisionFlag] == 0 then return the quotient else return the remainder
    code.push_back(AssemblyInstruction(Instruction::LABEL_INSTRUCTION, "Check what to return"));
    code.push_back(AssemblyInstruction(Instruction::LOAD, Memory::divisionFlag));
    code.push_back(AssemblyInstruction(Instruction::JZERO, 3));
    code.push_back(AssemblyInstruction(Instruction::LOAD, REMAINDER_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::JUMP, 2));
    code.push_back(AssemblyInstruction(Instruction::LOAD, QUOTIENT_ADDRESS));
    code.push_back(AssemblyInstruction(Instruction::JUMP, 2));
    code.push_back(AssemblyInstruction(Instruction::SUB, 0));
    code.push_back(AssemblyInstruction(Instruction::RTRN, Memory::returnDivision));
    code.push_back(AssemblyInstruction(Instruction::LABEL_ENDPROCEDURE, Arithmetic::divisionProcedureName + "\n"));

    return code;
}
