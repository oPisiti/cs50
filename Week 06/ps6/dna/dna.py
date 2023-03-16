import csv
import sys
import re

def main():

    if len(sys.argv) != 3:
        print("Usage: python dna.py <database.csv> <dna-sequence.txt>")
        return 

    # Read database file into a variable
    with open(sys.argv[1], "r") as f:
        database_raw = csv.DictReader(f)
        database = []
        for d in database_raw: database.append(d)

    # Read DNA sequence file into a variable
    with open(sys.argv[2], "r") as f:
        dna = f.read()

    # Find longest match of each STR in DNA sequence
    STR_count = dict()
    for STR in database[0].keys():
        if STR == "name": continue        
        STR_count[STR] = str(longest_match(dna, STR))

    # Check database for matching profiles
    for profile in database:
        found_match = True
        for profile_STR in profile.keys():
            if profile_STR == "name": continue
            if profile[profile_STR] != STR_count[profile_STR]:
                found_match = False
                break
        
        if found_match: sys.exit(profile["name"])

    sys.exit("No match")

    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1
            
            # If there is no match in the substring
            else:
                break
        
        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in sequence, return longest run found
    return longest_run


if __name__ == "__main__":
    main()
