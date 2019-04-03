import os
import pathlib
from pprint import pprint

userHome = pathlib.Path.home()
targetFolder = "dev/cs460/final_project/testFolder"
startPath = os.path.join(userHome, targetFolder)
print("Running folder scan on " + startPath)

dirList = []
fileList = []

for root, dirs, files in os.walk(startPath):
    for name in files:
        # print(os.path.join(root, name))
        fileList.append(os.path.join(root, name))
    for name in dirs:
        # print(os.path.join(root, name))
        dirList.append(os.path.join(root, name))

print("\nDirectories:")
print(dirList)
print("\nFolders:")
pprint(fileList)