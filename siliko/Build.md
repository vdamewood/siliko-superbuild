# Building Siliko

Siliko includes files to use CMake (<https://www.cmake.org>) to
generate a build system. Assuming you have the source code for Siliko
in `~/src/siliko`, you should be able to build Siliko with the
following commands on Linux, FreeBSD, and macOS:

```Command-line
mkdir -p ~/tmp/build/siliko
cd ~/tmp/build/siliko
cmake -DCMAKE_BUILD_TYPE:STRING=Release ~/src/siliko
cmake --build .
```

And the following on Windows, assuming the source code in
`C:\Users\coder\src\siliko`:

```Command-line
mkdir C:\Users\coder\tmp\build
cd C:\Users\coder\tmp\build
cmake C:\Users\coder\src\siliko
cmake --build . --config Release
```

## Requirements

Siliko requires the following to build:

* CMake (<https://cmake.org>). On Linux, you can install using
  the package on the CMake site, or using a package provided by your
  distribution. On FreeBSD you will have to install from packages or
  ports, as CMake does not provide a FreeBSD build.
* A Toolchain. Any one of the following:
  * Clang (part of LLVM; <https://releases.llvm.org>), both the official
    distribution and Apple's custom build, included with Xcode.
    Clang is the default toolchain on FreeBSD, and is available as a
    package from many Linux distributions.
  * GNU Compiler Collection (GCC; <https://gcc.gnu.org>). GCC is
    provided by most Linux distributions' package managers. On Windows,
    a Windows port called MinGW-w64 can be obtained as part of MSYS2
    (<https://www.msys2.org>), which provided a complete build
    environment. Siliko doesn't support GCC on FreeBSD or macOS.
  * Microsoft Visual C++ 2022. Visual C++ is included with Visual
    Studio, available at <https://visualstudio.microsoft.com>. MSVC++
    is only supported on Windows.
* A build tool. Siliko is tested with the following:
  * Make, Make is normally included with Linux and FreeBSD, and is
    available on macOS with Xcode, and on Windows with MSYS2.
  * XcodeBuild. XcodeBuild is included with Xcode. XcodeBuild is only
    supported on macOS.
  * MSBuild. MSBuild is available with Visual Studio, and is only
    supported on Windows.
  * Ninja (<https://ninja-build.net>). Ninja is available for Linux,
    macOS, and Windows at the project site. Ninja should be available
    on FreeBSD as a package or through ports.

Additionally the following are optionally needed:
* Readline (or a compatible clone). Enhances the interface for the
  commandline program on Linux, FreeBSD, and macOS. FreeBSD and macOS
  include a compatible clone. Most Linux distributions include packages
  for Readline. Readline is not needed on Windows.
* GTK+ 4. On Linux and FreeBSD, this is used to build the GUI program
  for Linux and FreeBSD.

Note for Linux: On many distributions, libraries such as Readline and
GTK+ have seperate development packages to install if you wish to build
software that uses such libraries. Make sure you have those installed,
too.

Note for Visual Studio: You will need to set up appropriate environment
variables so CMake can find your toolchain, build tool, and libraries.
You can use the the various developer command prompt items installed
with Visual Studio to ensure these environment variables are set. Check
the Visual Studio submenu in the Start menu.

## Build Tools and CMake Generators

CMake uses a *generator* to generate files for various build tools.
Siliko is tested with the following CMake generators:

* `Unix Makefiles` for Make
* `Xcode` for Xcode and XcodeBuild
* `Visual Studio 17 2022` for Visual C++ 2022 and MSBuild
* `Ninja` for Ninja
* `Ninja Multi-Config`, also for Ninja, but supports multiple
  configurations

The default generator on macOS, Linux, and FreeBSD is `Unix Makefiles`.
On Windows, the default is `Visual Studio 17 2022`. Whichever build
system you use, CMake can handle the build for you using the
`cmake --build` sub-command. Some build systems can handle multiple
build types, such as *debug* or *release*, at once. If you use the
`cmake --build` command with a *multi-configuration* build system,
including `Xcode`, `Visual Studio 17 2022`, or `Ninja Multi-Config`,
you will need to specify which configuration you wish to build if you
don't want to use the default, often debug. Siliko is developed for
*debug* and *release* builds. Other build types are not supported.

### Build Process

Building Siliko is done in two phases, first there is configuration,
and second is the build proper. After that, Siliko can be installed
or packaged. Under normal conditions, you can configure Siliko by
first making an empty directory, then running the `cmake` command as
follows:

```Command-Line
cmake [-G<generator>] [-D<option>:<type>=<value> ...] <Path to Siliko source>
```

The optional `-G` flag tells CMake which generator to use. For example,
to use the Ninja build system, you can specify `-GNinja`. If the -G flag
is not used, CMake will use the default specified above. The `-D` option
can be specified multiple times, and passes information to CMake to
alter the behavior of the generated build system. To include these
options, use the `-D` flag with the name of the value, a colon, the
value's type, such `BOOL` or `STRING`, an equals sign, and the value you
wish to set.

For example, if you have the source code for Siliko in
`~/code/siliko`, the following will make a release build, without
building a command-line program or GUI program, using Ninja to build:

```Command-line
cmake -GNinja \
  -DCMAKE_BUILD_TYPE:STRING=Release \
  -DSILIKO_BUILD_CLI:BOOL=Off \
  -DSILIKO_BUILD_GUI:BOOL=Off \
  ~/code/siliko
```

Once Siliko is configured, you can use your selected build tool to
run the built proper. You can also use `cmake --build`, and cmake will
call the appropriate build tool for you. For example:

```Command-line
cmake --build .
```

Note: the `.` in the command represents the current directory, when
you use CMake to run your build tool for you, cmake needs to be pointed
to the directory where the build files are, so a single dot is used here
assuming you're running `cmake --build` right after running CMake to
configure your project.

## Configuration Options

Values of type `BOOL` can be set to `On` or `True` to enable them, or
to `Off` or `False` to disable them. Siliko supports these values for
all systems:

* `CMAKE_BUILD_TYPE` (`STRING`): When building with a
  single-configuration generator, such as `Unix Makefiles`, or `Ninja`,
  set to `Release` for a release build, or to `Debug` for a debug build.
  Leave this unset if you are using a Multi-Config build tool, including
  Xcode and Visual Studio.
* `SILIKO_BUILD_CLI` (`BOOL`): Build Silikilo, a command-line
  interface for Siliko. Default: `On`
* `SILIKO_BUILD_GUI` (`BOOL`): Build Silikujo, a GUI interface for
  Siliko. Default: `On`
* `SILIKO_PACKAGE` (`BOOL`): Add `package` and `package_source` build
  targets to make installation and source packages. Default: `On`
* `SILIKO_PACKAGE_ZIP` (`BOOL`): Build a zip file package (ignored if
  `SILIKO_PACKAGE` is off). Default: `On`
* `SILIKO_PACKAGE_7ZIP` (`BOOL`): Build a 7zip file package (ignored
  if `SILIKO_PACKAGE` is off). Default: `On`
* `SILIKO_PACKAGE_TXZ` (`BOOL`): Build an XZ-compressed tar file
  package (ignored if `SILIKO_PACKAGE` is off). Default: `On`
* `SILIKO_PACKAGE_VENDOR` (`STRING`): The name of the vendor when
  generating packages (ignored if `SILIKO_PACKAGE` is off).
  Default: `"Unknown Vendor"`
* `SILIKO_INSTALL_CMAKEDIR` (`STRING`): Locaton to install CMake files
  used by other projects to find Siliko (ignored if
  `SILIKO_BUILD_FRAMEWORK` is on). Specify a relative path to the
  install location.Default: `lib/cmake/Siliko` or similar, depending
  on the system

These values are specific to macOS:

* `CMAKE_OSX_ARCHITECTURES` (`STRING`): Architectures to build for. Set
  to `"arm64;x86_64"` for a Universal 2 build, leave blank to build for
  your current system, Intel or Apple Silicon. Default: empty
* `SILIKO_BUILD_FRAMEWORK` (`BOOL`): Build Siliko as a framework.
  Default: `On`
* `SILIKO_PACKAGE_DMG` (`BOOL`): Build disk image (DMG) file package
  (ignored if `SILIKO_PACKAGE` is off). Default:  `On`
* `SILIKO_INSTALL_APPDIR` (`STRING`): Locaton to install application
  bundles. Specify relative to the installation location.
  Default: `"Applications"`
* `SILIKO_INSTALL_FRAMEWORKDIR` (`STRING`): Locaton to install
  framework (ignored if `SILIKO_BUILD_FRAMEWORK` is off). Specify
  relative to the installation location. Default: "Frameworks"

These values are specific to Window:

* `SILIKO_PACKAGE_MSI` (`BOOL`): Package as MSI Installer using WiX
  Toolkit (Ignored if `SILIKO_PACKAGE` is off)". Default: `On`
* `SILIKO_MSI_GUID` (`STRING`): The Upgrade GUID used for MSI
  installers, This value is used by Windows Installer to identify
  installers that are new or upgraded versions of previous installers.
  (Ignored if either `SILIKO_PACKAGE` or `SILIKO_PACKAGE_MSI` is
  off). Default: `"00000000-0000-0000-0000-000000000000"`

## Building Packages

If the value `SILIKO_PACKAGE` is `On` when configuring Siliko,
You can build packages using the `package` build target such as
`make package`. You can specify the following options when running
`cmake` to generate various package formats:

* `SILIKO_PACKAGE_ZIP`: a simple zip file.
* `SILIKO_PACKAGE_7ZIP`: a simple 7zip file.
* `SILIKO_PACKAGE_TXZ`: a simple XZ-compressed tape archive (tar).
* `SILIKO_PACKAGE_DMG`: (macOS only) A macOS disk image file.
* `SILIKO_PACKAGE_MSI`: (Windows only) An MSI installer file. This
  option requires the WiX Toolset (<https://wixtoolset.org>) to be
  installed.

For building distributon-specific packages, Siliko has been designed to
allow package maintainers to build packages using their own processes.
