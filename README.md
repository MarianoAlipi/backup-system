# Mission 6

Work-in-progress mission 6.

Linux only.

## Files
- **client.c**: identifies files and directories in the current path (.) and saves them in a vector defined in **FileVector.h**. Detects creation, modification and deletion of files.
- **FileVector.h**: defines a custom vector `struct` and a file `struct` to keep track of every file.
- **FileSender.h**: establishes the connection to the server and sends the instructions along with the required files to do the backup.
- **server.c**: receives instructions from the client program (**test.c** at the time) and then receives the contents of files (for **create** and **modify** instructions) or deletes the specified file (for **delete** instruction).
- **test.c**: the file **FileSender.h** was based on. The user types in commands that are sent to **server.c** to update the backuped files.

### Client commands sent through **FileSender.h**
```
create:filename
modify:filename
delete:filename
```

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
Make sure you have gcc installed on your **Linux** machine:
```
sudo apt install gcc
```
In the same path as the **Makefile** file, run:
```
make
```

## Run instructions
Make sure to place the **client** executable and the **server** executable in different directories at the same hierarchy.
For example:
- files
  - serverFiles
    - server (executable)
  - clientFiles
    - client (executable)

### Client
Run:
```
./client
```

### Server
Run:
```
./server portNumber
```
The default port number on the client side is *1234*. Make sure to enter it or the program won't be able to communicate.