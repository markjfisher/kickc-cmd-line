# cmd-line

Command line args for 
[kickc](https://gitlab.com/camelot/kickc) c compiler.


## Building

```
# build the xex
make

# This will create cmd-line.xex which needs to be copied onto an atr disk and booted into SDX.

# copy bin file and create mjf.atr from contents of directory
cp bin/cmd-line.xex ../../for-atr/
dir2atr -Q ../../tnfsd/mjf.atr ../../for-atr

```

## Development

### CLion under WSL

Create a Makefile Application with following env vars:

```
ALTIRRA_HOME=/your/path/to/Altirra-3.90
KICKC_HOME=/your/path/to/kickc
```
