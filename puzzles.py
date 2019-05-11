# print("There was a man who was born before his father, killed his mother, and married his sister. Yet, there was nothing wrong with what he had done. Why?");
def puzzleOne():
    answer = ""
    while(answer != "g" or answer != "G"):
        answer = input("What always ends everything?\n")
        if(answer == "g" or answer == "G"):
            print("You got it!\n")
            return 1;
        else:
            print("That's not right you dum dum!\n")
            value = input("Do you give up? (Y/N)\n")
            if(value == "Y"):
                print("HAHAHA YOU LOSE\n")
                return 0;
            else:
                print("Alright, lets try again.\n")
    return 0;

def puzzleTwo():
    answer = ""
    while(answer != "g" or answer != "G"):
        answer = input("I can fly, but I don't have wings. I don't have eyes, but I can cry! What I am?\n")
        if(answer == "A cloud" or answer == "a cloud" or answer == "cloud" or answer == "Cloud"  or answer == "A Cloud"  or answer == "a Cloud"):
            print("That's right, one more and you can have your files back!\n")
            return 1;
        else:
            print("Big oof, incorrect my dude!")
            value = input("Do you give up? (Y/N)\n")
            if(value == "Y"):
                print("HAHAHA YOU LOSE\n")
                return 0;
            else:
                print("Have another go!\n")
    return 0;

# Call encryption function here
# call key encryption function here
num = 0;
print("You've been HACKED! \nSoRrY bUt AlL yOuR fIlEs HaVe bEeN eNcRyPtEd. \nIf YoU wAnT tHeM bAcK, sOlVe thEsE pUzZleS. OThErwIse, wE'lL delEte EveRytHinG!")
num = num + puzzleOne()
num = num + puzzleTwo()
if(num == 2):
    # decrypt files here
    print("Your files have been decrypted, enjoy your day!\n")
else:
    print("Sorry, all your files are gone.\n")
