p = 200
q = p + 800
p_cop = p
for i in range(p_cop, q+1):
    for j in range(-10, -101, -1):
        p -= 3

print(p)