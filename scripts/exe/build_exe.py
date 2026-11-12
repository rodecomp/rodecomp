"""Build every unit in data/compile.json, link them into Ragexe.exe (+ .pdb, .map) and post-process it.

Optional and local: clank checks and merges never run this, and it does not
need td. It compiles with the exact compiler flags of data/compile.json using
the first MSVC v142 (14.29) + Windows SDK 10.0.19041 toolchain it finds:

  1. --toolchain DIR, or the RODECOMP_TOOLCHAIN environment variable: a
     directory laid out like td's toolchain (VC/Tools/MSVC/14.29.30133 and
     Program Files/Windows Kits/10).
  2. .td/config.json, when `td setup` / `td toolchain install` ran here.
  3. A regular Visual Studio / Build Tools install found with vswhere, with
     the "MSVC v142 - VS 2019 C++ x64/x86 build tools (v14.29-16.11)" and
     "Windows 10 SDK (10.0.19041.0)" components:
         vs_buildtools.exe --add Microsoft.VisualStudio.Component.VC.14.29.16.11.x86.x64
                           --add Microsoft.VisualStudio.Component.Windows10SDK.19041

Vendor DLL imports (Bink, Granny, Stove, ...) come from scripts/exe/imports/*.def,
the exact import table of the original executable, so a declaration that does
not match the real DLL export fails at link time.

Compiles are incremental: a unit is rebuilt only when its flags, its source or
one of the headers it included (recorded with /showIncludes) changed; a project
file whose mtime moved but whose content did not (checkout, branch switch) does
not count. Objects go through a machine-wide cache (~/.clank/cache/exe, native
toolchains only; RODECOMP_OBJECT_CACHE=off or =<dir>), so a fresh worktree reuses
what another checkout compiled. Compilers take machine-wide slots shared with td
(~/.clank/slots; TD_COMPILE_SLOTS, default one per CPU), so several builds at
once do not oversubscribe the machine. Nothing recompiled and an exe from the
same link inputs present: the link is skipped.

Pipeline, in this order:
  1. compile   every unit of data/compile.json (+ EXE_ONLY_UNITS)
  2. link      Ragexe.exe, Ragexe.pdb, Ragexe.map
  3. patch     scripts/exe/postprocess.py applies scripts/exe/patches/NNNN_*.py in order: byte edits the
               shipped exe received after it was compiled, which no C++ source reproduces. Skipped with
               --no-postprocess (gives the exe exactly as the source compiles).

    python scripts/exe/build_exe.py                     # compile + link + patch, fail on any error
    python scripts/exe/build_exe.py --force-multiple    # accept duplicate definitions
    python scripts/exe/build_exe.py --link-only         # relink objects from a previous run (+ patch)
    python scripts/exe/build_exe.py --no-postprocess    # leave the linked exe unpatched
    python scripts/exe/build_exe.py --toolchain DIR     # use this toolchain directory
"""

import argparse
import concurrent.futures
import hashlib
import json
import os
import re
import shutil
import subprocess
import sys
import threading
import time
from collections import Counter
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import postprocess  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent.parent
EXE_DIR = Path(__file__).resolve().parent
INCLUDE_NOTE = "Note: including file:"
MSVC_VERSION = "14.29.30133"
SDK_VERSION = "10.0.19041.0"
SYSTEM_LIBS = [
    "kernel32.lib", "user32.lib", "gdi32.lib", "advapi32.lib", "winmm.lib", "ws2_32.lib",
    "ole32.lib", "oleaut32.lib", "uuid.lib", "shell32.lib", "imm32.lib", "version.lib",
    "comctl32.lib", "comdlg32.lib", "shlwapi.lib", "wininet.lib", "gdiplus.lib", "bcrypt.lib",
    "dxguid.lib", "ddraw.lib", "d3d9.lib", "dinput8.lib", "dbghelp.lib", "crypt32.lib",
    "iphlpapi.lib", "psapi.lib", "wintrust.lib", "winhttp.lib", "netapi32.lib",
]


class ToolchainNotFound(Exception):
    pass


