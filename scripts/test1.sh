#!/bin/bash

compiler=./build/ge
vm=./build/vm/vm

# Define input and output paths
relative_input=$1
input="./tests$relative_input"
base_name=$(basename "$input" .imp)
dir_name="./compiled$(dirname "$relative_input")"

# Define the output path
output="$dir_name/$base_name.mr"

# Compile the input file
$compiler "$input" "$output"

# Run the output if compilation succeeded
if [ $? -eq 0 ]; then
    $vm "$output"
fi
