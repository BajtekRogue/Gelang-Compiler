# Preprocessing...
# Store const = 1 at p[69]
SET 1
STORE 69
# Finished preprocessing

# MAIN 
# s := 0
SUB 0
STORE 1001
# Initializing FOR_UP _1_: 
SET 100
STORE 1003
LOAD 69
STORE 1002
# FOR_UP _1_: 1 TO 100
SUB 1003
JPOS 8
# s := s + i
LOAD 1002
ADD 1001
STORE 1001
# i++
LOAD 1002
ADD 69
STORE 1002
JUMP -8
# ENDFOR _1_.
# WRITE s
PUT 1001
HALT
