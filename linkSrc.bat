@REM Creates Symbolic Links to Source Files. Allows source files to be shared 
@REM across skecthes without code duplication, which leads to errors.
@REM Run via CMD prompt. Does not work with PowerShell.
@REM  If Developing on Linux, try ln -s \source \destination instead.

mklink /j  tests\src src