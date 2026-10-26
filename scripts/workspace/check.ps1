# Back up a slot's edits, then run `td check` from that slot.
# Usage: pwsh scripts/workspace/check.ps1 <slot> <file>... [--includers] [--full]
#   --full   print td's whole output instead of one line per function
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
if ($args.Count -lt 2) { Write-Error 'usage: check.ps1 <slot> <file>... [--includers] [--full]'; exit 2 }
$name = $args[0]
$slot = Join-Path $Root "workers\$name"
if (-not (Test-Path (Join-Path $slot '.git'))) { Write-Error "no slot: $slot (run slot.ps1 $name)"; exit 1 }

$rest = @($args | Select-Object -Skip 1)
$full = $rest -contains '--full'
$tdArgs = @($rest | Where-Object { $_ -ne '--full' })

$work = Join-Path $Root "_work\$name"
New-Item -ItemType Directory -Force $work | Out-Null
$stamp = Get-Date -Format 'yyyyMMdd-HHmmss'
$patch = Join-Path $work "pre-check-$stamp.patch"
$log = Join-Path $work "check-$stamp.log"

# git writes the patch itself: PowerShell pipelines would re-encode it and break `git apply`.
git -C $slot diff HEAD --binary "--output=$patch"
$before = (git hash-object -- $patch | Out-String).Trim()

Push-Location $slot
try {
    $out = & td check @tdArgs 2>&1 | ForEach-Object { "$_" }
    $rc = $LASTEXITCODE
} finally { Pop-Location }
[IO.File]::WriteAllLines($log, [string[]]$out, [Text.UTF8Encoding]::new($false))

if ($full) {
    $out
} else {
    $lines = $out | Where-Object { $_ -match '^\s*(fn|gl|vt|st)_\d+ |error|unannotated' }
    if ($lines) { $lines } else { $out }
}

$afterFile = Join-Path $work "post-check-$stamp.patch"
git -C $slot diff HEAD --binary "--output=$afterFile"
$after = (git hash-object -- $afterFile | Out-String).Trim()
Remove-Item $afterFile
if ($before -ne $after) { Write-Warning "working tree changed during td check; edits saved in $patch" }
Write-Host "log: $log  backup: $patch  exit: $rc"
exit $rc
