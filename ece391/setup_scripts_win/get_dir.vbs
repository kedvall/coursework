Dim fso, folder, read, f1, f2
Set fso = CreateObject("Scripting.FileSystemObject")

folder = fso.GetAbsolutePathName(".")

Set f1 = fso.OpenTextFile(folder & "\setup_scripts_win\create_vm.bat")
read = f1.ReadAll
f1.close
read = Replace(read, "REPLACE", folder)

set f2 = fso.CreateTextFile("temp.bat")
f2.write(read)
f2.close

Set f1 = fso.OpenTextFile(folder & "\setup_scripts_win\create_link.vbs")
read = f1.ReadAll
f1.close
read = Replace(read, "REPLACE", folder)

set f2 = fso.CreateTextFile("temp.vbs")
f2.write(read)
f2.close
