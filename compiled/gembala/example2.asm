# Preprocessing...
# Store const = 44 at p[301]
SET 44
STORE 301
# Store const = 37 at p[302]
SET 37
STORE 302
# Store const = 30 at p[303]
SET 30
STORE 303
# Store const = 22 at p[304]
SET 22
STORE 304
# Store const = 15 at p[305]
SET 15
STORE 305
# Finished preprocessing

# Jump to MAIN procedure
JUMP 74
# 

# PROCEDURE pa
# a := a + b
LOADI 1002
ADDI 1001
STOREI 1001
# b := a - b
LOADI 1001
SUBI 1002
STOREI 1002
# RETURN pa
RTRN 1003
# ENDPROCEDURE pa

# PROCEDURE pb
# CALL pa (a, b)
LOAD 2001
STORE 1001
LOAD 2002
STORE 1002
# Setting return address and jumping to pa
SET 25
STORE 1003
JUMP -13
# CALL pa (a, b)
LOAD 2001
STORE 1001
LOAD 2002
STORE 1002
# Setting return address and jumping to pa
SET 32
STORE 1003
JUMP -20
# RETURN pb
RTRN 2003
# ENDPROCEDURE pb

# PROCEDURE pc
# CALL pb (a, b)
LOAD 3001
STORE 2001
LOAD 3002
STORE 2002
# Setting return address and jumping to pb
SET 40
STORE 2003
JUMP -21
# CALL pb (a, b)
LOAD 3001
STORE 2001
LOAD 3002
STORE 2002
# Setting return address and jumping to pb
SET 47
STORE 2003
JUMP -28
# CALL pb (a, b)
LOAD 3001
STORE 2001
LOAD 3002
STORE 2002
# Setting return address and jumping to pb
SET 54
STORE 2003
JUMP -35
# RETURN pc
RTRN 3003
# ENDPROCEDURE pc

# PROCEDURE pd
# CALL pc (a, b)
LOAD 4001
STORE 3001
LOAD 4002
STORE 3002
# Setting return address and jumping to pc
SET 62
STORE 3003
JUMP -28
# CALL pc (a, b)
LOAD 4001
STORE 3001
LOAD 4002
STORE 3002
# Setting return address and jumping to pc
SET 69
STORE 3003
JUMP -35
# CALL pc (a, b)
LOAD 4001
STORE 3001
LOAD 4002
STORE 3002
# Setting return address and jumping to pc
SET 76
STORE 3003
JUMP -42
# CALL pc (a, b)
LOAD 4001
STORE 3001
LOAD 4002
STORE 3002
# Setting return address and jumping to pc
SET 83
STORE 3003
JUMP -49
# RETURN pd
RTRN 4003
# ENDPROCEDURE pd

# MAIN 
# READ a
GET 5001
# READ b
GET 5002
# CALL pd (a, b)
SET 5001
STORE 4001
SET 5002
STORE 4002
# Setting return address and jumping to pd
SET 93
STORE 4003
JUMP -37
# WRITE a
PUT 5001
# WRITE b
PUT 5002
HALT
