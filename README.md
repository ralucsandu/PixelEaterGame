# PixelEaterGame #

## Components 

* joystick
* 74hc595 shift register
* 8x8 LED matrix
* LCD display
* buzzer
* ceramic capacitor
* electrolytic capacitor
* wires
* resistors 
* breadboard

## Task

<details open>
<summary><b>Menu requirements</b></summary>
  <br>
Create a menu for your matrix game. You should use the joystick to scroll on the LCD. The menu should include the following <b>functionalities</b>:
  

  <b>a) Start game:</b> starts the initial level of your game
 
  <b>b) Highscore:</b> initially, it is 0. Update it when the game is done. Save the top 5 values in EEPROM with name and score

  <b>c) Settings:</b> reset highscore, enter name, level, LCD brightness/contrast control(saved to EEPROM), matrix brightness control(saved to EEPROM), sound on/off (saved to eeprom)

  <b>d) About:</b> details about the creator (game name, author, GitHub user)

  <b>e) How to play:</b> short and informative description

Also: 

  <b>- When powering up the game:</b> a greeting message should be shown for a few moments;

  <b>- While playing the game:</b> display all the relevant info(lives, score, time);

  <b>- Upon game ending:</b> message to inform the player if they beat the highscore; 
</details>

<details open>
  <summary><b>Game requirements</b></summary>
  <br>
  The requirements for the game are the following:
  
  <b>a) Minimal components:</b> an LCD, a joystick, a buzzer and the led matrix;
  
  <b>b) Basic sounds to the game:</b> when "eating" a pixel;
  
  <b>c)</b> It must be <b>intuitive</b> and <b>fun</b> to play;
  
  <b>d)</b> It must <b>make sense</b> in the current setup;
  
  <b>e)</b> The levels must progress <b>dynamically</b>;
  
  </details>
  
  ## Game Description and Inspiration
  
  Everybody knows the <b>Pac-Man</b> game, which was released for the first time in May 22th, 1980. 
  
  The idea behind <b>Pixel Eater</b> comes from this so loved game, which all off us played at some point of our lives. 
  
  In <b>Pixel Eater</b>, the goal of the player is to eat all of the pixels making up the shapes displayed on the led matrix, while doing his/her best to achieve a highscore. 
  

## How to use the menu

  When powering on the setup, a welcome message will be displayed on the LCD screen.  
  
  After that, the menu will be displayed on the screen.
  
  By toggling the X axis to the right (as the ">" symbol indicates), the user can enter a subsection of the menu. 
  
  By toggling the Y axis to the left (as the "<" symbol indicates), the user can exit from a subsection of the menu.
  
  By toggling the Y axis, the user can scroll up and down through the menu and its subsections.
  
  *  __Game Menu__ : a static title;
    
  * __Reset HS__ : a question is displayed on the screen. The user can press YES or NO if he/she wants/does not want to reset the highscore table;
    
  * __Start Game__ : the game starts on the led matrix. The timer, the lives and the score are displayed on the LCD screen;
    
  * __Highscores__ : top 5 values are displayed on the LCD. The default values have the following format: TBD(to be decided): 0.
    
   * __Settings__ : 
        *Level
   
    
    
  
