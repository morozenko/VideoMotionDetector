Project uses gstreamer as video processing library.
On current stage gstreamer is included/linked using direct pointing directives to windows installation folders. TODO: replace with PkgConfig tool for CMake.
Project use custom build of gstreamer library (usual package doesn't have required QT integration).
All instruction and repository is here:
https://gitlab.freedesktop.org/gstreamer/cerbero

Modify cerbero/data/inno/base.iss
In this file methods, which check VS availability should return true. Othervise inno installer will disable needed components. Modify them to always return true.

To build gstreamer with cerbero
Use commands:
./cerbero-uninstalled -c config/win64.cbc bootstrap
./cerbero-uninstalled -c config/win64.cbc -v mingw -v qt6 package gstreamer-1.0

WixInstaller

Deploy folder contains everything for VideoMotionDetector app - dlls, plugins and executable. Also, copy gstreamer installer there - wix installer will pack it.
In VideoMotionDetectorEXE run command

dotnet build VideoMotionDetector.wixproj -p:DeployedFilesPath="D:\Projects\Deploy"