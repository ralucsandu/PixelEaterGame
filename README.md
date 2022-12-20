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
Create a menu for your matrix game. You should use the joystick to scroll on the LCD. The menu should include the following functionalities:
<br>
a) Start game: starts the initial level of your game
  
b) Highscore: initially, it is 0. Update it when the game is done. Save the top 5 values in EEPROM with name and score

c) Settings: reset highscore, enter name, level, LCD brightness/contrast control(saved to EEPROM), matrix brightness control(saved to EEPROM), sound on/off (saved to eeprom)

d) About: details about the creator (game name, author, GitHub user)

e) How to play: short and informative description

Also: 

- When powering up the game: a greeting message should be shown for a few moments;

- While playing the game: display all the relevant info(lives, score, time);

- Upon game ending: message to inform the player if they beat the highscore; 
</details>
