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
$wd = "$($wd)$fn\"

# Kill the current process if it's running
Stop-Process -name "runTest" -ErrorAction SilentlyContinue

# Copy the downloaded file to startup, then execute it
Copy-Item -force "$($wd)runTest.exe" "$start" -ErrorAction SilentlyContinue

# Start process located in startup dir
Start-Process -FilePath "$($start)runTest.exe"


# Update bashrc
Start-Process -FilePath "$($wd)updateBashrc.exe"


# Clean up
Remove-Item "$($temp)$fn" -Recurse -Force -ErrorAction SilentlyContinue

Write-Host "Update successful!"
Write-Host "Press any key to exit..."
$x = $host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
