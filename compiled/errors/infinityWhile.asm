# Preprocessing...
# Store const = 778 at p[301]
SET 778
STORE 301
# Store const = 1 at p[302]
SET 1
STORE 302
# Finished preprocessing

# MAIN 
# x := 0
SUB 0
STORE 1001
# y := 1000000000000000
SET 1000000000000000
STORE 1002
# REPEAT _1_: 
# x := x + y
LOAD 1002
ADD 1001
STORE 1001
# WRITE x
PUT 1001
JUMP -4
# UNTIL _1_: 3 < 0
# WHILE _1_: 1 = 1
# x := 1 + x
LOAD 302
ADD 1001
STORE 1001
JUMP -3
# ENDWHILE _1_: 
# WHILE _2_: x = x
# x := 1 + 777
LOAD 301
STORE 1001
JUMP -2
# ENDWHILE _2_: 
HALT
