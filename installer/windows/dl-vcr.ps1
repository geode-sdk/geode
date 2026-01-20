# Downloads the Visual Studio runtime that is necessary for building the Windows installer.
$Out = 'VC_redist.x64.exe'
if (Test-Path $Out -PathType Leaf) {
    Return
}
$ProgressPreference = 'SilentlyContinue'
Invoke-WebRequest -UseBasicParsing 'https://aka.ms/vs/17/release/vc_redist.x64.exe' -OutFile $Out