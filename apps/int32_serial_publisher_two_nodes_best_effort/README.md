Compatible and tested with the nucleo_f401re board under zephyr RTOS.

Contains 2 int32 publishers in two seperate nodes with Best_Effort RMW-QoS settings.
Resulting in 250Hz rate at 115200 baud each.

Change the Baud via <boardname>.overlay and the corresponding USART device. Default settings can be found in zephyrproject/zephyr/boards/<arch>/<boardname>/<boardname>.dts
i.e. here: https://github.com/zephyrproject-rtos/zephyr/blob/master/boards/arm/nucleo_f401re/nucleo_f401re.dts

Corresponding to https://docs.zephyrproject.org/1.14.0/application/index.html#device-tree-overlays <boardname>.overlay files are automatically detected without additional specification in the CMakeList.txt files