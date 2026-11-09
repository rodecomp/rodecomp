"""Generate IDE files from data/compile.json (the server's per-unit compiler flags).

    python scripts/exe/gen_ide.py

Writes, all under build/ (git-ignored, regenerate after `td sync`):

  build/Ragexe.sln, build/Ragexe.vcxproj (+ .filters)
      A Visual Studio "Makefile" project. Build runs scripts/exe/build_exe.py (what `td compile` runs), Rebuild
      runs it after deleting build/exe, Clean deletes build/exe, and Debug starts build/exe/Ragexe.exe. Every
      unit appears in Solution Explorer under its folder; IntelliSense gets the defines and include paths of
      each unit's flag set.
  build/editor-x86/compile_commands.json
      One entry per unit with its exact cl.exe command, for clangd (.clangd points here), VS Code and CLion.

The build itself stays build_exe.py; these files only call it.
"""

import json
import os
import re
import sys
import uuid
from pathlib import Path
from xml.sax.saxutils import escape

sys.path.insert(0, str(Path(__file__).resolve().parent))
import build_exe  # noqa: E402

ROOT = build_exe.ROOT
BUILD = ROOT / "build"
NAME = "Ragexe"
NS = uuid.UUID("6f4c1f1e-2b8a-4f0e-9a43-6c8d5b7a1e20")


def guid(text):
    return "{" + str(uuid.uuid5(NS, text)).upper() + "}"


def toolchain_root():
    try:
        config = json.loads((ROOT / ".td" / "config.json").read_text(encoding="utf-8"))
        return config["runners"]["ragnarok"]["toolchainRoot"]
    except (OSError, KeyError, ValueError):
        sys.exit("no .td/config.json toolchain: run 'td setup' / 'td toolchain install' first")


def expand(value, tc):
    return value.replace("${TC}", tc).replace("${SRC}", str(ROOT).replace("\\", "/"))


def defines_and_includes(flags):
    defines, includes = [], []
    for flag in flags:
        m = re.match(r"^[-/]D(.+)$", flag)
        if m:
            defines.append(m.group(1))
            continue
        m = re.match(r"^[-/]I(.+)$", flag)
        if m:
            includes.append(m.group(1))
    return defines, includes


def compile_commands(config, units, tc):
    compiler = expand(config["compiler"], tc)
    # cl.exe reads the system headers from %INCLUDE%; tools reading this database don't, so pass them explicitly
    system = [f"/imsvc{p}" for p in expand(config["env"]["INCLUDE"], tc).split(";") if p]
    entries = []
    for unit, (shape, extra) in sorted(units.items()):
        args = [compiler, *(expand(a, tc) for a in config["shapes"][shape]), *system]
        args += [f"/I{(ROOT / d).as_posix()}" for d in extra]
        args.append((ROOT / unit).as_posix())
        entries.append({"directory": ROOT.as_posix(), "file": (ROOT / unit).as_posix(), "arguments": args})
    out = BUILD / "editor-x86"
    out.mkdir(parents=True, exist_ok=True)
    (out / "compile_commands.json").write_text(json.dumps(entries, indent=1), encoding="utf-8")
    return out / "compile_commands.json"


def rel(path):
    return os.path.relpath(ROOT / path, BUILD).replace("/", "\\")