class Toolchain:
    """An MSVC v142 + Windows SDK pair, run natively on Windows or through wine elsewhere.

    vc is the directory holding Tools/MSVC/<version> (a Visual Studio "VC" folder),
    kits the Windows Kits/10 directory; td's toolchain keeps both under one root.
    """

    def __init__(self, vc, kits, kind, origin, env=None):
        self.vc = Path(vc)
        self.kits = Path(kits)
        self.kind = kind
        self.origin = origin
        self.msvc = self.vc / "Tools" / "MSVC" / MSVC_VERSION
        self.bin = self.msvc / "bin" / "Hostx64" / "x86"
        self.env = dict(os.environ)
        self.env.update(env or {})

    def problems(self, link=True):
        """What this toolchain lacks to compile (and, with link, to link) the exe."""
        missing = []
        if not (self.bin / "cl.exe").exists():
            missing.append(str(self.bin / "cl.exe"))
        if not (self.kits / "Include" / SDK_VERSION / "um" / "windows.h").exists():
            missing.append(str(self.kits / "Include" / SDK_VERSION / "um" / "windows.h"))
        if link:
            if not (self.msvc / "lib" / "x86" / "msvcrt.lib").exists():
                missing.append(str(self.msvc / "lib" / "x86" / "msvcrt.lib"))
            if not (self.kits / "Lib" / SDK_VERSION / "um" / "x86" / "kernel32.Lib").exists():
                missing.append(str(self.kits / "Lib" / SDK_VERSION / "um" / "x86" / "kernel32.Lib"))
        return missing

    def expand(self, value, native_paths=False):
        """Substitute data/compile.json's ${TC}/${SRC} placeholders with this toolchain's paths.

        With td's single-root layout ${TC} is that root; with a Visual Studio install the VC and
        Windows Kits directories live apart, so ${TC}<sep>VC and ${TC}<sep>Program Files<sep>Windows Kits<sep>10
        are replaced separately.
        """
        path = (lambda p: str(p)) if native_paths else self.path
        root = self.vc.parent
        if self.kits != root / "Program Files" / "Windows Kits" / "10":
            for sep in ("\\", "/"):
                value = value.replace(f"${{TC}}{sep}Program Files{sep}Windows Kits{sep}10", path(self.kits))
                value = value.replace(f"${{TC}}{sep}VC", path(self.vc))
        value = value.replace("${TC}", path(root))
        source = str(ROOT) if native_paths else self.path(ROOT)
        return value.replace("${SRC}", source.replace("\\", "/"))

    def path(self, p):
        if self.kind == "native":
            return str(p)
        return subprocess.run(["winepath", "-w", str(p)], capture_output=True, text=True, check=True).stdout.strip()

    def lib_env(self):
        sdk_lib = self.kits / "Lib" / SDK_VERSION
        return ";".join(self.path(p) for p in (
            self.msvc / "lib" / "x86", self.msvc / "atlmfc" / "lib" / "x86", sdk_lib / "ucrt" / "x86", sdk_lib / "um" / "x86"))

    def run(self, tool, args, env=None, cwd=None):
        exe = str(self.bin / tool)
        command = [exe, *args] if self.kind == "native" else ["wine", exe, *args]
        return subprocess.run(command, env=env or self.env, cwd=cwd, capture_output=True, text=True, errors="replace")


def _layout(root, kind, origin, env=None):
    """td's single-root layout: <root>/VC and <root>/Program Files/Windows Kits/10."""
    root = Path(root)
    return Toolchain(root / "VC", root / "Program Files" / "Windows Kits" / "10", kind, origin, env)


def _from_td():
    try:
        config = json.loads((ROOT / ".td" / "config.json").read_text(encoding="utf-8"))
        runner = config["runners"]["ragnarok"]
        return _layout(runner["toolchainRoot"], runner["type"], ".td/config.json", runner.get("env", {}))
    except (OSError, KeyError, ValueError, TypeError):
        return None


def _windows_kits_roots():
    roots = []
    try:
        import winreg
        for view in (winreg.KEY_WOW64_32KEY, winreg.KEY_WOW64_64KEY):
            try:
                with winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE, r"SOFTWARE\Microsoft\Windows Kits\Installed Roots",
                                    0, winreg.KEY_READ | view) as key:
                    roots.append(Path(winreg.QueryValueEx(key, "KitsRoot10")[0]))
            except OSError:
                pass
    except ImportError:
        pass
    for base in (os.environ.get("ProgramFiles(x86)"), os.environ.get("ProgramFiles")):
        if base:
            roots.append(Path(base) / "Windows Kits" / "10")
    return list(dict.fromkeys(roots))


