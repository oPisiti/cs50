def credit():
    # Getting input
    while True:
        try:
            credit_number = input("Number: ")
            credit_number_int = int(credit_number)
            break
        except ValueError as e:
            continue

    # Checking number
    double = False
    sum_Luhn=0
    int_decimal = 0
    for i in range(len(credit_number)-1, -1, -1):
        int_decimal = int(credit_number[i])
        if double: sum_Luhn += sum([int(d) for d in str(2 * int_decimal)])
        else:      sum_Luhn += int_decimal

        double = not double
    
    if sum_Luhn % 10 != 0: 
        print("INVALID")
        return

    first_two_dig = credit_number[:2]
    credit_number_len = len(credit_number)
    if first_two_dig in ["34", "37"] and credit_number_len == 15:
        print("AMEX")
    elif first_two_dig in ["51", "52", "53", "54", "55"] \
         and credit_number_len == 16:
        print("MASTERCARD")
    elif first_two_dig[0] in ["4"] and credit_number_len in [13, 16]:
        print("VISA")
    else:
        print("INVALID")
    
    return



if __name__ == "__main__":
    credit()