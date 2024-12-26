# Translating the given pseudocode into Python
def main():
    n = 24
    ta = [0] * 25  # Arrays ta, tb, tc with size 25 (0 to 24 inclusive)
    tb = [0] * 25
    tc = [0] * 25

    tc[0] = n
    tc[n] = n - tc[0]
    j = tc[0] + 1

    for i in range(tc[0], tc[n] - 1, -1):  # Loop from tc[0] down to tc[n]
        ta[i] = i
        tb[i] = n - i

    j = tc[n]
    while j <= tc[0]:  # Loop while j is less than or equal to tc[0]
        tc[j] = ta[j] * tb[j]
        j += 1

    for i in range(0, n + 1):  # Loop from 0 to n inclusive
        print(tc[i])

# Execute the main program
if __name__ == "__main__":
    main()
