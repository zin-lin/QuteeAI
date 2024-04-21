# Firmware
Please read the main Readme before reading this. 
There solutions are provided 
- Using `esp32s3` (8MB for RAM) - Developed by [adaptive-intelligent-robotics](https://github.com/adaptive-intelligent-robotics/Qutee_v2)
  - wifi ✓, self-processing ✓, usb-debugging ❌
- Using `Arduino Uno` and potentially wifi `Nano-esp32` (very powerful at 16MB of RAM for a microcontroller). Self-development
  - wifi ✓, self-processing ✓, usb-debugging -testing
- Using `U2D2 device` (No RAM). Self-development
  - wifi ❌, self-processing ❌, usb-debugging ✓
###
- #### For the first solution please visit the repository made by the original developers.

### Hardware preparation 

- Before we do anything with the code the Dynamixel motors are needed to be set up so that all three solutions are usable.
- The servos when they come in, all have the `ID = 1`, which needed to be changed to `11`, `12`, `13`, `21`, `22`, `23`, `31`, `32`, `33`, `41`, `42`, `43`
- This can be done using the `U2S2` device and `Dynamixel Wizard` Application [Dynamixel](https://emanual.robotis.com/docs/en/software/dynamixel/dynamixel_wizard2/)
  - Please download this application on your PC.
  - Then connect the U2D2 device with the PC using USB-A and start the `Dynamixel Wizard`.
  - Just to be sure you must select to update the firmware on the servo
    - Select the right model and upload firmware
    - Then hit scan 
    - and now you will see your servo
    - set the ID to the ones given above
    - Note: do this one by one: do not daisy-chain them 
    - ![Wizard](snp.png)
    


