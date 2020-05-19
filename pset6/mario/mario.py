# Imports
from cs50 import get_int

# Get int between 0 and 8
while True:
    height = get_int("Height: ")
    if height in range(1, 9):
        break

# Print each row
for r in range(height):
    spaces = " " * (height - (r + 1))
    blocks = "#" * (r + 1)
    print(spaces + blocks + "  " + blocks)