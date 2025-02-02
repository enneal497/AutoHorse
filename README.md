# CommonLibSSE-NG Plugin Template

## Setup

- Clone the repository
- Run `cd CLibNGPluginTemplate`
- If using Visual Studio, set the formatter path to the `clang-format.exe` included in your LLVM install:
  - Open Visual Studio settings
  - Navigate to `Tools > Options > Text editor > C++ > Formatting`
  - Set the path to `clang-format.exe` (Usually `C:\Program Files\LLVM\bin\clang-format.exe`)
- If your CMake install is **not** located at `C:\Program Files\CMake\bin\`, fix the path in the `"cmakeExecutable"` key in `CMakePresets.json`

## Building your project

- Select one of the CMake presets (release or debug), configure, and build.
- The .dll and .pdb files will be placed in `contrib\PluginRelease` or `contrib\PluginDebug` depending on your selected preset

## Requirements

- [vcpkg](https://github.com/microsoft/vcpkg)
  - Create a new environment variable called `VCPKG_ROOT` which points to your vcpkg install directory
- [CMake](https://cmake.org)
- [LLVM](https://github.com/llvm/llvm-project/releases) (for `clang-format` and `clang-tidy`)
- Visual Studio 2022 build tools

## Resources

- [Address Library Database](https://github.com/meh321/AddressLibraryDatabase)
- [Address Library Manager](https://github.com/meh321/AddressLibraryManager)
- [Steamless](https://github.com/atom0s/Steamless/releases)