def _from_visual_studio(link):
    if os.name != "nt":
        return None
    kits = _windows_kits_roots()
    for vc in (path / "VC" for path in _vs_installs()):
        for kit in kits:
            candidate = Toolchain(vc, kit, "native", f"Visual Studio ({vc.parent})")
            if not candidate.problems(link):
                return candidate
    return None


VS_COMPONENTS = ["Microsoft.VisualStudio.Component.VC.14.29.16.11.x86.x64", "Microsoft.VisualStudio.Component.Windows10SDK.19041"]
BUILD_TOOLS_URL = "https://aka.ms/vs/17/release/vs_buildtools.exe"


def _vs_installs():
    base = os.environ.get("ProgramFiles(x86)") or r"C:\Program Files (x86)"
    vswhere = Path(base) / "Microsoft Visual Studio" / "Installer" / "vswhere.exe"
    if not vswhere.exists():
        return []
    result = subprocess.run([str(vswhere), "-all", "-products", "*", "-format", "json", "-utf8"],
                            capture_output=True, text=True, encoding="utf-8", errors="replace")
    try:
        return [Path(i["installationPath"]) for i in json.loads(result.stdout or "[]") if i.get("installationPath")]
    except ValueError:
        return []


def install_build_tools():
    """Add the MSVC v142 14.29 + Windows SDK 19041 components with the Visual Studio Installer.

    Modifies an existing Visual Studio / Build Tools instance when there is one, otherwise downloads the
    Build Tools bootstrapper and installs a new instance. The installer asks for elevation (UAC) itself.
    """
    add = [arg for component in VS_COMPONENTS for arg in ("--add", component)]
    installs = _vs_installs()
    installer = Path(os.environ.get("ProgramFiles(x86)") or r"C:\Program Files (x86)") / "Microsoft Visual Studio" / "Installer" / "setup.exe"
    if installs and installer.exists():
        command = [str(installer), "modify", "--installPath", str(installs[0]), *add, "--passive", "--norestart"]
        print(f"adding the components to {installs[0]} with the Visual Studio Installer...", flush=True)
    else:
        import tempfile
        import urllib.request
        bootstrapper = Path(tempfile.gettempdir()) / "vs_buildtools.exe"
        print(f"downloading {BUILD_TOOLS_URL}...", flush=True)
        urllib.request.urlretrieve(BUILD_TOOLS_URL, bootstrapper)
        command = [str(bootstrapper), *add, "--passive", "--wait", "--norestart"]
        print("installing Visual Studio Build Tools (several GB, this takes a while)...", flush=True)
    code = subprocess.run(command).returncode
    # 3010: success, restart required
    if code not in (0, 3010):
        raise ToolchainNotFound(f"the Visual Studio Installer failed with exit code {code}")


def _not_found_message(td, link):
    hint = ""
    if td is not None:
        hint = f"\n.td/config.json points to {td.vc.parent}, which lacks:\n  " + "\n  ".join(td.problems(link))
    return (
        f"no MSVC {MSVC_VERSION} (v142) + Windows SDK {SDK_VERSION} toolchain found.{hint}\n"
        "Either install them with the Visual Studio Installer (Build Tools 2019 or later):\n"
        f"  vs_buildtools.exe --add {VS_COMPONENTS[0]} --add {VS_COMPONENTS[1]}\n"
        "or pass --toolchain DIR / set RODECOMP_TOOLCHAIN to a td-layout toolchain directory\n"
        "(e.g. one installed by `td toolchain install`).")


