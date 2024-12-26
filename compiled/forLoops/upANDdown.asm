# Preprocessing...
# Store const = 1 at p[69]
SET 1
STORE 69
# Store const = 3 at p[302]
SET 3
STORE 302
# Store const = -10 at p[303]
SET -10
STORE 303
# Store const = -100 at p[304]
SET -100
STORE 304
# Finished preprocessing

# MAIN 
# p := 200
SET 200
STORE 1001
# q := p + 800
SET 800
ADD 1001
STORE 1002
# Initializing FOR_UP _1_: 
LOAD 1002
STORE 1004
LOAD 1001
STORE 1003
# FOR_UP _1_: p TO q
SUB 1004
JPOS 20
# Initializing FOR_DOWN _2_: 
LOAD 304
STORE 1006
LOAD 303
STORE 1005
# FOR_DOWN _2_: -10 DOWNTO -100
SUB 1006
JNEG 10
# p := p - 3
LOAD 302
STORE 1
LOAD 1001
SUB 1
STORE 1001
# __itera__tor___--
LOAD 1005
SUB 69
STORE 1005
JUMP -10
# ENDFOR _2_.
# iterator__++
LOAD 1003
ADD 69
STORE 1003
JUMP -20
# ENDFOR _1_.
# WRITE p
PUT 1001
HALT
