# Siliko

<https://vdamewood.com/software/projects/siliko>

Siliko is library written in C for parsing and evaluating
mathematical expressions. It was made to be an example for teaching
software development. It demonstrates how to organize a project, how
to write modular code, how to write a recursive-descent parser, and how
to build a project with CMake to make a cross-platform project.

## Organization

The project includes, Siliko the core library, and interface programs
called Silikilo, and Silikujo. Silikilo is a command-line tool,
and Silikujo is a GUI program. There are three different versions of
Silikujo:

* For Windows, using the Win32 API
* for macOS, using Cocoa
* for Linux and FreeBSD using GTK+

Other Unix-like systems, may work using the version for Linux and
FreeBSD, but they have not been tested.

## The Language

Siliko has a built-in infix parser that handles the following:

* Four-function calculations, such as `2 + 2`, `3 * 4.0`, `9.0 - 3.0`
  and `8 / 2`.
* Exponents with a `^` operator. For example `2^3` is `8`.
* Negative numbers, such as `-4`, `-4.0 + 20`.
* constants, `e` and `pi` may be used as numbers.
* parenthetic expressions, such as `2 * (3 + 4)`.
* Random number generation using a `d` operator to simulate die rolls.
  such as `3d6` or `1d20`.
* Functions: The syntax to call functions is
  `<function name>(<argument>[, ...])`. Functions take one or more
  arguments. The functions `acos`, `asin`, `atan`, `ceil`, `cos`,
  `cosh`, `exp`, `floor`, `log`, `log10`, `sin`, `sinh`, `sqrt`, `tan`,
  and `tanh`. are available, and behave the same as their analogs in
  the C standard library. There is an `abs` function that will work as
  the `abs` function in the C standard library if the argument given is
  an integer, or the `fabs` functions if the argument given if a
  floating-point value.

Siliko ignores space and tab characters in its input, except to
separate tokens.

Siliko supports integer and floating-point arithmetic. In most
situations, integer expressions will yield integer results, and
floating-point expressions will yield floating-point results. The
exceptions are that the above-mentioned functions, except `abs`, always
return floating-point values, and any integer division where the result
would be truncated will result in a floating-point quotient.

## Building Siliko

See the file Build.md in the source tree for instructions on building
Siliko.

## License Information

Copyright 2012-2022 Vincent Damewood

Siliko is licensed under the terms of the GNU Lesser General Public
license, version 3 or later. Silikilo, Silikujo for macOS,
Silikujo for Windows, and Silikujo for Unix, are licensed under the
terms of the GNU General Public license, version 3 or later.

Siliko, Silikilo, and Silikilo, Silikujo for macOS, Silikujo
for Windows, and Silikujo for Unix are distributed in the hope that
they will be useful, but WITHOUT ANY WARRANTY; without even the implied
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
