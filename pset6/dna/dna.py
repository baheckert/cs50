from sys import argv, exit
import csv

# Check argument usage
if len(argv) != 3:
    print("Usage: python dna.py <database.csv> <sequence.txt>")
    exit(1)

# Read database file
database = []
with open(argv[1], "r") as file:
    for row in csv.reader(file):
        database.append(row)

# Store STRs from database
strs = database[0][1:]

# Remove header row from database
database = database[1:]

# Initialize list of STR counts
counts = []

# Read sequence file
with open(argv[2], "r") as file:
    sequence = file.read()


def main():
    # Fill counts with longest run of each STR
    for s in strs:
        counts.append(str(getCount(s, sequence)))

        # DEBUG
        # print(counts)

    # Search database for match and print name
    for row in database:
        if row[1:] == counts:
            print(row[0])
            exit(0)

    # Search did not find match
    print("No match")
    exit(0)


def getCount(s, seq):
    # Initiate variables
    longest = 0
    run = 0
    # Store lnegth of STR
    l = len(s)

    while True:
        # DEBUG
        # print(s, seq[:l], longest, run)

        # Check for end of string
        if len(seq) < l:
            if run > longest:
                return run
            else:
                return longest

        # Check for match at front of string
        if seq[:l] == s:
            run += 1
            seq = seq[l:]
        else:
            if run > longest:
                longest = run

            run = 0
            seq = seq[1:]


main()

