# print("There was a man who was born before his father, killed his mother, and married his sister. Yet, there was nothing wrong with what he had done. Why?");
def puzzleOne():
    answer = ""
    check = 0
    while(answer != "g" or answer != "G"):
        answer = input("What always ends everything?\n")
        if(answer == "g" or answer == "G"):
            print("You got it!\n")
            return 1;
        else:
            while(check != 1):
                print("That's not right you dum dum!\n")
                value = input("Do you give up? (Y/N)\n")
                if(value == "Y"):
                    print("HAHAHA YOU LOSE\n")
                    check = 1
                    return 0;
                elif(value == "N"):
                    check = 1
                    print("Alright, lets try again!\n")
                else:
                    print("Invalid Input")
    return 0;

def puzzleTwo():
    answer = ""
    check = 0
    while(answer != "g" or answer != "G"):
        answer = input("I can fly, but I don't have wings. I don't have eyes, but I can cry! What I am?\n")
        if(answer == "A cloud" or answer == "a cloud" or answer == "cloud" or answer == "Cloud"  or answer == "A Cloud"  or answer == "a Cloud"):
            print("That's right, one more and you can have your files back!\n")
            return 1;
        else:
            while(check != 1):
                print("Big oof, incorrect my dude!")
                value = input("Do you give up? (Y/N)\n")
                if(value == "Y"):
                    check = 1
                    print("HAHAHA YOU LOSE\n")
                    return 0;
                elif(value == "N"):
                    check = 1
                    print("Have another go!\n")
                else:
                    print("Invalid Input")
    return 0;

def puzzleThree():
    answer = ""
    check = 0
    while(answer != "g" or answer != "G"):
        answer = input("Two sisters we are, one is dark and one is fair, \nIn twin towers dwelling we’re quite the pair, \nOne from land and one from sea, \nTell us truly, who are we?\n")
        if(answer == "Salt and Pepper" or answer == "salt and pepper" or answer == "salt and Pepper" or answer == "Salt and pepper"  or answer == "Salt And Pepper"):
            print("That's right, you got it!\n")
            return 1;
        else:
            while(check != 1):
                print("Rip, incorrect dum dum!")
                value = input("Do you give up? (Y/N)\n")
                if(value == "Y"):
                    check = 1
                    print("HAHAHA YOU LOSE\n")
                    return 0;
                elif(value == "N"):
                    check = 1
                    print("Have another go!\n")
                else:
                    print("Invalid Input")
    return 0;

# Call encryption function here
# call key encryption function here
num = 0;
print("You've been HACKED! \nSoRrY bUt AlL yOuR fIlEs HaVe bEeN eNcRyPtEd. \nIf YoU wAnT tHeM bAcK, sOlVe thEsE pUzZleS. OThErwIse, wE'lL delEte EveRytHinG!")
num = num + puzzleOne()
if(num == 1):
    num = num + puzzleTwo()
    if(num == 2):
        num = num + puzzleThree()
if(num == 3):
    # decrypt files here
    print("Your files have been decrypted, enjoy your day!\n")
else:
    print("Sorry, all your files are gone.\n")
