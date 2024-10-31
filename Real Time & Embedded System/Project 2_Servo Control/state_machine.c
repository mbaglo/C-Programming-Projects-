#include "state_machine.h"
#include "recipes.h"
#include "main.h"
#include <stdlib.h>
#include <string.h>

extern servo_t servo1;
extern servo_t servo2;
char debug_buffer[128];
// Function to start a move (adjust PWM and start timing based on position)
// Function to start a move (adjust PWM and start timing based on current position)
void start_move(servo_t *servo, TIM_HandleTypeDef *htim, uint32_t channel)
{
    if (servo->current_position < 0 || servo->current_position > 5) {
        servo->state = state_error;
        usart2_transmit("\r\nError: Invalid position\r\n");
        return;
    }

    // Update the servo position based on current_position
    set_servo_position(htim, channel, servo->current_position);

    // Update state to moving
    servo->state = state_moving;
}

// Function to set the servo PWM position
void set_servo_position(TIM_HandleTypeDef *htim, uint32_t channel, uint8_t position)
{
    // Calculate the duty cycle for the desired position (0.5ms to 2ms)
    uint32_t duty_cycle = 500 + (position * 400);
    __HAL_TIM_SET_COMPARE(htim, channel, duty_cycle);
}

// Function to move the servo to a new position and add delay based on distance
void move_servo_to_position(servo_t *servo, int position_delta, TIM_HandleTypeDef *htim, uint32_t channel)
{
    // Reverse direction if required
    if (servo->servo_reverse) {
        servo->current_position = 5 - servo->current_position;
    }

    // Apply position delta to the current position
    servo->current_position += position_delta;

    // Ensure position stays within valid limits (0-5)
    if (servo->current_position > 5) {
        servo->current_position = 5;
    }
    if (servo->current_position < 0) {
        servo->current_position = 0;
    }

    // Start moving
    start_move(servo, htim, channel);

    // Add delay based on how far the servo is moving
    delay_based_on_position(position_delta);
}

// Function to delay based on the distance of movement
void delay_based_on_position(int position_delta)
{
    int abs_position_diff = abs(position_delta);
    HAL_Delay(200 * abs_position_diff);  // Assuming 200ms per position change
}

