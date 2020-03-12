' Set up variables
Dim oFS, wshShell, userPath, qemuPath, cDir, develTarget, testTarget, devel, test
Set wshShell = WScript.CreateObject("WScript.Shell")
Set oFS = CreateObject("Scripting.FileSystemObject")


' Set up paths
userPath = wshShell.ExpandEnvironmentStrings("%USERPROFILE%")
qemuPath = userPath & "\dev\ece391\qemu_win"
startPath = userPath & "\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup"
cDir = wshShell.CurrentDirectory
develTarget = userPath & "\dev\ece391\ece391_share\work\vm\devel.qcow"
testTarget = userPath & "\dev\ece391\ece391_share\work\mp3\student-distrib\mp3.img"


' Create shortcuts!
' Set up devel shortcut
Set devel = wshShell.CreateShortcut(qemuPath + "\devel.lnk")
devel.TargetPath = """" & qemuPath & "\qemu-system-i386w.exe" & """"
devel.WorkingDirectory = qemuPath
devel.Arguments = " -hda " & """" & develTarget & """" & " -m 512 -name devel"
devel.IconLocation = qemuPath & "\qdevel.ico,0"
devel.Save


' Set up test shortcut
Set test = wshShell.CreateShortcut(qemuPath + "\testDebug.lnk")
test.TargetPath = """" & qemuPath & "\qemu-system-i386w.exe" & """"
test.WorkingDirectory = qemuPath
test.Arguments = " -hda " & """" & testTarget & """" & " -m 256 -gdb tcp:127.0.0.1:1234 -S -name mp3"
test.IconLocation = qemuPath & "\qtest.ico,0"
test.Save