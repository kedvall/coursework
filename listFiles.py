import os
import pathlib
from pprint import pprint

user_home = pathlib.Path.home()
target_folder = "dev/cs460/finalProject/testFolder"
start_path = os.path.join(user_home, target_folder)
print("Running folder scan on " + start_path)

dir_list = []
file_list = []

for root, dirs, files in os.walk(start_path):
    for name in files:
        # print(os.path.join(root, name))
        # add extensions here for whichever files you want to encrypt
        if(name.lower().endswith(('.txt', '.png', '.jpg', '.docx'))):
            file_list.append(os.path.join(root, name))
    for name in dirs:
        # print(os.path.join(root, name))
        dir_list.append(os.path.join(root, name))

print("\nDirectories:")
print(dir_list)
print("\nFiles:")
pprint(file_list)
