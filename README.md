# Mission 6

Work-in-progress mission 6.

Linux only.

## Files
- **client.c**: identifies files and directories in the current path (.) and saves them in a vector defined in **FileVector.h**. Detects creation, modification and deletion of files.
- **FileVector.h**: defines a custom vector `struct` and a file `struct` to keep track of every file.

## Logic
1. Identify initial files and directories (browse directories recursively too).
2. Save them in a vector.
3. Send information to server.
4. Apply changes in server.
5. Identify changes.
   - When an untracked file appears (**created**), add it to the vector.
   - When a tracked file disappears (**deleted**), delete it from the vector.
   - When a tracked file's modification date changes (**modified**), update it in the vector.
6. Send information to server.
7. Apply changes in server.
8. Go to step 5 until the user manually stops the program.

## Compile instructions
On a **Linux** system, run:
```
make
```

## Run instructions
On a **Linux** system, after compiling the program and being in the same path as the executable, run:
```
./client
```