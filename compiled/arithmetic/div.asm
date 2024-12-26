# Preprocessing...
# Store const = 1 at p[69]
SET 1
STORE 69
# Finished preprocessing

# Jump to MAIN procedure
JUMP 84
# 

# PROCEDURE /@\
# Negate x if needed
LOAD 72
STORE 30
JZERO 80
JPOS 4
SUB 0
SUB 30
STORE 72
# Negate y if needed
LOAD 73
STORE 60
JZERO 73
JPOS 4
SUB 0
SUB 60
STORE 73
# Set quotient to 0, remainder to x, current to y and powet to 1
LOAD 72
STORE 120
SUB 0
STORE 90
LOAD 73
STORE 150
LOAD 69
STORE 180
# Shift y left until just before it exceeds x
LOAD 120
HALF
STORE 210
LOAD 150
SUB 210
JPOS 8
LOAD 150
ADD 0
STORE 150
LOAD 180
ADD 0
STORE 180
JUMP -9
# While power > 0
LOAD 180
JZERO 15
# If remainder >= current
LOAD 120
SUB 150
JNEG 5
STORE 120
LOAD 90
ADD 180
STORE 90
# Left shift current and power
LOAD 150
HALF
STORE 150
LOAD 180
HALF
STORE 180
JUMP -15
# If x < 0 then check y
LOAD 30
JPOS 14
# Negate the result if y < 0
LOAD 60
JPOS 5
SUB 0
SUB 120
STORE 120
JUMP 17
# Shft the result if y > 0
SUB 0
SUB 90
SUB 69
STORE 90
LOAD 73
SUB 120
STORE 120
# Shft the result if y < 0
LOAD 60
JPOS 8
SUB 0
SUB 90
SUB 69
STORE 90
LOAD 120
SUB 73
STORE 120
# Check what to return
LOAD 75
JZERO 3
LOAD 120
JUMP 2
LOAD 90
JUMP 2
SUB 0
RTRN 161
# ENDPROCEDURE /@\

# MAIN 
# a := 153467
SET 153467
STORE 1001
# b := 72
SET 72
STORE 1002
# a_ := -153467
SET -153467
STORE 1005
# b_ := -72
SET -72
STORE 1006
# c := a / b
LOAD 1002
STORE 73
LOAD 1001
STORE 72
SUB 0
STORE 75
# Setting return address and jumping to /@\
SET 103
STORE 161
JUMP -99
STORE 1003
# d := a % b
LOAD 1002
STORE 73
LOAD 1001
STORE 72
LOAD 69
STORE 75
# Setting return address and jumping to /@\
SET 113
STORE 161
JUMP -109
STORE 1004
# WRITE c
PUT 1003
# WRITE d
PUT 1004
# c := a_ / b
LOAD 1002
STORE 73
LOAD 1005
STORE 72
SUB 0
STORE 75
# Setting return address and jumping to /@\
SET 125
STORE 161
JUMP -121
STORE 1003
# d := a_ % b
LOAD 1002
STORE 73
LOAD 1005
STORE 72
LOAD 69
STORE 75
# Setting return address and jumping to /@\
SET 135
STORE 161
JUMP -131
STORE 1004
# WRITE c
PUT 1003
# WRITE d
PUT 1004
# c := a / b_
LOAD 1006
STORE 73
LOAD 1001
STORE 72
SUB 0
STORE 75
# Setting return address and jumping to /@\
SET 147
STORE 161
JUMP -143
STORE 1003
# d := a % b_
LOAD 1006
STORE 73
LOAD 1001
STORE 72
LOAD 69
STORE 75
# Setting return address and jumping to /@\
SET 157
STORE 161
JUMP -153
STORE 1004
# WRITE c
PUT 1003
# WRITE d
PUT 1004
# c := a_ / b_
LOAD 1006
STORE 73
LOAD 1005
STORE 72
SUB 0
STORE 75
# Setting return address and jumping to /@\
SET 169
STORE 161
JUMP -165
STORE 1003
# d := a_ % b_
LOAD 1006
STORE 73
LOAD 1005
STORE 72
LOAD 69
STORE 75
# Setting return address and jumping to /@\
SET 179
STORE 161
JUMP -175
STORE 1004
# WRITE c
PUT 1003
# WRITE d
PUT 1004
HALT
