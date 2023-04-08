import board
import digitalio
import usb_hid

from hid_gamepad import Gamepad

# Atari joystick pinout - seen from the front
# \1 2 3 4 5/
#  \6 7 8 9/
#
# 1 - up, 2 - down, 3 - left, 4 - right, 5 - not connected 
# 6 - fire 7 - not connected, 8 - GND (LOW), 9 - not connected
#
# Spectrum +2A joystick pinout - seen from the front
# \1 2 3 4 5/
#  \6 7 8 9/
#
# 1 - not connected, 2 - ground, 3 - not connected, 4 - fire, 5 - up 
# 6 - right 7 - left, 8 - GND (LOW), 9 - down

PIN_DB9_1 = board.GP15
PIN_DB9_2 = board.GP26
PIN_DB9_3 = board.GP27
PIN_DB9_4 = board.GP28
PIN_DB9_5 = board.GP29
PIN_DB9_6 = board.GP14
PIN_DB9_7 = board.GP4
PIN_DB9_8 = board.GP3
PIN_DB9_9 = board.GP6

gnd = digitalio.DigitalInOut(PIN_DB9_8)
gnd.direction = digitalio.Direction.OUTPUT
gnd.value = False

input1 = digitalio.DigitalInOut(PIN_DB9_1)
input2 = digitalio.DigitalInOut(PIN_DB9_2)
input3 = digitalio.DigitalInOut(PIN_DB9_3)
input4 = digitalio.DigitalInOut(PIN_DB9_4)
input5 = digitalio.DigitalInOut(PIN_DB9_5)
input6 = digitalio.DigitalInOut(PIN_DB9_6)
input7 = digitalio.DigitalInOut(PIN_DB9_7)
input9 = digitalio.DigitalInOut(PIN_DB9_9)

inputs = [input1, input2, input3, input4, input5, input6, input7, input9]

for input in inputs:
	input.direction = digitalio.Direction.INPUT
	input.pull = digitalio.Pull.UP

up = input1
down = input2
left = input3
right = input4
fire = input6

if not input4.value: # Spectrum +2A fire pressed
	up = input5
	down = input9
	left = input7
	right = input6
	fire = input4
else: # enable +5V for auto fire for standard Atari/Amiga joysticks
	input7.direction = digitalio.Direction.OUTPUT
	input7.value = True
	

joystick = Gamepad(usb_hid.devices)

while True:
	joystick.move_joysticks(
		x = -127 if not left.value else 127 if not right.value else 0,
		y = -127 if not up.value else 127 if not down.value else 0,
	)
	
	if fire.value:
		joystick.release_buttons(1)
	else:
		joystick.press_buttons(1)
