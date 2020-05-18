Compatible with the nucleo_f401re board under zephyr RTOS.

Standard Service example to test with micro-ROS and zephyr RTOS.
Speed result with relaiablity QoS Settings: ???

Change the Baud via <boardname>.overlay and the corresponding USART device. Default settings can be found in zephyrproject/zephyr/boards/<arch>/<boardname>/<boardname>.dts
i.e. here: https://github.com/zephyrproject-rtos/zephyr/blob/master/boards/arm/nucleo_f401re/nucleo_f401re.dts

Corresponding to https://docs.zephyrproject.org/1.14.0/application/index.html#device-tree-overlays <boardname>.overlay files are automatically detected without additional specification in the CMakeList.txt files