## Submission format

The directory structure of our submission is the following:

```txt
./
├── README.md         # This file =)
├── ex1
│   ├── ex1-video.mp4 # Video rendition of ex1
│   ├── main
│   │   └── main.ino  # C++ Arduino source code
│   └── plug.sh       # Optional: POSIX-sh script to compile, upload the sketch
├── ex2
│   ├── ex2-video.mp4 # Video rendition of ex1
│   ├── main
│   │   └── main.ino  # C++ Arduino source code
│   └── plug.sh       # Optional: POSIX-sh script to compile, upload the sketch
└── ex3
    ├── ex3-video.mp4 # Video rendition of ex1
    ├── main
    │   └── main.ino  # C++ Arduino source code
    └── plug.sh       # Optional: POSIX-sh script to compile, upload the sketch
```

### Submission Code: `HN1DS1`

### The `plug.sh` script

The script depends on the `arduino-cli` tool to compile and upload the sketches
to the Arduino board. The script is optional, but it is provided to simplify
the process of compiling and uploading the sketches. The script is POSIX-based
and can be executed on any POSIX-compliant shell. Indeed, `arduino-cli` must
be installed and available in the system's `PATH`.
