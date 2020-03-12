Dim wshShell, desktopPath, qemuPath, develPath, devel, testDebug, testNoDebug, kernelPath
Set wshShell = WScript.CreateObject("WScript.Shell")

desktopPath = wshShell.ExpandEnvironmentStrings("%userprofile%\Desktop")

qemuPath = "REPLACE\qemu_win"
develPath = "REPLACE\ece391_share\work\vm\devel.qcow"
testPath = "REPLACE\ece391_share\work\vm\test.qcow"
kernelPath = "REPLACE\ece391_share\work\source\linux-2.6.22.5\bzImage"

Set devel = wshShell.CreateShortcut(desktopPath + "\devel.lnk")

devel.TargetPath = """" & qemuPath & "\qemu-system-i386w.exe" & """"
devel.WorkingDirectory = qemuPath
devel.Arguments = " -hda " & """" & develPath & """" & " -m 512 -name devel"
devel.IconLocation = qemuPath & "\qemu-icon.ico,0"

devel.Save

Set testDebug = wshShell.CreateShortcut(desktopPath + "\test_debug.lnk")

testDebug.TargetPath = """" & qemuPath & "\qemu-system-i386w.exe" & """"
testDebug.WorkingDirectory = qemuPath
testDebug.Arguments = " -hda " & """" & testPath & """" & " -kernel " & """" & kernelPath & """" & " -m 512 -gdb tcp:127.0.0.1:1234 -S -name test"
testDebug.IconLocation = qemuPath & "\qemu-icon.ico,0"

testDebug.Save

Set testNoDebug = wshShell.CreateShortcut(desktopPath + "\test_nodebug.lnk")

testNoDebug.TargetPath = """" & qemuPath & "\qemu-system-i386w.exe" & """"
testNoDebug.WorkingDirectory = qemuPath
testNoDebug.Arguments = " -hda " & """" & testPath & """" & " -kernel " & """" & kernelPath & """" & " -m 512 -gdb tcp:127.0.0.1:1234 -name test"
testNoDebug.IconLocation = qemuPath & "\qemu-icon.ico,0"

testNoDebug.Save
