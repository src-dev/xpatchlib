# xipslib

A small C library to apply **IPS (International Patching System)** patches to binary files on Xbox. It also supports creating backup copies of the original file before patching.

### Features

- Apply IPS patches to any binary file.
- Create backups of files before patching.

### Usage
**Creating a Backup**
```c
switch (createBak(SRC_FILE, BAK_EXT, bool true)) {
  case E_NO_ERROR:
    //Backup created successfully
    break;

  default:
    //Backup creation failed
}
```

**Restoring a Backup**
```c
switch (restoreBak(SRC_FILE, BAK_EXT, bool true)) {
  case E_NO_ERROR:
    //Backup restored successfully
    break;

  default:
    //Backup restoration failed
}
```

**Applying an IPS Patch**
```c
switch (applyIPS(IPS_FILE, SRC_FILE)) {
  case E_NO_ERROR:
    //File patched successfully
    break;

  default:
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
- Add a patch memory function
- ???

### Credit
- [Rachmaninoff](https://github.com/src-dev)
- [CrunchBite](https://github.com/CrunchBite) (Helper functions in example.cpp)
- [Haguero](https://github.com/HoRnEyDvL) (Return to dashboard function in example.cpp)
