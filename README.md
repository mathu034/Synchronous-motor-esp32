Project setup:
Requirements :
ESP-IDF 5.1 https://github.com/espressif/esp-idf/tree/release/v5.1
Arduino as component : https://github.com/espressif/arduino-esp32/tree/idf-release/v5.1

Instruction for the installation of Arduino as component: https://espressif-docs.readthedocs-hosted.com/projects/arduino-esp32/en/latest/esp-idf_component.html
If you'd like to use Arduino as component in more projects, install it in the User/ESP /5.1/esp-idf/components
Use this command to clone Arduino core to esp-idf:
Navigate to esp-> 5.1 -> esp-idf -> components ->
"git clone -b idf-release/v5.1 --recursive https://github.com/espressif/arduino-esp32.git arduino"

 Once installed arduino can be used in any of your projects. 

This version has only been tested with the above given ESP and Arduino versions. However, now the ESP32C6, is supported by Arduino as long as the development branch is selected from ESP.


Once everything is installed.
Ctrl+Shift+P -> command palate on VScode
Create new idf project.
Set location -> select blank sample project
Clear the contents of the main.c file
Replace the contents with the code given. 
Rename file to main.cpp, ensure it is main.cpp in the CMakelists.txt as well.

Once the project is built, go to 'sdkconfig', and make this change: CONFIG_FREERTOS_HZ=1000

TE_SM9000 sensor:
https://www.arduino.cc/reference/en/libraries/te-sm9000-series/
Create component, replace component.c and component.h with the appropriate c++ and header files from the SM9000 library.

CMakelists:
idf_component_register(SRCS "TE_SM9000_series_1.2.1.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES arduino-esp32)



Three C++ files; main, task_motor, task_sensor. 
Once they are all in the same folder, add the following to the CMakelists
Under the CMake list in main: 
idf_component_register(SRCS "task_motor.cpp" "main.cpp" "task_sensor.cpp"
                    INCLUDE_DIRS ".")

This will let the CMake compiler know there are 3 files to be compiled.


Full clean:

Uninstall extension from VSCode. 

Navigate to C:\Users\User\esp

Delete the files. 

Then C:\Users\User\.vscode\extensions

and delete the ESP-espressif extension.

Next Delete the C:\Users\User\.espressif folder
