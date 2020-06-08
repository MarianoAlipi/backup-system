# Backup system

These two programs work as a backup system.
The client tracks files and subdirectories and sends them to the server, which replicates the changes.

This program is designed to run on Linux machines.

## Files
- **client.c**: identifies files and directories in the current path and saves them in a vector defined in **FileVector.h**. It detects creation, modification and deletion of files.
- **FileVector.h**: defines a custom vector `struct` and a file `struct` to keep track of every file and subdirectory.
- **FileSender.h**: establishes the connection to the server and sends the instructions along with the required files to do the backup.
- **server.c**: receives instructions from the client program (**client.c**) and then receives the contents of files (for **create** and **modify** instructions) or deletes the specified file (for the **delete** instruction).

## Logic
1. Identify initial files and subdirectories.
2. Track the files and subdirectories in a vector.
3. Send information to server.
4. Apply changes in server.
5. Identify changes.
   - When an untracked file or subdirectory appears (**created**), add it to the vector.
   - When a tracked file or subdirectory disappears (**deleted**), delete it from the vector.
   - When a tracked file's modification date changes (**modified**), update it in the vector.
6. Send information to server.
7. Apply changes in server.
8. Go to step 5 until the user manually stops the program.

## Build instructions
Make sure you have **gcc** installed on your **Linux** machine:
```
sudo apt install gcc
```
In the same path as the **Makefile** file, run:
```
make
```
If you don't have **make** installed, compile both files manually:
```
gcc client.c -o client
gcc server.c -o server
```

## Execution instructions
If you are running both programs on the same computer, make sure to place the **client** executable and the **server** executable in different directories at the same hierarchy.
For example:
- files
  - serverFiles
    - server (executable)
  - clientFiles
    - client (executable)

### Server
Run:
```
./server portNumber
```

### Client
Run:
```
./client hostname portNumber
```
If you are running both programs on the same computer, you can type ```localhost``` or ```127.0.0.1``` for the hostname.
Make sure to enter the same port number for both programs.

## Usage instructions
1. Compile the programs.
2. Place each executable in a separate directory (or computer).
3. Run the **server** program.
4. Run the **client** program.
5. Make changes in the client program's path:
   - Add files.
   - Create subdirectories.
   - Modify files.
   - Delete files.
   - Delete subdirectories.
6. The server should mirror the changes.
7. To stop the programs, press <kbd>Ctrl</kbd> + <kbd>C</kbd> on the terminal running each one.