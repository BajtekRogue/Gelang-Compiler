# Preprocessing...
# Store const = 778 at p[301]
SET 778
STORE 301
# Store const = 1 at p[302]
SET 1
STORE 302
# Store const = -1 at p[303]
SET -1
STORE 303
# Finished preprocessing

# MAIN 
# x := 4
SET 4
STORE 1001
# REPEAT _1_: 
# x := -1 - x
LOAD 303
SUB 1001
STORE 1001
JUMP -3
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