// Function to process a single event
void process_event(enum events one_event, servo_t *servo, TIM_HandleTypeDef *htim, uint32_t channel)
{
    switch (servo->state)
    {
        // Servo is stationary at a known position
        case state_at_position:
            if (one_event == user_entered_left && servo->current_position < 5 )
            {
                servo->current_position--;  // Move left
                start_move(servo, htim, channel);
                servo->state = state_moving;  // Transition to moving state
                update_status_leds(status_running);  // Set LEDs to show running status
                usart2_transmit("Moving left.\r\n");
            }
            else if (one_event == user_entered_right && servo->current_position > 0)
            {
                servo->current_position++;  // Move right
                start_move(servo, htim, channel);
                servo->state = state_moving;  // Transition to moving state
                usart2_transmit("Moving right.\r\n");
            }
            else if (one_event == user_paused)
            {
                servo->state = state_paused;  // Transition to paused state
                usart2_transmit("Servo paused.\r\n");
            }
            else if (one_event == user_continued)
            {
              servo->state = state_moving;  // Change the state to moving
              servo->recipe_index++;
              usart2_transmit("Recipe continue moving from where it stopped.\r\n");
             }

            else if (one_event == recipe_restarted)
            {
             // Reset recipe parameters to start from the beginning
             servo->recipe_index = 0;  // Start from the first command in the recipe
             servo->recipe_counter = 0; // Start from the first recipe
             servo->state = state_moving;  // Change the state to moving
             usart2_transmit("Recipe restarted from the beginning.\r\n");

             // Reset other parameters if needed (like wait_time, loop_counter, etc.)
             servo->wait_time = 0;
             servo->loop_counter = 0;
             }

            else if (one_event == recipe_ended)
            {
                servo->state = state_recipe_ended;  // Recipe has ended
                usart2_transmit("Recipe ended.\r\n");
            }
            else if (one_event == user_reversed )
            {
                // Move to the opposite extreme (0 or 5) based on current position
                if (servo->current_position == 0)
                {
                    servo->current_position = 5;  // Move to the rightmost position
                }
                else if (servo->current_position == 5)
                {
                    servo->current_position = 0;  // Move to the leftmost position
                }

                start_move(servo, htim, channel);  // Move the servo
                usart2_transmit("Servo direction reversed to the extreme while stationary.\r\n");
            }
            update_status_leds(status_running);
            break;

        // Servo is currently moving
        case state_moving:
            if (one_event == servo_at_position)
            {
                servo->state = state_at_position;  // Transition back to stationary state
                usart2_transmit("Servo has reached position.\r\n");
            }
            else if (one_event == error_detected)
            {
                servo->state = state_error;  // Enter error state
                usart2_transmit("Error detected!\r\n");
            }
            else if (one_event == user_paused)
            {
                servo->state = state_paused;  // Pause while moving
                usart2_transmit("Servo paused while moving.\r\n");
            }
            else if (one_event == recipe_paused)
            {
                servo->state = state_paused;  // Pause recipe
                usart2_transmit("Recipe paused.\r\n");
            }
            else if (one_event == user_reversed )
            {
              // Move to the opposite extreme (0 or 5) based on current position
             if (servo->current_position == 0)
                {
                  servo->current_position = 5;  // Move to the rightmost position
                }
                else if (servo->current_position == 5)
                {
                   servo->current_position = 0;  // Move to the leftmost position
                }
             	 	 start_move(servo, htim, channel);  // Move the servo
                   usart2_transmit("Servo direction reversed to the extreme while running.\r\n");
            }
            update_status_leds(status_running);
            break;

        // Servo is paused
        case state_paused:
            if (one_event == user_reversed)
            {
                // Toggle reverse direction and move between the extremes (0 or 5), only while paused
                if (servo->current_position == 0)
                {
                    servo->current_position = 5;  // Move to the rightmost position
                }
                else if (servo->current_position == 5)
                {
                    servo->current_position = 0;  // Move to the leftmost position
                }

                start_move(servo, htim, channel);
                usart2_transmit("Servo direction reversed and moved to the extreme while paused.\r\n");
            }
            else if (one_event == user_entered_right && servo->current_position > 0)
            {
                servo->current_position--;  // Move right while paused
                start_move(servo, htim, channel);
                usart2_transmit("Moving right while paused.\r\n");
            }
            else if (one_event == user_entered_left && servo->current_position < 5)
            {
                servo->current_position++;  // Move left while paused
                start_move(servo, htim, channel);
                usart2_transmit("Moving left while paused.\r\n");
            }
            else if (one_event == user_continued)
            {
              servo->state = state_moving;  // Change the state to moving
              servo->recipe_index++;
              usart2_transmit("Recipe continue moving from where it stopped.\r\n");
             }

            else if (one_event == recipe_restarted)
            {
             // Reset recipe parameters to start from the beginning
             servo->recipe_index = 0;  // Start from the first command in the recipe
             servo->recipe_counter = 0; // Start from the first recipe
             servo->state = state_moving;  // Change the state to moving
             usart2_transmit("Recipe restarted from the beginning.\r\n");

             // Reset other parameters if needed (like wait_time, loop_counter, etc.)
             servo->wait_time = 0;
             servo->loop_counter = 0;
             }

            else
            {
                usart2_transmit("Servo is paused. Ignoring command.\r\n");
            }
            update_status_leds(status_paused);
            break;

        // Servo encountered an error
        case state_error:
        {

            // Handle recovery: If user requests to continue or restart the recipe after error
            if (one_event == user_continued)
            {
              servo->state = state_moving;  // Change the state to moving
              servo->recipe_index++;
              usart2_transmit("Recipe continue moving from where it stopped.\r\n");
             }

            else if (one_event == recipe_restarted)
            {
             // Reset recipe parameters to start from the beginning
             servo->recipe_index = 0;  // Start from the first command in the recipe
             servo->recipe_counter = 0; // Start from the first recipe
             servo->state = state_moving;  // Change the state to moving
             //usart2_transmit("Recipe restarted from the beginning.\r\n");

             // Reset other parameters if needed (like wait_time, loop_counter, etc.)
             servo->wait_time = 0;
             servo->loop_counter = 0;
             }


            // Check if a nested loop error occurred
            if (one_event == error_detected && servo->loop_counter > 0) {
                update_status_leds(status_nested_error);  // Both D1 and D2 ON
            } else {
                update_status_leds(status_command_error); // Only D2 ON
            }

            break;
        }

        // Recipe has ended
        case state_recipe_ended:
            if (one_event == user_continued)
            {
              servo->recipe_counter++;
              servo->state = state_moving;  // Change the state to moving
              //usart2_transmit("Recipe continue moving from where it stopped.\r\n");
             }

            else if (one_event == recipe_restarted)
            {
             // Reset recipe parameters to start from the beginning
             servo->recipe_index = 0;  // Start from the first command in the recipe
             servo->recipe_counter = 0; // Start from the first recipe
             servo->state = state_moving;  // Change the state to moving
             //usart2_transmit("Recipe restarted from the beginning.\r\n");

             // Reset other parameters if needed (like wait_time, loop_counter, etc.)
             servo->wait_time = 0;
             servo->loop_counter = 0;
             }

            else if (one_event == recipe_paused)
            {
            servo->state = state_paused;  // Pause recipe
            usart2_transmit("Recipe paused.\r\n");
            }
            else
            {
               // usart2_transmit("Recipe finished for this servo.\r\n");
            }
            update_status_leds(status_paused);
            break;

        // Servo is waiting (due to WAIT command)
        case state_waiting:
            if (one_event == wait_completed)
            {
                servo->state = state_moving;  // Resume moving after wait
               // usart2_transmit("Wait completed. Resuming movement.\r\n");
            }
            else if (one_event == user_paused)
            {
                servo->state = state_paused;  // Pause while waiting
               // usart2_transmit("Servo paused while waiting.\r\n");
            }
            update_status_leds(status_running);
            break;

        // Servo is in a loop (due to LOOP command)
        case state_looping:
            if (one_event == end_loop)
            {
                if (servo->loop_counter > 0)
                {
                    servo->loop_counter--;
                    servo->current_position = servo->loop_start;  // Go back to the start of the loop
                    usart2_transmit("Looping...\r\n");
                }
                else
                {
                    servo->state = state_moving;  // Exit the loop after all iterations
                    //usart2_transmit("Loop completed. Exiting loop.\r\n");
                }
            }
            else if (one_event == user_paused)
            {
                servo->state = state_paused;  // Pause while looping
               // usart2_transmit("Servo paused while looping.\r\n");
            }
            update_status_leds(status_running);
            break;

        // Unknown state (shouldn't happen)
        default:
            usart2_transmit("Unknown state encountered!\r\n");
            update_status_leds(status_command_error);
            break;
    }
}


