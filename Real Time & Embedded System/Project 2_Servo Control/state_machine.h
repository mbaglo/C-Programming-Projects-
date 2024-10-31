#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "stm32l4xx_hal.h"  // Ensure HAL includes are added for TIM_HandleTypeDef and other peripherals

// Define all of the commands that are valid
#define MOV (0x20)        // 001xxxxx
#define WAIT (0x40)       // 010xxxxx
#define LOOP (0x80)       // 100xxxxx
#define END_LOOP (0xA0)   // 101xxxxx
#define RECIPE_END (0x00) // 000xxxxx
#define REVERSE (0x60)    // 011xxxxx - now made legal/valid

// Define all of the commands that are invalid
#define ILLEGAL_OPCODE_1 (0xC0)   // 110xxxxx
#define ILLEGAL_OPCODE_2 (0xE0)   // 111xxxxx

// USER Commands
#define CMD_PAUSE 'P'
#define CMD_CONTINUE 'C'
#define CMD_MOVE_RIGHT 'R'
#define CMD_MOVE_LEFT 'L'
#define CMD_NO_OP 'N'
#define CMD_BEGIN_RECIPE 'B'
#define CMD_CANCEL 'X'
#define CMD_REVERSE 'V'  // User command for reversing the direction


extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart2;

// Servo Status for Display
enum status
{
    status_running,
    status_paused,
    status_command_error,
    status_nested_error
};

// Complete Servo States
enum servo_states {
    state_at_position,    // The servo is stationary at a known position
    state_moving,         // The servo is currently moving
    state_paused,         // The servo is paused
    state_recipe_ended,   // The recipe has finished
    state_error,          // An error state
    state_waiting,        // Servo is waiting (used with WAIT command)
    state_looping         // Servo is in a loop (used with LOOP command)
};

enum events {
    user_entered_left,    // User command: move left
    user_entered_right,   // User command: move right
    recipe_ended,         // Recipe ended
    user_paused,          // User command: pause
    user_continued,       // User command: continue
    user_reversed,        // User command: reverse direction
    loop_started,         // Loop command encountered in recipe
    error_detected,       // Error detected in recipe or command
    recipe_paused,        // Recipe was paused
    recipe_restarted,       // Recipe was restarted
    wait_completed,       // Wait command completed
    servo_at_position,    // Servo reached its position
	end_loop
};


// Servo structure to hold relevant information about the servo
typedef struct {
    enum servo_states state;       // The current state of the servo
    uint32_t current_position;     // The current position of the servo (0 to 5)
    uint32_t wait_time;            // Time to wait before executing the next command
    uint32_t loop_start;           // Start index for loops in the recipe
    uint32_t loop_counter;         // Number of times to loop
    uint32_t servo_reverse;         // 1 if the servo direction is reversed, 0 otherwise
    uint32_t recipe_index;          // Index of the current command in the recipe
    uint32_t recipe_counter;        // Tracks which recipe the servo is currently executing
} servo_t;

// Prototypes
void start_move(servo_t *servo, TIM_HandleTypeDef *htim, uint32_t channel);
void process_event(enum events one_event, servo_t *servo, TIM_HandleTypeDef *htim, uint32_t channel);
void process_servo_recipe(servo_t *servo, unsigned char *recipe, TIM_HandleTypeDef *htim, uint32_t channel);
void set_servo_position(TIM_HandleTypeDef *htim, uint32_t channel, uint8_t position);
void move_servo_to_position(servo_t *servo, int position_delta, TIM_HandleTypeDef *htim, uint32_t channel);
void delay_based_on_position(int position_delta);
void process_user_commands(char *buffer);
void execute_servo_command(char command, servo_t *servo, TIM_HandleTypeDef *htim, uint32_t channel);
extern void usart2_transmit(char *message);
extern void update_status_leds(enum status current_status);
extern void update_status_leds(enum status current_status);
#endif // STATE_MACHINE_H
