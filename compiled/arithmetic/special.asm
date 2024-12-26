# Preprocessing...
# Store const = 1 at p[69]
SET 1
STORE 69
# Finished preprocessing

# Jump to MAIN procedure
JUMP 142
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
# ENDPROCEDURE *$*

# MAIN 
# t := 100
SET 100
STORE 1001
# s := 0
SUB 0
STORE 1002
# u := t / s
LOAD 1002
STORE 73
LOAD 1001
STORE 72
SUB 0
STORE 75
# Setting return address and jumping to /@\
SET 157
STORE 161
JUMP -153
STORE 1003
# v := t % s
LOAD 1002
STORE 73
LOAD 1001
STORE 72
LOAD 69
STORE 75
# Setting return address and jumping to /@\
SET 167
STORE 161
JUMP -163
STORE 1004
# w := t * s
LOAD 1002
STORE 12
LOAD 1001
STORE 11
# Setting return address and jumping to *$*
SET 175
STORE 256
JUMP -88
STORE 1005
# p := t + s
LOAD 1002
ADD 1001
STORE 1006
# q := t - s
LOAD 1001
SUB 1002
STORE 1007
# WRITE u
PUT 1003
# WRITE v
PUT 1004
# WRITE w
PUT 1005
# WRITE p
PUT 1006
# WRITE q
PUT 1007
HALT
