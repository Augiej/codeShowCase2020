This code utilizes GPIO pins to read a voltage from a voltage generator when a button is pressed a certain number of times. Everytime the button on the board is pressed a counter
is iterated by + 1 until it gets to 5. When it hits 5 it reads an analog voltage from a GPIO pin then converts the voltage through an ADC. 
This digital voltage value is then transmitted via UART to the computer.