#!/bin/bash

counter=0
num_elements=$(( (1024*1024)/4 ))
OUTPUT_FILE="./test_data2.txt"
MAX_VALUE=40

while [ $counter -lt $num_elements ]; do
  echo "$((counter % $MAX_VALUE ))" >> $OUTPUT_FILE
  counter=$(( $counter + 1 ))
done
