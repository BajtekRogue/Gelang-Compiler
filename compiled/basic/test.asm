# Jump to MAIN procedure
JUMP 4
# 

# PROCEDURE function
# WRITE 1234
SET 1234
PUT 0
# RETURN function
RTRN 1002
# ENDPROCEDURE function

# MAIN 
# x := 3
SET 3
STORE 2001
# y := x
LOAD 2001
STORE 2002
# IF _1_: x = x
# CALL function (y)
SET 2002
STORE 1001
# Setting return address and jumping to function
SET 13
STORE 1002
JUMP -11
# ENDIF _1_.
HALT
