# CS460

TO TEST MALWARE:

1. Install Python 3.7
2. Create a directory path $HOME$/dev/cs460/finalProject/testFolder  -> In this case, $HOME$ is your home directory. 
3. Clone the directory and copy the testFolder into the path above. 
4. Run the malware using this command -> python3.7 definitelynotavirus.py

DESCRIPTION:
Our project is based off of the CryptoLocker ransomware. We intended to write a simplified version of this malware that will infect a user’s computer, encrypt all the user’s data, and hold it for ransom. Instead of a Bitcoin ransom, the user will need to solve a series of riddles to decrypt their files. 
Our simplified version of the malware is able to find and encrypt a user’s files and provide a series of technical puzzles for the user to solve in order to retrieve their files.

Our malware is written in python to take advantage of the os library, making our malware executable on different types of operating systems. We use the os libraries to find the user's home directory and systematically go through all the directories. Using AES encryption, we encrypt every file with a proper extensions: .txt, .png, .pdf, .dox, .jpg. After the file is encrypted, a .enc extension is added and the original file is deleted. 

Once encryption is complte, the malware presents a series of three riddles to the user. The user gets unlimited tries to solve each puzzle, but if they get one answer wrong or if they give up, then the code exits and their files stay encrypted. 

If the user successfully solves all three riddles, the malware will go back through the files starting from the home directory and decrypt every file with a .enc extension. 

ADDITIONAL FEATURES:

1. AES Key encryption. In order to prevent a user from simply finding the AES key in our code and manually decrypting their files, we wanted to use an RSA public key to encrypt the AES key used for encryption. Once the user had solved all three puzzles, the malware would ping a remote server to retrieve the private key in the RSA key pair. Then the malware would decrypt the AES key and continue to decrypt the files. 

2. Perform basic removal of shadow copies or other ways of restoring the files. Due to time constraints, we were not able to fully integrate this feature. We were able to successfully delete original copies of files. Fully removing any way of restoring files would ensure a robust piece of malware and would be a good next added feature. 

3. Autonomously spread to other connected machines. After fully integrating the above features, having the malware autonomously spread would increase its effectiveness. 

