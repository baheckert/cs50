# Imports
from cs50 import get_string

# Get card number
cardNum = get_string("Number: ")

# Calculate checksum

# Get digits for Luhn's Algorithm
slicedNum = cardNum[(len(cardNum) - 2)::-2]

# Multiply each digit by 2 and add to a string
products = ""
for d in slicedNum:
    products += str(int(d) * 2)

# Get sum of products' digits
prodSum = 0
for d in products:
    prodSum += int(d)

# Add other digits from card number to sum of products
for d in cardNum[len(cardNum) - 1::-2]:
    prodSum += int(d)

# If valid number check card type
if prodSum % 10 == 0:
    # DEBUG
    # print(len(cardNum), cardNum[0], cardNum[0:1])

    # American Express
    # 15 digits, starts with 34 or 37
    if len(cardNum) == 15 and (cardNum[0:2] == "34" or cardNum[0:2] == "37"):
        print("AMEX")
    # Mastercard
    # 16 digits, starts with 51-55
    if len(cardNum) == 16 and int(cardNum[0:2]) > 50 and int(cardNum[0:2]) < 56:
        print("MASTERCARD")
    # VISA
    # 13 or 16 digits, starts with 4
    if (len(cardNum) == 13 or len(cardNum) == 16) and cardNum[0] == "4":
        print("VISA")
# Must be invalid card number
else:
    print("INVALID")