# Downloads the Visual Studio runtime that is necessary for building the Windows installer.
if (Test-Path VC_redist.x64.exe -PathType leaf) {
    Return
}
$ProgressPreference = 'SilentlyContinue'
Invoke-WebRequest -useb https://aka.ms/vs/17/release/vc_redist.x64.exe -outfile VC_redist.x64.exe