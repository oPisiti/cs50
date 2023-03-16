import re

def readability():
    text = input("Text: ")

    letter_count    = len(re.findall("\w", text))
    word_count      = len(re.findall("[\w|'|-]+", text))
    sentences_count = len(re.findall("[\.|!|\?]+", text))

    L = 100 * letter_count / word_count
    S = 100 * sentences_count / word_count

    index = round(0.0588 * L - 0.296 * S - 15.8)

    if index < 1:     print("Before Grade 1")
    elif index >= 16: print("Grade 16+")
    else:             print(f'Grade {index}')


if __name__ == '__main__':
    readability()