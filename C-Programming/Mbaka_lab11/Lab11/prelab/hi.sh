
#!/bin/bash

# Step 1: Prompt for a name using the echo command
echo "Please enter your first and surname:"
read name

# Step 2: Print "Hello1 '<name>'" using echo
echo "Hello1 '$name'"

# Step 3: Prompt for a name AND ENTER DATA AT THE END OF THE PROMPT using the printf command
printf "Please enter your names again: "
read name

# Step 4: Print "Hello2 '<name>'" using printf with no format string
printf "Hello2 '$name'\n"

# Step 5: Print "Hello3 '<name>'" using printf with a single '%s' format string
printf "Hello3 '%s'\n" $name

# Step 6: Print "Hello4 '<name> <name>'" using printf with TWO '%s' format strings
printf "Hello4 '%s %s'\n" $name 