def find_toolchain(explicit=None, link=True, offer_install=True):
    """The first usable toolchain: explicit path / RODECOMP_TOOLCHAIN, then .td/config.json, then Visual Studio.

    Without one, on an interactive Windows console, offer to install the Build Tools components.
    """
    explicit = explicit or os.environ.get("RODECOMP_TOOLCHAIN")
    if explicit:
        tc = _layout(explicit, "native" if os.name == "nt" else "wine", f"--toolchain/RODECOMP_TOOLCHAIN ({explicit})")
        missing = tc.problems(link)
        if missing:
            raise ToolchainNotFound(f"{explicit} is not a usable toolchain, missing:\n  " + "\n  ".join(missing))
        return tc

    td = _from_td()
    if td is not None:
        if td.kind not in ("native", "wine"):
            raise ToolchainNotFound(f"td runner '{td.kind}' is not supported; use td's native (Windows) or wine runner")
        if not td.problems(link):
            return td

    vs = _from_visual_studio(link)
    if vs is not None:
        return vs

    message = _not_found_message(td, link)
    if not (offer_install and os.name == "nt" and sys.stdin.isatty()):
        raise ToolchainNotFound(message)
    print(message, file=sys.stderr)
    try:
        answer = input(f"\nInstall MSVC {MSVC_VERSION} + Windows SDK {SDK_VERSION} now with the Visual Studio Installer? [y/N] ")
    except EOFError:
        answer = ""
    if answer.strip().lower() not in ("y", "yes"):
        raise ToolchainNotFound("toolchain not installed")
    install_build_tools()
    vs = _from_visual_studio(link)
    if vs is None:
        raise ToolchainNotFound("the installer finished but the toolchain is still not found:\n" + message)
    return vs


def build_import_libs(tc, out_dir):
    lib_dir = out_dir / "importlibs"
    lib_dir.mkdir(exist_ok=True)
    libs = []
    for definition in sorted((EXE_DIR / "imports").glob("*.def")):
        lib = lib_dir / (definition.stem + ".lib")
        result = tc.run("lib.exe", ["/nologo", "/MACHINE:X86", f"/DEF:{tc.path(definition)}", f"/OUT:{tc.path(lib)}"])
        if result.returncode != 0:
            sys.exit(f"import library {definition.name} failed:\n{result.stdout}{result.stderr}")
        libs.append(lib)
    return libs

# Units only the standalone exe needs: stand-ins for third-party code the
# decompilation does not reproduce. Each is compiled with the flags of the
# listed data/compile.json unit plus any extra include directories (relative to
# the repository). Clank does not know about these files.
EXE_ONLY_UNITS = {
    "Recovered/nProtect/GameGuard.cpp": ("Source/Security/NProtectGameGuardMgr.cpp", []),
    "Recovered/jsoncons/JsonCppLoaderGlue.cpp": ("Source/Resource/Json/JsonCppLoader.cpp", ["Recovered/jsoncons/include"]),
    "Recovered/GPak/PakAuxiliary.cpp": ("Source/Archive/GPak.cpp", []),
    "Recovered/GGHash/GGHash.cpp": ("Source/Utility/Hash/GGHash.cpp", []),
}


def exe_units(config):
    """unit -> (shape index, extra include directories)."""
    units = {unit: (shape, []) for unit, shape in config["units"].items()}
    for unit, (flags_of, includes) in EXE_ONLY_UNITS.items():
        units[unit] = (config["units"][flags_of], includes)
    return units


def unit_prelude(unit):
    """The unit's linkage prelude (data/decomp.json unitPrelude), forced in with /FI, or None."""
    project = json.loads((ROOT / "data" / "decomp.json").read_text(encoding="utf-8"))
    directory = project.get("unitPrelude")
    if not directory:
        return None
    path = ROOT / directory / f"{unit}.h"
    return path if path.is_file() else None


def _alive(pid):
    """Whether process pid still runs (os.kill(pid, 0) would send CTRL_C_EVENT on Windows)."""
    if pid == os.getpid():
        return True
    if os.name == "nt":
        import ctypes
        kernel32 = ctypes.WinDLL("kernel32", use_last_error=True)
        handle = kernel32.OpenProcess(0x1000, False, pid)  # PROCESS_QUERY_LIMITED_INFORMATION
        if not handle:
            return ctypes.get_last_error() == 5  # access denied: it exists
        code = ctypes.c_ulong()
        ok = kernel32.GetExitCodeProcess(handle, ctypes.byref(code))
        kernel32.CloseHandle(handle)
        return bool(ok) and code.value == 259  # STILL_ACTIVE
    try:
        os.kill(pid, 0)
        return True
    except PermissionError:
        return True
    except OSError:
        return False


