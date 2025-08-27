param (
    [Parameter(Mandatory=$true, ValueFromRemainingArguments=$true)]
    [string[]]$ModFiles
)

if (-not $env:GEODE_GD_PATH) {
    Write-Error "GEODE_GD_PATH environment variable is not set."
    exit
}

$GEODE_GD_PATH = $env:GEODE_GD_PATH
$MODS_PATH = Join-Path $GEODE_GD_PATH "geode\mods"
$InstalledMods = @{}
$InstalledOrDownloaded = @{}

function Show-InstalledModsMessage {
    param([hashtable]$Mods)
    Add-Type -AssemblyName System.Windows.Forms
    if ($Mods.Count -eq 0) {
        [System.Windows.Forms.MessageBox]::Show(
            "No new mods were installed.",
            "Geode Mod Installer",
            [System.Windows.Forms.MessageBoxButtons]::OK,
            [System.Windows.Forms.MessageBoxIcon]::Information
        )
    } else {
        $msg = "Installed mods:`n`n"
        foreach ($mod in $Mods.Keys) {
            $msg += "- $mod`n"
        }
        [System.Windows.Forms.MessageBox]::Show(
            $msg,
            "Geode Mod Installer",
            [System.Windows.Forms.MessageBoxButtons]::OK,
            [System.Windows.Forms.MessageBoxIcon]::Information
        )
    }
}

function Parse-SemVer($version) {
    $regex = '^v?(\d+)\.(\d+)\.(\d+)(?:-(alpha|beta|prerelease|pr)(?:\.(\d+))?)?$'
    $match = [regex]::Match($version, $regex)
    if (-not $match.Success) { return $null }
    return @{
        Major = [int]$match.Groups[1].Value
        Minor = [int]$match.Groups[2].Value
        Patch = [int]$match.Groups[3].Value
        PreType = $match.Groups[4].Value
        PreNum = if ($match.Groups[5].Value) { [int]$match.Groups[5].Value } else { 0 }
    }
}

function Compare-SemVer($v1, $v2) {
    $v1p = Parse-SemVer $v1
    $v2p = Parse-SemVer $v2
    if (-not $v1p -or -not $v2p) { throw "Invalid SemVer: $v1 or $v2" }

    foreach ($part in 'Major','Minor','Patch') {
        if ($v1p[$part] -lt $v2p[$part]) { return -1 }
        if ($v1p[$part] -gt $v2p[$part]) { return 1 }
    }

    $preOrder = @('alpha','beta','prerelease','pr','')
    $idx1 = $preOrder.IndexOf($v1p.PreType)
    $idx2 = $preOrder.IndexOf($v2p.PreType)
    if ($idx1 -lt $idx2) { return -1 }
    if ($idx1 -gt $idx2) { return 1 }

    if ($v1p.PreNum -lt $v2p.PreNum) { return -1 }
    if ($v1p.PreNum -gt $v2p.PreNum) { return 1 }

    return 0
}

function Test-SemVerConstraint($version, $constraint) {
    if ([string]::IsNullOrEmpty($constraint)) { return $true }
    if ($constraint -match '^>=v?([\w\.\-]+)$') { return (Compare-SemVer $version $matches[1]) -ge 0 }
    if ($constraint -match '^<=v?([\w\.\-]+)$') { return (Compare-SemVer $version $matches[1]) -le 0 }
    if ($constraint -match '^>v?([\w\.\-]+)$') { return (Compare-SemVer $version $matches[1]) -gt 0 }
    if ($constraint -match '^<v?([\w\.\-]+)$') { return (Compare-SemVer $version $matches[1]) -lt 0 }
    if ($constraint -match '^v?([\w\.\-]+)$')  { return (Compare-SemVer $version $matches[1]) -eq 0 }
    return $true
}

function Extract-Mod {
    param([string]$Source, [string]$Destination)
    if (Test-Path $Destination) { Remove-Item -Recurse -Force $Destination }
    try {
        Add-Type -AssemblyName System.IO.Compression.FileSystem
        [System.IO.Compression.ZipFile]::ExtractToDirectory($Source, $Destination)
        return $true
    } catch {
        Write-Error ("Failed to extract {0}: {1}" -f $Source, $_)
        return $false
    }
}

function Install-Mod {
    param([string]$ModFilePath)

    $ModName = [System.IO.Path]::GetFileNameWithoutExtension($ModFilePath)
    if ($InstalledOrDownloaded.ContainsKey($ModName)) { return }

    $TempDir = Join-Path $env:TEMP "temp_$ModName"
    if (-not (Extract-Mod -Source $ModFilePath -Destination $TempDir)) { return }

    $DestModPath = Join-Path $MODS_PATH ([System.IO.Path]::GetFileName($ModFilePath))
    Copy-Item -Path $ModFilePath -Destination $DestModPath -Force

    $InstalledMods[$ModName] = $true
    $InstalledOrDownloaded[$ModName] = $true

    $ModJsonPath = Join-Path $TempDir "mod.json"
    if (Test-Path $ModJsonPath) {
        $ModJson = Get-Content $ModJsonPath -Raw | ConvertFrom-Json
        foreach ($dep in $ModJson.dependencies.PSObject.Properties) {
            $depId = $dep.Name
            $depValue = $dep.Value

            if ($depValue -is [PSCustomObject] -and $depValue.importance -eq "suggested") {
                Write-Host "Skipping suggested dependency $depId"
                continue
            }

            $depConstraint = if ($depValue -is [string]) { $depValue } 
                            elseif ($depValue -is [PSCustomObject] -and $depValue.version) { $depValue.version } 
                            else { "" }

            $DepModPath = Join-Path $MODS_PATH "$depId.geode"
            if ($InstalledOrDownloaded.ContainsKey($depId) -or (Test-Path $DepModPath)) { continue }

            Write-Host "Downloading dependency $depId..."
            try {
                $modInfoJson = Invoke-RestMethod -Uri "https://api.geode-sdk.org/v1/mods/$depId"
                $availableVersions = $modInfoJson.payload.versions | Where-Object { $_.status -eq "accepted" }
                $validVersions = $availableVersions | Where-Object { Test-SemVerConstraint $_.version $depConstraint }

                if ($validVersions.Count -eq 0) {
                    Write-Warning "No version of $depId satisfies constraint '$depConstraint', skipping."
                    continue
                }

                $latestVersion = $validVersions | Sort-Object { $_.version } -Descending | Select-Object -First 1
                $TempDepPath = Join-Path $env:TEMP "$depId.geode"
                Invoke-WebRequest -Uri $latestVersion.download_link -OutFile $TempDepPath -UseBasicParsing
                Install-Mod -ModFilePath $TempDepPath
            } catch {
                Write-Warning ("Failed to download or install dependency {0}: {1}" -f $depId, $_)
            }
        }
    }

    if (Test-Path $TempDir) { Remove-Item -Recurse -Force $TempDir }
}

if (-not (Test-Path $MODS_PATH)) { New-Item -ItemType Directory -Path $MODS_PATH | Out-Null }

foreach ($modFile in $ModFiles) {
    if (Test-Path $modFile) {
        Install-Mod -ModFilePath (Resolve-Path $modFile)
    } else {
        Write-Warning "$modFile does not exist."
    }
}

Show-InstalledModsMessage -Mods $InstalledMods
