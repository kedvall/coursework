from os import walk
from pathlib import Path

fileList = []
dirList = []

home = str(Path.home())

for (dirpath, dirnames, filenames) in walk(home):
    fileList.extend(filenames)
    dirList.extend(dirnames)
    break

print(fileList)
print("\n\n")
print(dirList)