class Slots:
    """Machine-wide limit on concurrent compilers, shared with td (clank-together src/shared/slots.js).

    A slot is ~/.clank/slots/<n>.slot, created exclusively, holding "<pid> <ms since epoch>"; one
    whose process is gone or that is older than 30 minutes is taken over. TD_COMPILE_SLOTS sets the
    count (default: one per logical CPU), so several agents building at once share the cores.
    """

    STALE_MS = 30 * 60_000

    def __init__(self):
        self.dir = Path.home() / ".clank" / "slots"
        self.dir.mkdir(parents=True, exist_ok=True)
        try:
            self.count = max(1, int(os.environ.get("TD_COMPILE_SLOTS", "")))
        except ValueError:
            self.count = os.cpu_count() or 1

    def _stale(self, path):
        try:
            text = path.read_text().split()
            pid, since = int(text[0]), int(text[1])
        except (OSError, ValueError, IndexError):
            try:
                return time.time() * 1000 - path.stat().st_mtime * 1000 > self.STALE_MS
            except OSError:
                return False
        return not _alive(pid) or time.time() * 1000 - since > self.STALE_MS

    def acquire(self):
        while True:
            for i in range(self.count):
                path = self.dir / f"{i}.slot"
                try:
                    fd = os.open(path, os.O_CREAT | os.O_EXCL | os.O_WRONLY)
                except FileExistsError:
                    if self._stale(path):
                        try:
                            path.unlink(missing_ok=True)
                        except PermissionError:
                            pass
                    continue
                with os.fdopen(fd, "w") as f:
                    f.write(f"{os.getpid()} {int(time.time() * 1000)}\n")
                return path
            time.sleep(0.05)

    @staticmethod
    def release(path):
        for _ in range(100):
            try:
                path.unlink(missing_ok=True)
                return
            except PermissionError:
                time.sleep(0.05)


def _sha(data):
    return hashlib.sha256(data).hexdigest()


class ObjectCache:
    """Machine-wide cache of compiled objects, shared by every checkout and worktree.

    Like ccache's direct mode: the manifest key covers the compiler, the arguments (with this
    checkout's root replaced by a placeholder and the output path left out) and the text of the
    unit and its prelude; each manifest entry holds the content hash of every project header the
    unit included. Toolchain headers are covered by the compiler path (toolchain directories do not
    change in place). The object's debug info keeps the source paths of the checkout that compiled
    it; code and data are the same.

    RODECOMP_OBJECT_CACHE=off disables it, =<dir> moves it (default ~/.clank/cache/exe).
    """

    MAX_ENTRIES = 8

    def __init__(self, tc, hash_file):
        setting = os.environ.get("RODECOMP_OBJECT_CACHE", "")
        # Native only: under wine /showIncludes names headers by Z:\ paths, which the dependency
        # hashes here cannot resolve, and a header the entry does not list would go unchecked.
        off = setting in ("off", "0") or tc.kind != "native"
        self.dir = None if off else Path(setting or Path.home() / ".clank" / "cache" / "exe")
        self.hash_file = hash_file
        self.root = str(ROOT)
        self.troot = tc.path(ROOT)
        cl = tc.bin / "cl.exe"
        info = cl.stat()
        self.compiler = f"{cl}\0{info.st_size}\0{info.st_mtime_ns}"

    def _project(self, path):
        return path.lower().startswith(self.root.lower())

    def key(self, args, unit, prelude, env):
        norm = [a.replace(self.troot, "${ROOT}").replace(self.root, "${ROOT}") for a in args if not a.startswith("/Fo")]
        parts = [self.compiler, json.dumps(env, sort_keys=True), *norm, unit, self.hash_file(str(ROOT / unit)),
                 str(prelude and prelude.relative_to(ROOT)), prelude and self.hash_file(str(prelude))]
        return _sha("\0".join(str(p) for p in parts).encode())

    def _manifest(self, key):
        return self.dir / "manifests" / key[:2] / f"{key}.json"

    def _entries(self, key):
        try:
            entries = json.loads(self._manifest(key).read_text(encoding="utf-8"))
            return entries if isinstance(entries, list) else []
        except (OSError, ValueError):
            return []

    def restore(self, key, obj):
        """Copies a cached object to obj; returns its project headers (repo-relative) or None."""
        if not self.dir:
            return None
        for entry in self._entries(key):
            if all(self.hash_file(str(ROOT / rel)) == h for rel, h in entry["deps"].items()):
                src = self.dir / "objects" / entry["out"][:2] / f"{entry['out']}.obj"
                try:
                    shutil.copyfile(src, obj)
                    os.utime(src)  # LRU mark
                except OSError:
                    continue
                return [str(ROOT / rel) for rel in entry["deps"]]
        return None

    def store(self, key, obj, headers):
        if not self.dir:
            return
        try:
            deps = {}
            for path in headers:
                if not self._project(path):
                    continue
                h = self.hash_file(path)
                if h is None:
                    return
                deps[os.path.relpath(path, self.root).replace("\\", "/")] = h
            data = obj.read_bytes()
            out = _sha(data)
            dst = self.dir / "objects" / out[:2] / f"{out}.obj"
            if not dst.exists():
                _write_atomic(dst, data)
            entries = [e for e in self._entries(key) if e["out"] != out and e["deps"] != deps]
            _write_atomic(self._manifest(key), json.dumps([{"out": out, "deps": deps}, *entries][:self.MAX_ENTRIES]).encode())
        except OSError:
            pass  # a full disk or a racing writer must not fail the build


