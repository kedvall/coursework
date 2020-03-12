Add-Type -AssemblyName System.IO.Compression.FileSystem

$fn = "scripts" #runTest
$url = "http://kedvall.com/resources/$fn.zip"
$wd = "C:\Users\$env:username\AppData\Local\Temp\"
$start = "C:\Users\$env:username\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup\"
$output = "$($wd)$fn.zip"

function Unzip
{
    param([string]$zipfile, [string]$outpath)
    [System.IO.Compression.ZipFile]::ExtractToDirectory($zipfile, $outpath)
}

# Remove old folder
Remove-Item "$($wd)$fn" -Recurse -Force -ErrorAction SilentlyContinue

# Download updated file
try {
    Invoke-WebRequest -Uri $url -OutFile $output -ErrorAction Stop
} catch {
    Write-Host "Sorry, the update file couldn't be found :("
    Write-Host "Press any key to exit..."
    $x = $host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
    exit 
}

# Extract it
Unzip "$output" "$($wd)$fn"

# Delete zip file
Remove-Item $output

# Change our working dir
$temp = $wd
$wd = "$($wd)$fn\$fn\"

# Stop Process if it is running
if ( !((Get-Process runTest -ErrorAction SilentlyContinue) -eq $null))
{
    Stop-Process -processname runTest
    Write-Output "Killed runTest process"
}

# Copy the downloaded file to startup, then execute it
Copy-Item -force "$($wd)runTest.exe" "$start" -ErrorAction SilentlyContinue
Write-Output "New runtest process copied to startup dir"

# Start process located in startup dir
Start-Process -FilePath "$($start)runTest.exe"
Write-Output "Restarted new runTest process"


# Update bashrc
Write-Output ""
$answer = Read-Host "Update bashrc? (Y/N) "
while("y","Y","n","N" -notcontains $answer)
{
    $answer = Read-Host "Please answer y or n"
}

Switch ($answer) 
{
   Y {Write-host "Updating!"; Start-Process -FilePath "$($wd)updateBashrc.exe" ; Write-host "Bashrc updated!"} 
   N {break}
}


# Clean up
Remove-Item "$($temp)$fn" -Recurse -Force -ErrorAction SilentlyContinue

Write-Output ""
Write-Host "Script updated successfully!"
Write-Host "Press any key to exit..."
$x = $host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
