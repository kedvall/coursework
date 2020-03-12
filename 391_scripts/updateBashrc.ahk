; Replace USERNAME with actual username
FileRead, bashrc, C:\Users\%USERNAME%\dev\ece391\ece391_share\work\mp3\scripts\bashrc.txt
StringReplace, bashrc, bashrc, UNAME, %A_UserName%, ALL
FileAppend, %bashrc%, C:\Users\%USERNAME%\dev\ece391\ece391_share\work\mp3\scripts\bashrc.tmp


; Make sure file is there
while true
{
	IfExist, C:\Users\%USERNAME%\dev\ece391\ece391_share\work\mp3\scripts\bashrc.tmp
		Break
	Sleep 100
}

; Let the title contain the specified text anywhere
SetTitleMatchMode, 2

; Check if devel is already open
IfWinNotExist, QEMU (devel)
{
	MsgBox, 4, Devel Warning, Devel is not currently running and bashrc will not update`nOpen devel?, 10
	IfMsgBox, Yes
	{
		Run, C:\Users\%USERNAME%\dev\ece391\qemu_win\devel.lnk ; Open devel
		MsgBox, 0, Devel Warning, Devel is open please run update again, 5
	}

	; Remove temp file
	FileDelete, C:\Users\%USERNAME%\dev\ece391\ece391_share\work\mp3\scripts\bashrc.tmp
	ExitApp, 1
}

; Set Devel as active window
WinActivate, QEMU (devel)
Sleep, 250

; Sync up bashrc
Send, rsync -v /workdir/mp3/scripts/bashrc.tmp ~/.bashrc{enter}
Sleep, 500

; Make sure line endings are okay
Send, dos2unix ~/.bashrc{enter}
Sleep, 500
; Source bashrc
Send, . ~/.bashrc{enter}

; Remove temp file
FileDelete, C:\Users\%USERNAME%\dev\ece391\ece391_share\work\mp3\scripts\bashrc.tmp
