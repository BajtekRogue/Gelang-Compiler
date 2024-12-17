#!/bin/bash

compiler=./build/ge
vm=./build/vm/vm

# Extract parts from the input path
input=$1
base_name=$(basename "$input" .imp)
dir_name=$(dirname "$input" | sed "s|./tests|./compiled|")

# Create the output directory if it doesn't exist
mkdir -p "$dir_name"

# Define the output path
output="$dir_name/$base_name.mr"

# Compile the input file
$compiler "$input" "$output" 

# Run the output if compilation succeeded
if [ $? -eq 0 ]; then
    $vm "$output"
fi
