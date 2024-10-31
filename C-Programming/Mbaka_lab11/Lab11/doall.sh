#!/bin/bash

# Check the number of input parameters
if [ "$#" -ne 2 ]; then
    echo "Error: Incorrect number of parameters" >&2
    echo "Usage: ./doall.sh input.c basename" >&2
    exit 1
fi

input_file=$1
base_name=$2

# Check for the existence of realDevice and myplot binaries
if [ ! -x "$(pwd)/realDevice" ]; then
    echo "Error: realDevice not found" >&2
    exit 1
fi

if [ ! -x "$(pwd)/myplot" ]; then
    echo "Error: myplot not found" >&2
    exit 1
fi

# Define compiler settings
CC=gcc
CFLAGS="-Wall -Werror"

# Define variation coefficients
coefficients=(
    "-D A2=-0.000358309 -D A1=1.24539 -D A0=-395.213"
    "-D A2=-0.000300 -D A1=1.200 -D A0=-390"
    "-D A2=-0.000360 -D A1=1.250 -D A0=-396"
    "-D A2=-0.000370 -D A1=1.260 -D A0=-397"
    "-D A2=-0.000380 -D A1=1.270 -D A0=-398"
    "-D A2=-0.000390 -D A1=1.280 -D A0=-399"
    "-D A2=-0.000400 -D A1=1.290 -D A0=-400"
    "-D A2=-0.000410 -D A1=1.300 -D A0=-401"
)

error_flag=0

# Compile, run, and plot for each variation
for i in "${!coefficients[@]}"; do
    variation_name="${base_name}$((i+1))"
    real_output="${variation_name}_real.txt"
    correction_output="${variation_name}_correction.txt"
    combined_output="${variation_name}_combined.txt"
    plot_output="${variation_name}.png"

    # Compile the correction program with the variation coefficients
    if ! $CC $CFLAGS ${coefficients[$i]} -o "$variation_name" "$input_file" >/dev/null 2>&1; then
        echo "Compilation failed for $variation_name" >&2
        error_flag=1
        continue
    fi

    # Run the real device and correction, and generate the combined data file
    if ! ./realDevice > "$real_output" 2>&1 || ! cat "$real_output" | ./"$variation_name" > "$correction_output" 2>&1; then
        echo "Execution failed for $variation_name" >&2
        error_flag=1
        continue
    fi

    # Combine the output into a four-column format (ideal, real, ideal, corrected)
    paste "$real_output" "$correction_output" | awk '{print $1, $2, $1, $4}' > "$combined_output" 2>/dev/null

    # Generate the plot
    if ! ./myplot -in "$combined_output" -out "$plot_output" 2>&1 >/dev/null; then
        echo "Plotting failed for $plot_output" >&2
        error_flag=1
        continue
    fi
done

# Global status report
if [ $error_flag -ne 0 ]; then
    echo "Errors were encountered during the execution." >&2
    exit 1
fi

exit 0
