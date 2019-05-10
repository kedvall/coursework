# print("There was a man who was born before his father, killed his mother, and married his sister. Yet, there was nothing wrong with what he had done. Why?");
def puzzleOne():
    answer = ""
    while(answer != "g" or answer != "G"):
        answer = input("What always ends everything?")
        if(answer == "g" or answer == "G"):
            print("You got it!")
            return 1;
        else:
            print("That's not right you dum dum!")
            value = input("Do you give up? (Y/N)")
            if(value == "Y"):
                print("HAHAHA YOU LOSE")
                return 0;
            else:
                print("Alright, lets try again.")

    return 0;


print("You've been HACKED! \nSoRrY bUt AlL yOuR fIlEs HaVe bEeN eNcRyPtEd. \nIf YoU wAnT tHeM bAcK, sOlVe thEsE pUzZleS. OThErwIse, wE'lL delEte EveRytHinG!")
if(puzzleOne() == 1):
    print("Your files have been decrypted, enjoy your day!")
else:
    print("Sorry, all your files are gone.")
