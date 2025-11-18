# xipslib

A small C library to apply **IPS (International Patching System)** patches to binary files on Xbox. It also supports creating backup copies of the original file before patching.

### Features

- Apply IPS patches to any binary file.
- Create backups of files before patching.

### Usage
**Creating a Backup**
```c
if (createBak(SRC_FILE, BAK_FILE)) {
  //Backup created successfully
} else {
  //Backup creation failed
}
```

**Applying an IPS Patch**
```c
if (applyIPS(IPS_FILE, SRC_FILE)) {
  //File patched successfully
} else {
  //File patching failed
}
```

### How it Works
- Verifies the patch file starts with the IPS magic bytes (PATCH).
- Reads offset (3 bytes) and size (2 bytes).
- Reads the patch data of specified size.
- Seeks to the offset in the target file and writes the patch data.
- Stops when it encounters the IPS EOF marker (EOF).

### TODO
- More testing
- Better error handling and reporting
- Add a patch memory function
- ???

### Credit
- [Rachmaninoff](https://github.com/src-dev)
- [CrunchBite](https://github.com/CrunchBite) (Helper functions in example.cpp)
- [Haguero](https://github.com/HoRnEyDvL) (Return to dashboard function in example.cpp)
