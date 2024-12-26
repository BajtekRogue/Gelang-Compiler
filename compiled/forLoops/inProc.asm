# Preprocessing...
# Store const = 5 at p[301]
SET 5
STORE 301
# Store const = 78 at p[302]
SET 78
STORE 302
# Store const = 1 at p[69]
SET 1
STORE 69
# Finished preprocessing

# Jump to MAIN procedure
JUMP 100
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

# PROCEDURE plus_i_minus
# Initializing FOR_UP _1_: 
SET 100
STORE 1006
LOAD 69
STORE 1005
# FOR_UP _1_: 1 TO 100
SUB 1006
JPOS 35
# u := u + i
LOAD 1005
ADDI 1001
STOREI 1001
# v := v - i
LOADI 1002
SUB 1005
STOREI 1002
# s := i * i
LOAD 1005
STORE 2
LOAD 1005
STORE 1
# Setting return address and jumping to *
SET 84
STORE 56
JUMP -76
STORE 1004
# w[5] := i + w[5]
LOAD 301
ADD 1003
STORE 28
LOAD 301
ADD 1003
LOADI 0
ADD 1005
STOREI 28
# w[5] := w[5] + s
LOAD 301
ADD 1003
STORE 28
LOAD 301
ADD 1003
LOADI 0
ADD 1004
STOREI 28
# i++
LOAD 1005
ADD 69
STORE 1005
JUMP -35
# ENDFOR _1_.
# RETURN plus_i_minus
RTRN 1007
# ENDPROCEDURE plus_i_minus

# MAIN 
# x := 456
SET 456
STORE 2001
# y := 123
SET 123
STORE 2002
# z[5] := 789
SET 789
STORE 2008
# CALL plus_i_minus (x, y, z)
SET 2001
STORE 1001
SET 2002
STORE 1002
SET 2003
STORE 1003
# Setting return address and jumping to plus_i_minus
SET 121
STORE 1007
JUMP -55
# WRITE x
PUT 2001
# WRITE y
PUT 2002
# WRITE z[5]
PUT 2008
HALT
