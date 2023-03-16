def mario():
    while True:
        try:
            height = int(input("Height: "))
        except ValueError as e:
            print("Please input a number between 1 and 8")
            continue

        if height >= 1 and height <= 8: break

    char = "#"
    for i in range(1, height + 1):
        print(" " * (height - i), end="")
        print(char * i + "  ", end="")
        print(char * i)


if __name__ == '__main__':
    mario()