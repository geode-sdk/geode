# Downloads the Visual Studio runtime that is necessary for building the Windows installer.
$Out = 'VC_redist.x64.exe'
if (Test-Path $Out -PathType Leaf) {
    Return
}
$ProgressPreference = 'SilentlyContinue'
Invoke-WebRequest -UseBasicParsing 'https://aka.ms/vc14/vc_redist.x64.exe' -OutFile $Out