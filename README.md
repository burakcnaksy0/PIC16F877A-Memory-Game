# PIC-Memory-Game

A memory game implemented using a PIC16F877A microcontroller, an LM016L LCD display, LEDs, and push buttons. The player must repeat a sequence of LED flashes by pressing the corresponding buttons. The game increases in difficulty with each level, and scores are displayed on the LCD along with the high score, which is stored in EEPROM.

## Features
- **Sequence Generation:** Random LED sequences are generated for the player to memorize.
- **LCD Display:** Shows the game status, score, level, and high score using an LM016L LCD.
- **LED Feedback:** Four LEDs (blue, green, red, yellow) indicate the sequence and user input.
- **EEPROM Storage:** High score is saved in the microcontroller's EEPROM to persist across power cycles.
- **Progressive Difficulty:** The sequence length increases with each level, and the speed of LED flashes increases as the game progresses.
- **Error Handling:** Detects invalid button presses before the game starts and provides feedback on the LCD.

## Hardware Requirements
- **Microcontroller:** PIC16F877A
- **LCD:** LM016L (16x2 character LCD)
- **LEDs:** 4 LEDs (blue, green, red, yellow)
- **Push Buttons:** 5 buttons (4 for input, 1 for start)
- **Resistors:** 
  - 10kΩ resistors (R5-R10) for LCD contrast and pull-ups
  - 330Ω resistors (R1-R4) for LEDs
  - 1kΩ resistor (RV1) for LCD contrast adjustment
- **Crystal Oscillator:** 8 MHz (with capacitors as needed)
- **Power Supply:** 5V DC

## Setup Instructions
1. **Hardware Setup:**
   - Connect the PIC16F877A to the LM016L LCD as per the schematic (pins RC0-RC5 for LCD control).
   - Attach the LEDs to PORTD (RD0-RD3) with 330Ω resistors.
   - Connect the push buttons to PORTB (RB0-RB4) with pull-up resistors.
   - Use a 1kΩ potentiometer (RV1) to adjust the LCD contrast.
   - Power the circuit with a 5V supply.

2. **Software Setup:**
   - Use MPLAB X IDE with the XC8 compiler to compile the code.
   - Flash the compiled code onto the PIC16F877A using a programmer (e.g., PICkit).

3. **Code Configuration:**
   - The code uses an 8 MHz crystal oscillator (`_XTAL_FREQ 8000000`).
   - Ensure the configuration bits are set as defined in the code (e.g., `FOSC = HS`, `WDTE = OFF`).

## Usage
1. Power on the circuit.
2. The LCD will display "The Memory Game" and prompt you to "Press START".
3. Press the START button (RB4) to begin the game.
4. Watch the sequence of LED flashes and repeat it by pressing the corresponding buttons (RB0-RB3).
5. The LCD will show your score, level, and high score. If you make a mistake, the game ends, and the final score is displayed.
6. If you complete all levels (max sequence length of 10), a "CONGRATULATIONS!" message is shown.

## Code Overview
- **Main Loop:** Handles game states (waiting, running, game over) and manages the game flow.
- **Sequence Generation:** Uses `rand()` to generate random sequences stored in an array.
- **LCD Functions:** Controls the LM016L LCD for displaying game information.
- **EEPROM Functions:** Saves and retrieves the high score from EEPROM.
- **Input Handling:** Reads button presses and validates user input against the sequence.

## Future Improvements
- Add sound feedback using a buzzer for each LED flash.
- Implement a difficulty selection mode (e.g., easy, medium, hard) with different speeds.
- Add a reset button to restart the game without powering off.

## Acknowledgments
- Built using MPLAB X IDE and XC8 compiler.
