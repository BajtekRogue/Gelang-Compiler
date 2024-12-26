# Preprocessing...
# Store const = 1 at p[301]
SET 1
STORE 301
# Store const = 100 at p[302]
SET 100
STORE 302
# Finished preprocessing

# MAIN 
# i := 0
SUB 0
STORE 1001
# s := 0
SUB 0
STORE 1002
# WHILE _1_: i <= 100
LOAD 302
SUB 1001
JNEG 8
# s := s + i
LOAD 1001
ADD 1002
STORE 1002
# i := i + 1
LOAD 301
ADD 1001
STORE 1001
JUMP -9
# ENDWHILE _1_: 
# WRITE s
PUT 1002
HALT
