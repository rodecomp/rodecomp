[CmdletBinding()]
param()

$ErrorActionPreference = 'Stop'
$repoRoot = Split-Path -Parent $PSScriptRoot
$root = $repoRoot.Replace('\', '/')
$project = Get-Content -LiteralPath "$repoRoot/data/decomp.json" -Raw | ConvertFrom-Json
$compile = Get-Content -LiteralPath "$repoRoot/$($project.compileCommands)" -Raw | ConvertFrom-Json
$workspace = Get-Content -LiteralPath "$repoRoot/.td/config.json" -Raw | ConvertFrom-Json
$runner = $workspace.runners.PSObject.Properties[$project.toolchain].Value

if (-not $runner -or $runner.type -ne 'native' -or -not $runner.toolchainRoot) {
  throw "clangd configuration requires this workspace's native Windows toolchain."
}

$toolchain = $runner.toolchainRoot.Replace('\', '/')
$compiler = $compile.compiler.Replace('${TC}', $toolchain).Replace('${SRC}', $root)

if (-not (Test-Path -LiteralPath $compiler -PathType Leaf)) {
  throw "Configured compiler not found: $compiler"
}

$systemIncludes = @($compile.env.INCLUDE.Replace('${TC}', $toolchain).Replace('${SRC}', $root).Split(';') | Where-Object { $_ })

foreach ($include in $systemIncludes) {
  if (-not (Test-Path -LiteralPath $include -PathType Container)) {
    throw "Configured include directory not found: $include"
  }
}

$commands = @(
  foreach ($unit in $compile.units.PSObject.Properties) {
    $file = "$root/$($unit.Name)"

    if (-not (Test-Path -LiteralPath $file -PathType Leaf)) {
      throw "Configured source file not found: $file"
    }

    $arguments = @($compiler)
    $arguments += @($compile.shapes[[int]$unit.Value] | ForEach-Object {
      $_.Replace('${TC}', $toolchain).Replace('${SRC}', $root)
    })
    $arguments += @($systemIncludes | ForEach-Object { "/imsvc$_" })
    $arguments += $file

    [ordered]@{
      directory = $root
      file = $file
      arguments = $arguments
    }
  }
)

$outputDirectory = Join-Path $repoRoot 'build/editor-x86'
[void](New-Item -ItemType Directory -Path $outputDirectory -Force)
$output = Join-Path $outputDirectory 'compile_commands.json'
$json = ConvertTo-Json -InputObject $commands -Depth 6
[System.IO.File]::WriteAllText($output, $json + "`n", [System.Text.UTF8Encoding]::new($false))
Write-Output "Generated $($commands.Count) clangd compilation commands: $output"