def vcxproj(config, units, tc):
    system_includes = expand(config["env"]["INCLUDE"], tc)
    python = sys.executable
    script = rel("scripts/exe/build_exe.py")
    exe_dir = "$(SolutionDir)exe"
    build_cmd = f'"{python}" "$(SolutionDir){script}" --out "{exe_dir}"'
    clean_cmd = f'if exist "{exe_dir}" rmdir /s /q "{exe_dir}"'

    # IntelliSense: per-unit defines/includes, grouped by flag set
    shapes = {}
    for unit, (shape, extra) in units.items():
        defines, includes = defines_and_includes(expand(a, tc) for a in config["shapes"][shape])
        includes += [str(ROOT / d) for d in extra]
        shapes.setdefault(shape, (defines, includes))
    all_defines = sorted({d for defines, _ in shapes.values() for d in defines})
    all_includes = sorted({i.replace("/", "\\") for _, includes in shapes.values() for i in includes})

    items = []
    for unit, (shape, extra) in sorted(units.items()):
        defines, includes = defines_and_includes(expand(a, tc) for a in config["shapes"][shape])
        includes = [i.replace("/", "\\") for i in includes] + [str(ROOT / d) for d in extra]
        items.append(
            f'    <ClCompile Include="{escape(rel(unit))}">\n'
            f'      <PreprocessorDefinitions>{escape(";".join(defines))}</PreprocessorDefinitions>\n'
            f'      <AdditionalIncludeDirectories>{escape(";".join(includes))}</AdditionalIncludeDirectories>\n'
            f'    </ClCompile>')
    headers = sorted(p.relative_to(ROOT).as_posix() for d in ("Source", "Recovered") if (ROOT / d).is_dir()
                     for p in (ROOT / d).rglob("*") if p.suffix.lower() in (".h", ".hpp", ".inl", ".inc"))
    header_items = [f'    <ClInclude Include="{escape(rel(h))}" />' for h in headers]

    project = f"""<?xml version="1.0" encoding="utf-8"?>
<Project DefaultTargets="Build" ToolsVersion="Current" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <ItemGroup Label="ProjectConfigurations">
    <ProjectConfiguration Include="Release|Win32">
      <Configuration>Release</Configuration>
      <Platform>Win32</Platform>
    </ProjectConfiguration>
  </ItemGroup>
  <PropertyGroup Label="Globals">
    <VCProjectVersion>16.0</VCProjectVersion>
    <ProjectGuid>{guid(NAME)}</ProjectGuid>
    <Keyword>MakeFileProj</Keyword>
    <RootNamespace>{NAME}</RootNamespace>
  </PropertyGroup>
  <Import Project="$(VCTargetsPath)\\Microsoft.Cpp.Default.props" />
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|Win32'" Label="Configuration">
    <ConfigurationType>Makefile</ConfigurationType>
    <UseDebugLibraries>false</UseDebugLibraries>
    <PlatformToolset>$(DefaultPlatformToolset)</PlatformToolset>
  </PropertyGroup>
  <Import Project="$(VCTargetsPath)\\Microsoft.Cpp.props" />
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">
    <OutDir>{exe_dir}\\</OutDir>
    <IntDir>$(SolutionDir)vs\\</IntDir>
    <NMakeOutput>{exe_dir}\\{NAME}.exe</NMakeOutput>
    <NMakeBuildCommandLine>{escape(build_cmd)}</NMakeBuildCommandLine>
    <NMakeReBuildCommandLine>{escape(clean_cmd)} &amp;&amp; {escape(build_cmd)}</NMakeReBuildCommandLine>
    <NMakeCleanCommandLine>{escape(clean_cmd)}</NMakeCleanCommandLine>
    <NMakePreprocessorDefinitions>{escape(";".join(all_defines))}</NMakePreprocessorDefinitions>
    <NMakeIncludeSearchPath>{escape(";".join(all_includes))}</NMakeIncludeSearchPath>
    <AdditionalOptions>/std:c++20 /Zc:__cplusplus</AdditionalOptions>
    <IncludePath>{escape(system_includes)}</IncludePath>
    <ExternalIncludePath>{escape(system_includes)}</ExternalIncludePath>
    <LocalDebuggerCommand>{exe_dir}\\{NAME}.exe</LocalDebuggerCommand>
    <LocalDebuggerWorkingDirectory>{exe_dir}\\</LocalDebuggerWorkingDirectory>
    <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
  </PropertyGroup>
  <ItemGroup>
{chr(10).join(items)}
  </ItemGroup>
  <ItemGroup>
{chr(10).join(header_items)}
  </ItemGroup>
  <Import Project="$(VCTargetsPath)\\Microsoft.Cpp.targets" />
</Project>
"""
    (BUILD / f"{NAME}.vcxproj").write_text(project, encoding="utf-8-sig")

    # filters: mirror the folder tree
    files = [*units, *headers]
    folders = set()
    for f in files:
        parts = Path(f).parent.parts
        for i in range(1, len(parts) + 1):
            folders.add("\\".join(parts[:i]))
    filter_defs = "\n".join(
        f'    <Filter Include="{escape(f)}">\n      <UniqueIdentifier>{guid("filter:" + f)}</UniqueIdentifier>\n    </Filter>'
        for f in sorted(folders))

    def filter_item(tag, f):
        folder = "\\".join(Path(f).parent.parts)
        return f'    <{tag} Include="{escape(rel(f))}">\n      <Filter>{escape(folder)}</Filter>\n    </{tag}>'
    filters = f"""<?xml version="1.0" encoding="utf-8"?>
<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <ItemGroup>
{filter_defs}
  </ItemGroup>
  <ItemGroup>
{chr(10).join(filter_item("ClCompile", u) for u in sorted(units))}
  </ItemGroup>
  <ItemGroup>
{chr(10).join(filter_item("ClInclude", h) for h in headers)}
  </ItemGroup>
</Project>
"""
    (BUILD / f"{NAME}.vcxproj.filters").write_text(filters, encoding="utf-8-sig")

    project_guid = guid(NAME)
    solution = f"""
Microsoft Visual Studio Solution File, Format Version 12.00
# Visual Studio Version 16
VisualStudioVersion = 16.0.28701.123
MinimumVisualStudioVersion = 10.0.40219.1
Project("{{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}}") = "{NAME}", "{NAME}.vcxproj", "{project_guid}"
EndProject
Global
\tGlobalSection(SolutionConfigurationPlatforms) = preSolution
\t\tRelease|x86 = Release|x86
\tEndGlobalSection
\tGlobalSection(ProjectConfigurationPlatforms) = postSolution
\t\t{project_guid}.Release|x86.ActiveCfg = Release|Win32
\t\t{project_guid}.Release|x86.Build.0 = Release|Win32
\tEndGlobalSection
\tGlobalSection(SolutionProperties) = preSolution
\t\tHideSolutionNode = FALSE
\tEndGlobalSection
EndGlobal
"""
    (BUILD / f"{NAME}.sln").write_text(solution, encoding="utf-8-sig")
    return BUILD / f"{NAME}.sln", len(units), len(headers)


def main():
    config = json.loads((ROOT / "data" / "compile.json").read_text(encoding="utf-8"))
    units = build_exe.exe_units(config)
    tc = toolchain_root()
    BUILD.mkdir(exist_ok=True)
    db = compile_commands(config, units, tc)
    sln, n_units, n_headers = vcxproj(config, units, tc)
    print(f"wrote {sln} ({n_units} units, {n_headers} headers)")
    print(f"wrote {db}")


if __name__ == "__main__":
    main()
