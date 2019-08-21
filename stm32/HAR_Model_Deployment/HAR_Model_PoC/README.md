A sample project for deploying HAR - Human Activity Recognition keras model on to STM32F7 board.

References:

https://www.st.com/content/st_com/en/stm32-ann.html

There is a getting started video which is the baseline for this project.

Steps:

Code generated using STMCubeIDE after selecting the model and the board on which it will be deployed. Enable USART3 for printing the messages.

Model link: https://github.com/Shahnawax/HAR-CNN-Keras

Download and integrate BNO055 sensor

Link: https://ivyknob.com/blog/bno055-gyro-library-for-stm32/

aiValidation.c & main.c have been modified to integrate BNO055 sensor values sent to HAR model and output received on USART- Serial Monitor.