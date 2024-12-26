# Preprocessing...
# Store const = 112 at p[301]
SET 112
STORE 301
# Store const = 98 at p[302]
SET 98
STORE 302
# Store const = 84 at p[303]
SET 84
STORE 303
# Store const = 145 at p[304]
SET 145
STORE 304
# Store const = 80 at p[305]
SET 80
STORE 305
# Store const = 131 at p[306]
SET 131
STORE 306
# Store const = 1 at p[307]
SET 1
STORE 307
# Finished preprocessing

# Jump to MAIN procedure
JUMP 163
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

# PROCEDURE de
# a := m
LOADI 1001
STORE 1006
# b := n
LOADI 1002
STORE 1007
# x := 1
LOAD 307
STOREI 1003
# y := 0
SUB 0
STOREI 1004
# r := n
LOADI 1002
STORE 1008
# s := m - 1
LOAD 307
STORE 1
LOADI 1001
SUB 1
STORE 1009
# WHILE _1_: b > 0
LOAD 1007
JNEG 85
JZERO 84
# reszta := a % b
# Setting return address and jumping to %
SET 94
STORE 35
JUMP -80
STORE 1010
# iloraz := a / b
# Setting return address and jumping to /
SET 98
STORE 47
JUMP -84
STORE 1011
# a := b
LOAD 1007
STORE 1006
# b := reszta
LOAD 1010
STORE 1007
# rr := r
LOAD 1008
STORE 1012
# tmp := iloraz * r
LOAD 1008
STORE 2
LOAD 1011
STORE 1
# Setting return address and jumping to *
SET 112
STORE 56
JUMP -96
STORE 1014
# IF _1_: x < tmp
LOAD 1014
STORE 1
LOADI 1003
SUB 1
JPOS 11
JZERO 10
# r := n * iloraz
LOAD 1011
STORE 2
LOADI 1002
STORE 1
# Setting return address and jumping to *
SET 126
STORE 56
JUMP -110
STORE 1008
JUMP 3
# ELSE _1_: 
# r := 0
SUB 0
STORE 1008
# ENDIF _1_.
# r := r + x
LOADI 1003
ADD 1008
STORE 1008
# r := r - tmp
LOAD 1008
SUB 1014
STORE 1008
# ss := s
LOAD 1009
STORE 1013
# tmp := iloraz * s
LOAD 1009
STORE 2
LOAD 1011
STORE 1
# Setting return address and jumping to *
SET 145
STORE 56
JUMP -129
STORE 1014
# IF _2_: y < tmp
LOAD 1014
STORE 1
LOADI 1004
SUB 1
JPOS 11
JZERO 10
# s := m * iloraz
LOAD 1011
STORE 2
LOADI 1001
STORE 1
# Setting return address and jumping to *
SET 159
STORE 56
JUMP -143
STORE 1009
JUMP 3
# ELSE _2_: 
# s := 0
SUB 0
STORE 1009
# ENDIF _2_.
# s := s + y
LOADI 1004
ADD 1009
STORE 1009
# s := s - tmp
LOAD 1009
SUB 1014
STORE 1009
# x := rr
LOAD 1012
STOREI 1003
# y := ss
LOAD 1013
STOREI 1004
JUMP -85
# ENDWHILE _1_: 
# z := a
LOAD 1006
STOREI 1005
# RETURN de
RTRN 1015
# ENDPROCEDURE de

# MAIN 
# READ m
GET 2001
# READ n
GET 2002
# CALL de (m, n, x, y, nwd)
SET 2001
STORE 1001
SET 2002
STORE 1002
SET 2003
STORE 1003
SET 2004
STORE 1004
SET 2005
STORE 1005
# Setting return address and jumping to de
SET 192
STORE 1015
JUMP -118
# WRITE x
PUT 2003
# WRITE y
PUT 2004
# WRITE nwd
PUT 2005
HALT
