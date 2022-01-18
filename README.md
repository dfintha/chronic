# chronic

## Overview

`chronic` is a very simple terminal clock application, with a few extra
features. Largely, the concept of `chronic` was based on `tty-clock`, however,
it was written from scratch, without using any existing code.

Along with the current date and time, `chronic` displays the percentage of the
current year (this was inspired by the @year_progress Twitter app), and the
weather at the user's location (by utilizing [wttr.in](https://wttr.in/)).

The time is updated every second, and the weather is updated every 10 minutes.

## Requirements

`chronic` requires the following libraries to function:

- `curl` (`libcurl.so`)
- `ncurses` (`libncurses.so`)
- POSIX threads library (`libpthread.so`)
- GCC runtime libraries (`libatomic.so`)

On Arch Linux, the following command installs the packages of all the required
dependencies.

`pacman -S curl ncurses glibc gcc-libs`

## Controls

During execution the following key bindings are in effect.

- `1`: Set the color to red.
- `2`: Set the color to green.
- `3`: Set the color to yellow.
- `4`: Set the color to blue.
- `5`: Set the color to magenta.
- `6`: Set the color to teal.
- `7`: Set the color to white.
- `B`: Toggle bold fonts.
- `Q`: Quit `chronic`.
