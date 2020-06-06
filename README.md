# Mission 6

Work-in-progress mission 6.

Linux only.

## Files
- **test.c**: identifies files and directories in the current path (.) and saves them in a vector defined in **FileVector.h**.
- **FileVector.h**: defines a custom vector structure and a file structure to keep track of every file.

## Logic
1. Identify initial files and directories (browse directories recursively too).
2. Save them in a vector.
3. Send information to remote client.
4. Apply changes in remote client.
5. Identify changes.
   - When an untracked file appears (**created**), add it to the vector.
   - When a tracked file disappears (**deleted**), delete it from the vector.
   - When a tracked file's modification date changes (**modified**), update it in the vector.
6. Send information to remote client.
7. Apply changes in remote client.
8. Go to step 5 until the user manually stops the program.

## Compile instructions
On a **Linux** system, run:
`make`