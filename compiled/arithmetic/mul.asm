# Jump to MAIN procedure
JUMP 59
# 

# PROCEDURE *
# Negate x if needed
LOAD 1
STORE 4
JZERO 54
JPOS 4
SUB 0
SUB 4
STORE 1
# Negate y if needed
LOAD 2
STORE 5
JZERO 47
JPOS 4
SUB 0
SUB 5
STORE 2
# Swap x and y if needed
LOAD 1
SUB 2
JPOS 8
JZERO 7
LOAD 1
STORE 3
LOAD 2
STORE 1
LOAD 3
STORE 2
# Set result to 0
SUB 0
STORE 3
LOAD 2
# Start the loop
JZERO 16
JNEG 15
# Compute y % 2
STORE 2
HALF
ADD 0
SUB 2
# If y % 2 != 0 then add x to the result
JZERO 4
LOAD 3
ADD 1
STORE 3
# Double x and half y
LOAD 1
ADD 1
STORE 1
LOAD 2
HALF
JUMP -15
# Negate the result if x < 0
LOAD 4
JPOS 4
SUB 0
SUB 3
STORE 3
# Negate the result if y < 0
LOAD 5
JPOS 4
SUB 0
SUB 3
JUMP 2
LOAD 3
# Return
JPOS 3
JNEG 2
SUB 0
RTRN 56
# ENDPROCEDURE *

# MAIN 
# x := 56789
SET 56789
STORE 1001
# y := 1234
SET 1234
STORE 1002
# z := x * y
LOAD 1002
STORE 2
LOAD 1001
STORE 1
# Setting return address and jumping to *
SET 70
STORE 56
JUMP -68
STORE 1003
# WRITE z
PUT 1003
# x := 0 - x
SUB 0
SUB 1001
STORE 1001
# z := x * y
LOAD 1002
STORE 2
LOAD 1001
STORE 1
# Setting return address and jumping to *
SET 82
STORE 56
JUMP -80
STORE 1003
# WRITE z
PUT 1003
# y := 0 - y
SUB 0
SUB 1002
STORE 1002
# z := x * y
LOAD 1002
STORE 2
LOAD 1001
STORE 1
# Setting return address and jumping to *
SET 94
STORE 56
JUMP -92
STORE 1003
# WRITE z
PUT 1003
# x := 0 - x
SUB 0
SUB 1001
STORE 1001
# z := x * y
LOAD 1002
STORE 2
LOAD 1001
STORE 1
# Setting return address and jumping to *
SET 106
STORE 56
JUMP -104
STORE 1003
# WRITE z
PUT 1003
HALT