def _write_atomic(path, data):
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = path.with_name(f"{path.name}.{os.getpid()}.{threading.get_ident()}.tmp")
    tmp.write_bytes(data)
    try:
        os.replace(tmp, path)
    except OSError:
        tmp.unlink(missing_ok=True)  # Windows: another process holds the target open; its copy is as good


def compile_units(tc, config, out_dir, jobs):
    env = dict(tc.env)
    for key, value in config["env"].items():
        env[key] = tc.expand(value)

    # Memoized per run: a header is included by hundreds of units, and the up-to-date test used
    # to stat it once per unit (265k stats for ~3k files).
    stamps = {}
    hashes = {}

    def stamp(path):
        if path not in stamps:
            try:
                info = os.stat(path)
                stamps[path] = [info.st_mtime_ns, info.st_size]
            except OSError:
                stamps[path] = None
        return stamps[path]

    def hash_file(path):
        if path not in hashes:
            try:
                hashes[path] = _sha(Path(path).read_bytes())
            except OSError:
                hashes[path] = None
        return hashes[path]

    def unchanged(path, saved):
        # saved: [mtime_ns, size] (+ content hash for project files). A checkout or branch switch
        # touches mtimes without changing text; the hash keeps those files from forcing a rebuild.
        now = stamp(path)
        if now is None:
            return False
        if now == saved[:2]:
            return True
        return len(saved) > 2 and now[1] == saved[1] and hash_file(path) == saved[2]

    def record_dep(path):
        s = stamp(path)
        if s is None:
            return None
        return [*s, hash_file(path)] if path.lower().startswith(str(ROOT).lower()) else s

    def up_to_date(obj, record, key):
        if not obj.exists() or record is None or record.get("key") != key:
            return False
        return all(unchanged(path, saved) for path, saved in record["deps"].items())

    cache = ObjectCache(tc, hash_file)
    slots = Slots()

    def build(item):
        unit, (shape, includes) = item
        obj = out_dir / (unit.replace("/", "_") + ".obj")
        deps_file = obj.with_suffix(".deps.json")
        extra = [f"/I{tc.path(ROOT / directory)}" for directory in includes]
        prelude = unit_prelude(unit)
        forced = [f"/FI{tc.path(prelude)}"] if prelude else []
        args = [*(tc.expand(a) for a in config["shapes"][shape]), *extra, *forced, "/Z7", f"/Fo{tc.path(obj)}", tc.path(ROOT / unit)]
        key = "\x00".join(args)
        try:
            record = json.loads(deps_file.read_text(encoding="utf-8"))
        except (OSError, ValueError):
            record = None
        if up_to_date(obj, record, key):
            return unit, obj, 0, "", "up to date"
        deps_file.unlink(missing_ok=True)
        cache_key = cache.key(args, unit, prelude, config["env"])
        headers = cache.restore(cache_key, obj)
        if headers is not None:
            # Toolchain headers are not in the cache entry; the compiler path in the key covers them.
            deps = {path: record_dep(path) for path in [str(ROOT / unit), *([str(prelude)] if prelude else []), *headers]}
            if None not in deps.values():
                deps_file.write_text(json.dumps({"key": key, "deps": deps}), encoding="utf-8")
            return unit, obj, 0, "", "cached"
        slot = slots.acquire()
        try:
            result = tc.run("cl.exe", [*args, "/showIncludes"], env=env, cwd=ROOT)
        finally:
            slots.release(slot)
        output = []
        headers = []
        for line in (result.stdout + result.stderr).splitlines():
            if line.startswith(INCLUDE_NOTE):
                headers.append(line[len(INCLUDE_NOTE):].strip())
            else:
                output.append(line)
        if result.returncode == 0:
            deps = {path: record_dep(path) for path in [str(ROOT / unit), *headers]}
            if None not in deps.values():
                deps_file.write_text(json.dumps({"key": key, "deps": deps}), encoding="utf-8")
            cache.store(cache_key, obj, headers)
        return unit, obj, result.returncode, "\n".join(output), "compiled"

    items = list(exe_units(config).items())
    results = [None] * len(items)
    progress = Progress(len(items))
    with concurrent.futures.ThreadPoolExecutor(jobs) as pool:
        pending = {pool.submit(build, item): index for index, item in enumerate(items)}
        for future in concurrent.futures.as_completed(pending):
            result = future.result()
            results[pending[future]] = result
            progress.update(result)
    progress.finish()
    failures = [r for r in results if r[2] != 0]
    for unit, _, _, output, _ in failures:
        print(f"== {unit}")
        print("\n".join(line for line in output.splitlines() if " error " in line or "fatal error" in line))
    how = Counter(r[4] for r in results)
    print(f"compiled {len(results) - len(failures)}/{len(results)} units ({how['up to date']} up to date, "
          f"{how['cached']} from the object cache, {how['compiled']} compiled)")
    return [r[1] for r in results if r[2] == 0], len(failures), how["up to date"] == len(results)


