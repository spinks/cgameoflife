# Game of Life Implementation

A C dynamically-linked library implementation of Conways' Game of Life. Completed for Durham University Coursework, see [coursework.pdf](./coursework.pdf).

## Usage

To build exec `export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH` to link library correctly.

```
make all
./gameoflife
```

Or it can be run with the test script, `bash test_script.sh`.

### Options

`-i` input_filename to read initial universe from file. If not specified takes input from stdin.

`-o` output_filename to output final universe to. If not specified outputs to stdout.

`-g` number_of_generations the number of new generations for which the game should be run. If not specified defaults to 5.

`-s` to print statistics after the final generation has been output.

`-t` to use torus topology for the rule. If not specified cells outside universe are dead.

### Valid Input

Must be a maximum of 512 columns. Input must consist only of `*` & `.` and `\n`.
