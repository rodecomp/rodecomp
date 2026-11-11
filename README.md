<p align="center">
  <img src="docs/banner.jpg" alt="rodecomp" width="720">
</p>

<p align="center"><b>A matching decompilation of the Ragnarok Online client.</b></p>

## How to Build

```sh
python scripts/exe/gen_ide.py     # generate build/Ragexe.sln
python scripts/exe/build_exe.py    # or build the .sln -> build/exe/Ragexe.exe
```

`build_exe.py` runs three stages in order: **compile**, **link** (`Ragexe.exe`, `.pdb`, `.map`), then **patch**.
The patch stage (`scripts/exe/postprocess.py`) re-applies byte edits that the shipped executable received after
it was compiled and that no C++ source can express (e.g. a `jmp` written over a switch head). The source keeps
the code as it was compiled; each edit is one documented module in `scripts/exe/patches/NNNN_name.py`, applied
in `NNNN` order, located by symbol and checked against the exact expected bytes. A patch that does not fit fails
the build. `--no-postprocess` gives the exe exactly as the source compiles. Clank checks never see these patches.