// Function to process the servo recipe
void process_servo_recipe(servo_t *servo, unsigned char *recipe, TIM_HandleTypeDef *htim, uint32_t channel)
{

    if (servo->wait_time > 0) {
        // If still waiting, decrement the wait time and return
        servo->wait_time -= 100; // Assuming the function is called every 100ms
        return;
    }

    unsigned char command = recipe[servo->recipe_index]; // Get current command

    // Corrected snprintf usage
    snprintf(debug_buffer, sizeof(debug_buffer), "Processing command: 0x%02X for Servo %d at position %lu\r\n", command, (servo == &servo1 ? 1 : 2), (unsigned long)servo->current_position);
    usart2_transmit(debug_buffer); // Transmit debug information

    switch (command & 0xE0) {  // Only use the upper 3 bits for command
        case MOV:
        {
            uint32_t position = command & 0x1F;
            if (position <= 5) {
                int position_delta = position - servo->current_position;
                move_servo_to_position(servo, position_delta, htim, channel);
                servo->current_position = position;  // Update current position
                servo->state = state_moving;  // Set to moving state
            } else {
                servo->state = state_error; // Invalid position, enter error state
                usart2_transmit("Error: Invalid position in recipe.\r\n");
            }
            servo->recipe_index++;  // Move to the next command
        }
        break;

        case WAIT:
        {
            uint32_t wait_duration = (command & 0x1F) + 1;  // Add 1 since WAIT+0 means 100ms
            servo->wait_time = wait_duration * 100; // Convert to milliseconds
            servo->state = state_waiting; // Set state to waiting
            servo->recipe_index++;  // Move to the next command
        }
        break;

        case LOOP:
        {
            if (servo->loop_counter > 0) {
                // Error: Nested loops are not allowed, because we're already in a loop
                servo->state = state_error;
                usart2_transmit("Error: Nested loops are not allowed.\r\n");
            } else {
                // This is the first loop, so we set the start of the loop and loop count
                servo->loop_start = servo->current_position + 1;  // Store where the loop starts
                servo->loop_counter = (command & 0x1F);  // Get the loop count (from the 5 least significant bits)

                // Check if the loop count is valid (it should not be 0 or negative)
                if (servo->loop_counter == 0) {
                    usart2_transmit("Error: Invalid loop parameter.\r\n");
                    servo->state = state_error;
                } else {
                    // Continue executing the next command after the loop initialization
                    servo->recipe_index++;   // Move to the next command
                    usart2_transmit("Loop started.\r\n");
                }
            }
        }
        break;


        case END_LOOP:
        {
            if (servo->loop_counter > 0) {
                servo->loop_counter--;
                servo->current_position = servo->loop_start;  // Go back to the loop start
            } else {
                servo->recipe_index++;  // Move to the next command
            }
        }
        break;

        case REVERSE:
        {
            // Toggle the reverse direction flag
            servo->servo_reverse = !servo->servo_reverse;

            // Print debug message for reversal
            usart2_transmit("Servo direction reversed.\r\n");

            // Move the servo to the opposite extreme based on current position
            if (servo->current_position == 0)
            {
                servo->current_position = 5;  // Move to the rightmost position
            }
            else if (servo->current_position == 5)
            {
                servo->current_position = 0;  // Move to the leftmost position
            }

            // Apply the movement
            start_move(servo, htim, channel);

            // Advance to the next command in the recipe
            servo->recipe_index++;
        }
        break;


        case RECIPE_END:
        {
        	// Notify that the current recipe has ended
        	snprintf(debug_buffer, sizeof(debug_buffer), "Recipe %lu has ended for Servo %d.\r\n", (unsigned long)servo->recipe_counter + 1, (servo == &servo1 ? 1 : 2));
        	usart2_transmit(debug_buffer);

        	servo->state = state_recipe_ended;  // Set the state to recipe ended

        	// Move to the next recipe
        	servo->recipe_counter++;
        	if (servo->recipe_counter >= 5) {
        	servo->recipe_counter = 0; // Loop back to the first recipe
        	}

        	// Reset the recipe index and state for the new recipe
        	servo->recipe_index = 0;
        	servo->state = state_moving;
        }
        break;

        default:
        {
            // Set the servo state to error
            servo->state = state_error;

            // Print a message to indicate that an invalid command was encountered
            usart2_transmit("Error: Invalid command encountered. Entering error state.\r\n");

            // Update the LED to reflect the command error status (D2 ON)
            update_status_leds(status_command_error);

            break;
        }

    }

    // Print debug information after processing the command
    //snprintf(debug_buffer, sizeof(debug_buffer), "Next command position: %lu\r\n", (unsigned long)servo->current_position);
   // usart2_transmit(debug_buffer);
}


