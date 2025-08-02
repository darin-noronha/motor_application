<img src="./Images/IFX_LOGO_600.gif" align="right" width="150" />  

# Motor Control Project AURIX TC375 / TLE92104-232QX Driver IC  
**A BDC Motor is controlled by a single button. A single click toggles the rotation direction of the Motor.**  

## Device  
The devices used in this example is AURIX&trade; TC37xTP_A-Step and TLE92104-232QX MOTIX™ Multi MOSFET Driver IC.  

## Board  
The board used for testing is the AURIX&trade; TC375 lite Kit (KIT_A2G_TC375_LITE).  

## Scope of work  
The state of the motor is control via the reset button and the input button on the AURIX. Upon flashing, the motor will begin rotating. To switch the direction of the rotation click the button. Each button click will invert the direction that the motor is spinning. To stop the motor click the reset button  

## Introduction  
- The port of AURIX&trade; can individually be configured either as input or as output  
- When configured as input, the port pin has high impedance. Alternatively, an internal weak pull-up resistor or pull-down resistor can be selected  
- As output the port pin can be configured in push-pull output state or open-drain state  
- The state of a port pin can always be read back, independent whether a port pin is configured as input or output  

The code is written for the TLE92104 Motor Driver
## Implementation  
**Initialization of GPIO**  
Harware/Software
The function *initGtmTomPwm()* configures the X303 Pin P02.6 port pin to drive the motors PWM signal. The PWM signal is connected to the PWM 1 pin on the TLE 92104's gUI port. The function also initializes the CLK, MOSI, MISO, and CSN pins. Pins P10.2, 10.3, 10.1, and 10.5, respectively. These are wired to the TLE SPI port. Be sure to connect X302 3.3V, 5V and GND to the TLE EN, VDD, and GND pins, respectively.

**Check Button**  
The *checkButton()* function checks if the button has been pressed toggles the rotation of the motor to flip

**TLE ApplLayer Init**  
The *TLE9210x_ApplLayer_Init()* function initializes the API for the TLE registers. After initializing this function, you are able to configure the routing of the signals as well as the setting the TLE to be in nomral operating mode. You can also call different functions which trigger different functionalites such as staic mode, PWM mode, and deactivate. 


## Compiling and programming  
Before testing this code example:  
- Connect the board to the PC through the USB interface  
- Build the project using the dedicated Build button <img src="./Images/build_activeproj.gif" /> or by right-clicking the project name and selecting "Build Project"  
- To flash the device and immediately run the program, click on the dedicated Flash button <img src="./Images/micro.png" />  

## Run and Test  
After code compilation and flashing the device, you can observe the following behavior:  
- While the button is pressed once, the motor rotation starts/inverts  
- While the reset button is pressed, the motor stops

## References  
AURIX&trade; Development Studio is available online:  
- <https://www.infineon.com/aurixdevelopmentstudio>  
- Use the "Import..." function to get access to more code examples  

More code examples can be found on the GIT repository:  
- <https://github.com/Infineon/AURIX_code_examples>  

For additional trainings, visit our webpage:  
- <https://www.infineon.com/aurix-expert-training>  

For questions and support, use the AURIX&trade; Forum:  
- <https://community.infineon.com/t5/AURIX/bd-p/AURIX>  
