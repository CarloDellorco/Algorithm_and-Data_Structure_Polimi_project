# WordChecker

Final project for **Algorithms and Data Structures** (Politecnico di Milano, A.Y. 2021-2022).

The project is written in **C11** and uses only the C standard library. It reads from `stdin` and writes to `stdout`. No multithreading is used.

## The problem

WordChecker compares the letters of two words of the same length `k`, in a way similar to the game "Wordle". A word is a sequence of symbols from `a-z`, `A-Z`, `0-9`, `-` and `_`.

### Input

1. An integer `k`, the length of the words.
2. The set of **admissible words**, one per line, with no duplicates.
3. A sequence of commands and games.

| Command / line | Meaning |
|---|---|
| `+nuova_partita` | Starts a new game. |
| *(next line)* | Reference word `r`, which belongs to the set of admissible words. |
| *(next line)* | Maximum number `n` of words to compare. |
| *word* | Word `p` to compare against `r`. |
| `+stampa_filtrate` | Prints, in lexicographic (ASCII) order, the admissible words compatible with the constraints learned so far. |
| `+inserisci_inizio` / `+inserisci_fine` | Delimit a block of new words to add to the set of admissible words. They may appear during a game as well. |

### Output for each word `p` compared with `r`

- `not_exists` if `p` is not in the set of admissible words.
- `ok` if `p == r`. The game ends.
- Otherwise a string `res` of `k` characters, followed by the number of admissible words still compatible with the learned constraints:
  - `+` if `p[i] == r[i]` (correct position);
  - `/` if `p[i]` does not appear in `r`, or has already been matched the maximum number of times;
  - `|` if `p[i]` appears in `r` but in a different position.
- `ko` after the result of the `n`-th word, if none of them was equal to `r`.

### Example

```
r = abcabcabcabcabc
p = bbaabccbccbcabc
    /+|+++|++/+++++
```

After each comparison the program learns constraints: positions where a symbol must or must not appear, the minimum or exact number of occurrences, and symbols that are absent. These constraints are used to filter the admissible words.

## Repository structure

```
.
├── CMakeLists.txt      # build configuration
├── input_files/        # test inputs (test1.txt, ...)
├── expected_outputs/   # expected outputs (test1_output.txt, ...)
└── main.c
```

## Build

With CMake, from the project root:

```bash
cmake -S . -B build
cmake --build build
```

The executable is `build/Algorithm_project_2022` (`.exe` on Windows).

Direct compilation with `gcc`, using the flags required for the submission:

```bash
gcc -Wall -Werror -O2 -g3 source.c -o executable
```

## Running and testing

### Windows (PowerShell, also from VS Code)

```powershell
cmd /c "build\Algorithm_project_2022.exe < input_files\test1.txt > actual_output.txt"
fc.exe expected_outputs\test1_output.txt actual_output.txt
```

If `fc.exe` prints `FC: no differences encountered`, the output matches the expected one.

### Linux / macOS

```bash
./build/Algorithm_project_2022 < input_files/test1.txt > actual_output.txt
diff expected_outputs/test1_output.txt actual_output.txt
```

If `diff` prints nothing, the two files are identical.

## Debugging and profiling (optional)

```bash
# Memory access errors
gcc -g3 -fsanitize=address source.c -o executable

# Memory leaks
valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes ./executable < input_files/test1.txt

# Time per function / allocated memory
valgrind --tool=callgrind --callgrind-out-file=callgrind.out ./executable < input_files/test1.txt
valgrind --tool=massif --massif-out-file=massif.out ./executable < input_files/test1.txt
```

## Notes

Individual project developed for a university course.