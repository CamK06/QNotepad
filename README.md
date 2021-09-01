# QNotepad
A simple notepad program written in Qt, inspired by the Windows notepad.
  
# Install

After [building](https://github.com/Starman0620/QNotepad#build), run `sudo make install` in the build directory

# Build
Clone the repo:

```
git clone https://github.com/Starman0620/QNotepad --recursive
```

## Linux, BSD, macOS & Haiku
Ensure you have the Qt5 development dependencies installed. If you don't know what these are, you can search online for your respective system.

Build the code:

```
mkdir build && cd build
cmake ..
make
```
Note: on macOS, if qt5 was installed with Brew, you need to run cmake with the following command:
```
CMAKE_PREFIX_PATH=/usr/local/opt/qt@5 cmake ..
```

## Windows
TODO

# Screenshots
Linux | KDE:

![LinuxScreenshot](https://raw.githubusercontent.com/Starman0620/QNotepad/master/linux-screenshot.png  "Linux Screenshot")


macOS | Big Sur:

![macOSScreenshot](https://raw.githubusercontent.com/Starman0620/QNotepad/master/macos-screenshot.png  "macOS Screenshot")


Windows 10:

![windowsScreenshot](https://raw.githubusercontent.com/Starman0620/QNotepad/master/windows-screenshot.png "Windows Screenshot")


Haiku:

![haikuScreenshot](https://raw.githubusercontent.com/Starman0620/QNotepad/master/haiku-screenshot.png "Haiku Screenshot")

# Libraries
* [qt/qt5](https://github.com/qt/qt5)
* [gabime/spdlog](https://github.com/gabime/spdlog)
* [fmtlib/fmt](https://github.com/fmtlib/fmt)

# Contributors
* [justalemon](https://github.com/justalemon/)

# License
This project is licensed under the terms of the BSD 2-clause license
