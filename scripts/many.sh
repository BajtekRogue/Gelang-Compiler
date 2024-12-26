#!/bin/bash

compiler=./build/ge
vm=./build/vm

# Check if directory argument is provided
if [ -z "$1" ]; then
    echo "Usage: $0 <folder>"
    exit 1
fi

input_dir="./tests/$1"
compiled_dir="./compiled/$1"

# Create the compiled output directory if it doesn't exist
mkdir -p "$compiled_dir"

# Process each .gbl file in the input directory
for input in "$input_dir"/*.gbl; do
    base_name=$(basename "$input" .gbl)
    output="$compiled_dir/$base_name.asm"

    # Compile the input file
    $compiler "$input" "$output" "-g"

    # Run the output if compilation succeeded
    if [ $? -eq 0 ]; then
        $vm "$output"
    else
        echo "Compilation failed for $input"
    fi
done
