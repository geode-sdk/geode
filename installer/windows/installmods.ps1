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
        if ($ModJson.PSObject.Properties.Name -contains "dependencies" -and $ModJson.dependencies) {
            Write-Host "Installing dependencies - Mod versions may be incompatible with the mod."
            foreach ($dep in $ModJson.dependencies.PSObject.Properties) {
                $depId = $dep.Name
                $depValue = $dep.Value

                if ($depValue -is [PSCustomObject] -and $depValue.importance -eq "suggested") {
                    Write-Host "Skipping suggested dependency $depId"
                    continue
                }

                $DepModPath = Join-Path $MODS_PATH "$depId.geode"
                if ($InstalledOrDownloaded.ContainsKey($depId) -or (Test-Path $DepModPath)) { continue }

                Write-Host "Downloading latest dependency $depId..."
                try {
                    $TempDepPath = Join-Path $env:TEMP "$depId.geode"
                    Invoke-WebRequest -Uri "https://api.geode-sdk.org/v1/mods/$depId/versions/latest/download" -OutFile $TempDepPath -UseBasicParsing
                    Install-Mod -ModFilePath $TempDepPath
                } catch {
                    Write-Warning ("Failed to download or install dependency {0}: {1}" -f $depId, $_)
                }
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
