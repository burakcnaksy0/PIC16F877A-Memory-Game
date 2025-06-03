#define _XTAL_FREQ 8000000
#include <xc.h>
#include <stdlib.h>
#include <stdio.h>

#pragma config FOSC = HS        
#pragma config WDTE = OFF       
#pragma config PWRTE = OFF      
#pragma config BOREN = ON       
#pragma config LVP = OFF        
#pragma config CPD = OFF        
#pragma config WRT = OFF        
#pragma config CP = OFF         

#define RS RC0
#define EN RC1
#define D4 RC2
#define D5 RC3
#define D6 RC4
#define D7 RC5

#define BUTTON1 RB0  
#define BUTTON2 RB1  
#define BUTTON3 RB2  
#define BUTTON4 RB3  
#define START_BUTTON RB4 

#define LED1 RD0    
#define LED2 RD1    
#define LED3 RD2    
#define LED4 RD3    

#define MAX_SEQUENCE 10

typedef enum {
    GAME_WAITING,    
    GAME_RUNNING,    
    GAME_OVER        
} GameState;

void Lcd_Command(unsigned char);
void Lcd_Char(unsigned char);
void Lcd_Init();
void Lcd_Clear();
void Lcd_Set_Cursor(unsigned char, unsigned char);
void Lcd_String(const char *);
void Lcd_Number(unsigned int);
void generateSequence();
void showSequence();
unsigned char getUserInput();
void updateScoreDisplay();
unsigned char checkInvalidButtonPress();
void waitForStart();
void resetGame();
void initGame();
void updateHighScore();
unsigned char eeprom_read(unsigned char);
void eeprom_write(unsigned char, unsigned char);

unsigned char sequence[MAX_SEQUENCE];     
unsigned int score = 0;                   
unsigned int level = 1;                   
unsigned int inputIndex = 0;              
unsigned int showSpeed = 500;             
unsigned int highScore = 0;               
GameState gameState = GAME_WAITING;       

void delay_ms(unsigned int ms) {
    while (ms--) {
        __delay_ms(1);
    }
}

void generateSequence() {
    for (int i = 0; i < level; i++) {
        sequence[i] = (unsigned char)((rand() % 4) + 1);
    }
}

void showSequence() {
    for (int i = 0; i < level; i++) {
        switch(sequence[i]) {
            case 1: LED1 = 1; delay_ms(showSpeed); LED1 = 0; break;
            case 2: LED2 = 1; delay_ms(showSpeed); LED2 = 0; break;
            case 3: LED3 = 1; delay_ms(showSpeed); LED3 = 0; break;
            case 4: LED4 = 1; delay_ms(showSpeed); LED4 = 0; break;
        }
        delay_ms(showSpeed / 2);
    }
}

unsigned char getUserInput() {
    inputIndex = 0;
    while (inputIndex < level && gameState == GAME_RUNNING) { 
        if (!BUTTON1) {
            while(!BUTTON1);
            LED1 = 1;
            delay_ms(200);  
            LED1 = 0;
            if (sequence[inputIndex++] != 1) {
                return 0;
            }
            delay_ms(100);
        }
        else if (!BUTTON2) {
            while(!BUTTON2);
            LED2 = 1;
            delay_ms(200);
            LED2 = 0;
            if (sequence[inputIndex++] != 2) {
                return 0;
            }
            delay_ms(100);
        }
        else if (!BUTTON3) {
            while(!BUTTON3);
            LED3 = 1;
            delay_ms(200);
            LED3 = 0;
            if (sequence[inputIndex++] != 3) {
                return 0;
            }
            delay_ms(100);
        }
        else if (!BUTTON4) {
            while(!BUTTON4);
            LED4 = 1;
            delay_ms(200);
            LED4 = 0;
            if (sequence[inputIndex++] != 4) {
                return 0;
            }
            delay_ms(100);
        }
    }
    return 1;
}

unsigned char checkInvalidButtonPress() {
    if (gameState == GAME_WAITING) {
        if (BUTTON1 == 0 || BUTTON2 == 0 || BUTTON3 == 0 || BUTTON4 == 0) {
            Lcd_Clear();
            Lcd_Set_Cursor(1, 1);
            Lcd_String("ERROR!");
            Lcd_Set_Cursor(2, 1);
            Lcd_String("First press START");
            delay_ms(2000);
            while (BUTTON1 == 0 || BUTTON2 == 0 || BUTTON3 == 0 || BUTTON4 == 0);
            delay_ms(200);
            return 1;
        }
    }
    return 0;
}

void Lcd_Command(unsigned char cmd) {
    RS = 0;
    D4 = (cmd >> 4) & 1;
    D5 = (cmd >> 5) & 1;
    D6 = (cmd >> 6) & 1;
    D7 = (cmd >> 7) & 1;
    EN = 1; delay_ms(1); EN = 0; delay_ms(2);
    D4 = (cmd >> 0) & 1;
    D5 = (cmd >> 1) & 1;
    D6 = (cmd >> 2) & 1;
    D7 = (cmd >> 3) & 1;
    EN = 1; delay_ms(1); EN = 0; delay_ms(2);
}

