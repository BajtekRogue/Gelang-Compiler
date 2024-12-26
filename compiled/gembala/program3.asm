# Preprocessing...
# Store const = 220 at p[301]
SET 220
STORE 301
# Store const = 2001 at p[302]
SET 2001
STORE 302
# Store const = 2004 at p[303]
SET 2004
STORE 303
# Store const = 2003 at p[304]
SET 2003
STORE 304
# Store const = 1 at p[69]
SET 1
STORE 69
# Store const = 205 at p[306]
SET 205
STORE 306
# Store const = 179 at p[307]
SET 179
STORE 307
# Store const = 166 at p[308]
SET 166
STORE 308
# Finished preprocessing

# Jump to MAIN procedure
JUMP 182
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

# PROCEDURE check
# p := 0
SUB 0
STOREI 1003
# r := n % d
LOADI 1002
STORE 73
LOADI 1001
STORE 72
LOAD 69
STORE 75
# Setting return address and jumping to /@\
SET 169
STORE 161
JUMP -151
STORE 1004
# WHILE _1_: r = 0
LOAD 1004
JPOS 26
JNEG 25
# n := n / d
LOADI 1002
STORE 73
LOADI 1001
STORE 72
SUB 0
STORE 75
# Setting return address and jumping to /@\
SET 182
STORE 161
JUMP -164
STOREI 1001
# p := p + 1
LOAD 69
ADDI 1003
STOREI 1003
# r := n % d
LOADI 1002
STORE 73
LOADI 1001
STORE 72
LOAD 69
STORE 75
# Setting return address and jumping to /@\
SET 195
STORE 161
JUMP -177
STORE 1004
JUMP -26
# ENDWHILE _1_: 
# RETURN check
RTRN 1005
# ENDPROCEDURE check

# MAIN 
# READ n
GET 2001
# dzielnik := 2
SET 2
STORE 2004
# m := dzielnik * dzielnik
LOAD 2004
STORE 12
LOAD 2004
STORE 11
# Setting return address and jumping to *$*
SET 208
STORE 256
JUMP -107
STORE 2002
# WHILE _2_: n >= m
LOAD 2001
SUB 2002
JNEG 27
# CALL check (n, dzielnik, potega)
LOAD 302
STORE 1001
LOAD 303
STORE 1002
LOAD 304
STORE 1003
# Setting return address and jumping to check
SET 221
STORE 1005
JUMP -62
# IF _1_: potega > 0
LOAD 2003
JNEG 4
JZERO 3
# WRITE dzielnik
PUT 2004
# WRITE potega
PUT 2003
# ENDIF _1_.
# dzielnik := dzielnik + 1
LOAD 69
ADD 2004
STORE 2004
# m := dzielnik * dzielnik
LOAD 2004
STORE 12
LOAD 2004
STORE 11
# Setting return address and jumping to *$*
SET 236
STORE 256
JUMP -135
STORE 2002
JUMP -28
# ENDWHILE _2_: 
# IF _2_: n != 1
LOAD 69
SUB 2001
JZERO 4
# WRITE n
PUT 2001
# WRITE 1
LOAD 69
PUT 0
# ENDIF _2_.
HALT
