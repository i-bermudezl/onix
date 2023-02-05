# onix

This is a sketch repo and will be deleted in the next days.

# Limitations

For the moment in only works on windows but should work with linux with minor modifications.

# Pre-requisites

- Visual Studio 2022

# How to run

# Option 1 - Open it in Visual Studio 2022

This should automate most of the work

# Option 2 - comandline

## Do this on developer command prompt for VS2022

1. clone the repo

> git clone --recursive https://github.com/i-bermudezl/onix.git

2. cd into it

3. run cmake

> cmake --preset=<presets>

current presets are:

- "x64-debug"
- "x64-release"

4. cd into build directory and run 
> ninja

5. Run the app and have fun

find the executable and run it, it should create an image called render.exr in the same directory of the executable

> You can open the image in gimp or other image viewer supporting OpenEXR
