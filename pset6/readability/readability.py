from cs50 import get_string
import re

# Get text input from user
text = get_string("Text: ")

# Get length of list words in text
words = len(re.split('\s', text))

# Get length of list of sentences in text
# Subtract 1 due to split giving extra string at end
sentences = len(re.split('[.!?]', text)) - 1

# Get rid of spaces and punctuation in text
letters = text.replace(" ", "")
letters = letters.replace(".", "")
letters = letters.replace(",", "")
letters = letters.replace("!", "")
letters = letters.replace("'", "")
letters = letters.replace("\"", "")

# Get length of list of only characters in text
letters = len(letters.replace("?", ""))

# DEBUG
# print(letters, words, sentences)

# Apply Coleman-Liau index
l = letters / words * 100
s = sentences / words * 100
index = 0.0588 * l - 0.296 * s - 15.8

# DEBUG
# print(l, s, index)

# Output grade level
if index >= 16:
    print("Grade 16+")
elif index < 1:
    print("Before Grade 1")
else:
    print("Grade", int(round(index)))