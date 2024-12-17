def insertion_sort():
    # Initialize array (1-based indexing to match original)
    a = [None, 5, 3, -8, 1, -2, -9, 4, 7, 6, -10]  # None at index 0 to match 1-based indexing
    n = 10
    i = 2

    # Outer loop - iterate through array
    while i <= n:
        key = a[i]
        j = i - 1
        done = 0

        # Inner loop - shift elements and find insertion point
        while j >= 1:
            if done == 0:
                if a[j] > key:
                    # Shift element right
                    y = j + 1
                    a[y] = a[j]
                    j = j - 1
                else:
                    # Found insertion point
                    done = 1
            else:
                j = 0
        
        # Insert key at correct position
        y = j + 1
        a[y] = key
        i = i + 1

    # Print sorted array
    i = 1
    while i <= n:
        print(a[i])
        i = i + 1

if __name__ == "__main__":
    insertion_sort()