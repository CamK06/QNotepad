# QNotepad

![Screenshot](https://raw.githubusercontent.com/Starman0620/QNotepad/master/screenshot.png  "Screenshot")

  

A simple notepad program written in Qt, inspired by the Windows notepad.

Note: This is *very* early in development (~7 hours) as of writing this (Aug 29, 4AM EST), expect bugs.
  

# Install

After [building](https://github.com/Starman0620/QNotepad#build), run `sudo make install` in the build directory

  

# Build


## Linux
Ensure you have the necessary Qt dependencies before building. On Ubuntu and related distros, the `qt5-default` package should be enough.

Clone the repo:

```

git clone https://github.com/Starman0620/QNotepad --recursive

```

  

Build the code:

```

mkdir build && cd build

cmake ..

make

```

  

# Libraries
* [qt/qt5](https://github.com/qt/qt5)
* [gabime/spdlog](https://github.com/gabime/spdlog)
