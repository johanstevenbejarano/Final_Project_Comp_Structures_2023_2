# C4Model
Use the [structurizr-dsl](https://www.structurizr.com/dsl) online tool to render the diagrams with the code in [C4Model.dsl](C4Model.dsl)

## Context Diagram
The user can prompt the digital lock system either by using:
1. The debug terminal from a host PC through the ST-Link COM port.
2. The hex keypad for password related transactions.
3. The internet over WIFI.
![context](context.png)

## Container Diagram
The lock system features an STM32L476RG as the main processor and an [ESP8266](https://www.espressif.com/en/products/socs/esp8266) as an auxiliar for conenction with the internet (see [ESP-Link](https://github.com/jeelabs/esp-link)). The STM32 controls a hex keypad, ultrasonic sensor a VCOM port for debug, an OLED display for GUI, and a PWM output to simulate the actuator of the lock system.
![context](contenedor.png)

## Component Diagram
The STM32 runs a [controller-model-view](https://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93controller) pattern in which the controller captures the events from the keypad as well as the commands from the internet, then the model processes the events and updates the view accordingly.
![context](component.png)

## Code Diagram
*TODO:* Add the code diagrams here.
