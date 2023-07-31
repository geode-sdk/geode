Set-Location ./installer/windows
Invoke-WebRequest -useb https://aka.ms/vs/17/release/vc_redist.x86.exe -outfile 'VC_redist.x86.exe'
makensis -WX -V3 installer.nsi