# Embedded System Project - Hand Gesture Unlock

This repository showcases an embedded system project that utilizes a single accelerometer and/or gyro to record a hand movement sequence as a means to unlock a resource. The recorded sequence is saved on a microcontroller and can be replicated by the user to successfully unlock the resource. The project is implemented using the Atmel Circuit Playground Classic Microcontroller and the PlatformIO IDE.

## Objective

The objective of this project is to develop a hand gesture-based unlocking mechanism using the data collected from a single accelerometer and/or gyro. The specific goals are as follows:

- Record a hand movement sequence using the accelerometer/gyro to act as a key for unlocking.
- Save the recorded key sequence on the microcontroller.
- Replicate the key sequence within sufficient tolerances to unlock the resource.
- Provide a visual indication, such as an LED, to signify a successful unlock.

## Restrictions

The project has the following restrictions:

- Only one microcontroller and one accelerometer/gyro should be used, preferably the one integrated on the Atmel Circuit Playground Classic Microcontroller board.
- PlatformIO should be used as the development environment, as done throughout the class.
- The accelerometer/gyro should be held in a closed fist of either hand while performing the mechanical sequence.
- Implement an "enter key" and "record" functionality to guide the user during the unlocking and recording process, respectively.

## Installation and Usage

To use this project, follow these steps:

1. Clone the repository to your local machine.
2. Open the project in the PlatformIO IDE.
3. Connect the Atmel Circuit Playground Classic Microcontroller to your computer.
4. Build and upload the code to the microcontroller using PlatformIO.
5. Ensure that the accelerometer/gyro is securely held in a closed fist.
6. Press the right button on the Circuit Playground board to start recording the hand gesture sequence.
7. Once the recording is complete, the gesture data will be saved on the microcontroller.
8. To unlock the resource, press the left button on the Circuit Playground board.
9. Perform the hand gesture sequence in the same manner as during recording.
10. The system will compare the real-time gesture data with the saved gesture data.
11. If the correlation between the two is above a certain threshold, the resource will be unlocked and indicated by a visual cue.
12. If the correlation falls below the threshold, the unlocking attempt will be unsuccessful.

## Contributing

Contributions to this project are welcome. If you encounter any issues or have suggestions for improvements, please feel free to open an issue or submit a pull request.

## License

This project is licensed under the [MIT License](LICENSE). You are free to modify and distribute the code as per the terms of