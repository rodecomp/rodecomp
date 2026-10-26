# Create or refresh private clones (slots) under workers\<name>.
# Usage: pwsh scripts/workspace/slot.ps1 w1 [w2 ...]   (also works in Windows PowerShell 5.1)
# A dirty slot is never touched.
$ErrorActionPreference = 'Stop'

# Layout: <root>\<main clone>\scripts\workspace\<this>, or <root>\workers\<slot>\scripts\workspace\<this>.
$Clone = Split-Path -Parent (Split-Path -Parent $PSScriptRoot)
if ((Split-Path -Leaf (Split-Path -Parent $Clone)) -eq 'workers') {
    $Root = Split-Path -Parent (Split-Path -Parent $Clone)
    $Main = if ($env:RODECOMP_MAIN) { $env:RODECOMP_MAIN } else { Join-Path $Root 'rodecomp' }
} else {
    $Root = Split-Path -Parent $Clone
    $Main = if ($env:RODECOMP_MAIN) { $env:RODECOMP_MAIN } else { $Clone }
}
if ($args.Count -eq 0) { Write-Error 'usage: slot.ps1 <slot>...'; exit 2 }
if (-not (Test-Path (Join-Path $Main '.git'))) { Write-Error "main clone not found: $Main"; exit 1 }
$Url = (git -C $Main remote get-url origin).Trim()

function Invoke-Git {
    git @args
    if ($LASTEXITCODE -ne 0) { throw "git $($args -join ' ') failed ($LASTEXITCODE)" }
}

function Copy-Td([string]$Slot) {
    $dst = Join-Path $Slot '.td'
    New-Item -ItemType Directory -Force $dst | Out-Null
    foreach ($f in 'config.json', 'symbols.json', 'symbols.json.etag', 'target.exe') {
        $src = Join-Path $Main ".td\$f"
        if (Test-Path $src) { Copy-Item -Force $src (Join-Path $dst $f) }
    }
    $ex = Join-Path $Slot '.git\info\exclude'
    if (-not ((Test-Path $ex) -and (Select-String -Path $ex -SimpleMatch -Pattern '.td/' -Quiet))) {
        Add-Content -Path $ex -Value '.td/' -Encoding ascii
    }
}

$rc = 0
foreach ($name in $args) {
    if ($name -match '[\\/]' -or $name -in '', '.', '..') { Write-Warning "[$name] invalid slot name"; $rc = 1; continue }
    $slot = Join-Path $Root "workers\$name"
    New-Item -ItemType Directory -Force (Join-Path $Root 'workers'), (Join-Path $Root "_work\$name") | Out-Null
    if (-not (Test-Path (Join-Path $slot '.git'))) {
        try {
            Invoke-Git clone -q $Main $slot
            Invoke-Git -C $slot remote set-url origin $Url
            Invoke-Git -C $slot fetch -q origin
            Invoke-Git -C $slot checkout -q -B main origin/main
        } catch {
            if (Test-Path $slot) { Remove-Item -Recurse -Force $slot }
            Write-Warning "[$name] creation failed, removed partial clone: $_"; $rc = 1; continue
        }
        Copy-Td $slot
        Write-Host "[$name] created at $(git -C $slot rev-parse --short HEAD)"
        continue
    }
    if (git -C $slot status --porcelain) { Write-Warning "[$name] dirty, skipped"; $rc = 1; continue }
    Invoke-Git -C $slot fetch -q origin
    Invoke-Git -C $slot checkout -q -B main origin/main
    Copy-Td $slot
    Write-Host "[$name] refreshed at $(git -C $slot rev-parse --short HEAD)"
}
exit $rc
