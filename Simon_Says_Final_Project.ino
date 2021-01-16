/*Roshan Nanthapalan
 * May.25.2018
 * Final Project 
 * This is a game that resembles Simon Says. While the LEDs are blinking in order, push any button to START.
 * Then wait for an LED to blink and push the button that corresponds with the LED.
 * Everytime you get the pattern right, there will be one extra blink added to the pattern to make it harder.
 * If you click the wrong button or take too long, you LOSE.
 * The the program will restart and you can play again.
 */
 
#include<LiquidCrystal.h>         //We are including the LiquidCrystal Library in our program
LiquidCrystal lcd(12,11,5,4,3,2); //Declaring and initializing the LiquidCrystal Library with the pins on the Arduino

int buttons[] = {10, 9};          //The 2 button input pins
int leds[] = {6, 7};              // LED pins

int sequence[100];                //Keeps track of how many sequences to repeat

int largestIndex = 0;             //Keeps track of the largest index in the sequence so far

//These are the states of the game
const int START = 0;              //The start stage, the LEDs are flashing waiting for a button to be pressed
const int PLAY = 1;               //The play stage, you are actually playing the game during this stage
const int GAMEOVER = 2;           //The game over stage, you lose in this stage 

int gameState;                    //Keeps track of the games stage 

//Keeps track of time, this is used for how long the buttons are not pressed while playing the game
long currentMillis = 0;           //Current time since game has started
long previousMillis = 0;          //Time since currentMillis was recorded 


void setup() {
  lcd.begin(16,2);                //Telling the Arduino how many coloums and rows are on the LCD display
  lcd.setCursor(0,0);             //Set the cursor at the top left corner (0,0)
  
  //Intializing each LED as an OUTPUT by using a for loop
  for(int pin=0; pin<2; pin++)
  {
    pinMode(leds[pin], OUTPUT);
  }

  //Initalizing each pushbutton as an INPUT by using a for loop
  for(int pin=0; pin<2; pin++)
  {
    pinMode(buttons[pin], INPUT);
  }
  
  gameState = START; // Start the game (by using the START state)
  
  randomSeed(analogRead(40)); //Initializes the pseudo-random number generator to randomize the order the LEDs flash in
}

void loop() {
  
  if(gameState == START) //If the game state is in the START state
  {
    lcd.setCursor(0,0);                     //Set the cursor at the top left corner (0,0)
    lcd.print("Press Button to PLAY!!");    //Set the cursor at the top left corner (0,0)
    lcd.setCursor(0,1);                     //Change cursor location to bottom left corner
    lcd.print("PLAY!!");                    //Display the text on LCD Display
    
    waitToStart(); //Run the waitToStart subprogram

    lcd.clear();   //Clears the text on LCD Display
  }

  else if(gameState == PLAY) //If the game in the PLAY state
  {
    showSequence(); //Run the showSequence subprogram
    readSequence(); //Run the readSequence subprogram
  }
  
  else if(gameState == GAMEOVER) //If the game is in the GAMEOVER state
  {
    lcd.clear();                        //Clears the text
    lcd.setCursor(0,0);                 //Set the cursor at the top left corner (0,0)
    lcd.print("YOU LOSE ");             //Set the cursor at the top left corner (0,0)
    gMajorScale();                      //PLay the gMajor Scale 
    lcd.clear();                        //Clears the text
    lcd.setCursor(0,0);                 //Set the cursor at the top left corner (0,0)
    lcd.print("RESTARTING ");           //Set the cursor at the top left corner (0,0)
    
    blinkAll(5);                        //Runs the blinkAll subprogram 5 times
    gameState = START;                  //Restarts the game (By making the game state START again)
  }
}

