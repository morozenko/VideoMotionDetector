Project uses gstreamer as video processing library.
On current stage gstreamer is included/linked using direct pointing directives to windows installation folders. TODO: replace with PkgConfig tool for CMake.
Project use custom build of gstreamer library (usual package doesn't have required QT integration).
All instruction and repository is here:
https://gitlab.freedesktop.org/gstreamer/cerbero
Use commands:
./cerbero-uninstalled -c config/win64.cbc bootstrap
./cerbero-uninstalled -c config/win64.cbc -v mingw -v qt6 package gstreamer-1.0