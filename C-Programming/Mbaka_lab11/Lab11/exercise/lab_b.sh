#!/bin/bash
# lab_b.sh - Demonstrates bash functions, parameter passing, and variable scope.

# Function: parmFun
# Description: Prints out the values of two passed parameters.
parmFun() {
    echo "In the function, the passed parameters are: $1 $2"
}

# Function: returnNumber
# Description: Returns the number 999.
returnNumber() {
    echo "999"
}

# Function: globVar
# Description: Demonstrates local and global variable behaviors.
globVar() {
    echo "Entered globVar: $var1 $var2"
    local var1="dog"  # Local variable inside globVar
    var2="cat"        # Global variable modified inside globVar
    echo "Leaving globVar: $var1 $var2"
}

# Main script body
echo "Script started."

# Initialize var1 and var2 for testing parmFun
var1="one"
var2="two"

# Calling parmFun with var1 and var2
echo "Calling a function with 2 parameters: $var1 $var2"
parmFun "$var1" "$var2"

# Display the values before calling globVar
echo "Before calling globVar: $var1 $var2"

# Calling globVar function
globVar

# Display the effects of calling globVar
echo "After calling globVar: $var1 $var2"

# Showing the return of returnNumber function
returned_value=$(returnNumber)
echo "The function returnNumber returned: $returned_value"

# End of script
echo "Script ended."
