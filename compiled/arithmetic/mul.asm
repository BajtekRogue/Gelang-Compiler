# Jump to MAIN procedure
JUMP 59
# 

# PROCEDURE *$*
# Negate x if needed
LOAD 11
STORE 20
JZERO 54
JPOS 4
SUB 0
SUB 20
STORE 11
# Negate y if needed
LOAD 12
STORE 40
JZERO 47
JPOS 4
SUB 0
SUB 40
STORE 12
# Swap x and y if needed
LOAD 11
SUB 12
JPOS 8
JZERO 7
LOAD 11
STORE 80
LOAD 12
STORE 11
LOAD 80
STORE 12
# Set result to 0
SUB 0
STORE 80
LOAD 12
# Start the loop
JZERO 16
JNEG 15
# Compute y % 2
STORE 12
HALF
ADD 0
SUB 12
# If y % 2 != 0 then add x to the result
JZERO 4
LOAD 80
ADD 11
STORE 80
# Double x and half y
LOAD 11
ADD 11
STORE 11
LOAD 12
HALF
JUMP -15
# Negate the result if x < 0
LOAD 20
JPOS 4
SUB 0
SUB 80
STORE 80
# Negate the result if y < 0
LOAD 40
JPOS 4
SUB 0
SUB 80
JUMP 2
LOAD 80
# Return
JPOS 3
JNEG 2
SUB 0
RTRN 256
# ENDPROCEDURE $

# MAIN 
# x := 56789
SET 56789
STORE 1001
# y := 1234
SET 1234
STORE 1002
# x_ := -56789
SET -56789
STORE 1004
# y_ := -1234
SET -1234
STORE 1005
# z := x * y
LOAD 1002
STORE 12
LOAD 1001
STORE 11
# Setting return address and jumping to *$*
SET 74
STORE 256
JUMP -72
STORE 1003
# WRITE z
PUT 1003
# z := x_ * y
LOAD 1002
STORE 12
LOAD 1004
STORE 11
# Setting return address and jumping to *$*
SET 83
STORE 256
JUMP -81
STORE 1003
# WRITE z
PUT 1003
# z := x * y_
LOAD 1005
STORE 12
LOAD 1001
STORE 11
# Setting return address and jumping to *$*
SET 92
STORE 256
JUMP -90
STORE 1003
# WRITE z
PUT 1003
# z := x_ * y_
LOAD 1005
STORE 12
LOAD 1004
STORE 11
# Setting return address and jumping to *$*
SET 101
STORE 256
JUMP -99
STORE 1003
# WRITE z
PUT 1003
HALT