void Lcd_Char(unsigned char data) {
    RS = 1;
    D4 = (data >> 4) & 1;
    D5 = (data >> 5) & 1;
    D6 = (data >> 6) & 1;
    D7 = (data >> 7) & 1;
    EN = 1; delay_ms(1); EN = 0; delay_ms(2);
    D4 = (data >> 0) & 1;
    D5 = (data >> 1) & 1;
    D6 = (data >> 2) & 1;
    D7 = (data >> 3) & 1;
    EN = 1; delay_ms(1); EN = 0; delay_ms(2);
}

void Lcd_Init() {
    delay_ms(20);
    Lcd_Command(0x02);
    Lcd_Command(0x28);
    Lcd_Command(0x0C);
    Lcd_Command(0x06);
    Lcd_Command(0x01);
    delay_ms(2);
}

void Lcd_Clear() {
    Lcd_Command(0x01);
    delay_ms(2);
}

void Lcd_Set_Cursor(unsigned char row, unsigned char column) {
    unsigned char pos;
    if (row == 1)
        pos = 0x80 + column - 1;
    else
        pos = 0xC0 + column - 1;
    Lcd_Command(pos);
}

void Lcd_String(const char *str) {
    while (*str)
        Lcd_Char(*str++);
}

void Lcd_Number(unsigned int number) {
    char buffer[10];
    sprintf(buffer, "%u", number);
    Lcd_String(buffer);
}

void updateScoreDisplay() {
    Lcd_Clear();
    Lcd_Set_Cursor(1, 1);
    Lcd_String("Score:");
    Lcd_Number(score);
    Lcd_String(" Level:");
    Lcd_Number(level);
    Lcd_Set_Cursor(2, 1);
    Lcd_String("High:");
    Lcd_Number(highScore);
}

unsigned char eeprom_read(unsigned char address) {
    EEADR = address;
    EECON1bits.RD = 1;
    return EEDATA;
}

void eeprom_write(unsigned char address, unsigned char data) {
    EEADR = address;
    EEDATA = data;
    EECON1bits.EEPGD = 0;   
    EECON1bits.WREN = 1;
    INTCONbits.GIE = 0;
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;
    INTCONbits.GIE = 1;
    while(EECON1bits.WR);
    EECON1bits.WREN = 0;
}

void initGame() {
    unsigned char storedScore = eeprom_read(0x00);
    if (storedScore == 0xFF) {
        highScore = 0;
        eeprom_write(0x00, 0);
    } else {
        highScore = (unsigned int)storedScore;
    }
}

void updateHighScore() {
    if (score > highScore) {
        highScore = score;
        unsigned char scoreToSave = (highScore > 255) ? 255 : (unsigned char)highScore;
        eeprom_write(0x00, scoreToSave);
        Lcd_Clear();
        Lcd_Set_Cursor(1, 1);
        Lcd_String("NEW RECORD!");
        Lcd_Set_Cursor(2, 1);
        Lcd_String("Score: ");
        Lcd_Number(highScore);
        delay_ms(2000);
    }
}

void waitForStart() {
    gameState = GAME_WAITING;
    while(1) {
        Lcd_Clear();
        Lcd_Set_Cursor(1, 1);
        Lcd_String("The Memory Game");
        Lcd_Set_Cursor(2, 1);
        Lcd_String("Press START");
        while (START_BUTTON == 1) {
            if(checkInvalidButtonPress()) {
                break;
            }
            delay_ms(50);
        }
        if (START_BUTTON == 0) {
            while(START_BUTTON == 0);
            delay_ms(200);
            break;
        }
    }
    gameState = GAME_RUNNING;
}

void resetGame() {
    score = 0;
    level = 1;
    showSpeed = 500;
    gameState = GAME_WAITING;
    PORTD = 0x00;
}

void main(void) {
    TRISB = 0xFF;
    TRISD = 0x00;
    TRISC = 0x00;
    PORTD = 0x00;
    OPTION_REGbits.nRBPU = 1;
    ADCON1 = 0x06;
    Lcd_Init();
    Lcd_Clear();
    srand(TMR0);
    initGame();

    while (1) {
        waitForStart();
        while (gameState == GAME_RUNNING) {
            updateScoreDisplay();
            delay_ms(1000);
            generateSequence();
            showSequence();
            delay_ms(300);
            Lcd_Clear();
            Lcd_Set_Cursor(1, 1);
            Lcd_String("Guess now!");
            delay_ms(450);
            if (getUserInput()) {
                score += level;
                level++;
                if (showSpeed > 100)
                    showSpeed -= 50;
                if (level > MAX_SEQUENCE) {
                    Lcd_Clear();
                    Lcd_Set_Cursor(1, 1);
                    Lcd_String("CONGRATULATIONS!");
                    Lcd_Set_Cursor(2, 1);
                    Lcd_String("Game completed.");
                    delay_ms(3000);
                    updateHighScore();
                    resetGame();
                    break;
                }
                Lcd_Clear();
                Lcd_Set_Cursor(1, 1);
                Lcd_String("CORRECT!");
                Lcd_Set_Cursor(2, 1);
                Lcd_String("Next level");
                delay_ms(1000);
            } else {
                Lcd_Clear();
                Lcd_Set_Cursor(1, 1);
                Lcd_String("WRONG!");
                Lcd_Set_Cursor(2, 1);
                Lcd_String("Score: ");
                Lcd_Number(score);
                delay_ms(2000);
                updateHighScore();
                resetGame();
                break;
            }
        }
    }
}