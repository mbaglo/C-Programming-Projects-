#!/bin/bash
#----------------------------------------------------------------------------
# Simple command exercises
# student copy
#----------------------------------------------------------------------------

# Create a 2-word variable and print it out
variable="dog cat"
echo "The variabe is '$variable'"

# Read a number from the command line
echo "Enter a number:"
read number
echo "The number is $number"

# Use the number in a simple if statement
if [[ $number -gt 3 ]]; then
    echo "The number $number is greater than 3."
else
    echo "The number $number is less than or equal to 3."
fi

# Do some math with the number
new_number=$((number + 2))
echo "Added 2 to $number, resulting in $new_number."

# Do some math with the number using the alternative method
((number+=1))
echo "Incremented by 1 results in $number."

# Count from 1 to 4
echo "Counting from 1 to 4:"
count=1
while [ $count -le 4 ]; 
do
    echo "$count"
    ((count++))
done

# Cycle through a list
echo "Cycling through the list:"
for item in one two three; do
    echo "List item: $item"
done

# Verify string comparison functions 
echo "Enter 'dog' or 'cat' (or not):"
read animal_input
if [[ "$animal_input" == "dog" || "$animal_input" == "cat" ]]; then
    echo "$animal_input is either dog or cat"
else
    echo "$animal_input is not dog or cat"
fi


# Verify variable existence
echo "Enter a word or press enter for null:"
read input_word


if [ -n "$input_word" ]; then
    echo "-n check: string '$input_word' is not null."
else
    echo "-n check: string not defined."
fi

if [ -z "$input_word" ]; then
    echo "-z check: string is NULL."
else
    echo "-z check: string '$input_word' is defined."
fi
# Exercise the case statement
echo "Enter a number [1-3]:"
read number_input

case $number_input in
    1)
        echo "Case executed: You entered '1'."
        ;;
    2)
        echo "Case executed: You entered '2'."
        ;;
    3)
        echo "Case executed: You entered '3'."
        ;;
    4)
        echo "Case executed: You entered '4', which is outside the suggested range [1-3]."
        ;;
    *)
        echo "Case executed: You entered '$number_input', which is not 1, 2, 3, or 4."
        ;;
esac

#Execute the gcc version command to display GCC compiler version
echo "Checking GCC version:"
gcc --version

# Get and print a neat list of files, one on each line, with the 
# the following prefix string: "The files are: "  <file>
echo "Listing files in the current directory:"
for file in *; do
    echo "The files are: $file"
done