class Progress:
    """Compile progress on stderr: one rewritten line on a terminal, a line per 10% otherwise."""

    def __init__(self, total):
        self.total = total
        self.done = self.rebuilt = self.failed = 0
        self.tty = sys.stderr.isatty()
        self.next_step = 10
        self.width = 0

    def update(self, result):
        unit, _, code, _, how = result
        self.done += 1
        self.rebuilt += 1 if how == "compiled" else 0
        self.failed += 1 if code != 0 else 0
        counts = f"[{self.done}/{self.total}] {self.rebuilt} rebuilt" + (f", {self.failed} failed" if self.failed else "")
        if self.tty:
            line = f"{counts}  {unit}"
            self.width = max(self.width, len(line))
            sys.stderr.write("\r" + line.ljust(self.width))
            sys.stderr.flush()
        else:
            if code != 0:
                print(f"failed: {unit}", file=sys.stderr, flush=True)
            if self.done * 100 >= self.next_step * self.total:
                print(counts, file=sys.stderr, flush=True)
                while self.next_step * self.total <= self.done * 100:
                    self.next_step += 10

    def finish(self):
        if self.tty and self.width:
            sys.stderr.write("\r" + " " * self.width + "\r")
            sys.stderr.flush()


def link(tc, objects, out_dir, extra_args):
    rsp = out_dir / "link.rsp"
    rsp.write_text("\n".join([*(f'"{tc.path(o)}"' for o in objects), *extra_args]), encoding="mbcs" if sys.platform == "win32" else "utf-8")
    exe = out_dir / "Ragexe.exe"
    outputs = (exe, exe.with_suffix(".pdb"), exe.with_suffix(".map"))
    for stale in outputs:
        stale.unlink(missing_ok=True)
    args = ["/nologo", "/MACHINE:X86", "/SUBSYSTEM:WINDOWS", "/LARGEADDRESSAWARE",
            "/DEBUG:FULL", f"/PDB:{tc.path(exe.with_suffix('.pdb'))}", f"/MAP:{tc.path(exe.with_suffix('.map'))}",
            "/INCREMENTAL:NO", "/OPT:REF", "/OPT:ICF",
            "/ERRORLIMIT:0", f"/OUT:{tc.path(exe)}", f"@{tc.path(rsp)}", *SYSTEM_LIBS]
    result = tc.run("link.exe", args)
    if result.returncode != 0:
        for partial in outputs:
            partial.unlink(missing_ok=True)
    return result.returncode, result.stdout + result.stderr, exe