//A subprogram that makes the LEDs flash
//It also adds one to the end of each sequence then shows the whole sequence
void showSequence()
{  
  //Add a new index to the end of the sequence
  sequence[largestIndex] = random(0,2);

  largestIndex++; //Adds one to the sequence

  lcd.clear();
  lcd.print("YOU ARE PLAYING");
  
  for(int index=0; index<largestIndex; index++) //Using a for loop to loop through the sequence and flash the LEDs 
  {
     delay(500); //Stop code for 500 miliseconds (also shows if same LED blinks twice in a row)
     digitalWrite(leds[sequence[index]], HIGH); //ON the LED

     if((leds[0],HIGH)) //If the red LED is ON
     {
      tone(13, 392); //plays the G4 note on the speaker in pin 13
     }
     
     else if((leds[1],HIGH)) //If the blue LED is ON
     {
      tone(13, 440);  //plays the A4 note on the speaker in pin 13
     }
        
     delay(700); //Stop code for 700 miliseconds
     noTone(13); //Stop playing notes
     digitalWrite(leds[sequence[index]], LOW); //OFF the LED
  }  
}

//A subprogram that reads the sequence the player pushed the buttons in 
void readSequence()
{
  int positionPressed; //Stores which pushbutton was pressed (0 or 1)
  boolean madeMistake = false; //Keeps track if the player made a mistake (pressed wrong button)

  for(int index=0; index<largestIndex & madeMistake == false; index++)//Loops through sequence waiting for buttons to be pressed. If player makes mistake it exits the loop.
  {
    positionPressed = waitForButton(2000); //Waits 2 seconds for player to push a button

    if(positionPressed == -1 | positionPressed != sequence[index]) //If the player didn't press a button in time or pressed the wrong button
    {
      madeMistake = true; // Exit the for loop
      gameState = GAMEOVER; //Change the state to GAMEOVER state (since player loses)
    }
  }
}

// Returns the position of button pressed (0 or 1) or -1 if no button is pressed in the time period.
int waitForButton(int delay)
{
  int buttonPressed = -1; //Checks if button is pressed or not
  int input; //Reads the buttons input 
  boolean buttonBackUp = false; //Checks if the button was released or not
  
  currentMillis = millis();     //Records the number of miliseconds since the program started running
  previousMillis = currentMillis;   //Records the point when we start spinning the loop
  
  // Keep spinning the loop until "delay" seconds have passed or the button has been let back up
  while (currentMillis - previousMillis < delay & buttonBackUp == false)
  {
    //One at a time, check if the button is being pressed down
    for(int pin = 0; pin < 2 & buttonBackUp == false; pin++)
    {
      if(digitalRead(buttons[pin]) == HIGH) //If the button is pressed
      {
        buttonPressed = pin; //Records which button is being pressed
        
        //ONs the LED that corresponds with the button
        digitalWrite(leds[pin], HIGH);
       
        if(pin==0) //If the red LED is ON
        {
          tone(13, 392); //plays the G4 note on the speaker in pin 13
        }
        
        else if(pin==1) //If the blue LED is ON
        {
          tone(13, 440);  //plays the A4 note on the speaker in pin 13
        }

        //It is possible the button is still being pushed
        //This loop loops until the button is let up
        while (currentMillis - previousMillis < delay & buttonBackUp == false)
        {
          input = digitalRead(buttons[pin]); //Read if button is still pushed
          
          if(input == LOW) //If the buttons INPUT is low (let back up)
          {
            buttonBackUp = true; //The button is back up and exits the loop
          }
          currentMillis = millis(); //Record how much time has passed by 
        }
        
        //Turn the LED that corresponds with the button OFF
        digitalWrite(leds[pin], LOW);

        noTone(13); //Speaker plays no sounds
        
        //Check if player took too long to release the button
        if(currentMillis - previousMillis > delay)
        {
          buttonPressed = -1; //They took to long to let the button up so they lose (technically didn't press the button)
        }
      }
    }
    currentMillis = millis(); //Records the time
  }
  
  return buttonPressed; //Returns which button was pressed
}

