# sudo usermod -a -G dialout gutomaia

BOARD_TAG    = uno
ARDUINO_LIBS = SPI Ethernet
ARDUINO_SKETCHBOOK = .

default:
	echo 'test'

include Arduino.mk


