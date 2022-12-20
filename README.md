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

- When powering up the game: a greeting message should be shown for a few moments;

- While playing the game: display all the relevant info(lives, score, time);

- Upon game ending: message to inform the player if they beat the highscore; 
</details>
