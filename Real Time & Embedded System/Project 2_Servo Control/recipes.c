#include "recipes.h"
#include <stddef.h>

// Recipe 1
unsigned char recipe1[] = {
    MOV + 0,    // Move to position 0
    MOV + 5,    // Move to position 5
    MOV + 0,    // Move to position 0
    MOV + 3,    // Move to position 3
    LOOP + 0,   // Loop once
    MOV + 1,    // Move to position 1
    MOV + 4,    // Move to position 4
    END_LOOP,   // End the loop
    MOV + 0,    // Move to position 0
    MOV + 2,    // Move to position 2
    WAIT + 0,   // Wait for 100ms
    MOV + 3,    // Move to position 3
    WAIT + 0,   // Wait for 100ms
    MOV + 2,    // Move to position 2
    MOV + 3,    // Move to position 3
    WAIT + 31,  // Wait for 3.2 seconds
    WAIT + 31,  // Wait for 3.2 seconds
	WAIT + 31,  // Wait for 3.2 seconds
    MOV + 4,    // Move to position 4
    RECIPE_END  // End the recipe
};



// Additional Recipes for Testing

// Recipe 2: Verify moves to all possible positions
unsigned char recipe2[] = {
    MOV + 0,    // Move to position 0
    MOV + 1,    // Move to position 1
    MOV + 2,    // Move to position 2
    MOV + 3,    // Move to position 3
    MOV + 4,    // Move to position 4
    MOV + 5,    // Move to position 5
	MOV + 1,    // Move to position 1
	MOV + 4,    // Move to position 4
	END_LOOP,   // End the loop
	REVERSE,	// Move to position 0
    RECIPE_END  // End the recipe
};

unsigned char recipe3[] = {
    MOV + 0,    // Move to position 0
    LOOP + 2,   // Start a loop (repeat twice)
    MOV + 1,    // Move to position 1
    LOOP + 1,   // Nested loop (this should cause an error)
    MOV + 2,    // Move to position 2
    END_LOOP,   // End the nested loop (error state should occur before this)
    MOV + 3,    // Move to position 3
    END_LOOP,   // End the main loop
    RECIPE_END  // End the recipe
};

// Recipe 3: Ends normally, followed by MOV to verify CONTINUE override
unsigned char recipe4[] = {
    MOV + 0,    // Move to position 0
    MOV + 5,// Move to position 5
	REVERSE, //Move to position 0
    RECIPE_END, // End the recipe
    MOV + 3     // Move to position 3 (used to verify the CONTINUE override)
};

// Recipe 4: Deliberate error followed by a valid MOV command
unsigned char recipe5[] = {
    MOV + 0,    // Move to position 0
	REVERSE, //Move to position 5
    MOV + 1,    // Move to position 1
    MOV + 4,    // Move to position 4
    0xC0,       // Deliberate invalid opcode (error state)
    MOV + 3     // Move to position 3 after the error (shouldn't execute)
};

// Array of all recipes
unsigned char *recipes[] = { recipe1, recipe2, recipe3, recipe4, recipe5, NULL };
