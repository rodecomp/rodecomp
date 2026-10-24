## Clank Together

This tree is coordinated with Clank Together. Functions and data carry stable symbol IDs
instead of addresses (`// FUNCTION: fn_01234`, `// GLOBAL: gl_00042`); the coordinator's symbol database holds
their addresses in every client build, so a new client version needs new addresses, not a retagged tree. 
`decomp.json` and `config/compile.json` pin the VS compiler and the exact command line of every source.
See `AGENTS.md` for how to contribute.
