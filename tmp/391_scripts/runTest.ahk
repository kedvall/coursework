;;; Initial setup ;;;
#SingleInstance force

; Let the title contain the specified text anywhere
SetTitleMatchMode, 2
breakpt := false


;;; Hotkeys! ;;;

; First hotkey, used to kill GDB
^+c::
	WinActivate, QEMU (devel)
	Sleep, 250
	Send, ^c
	Sleep, 250
	Send, quit{enter}
	Sleep, 250
	Send, y{enter}
return


; Second hotkey, sets breakpoint to true then runs below code
^+b::
	breakpt := true

; Run our main routine!
^+x::
	; Quit GDB
	WinActivate, QEMU (devel)
	Sleep, 100
	Send, ^c
	Sleep, 250
	Send, quit{enter}
	Sleep, 100
	Send, y{enter}

	; If test window is present, close it so we can build
	IfWinExist, QEMU (mp3)
		WinClose ; Close our window for building

	; Set Devel as active window
	WinActivate, QEMU (devel)

	; Run make on Devel
	Send cd /workdir/mp3/student-distrib{enter}mk{enter}
	Sleep 500 ; Wait for inUse to be created

	; Wait until make is done
	while true
	{
		IfNotExist, C:\Users\%USERNAME%\dev\ece391\ece391_share\work\mp3\scripts\inUse
			Break
		Sleep, 250
	}

	; Reopen test window
	Run, C:\Users\%USERNAME%\dev\ece391\qemu_win\testDebug.lnk

	; Move test to right below devel
	WinGetPos, X, Y, Width, Height, QEMU (devel) ; Get devel coords
	X += Width

	Winwait, QEMU (mp3), , 30 ; Wait for the window to open (wait up to 30 sec)
	WinMove, %X%, %Y%

	; Run gdb on Devel
	WinActivate, QEMU (devel) ; Set Devel as active window 

	if (breakpt) ; Check is user wants to set break point
	{
		Send, gdb bootimg{enter}
		breakpt := false
		return
	}
	else
		Send, gdb bootimg{enter}
		Send, c{enter}

	; Set test as active window
	WinActivate, QEMU (mp3)
	Sleep, 500

	; Continue the boot process
	Send {enter}

; End this hotkey
return