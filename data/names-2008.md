# Names from the 2008 client

`names-2008.tsv` pairs 1514 functions of the target with the name the same function has in a 2008 build of the client whose debug information names every function. It is a list to take names from, nothing more: no code, no types and no file of that build are here, and nothing in the tree depends on it.

| Column | Meaning |
|---|---|
| `id`, `addr` | The function in this project's database and its address in the target. |
| `name` | Its name in the tree when the list was made (may be empty). |
| `name_2008` | Its name in the 2008 build. |
| `evidence` | How the two were paired (below). |
| `confidence` | `high` (1363), `moderate` (137) or `low` (14). |

How the pairs were found, strongest first:

- **vtable slot** (high): both builds have the class's vtable, and the functions sit in the same slot once the slots added since 2008 are accounted for. `voted`: several classes of the family agree on the slot mapping; `pattern`: the mapping follows from the family's slot pattern; `between`: the slot lies between two mapped ones with nothing added; `(shared)`: one function fills the slot for several classes. `against its strings`: the strings pointed at a sibling class's function and the slot corrected it.
- **strings** (high): the two functions refer to the same string literals and no other function of either build does. 8 of the 291 pairs found this way were wrong (strings shared by sibling classes); the vtables exposed them and they are listed under their vtable evidence instead.
- **calls** (moderate): the function is called at the same place in the call sequence of two functions already paired. `one vote`: only one such caller.
- **vtable writer** (low): the one remaining function that stores a class's vtable pointer, taken as its constructor or destructor. Inlined constructors make this unreliable: 2 of 3 held in the check below.

Check: with 30% of the string pairs withheld (five draws), the vtable slots paired 108 of the withheld functions again, 97 of them with the function their strings gave; the other 11 were the wrong string pairs. Calls paired 11 again, all the same.

Where the tree's name differs from the 2008 one, the 2008 one is the original: for instance every virtual `Paint` is `OnDraw` there, `Create` is `Clone`, `Unload` is `Reset`. 18 years and a change of compiler lie between the builds, so a name says what the function was, not that its body is unchanged.