//A subprogram that turns the LEDs ON one at a time and waits for a button to be pressed to start the game
void waitToStart()
{
  int buttonPressed = -1; //Reads of the button is pressed or not
  allOff(); //Runs the allOff subprogram (all the LEDs are off)

  //Turn each LED ON one at a time
  for(int pin = 0; pin < 2; pin++)
  {
    if(buttonPressed == -1) //If the button is not pushed 
    {
      digitalWrite(leds[pin], HIGH); //Turn the LED ON
      
      if(pin==0) //If the first LED turns ON
      {
        tone(13, 392); //plays the G4 note on the speaker in pin 13
      }

      else if(pin==1) //If the second LED turns ON
      {
        tone(13, 440);  //plays the A4 note on the speaker in pin 13
      }
      
      buttonPressed = waitForButton(800); //Wait for button to be pushed
      noTone(13); //Stop playing notes on speaker
      digitalWrite(leds[pin], LOW); // Turn the LED OFF
    }
  }
  
  if(buttonPressed != -1) //If the button is pressed
  {
    delay(2000); //Stop code ofr 2 seconds
    largestIndex = 0; //Start a new game
    gameState = PLAY; //Game is now in the PLAY state
  }
}

// A subprogram that turns all the LEDs OFF
void allOff()
{
  //Using a for loop to turn all the LEDs OFF
  for(int pin = 0; pin < 2; pin++)
  {
    digitalWrite(leds[pin], LOW); //Turn the LED OFF
  }
}

// A subprogram that turns all the LEDs ON
void allOn()
{
  //Using a for loop to turn all the LEDs ON
  for(int pin = 0; pin < 2; pin++)
  {
    digitalWrite(leds[pin], HIGH); //Turn the LED ON
  }
}

//Blinks all the LEDs a certain number of times
void blinkAll(int times)
{
  //Using a for loop to turn all the LEDs ON and OFF
  for(int count = 0; count < times; count++)
  {
    allOn(); //Runs the allOn subprogram
    delay(300); //Stop code for 300 miliseconds
    allOff(); //Runs the allOff subprogram
    delay(300); //Stop code for 300 miliseconds
  } 
}

void gMajorScale(){ //a subprogram called gMajorScale to play the notes in the G major scale
tone(13, 392, 750); //plays the G4 note for 750 milliseconds on the speaker in pin 13
delay(750); //stops running code for 750 milliseconds

tone(13, 440, 750); //plays the A4 note for 750 milliseconds on the speaker in pin 13
delay(750); //stops running code for 750 milliseconds
          
tone(13, 494, 750); //plays the B4 note for 750 milliseconds on the speaker in pin 13
delay(750); //stops running code for 750 milliseconds

tone(13, 523, 750); //plays the C5 note for 750 milliseconds on the speaker in pin 13
delay(750); //stops running code for 750 milliseconds

tone(13, 587, 750); //plays the D5 note for 750 milliseconds on the speaker in pin 13
delay(750); //stops running code for 750 milliseconds

tone(13, 659, 750); //plays the E5 note for 750 milliseconds on the speaker in pin 13
delay(750); //stops running code for 750 milliseconds


tone(13, 740, 750); //plays the F5 sharp note for 750 milliseconds on the speaker in pin 13
delay(750); //stops running code for 750 milliseconds


tone(13, 659, 750); //plays the E5 note for 750 milliseconds on the speaker in pin 13
delay(750); //stops running code for 750 milliseconds

tone(13, 587, 750); //plays the D5 note for 750 milliseconds on the speaker in pin 13
delay(750); //stops running code for 750 milliseconds

tone(13, 523, 750); //plays the C5 note for 750 milliseconds on the speaker in pin 13
delay(750); //stops running code for 750 milliseconds

tone(13, 494, 750); //plays the B4 note for 750 milliseconds on the speaker in pin 13
delay(750); //stops running code for 750 milliseconds

tone(13, 440, 750); //plays the A4 note for 750 milliseconds on the speaker in pin 13
delay(750); //stops running code for 750 milliseconds

tone(13, 392, 750); //plays the G4 note for 750 milliseconds on the speaker in pin 13
delay(750); //stops running code for 750 milliseconds
}
