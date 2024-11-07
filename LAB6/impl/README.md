## Overview

This `README` highlights features not in the original contract that were necessary to run the server and client architecture. All details mentioned in the question's `README` are implemented as is. Screenshot of the terminal windows for the given scneario are also attached.

## File structure

The files are arranged as follows, the files created by client and server are found in `client-files` and `server-files` folders respectively.
```
impl
├── server.c
└── client.c
├── client-files
└── server-files
```

## Server File Format

The file sent by the server follows the following protocol sequence:

```SOFT|<filename>.txt|<contents of the file>|EOFT```

**NOTE**: The `SOFT`, `<filename>.txt`, and `EOF` has to be sent in a single message as they are tokens. The contents can be sent in multiple messages.

 Messages have to be sent in the following order for complete file transfer, however messages can be broken into parts or sent as whole. The client handles this on its end.

1. **SOFT (Start of File Transfer)**: Marks the beginning of the file transmission. This is a fixed string indicating the start of the file. The string "SOFT" is followed by a delimiter ('|').

2. **Filename**: Specifies the name of the file being transmitted. This is a variable-length string that indicates the name of the file being transmitted. The filename is followed by a delimiter ('|').

3. **Messages**: Contains the content of the file being transmitted. This section includes the actual data of the file, which may be split into multiple messages. When the complete file contents are transferred, they are ended by the delimitter '|'.

4. **EOFT (End of File Transfer)**: Marks the end of the file transmission. This is a fixed string indicating the end of the file. The string "EOFT" is used to signify the end of the file.
