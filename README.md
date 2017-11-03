# SysProgHW3

A device driver implemented for Linux kernel, which acts as a messenger between operating system users.

## Installation

Installation requires superuser privileges.

    $ make
    $ sudo su
    $ insmod kmessaged.ko

## Tests

You might run integration tests with `run.sh` script found in `tests` folder.
This action, inevitably, requires superuser privileges.

## License

MIT