def summarize(log, out_dir):
    (out_dir / "link.log").write_text(log, encoding="utf-8")
    duplicates = sorted(set(re.findall(r'(?:error LNK2005|warning LNK4006): "([^"]+)"', log)))
    unresolved = sorted(set(a or b for a, b in re.findall(r'unresolved external symbol (?:"[^"]+" \((\S+)\)|(\S+))', log)))
    print(f"link log: {out_dir / 'link.log'}")
    print(f"duplicate definitions: {len(duplicates)}")
    for symbol in duplicates:
        print(f"  {symbol}")
    print(f"unresolved symbols: {len(unresolved)}")
    codes = Counter(re.findall(r"error (LNK\d+)", log))
    if codes:
        print("linker errors: " + ", ".join(f"{code} x{count}" for code, count in codes.most_common()))


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--out", default=str(ROOT / "build" / "exe"), help="output directory")
    parser.add_argument("--jobs", type=int, default=os.cpu_count(), help="parallel compiles")
    parser.add_argument("--force-multiple", action="store_true",
                        help="link despite duplicate definitions (/FORCE:MULTIPLE)")
    parser.add_argument("--link-only", action="store_true", help="reuse the objects from a previous run")
    parser.add_argument("--toolchain", help="toolchain directory (td layout); default: RODECOMP_TOOLCHAIN, "
                                            ".td/config.json, then a Visual Studio install")
    parser.add_argument("--no-postprocess", action="store_true",
                        help="skip scripts/exe/postprocess.py (binary patches of the shipped exe)")
    args = parser.parse_args()

    out_dir = Path(args.out)
    out_dir.mkdir(parents=True, exist_ok=True)
    config = json.loads((ROOT / "data" / "compile.json").read_text(encoding="utf-8"))
    try:
        tc = find_toolchain(args.toolchain)
    except ToolchainNotFound as error:
        sys.exit(str(error))
    tc.env["LIB"] = tc.lib_env()
    print(f"toolchain: {tc.msvc} + {tc.kits} ({tc.kind}, from {tc.origin})", flush=True)

    all_up_to_date = False
    if args.link_only:
        objects = [out_dir / (unit.replace("/", "_") + ".obj") for unit in exe_units(config)]
        compile_failures = 0
    else:
        objects, compile_failures, all_up_to_date = compile_units(tc, config, out_dir, args.jobs)

    extra = ["/FORCE:MULTIPLE"] if args.force_multiple else []
    # Nothing recompiled and the exe of the last link is still there, made from the same link
    # inputs: linking again would give the same file.
    exe = out_dir / "Ragexe.exe"
    stamp_file = out_dir / "link.stamp.json"
    inputs = link_inputs(objects, extra, args.no_postprocess)
    try:
        previous = json.loads(stamp_file.read_text(encoding="utf-8"))
    except (OSError, ValueError):
        previous = None
    if (all_up_to_date and not compile_failures and exe.exists() and previous
            and previous["inputs"] == inputs and previous["exe"] == exe.stat().st_mtime_ns):
        print(f"up to date: {exe}")
        sys.exit(0)
    stamp_file.unlink(missing_ok=True)

    print(f"linking {len(objects)} objects...", flush=True)
    code, log, exe = link(tc, [*objects, *build_import_libs(tc, out_dir)], out_dir, extra)
    summarize(log, out_dir)
    if code == 0:
        print(f"built {exe}")
        if not args.no_postprocess:
            try:
                postprocess.run(exe)
            except postprocess.PatchError as error:
                exe.unlink(missing_ok=True)
                sys.exit(f"postprocess failed, removed {exe}: {error}")
        if not compile_failures:
            stamp_file.write_text(json.dumps({"inputs": inputs, "exe": exe.stat().st_mtime_ns}), encoding="utf-8")
    sys.exit(1 if code != 0 or compile_failures else 0)


def link_inputs(objects, extra, no_postprocess):
    """What decides the linked exe besides the objects' contents: object list, flags, import .def
    files, this script and the post-link patches (by content)."""
    files = [Path(__file__), *sorted((EXE_DIR / "imports").glob("*.def")), *sorted((EXE_DIR / "patches").glob("*.py")),
             EXE_DIR / "postprocess.py"]
    digest = hashlib.sha256()
    for f in files:
        digest.update(f.name.encode() + b"\0" + f.read_bytes() + b"\0")
    return {"objects": [str(o) for o in objects], "extra": extra, "postprocess": not no_postprocess, "files": digest.hexdigest()}


if __name__ == "__main__":
    main()
