if(Test-path VC_redist.x64.exe -PathType leaf) {
    Return
}
Invoke-WebRequest -useb https://aka.ms/vs/17/release/vc_redist.x64.exe -outfile VC_redist.x64.exe