// Function to process user commands for Servo 1 and Servo 2
// Process user commands
void process_user_commands(char *buffer)
{
    // Ensure two characters are entered (one for each servo)
    if (strlen(buffer) < 2)
    {
        usart2_transmit("\r\nInvalid input. Two commands are required.\r\n");
        return;
    }

    // If 'X' or 'x' is entered, cancel the command
    if (buffer[0] == 'X' || buffer[0] == 'x' || buffer[1] == 'X' || buffer[1] == 'x')
    {
        usart2_transmit("\r\nCommand canceled.\r\n");
        return;
    }

    // Process the command for Servo 1
    usart2_transmit("\r\nServo 1: ");
    execute_servo_command(buffer[0], &servo1, &htim2, TIM_CHANNEL_1);

    // Process the command for Servo 2
    usart2_transmit("\r\nServo 2: ");
    execute_servo_command(buffer[1], &servo2, &htim2, TIM_CHANNEL_2);


}

// Helper function to execute a command for a given servo
void execute_servo_command(char command, servo_t *servo, TIM_HandleTypeDef *htim, uint32_t channel)
{


    switch (command)
    {
        case 'P': case 'p':  // Pause the servo
            process_event(user_paused, servo, htim, channel);
            break;

        case 'C': case 'c':  // Continue/resume the servo
            process_event(user_continued, servo, htim, channel);
            break;

        case 'R': case 'r':  // Move right if recipe is paused and not at the rightmost position
        	if (servo->state == state_paused && servo->current_position > 0) {
        	    process_event(user_entered_right, servo, htim, channel);
        	    } else if (servo->current_position == 0) {
        	      usart2_transmit("Servo is at the extreme right position. Cannot move left.\r\n");
        	    } else {
        	      usart2_transmit("Servo must be paused to move right.\r\n");
        	    }
        	      servo->state = state_paused;
        	      break;
        case 'L': case 'l':  // Move left if recipe is paused and not at the leftmost position

        	if (servo->state == state_paused && servo->current_position < 5) {
        	    process_event(user_entered_left, servo, htim, channel);
        	    } else if (servo->current_position == 5) {
        	    usart2_transmit("Servo is at the extreme left position. Cannot move right.\r\n");
        	    } else {
        	    usart2_transmit("Servo must be paused to move left.\r\n");
        	    }
        	    servo->state = state_paused;

        	    break;


        	if (servo->state == state_paused && servo->current_position > 0) {
                process_event(user_entered_left, servo, htim, channel);
            } else if (servo->current_position == 0) {
                usart2_transmit("Servo is at the extreme left position. Cannot move left.\r\n");
            } else {
                usart2_transmit("Servo must be paused to move left.\r\n");
            }
            servo->state = state_paused;
            break;

        case 'N': case 'n':  // Do nothing (No operation)


            usart2_transmit("No operation.\r\n");
            break;

        case 'B': case 'b':  // Begin or restart the recipe
            // Reset the current position and any relevant counters
        	process_event(recipe_restarted, servo, htim, channel);
            usart2_transmit("Recipe restarted.\r\n");
            break;

        case 'V': case 'v':  // Reverse the servo direction between 0 and 5
        	if (servo->current_position == 0) {
                servo->current_position = 5;  // Move to the extreme right
            } else if (servo->current_position == 5) {
            	servo->current_position = 0;  // Move to the extreme left
            }
              // Apply the new position
              start_move(servo, htim, channel);
              usart2_transmit("Servo moved to the opposite extreme.\r\n");
              servo->state = state_paused;
              break;

        default:
        {

                servo->state = state_error;  // Transition to error state
                usart2_transmit("Error: Invalid command encountered.\r\n");

                // Update LEDs to show error status
                update_status_leds(status_command_error);
            }
            break;
    }
}


