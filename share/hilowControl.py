import RPi.GPIO as GPIO            # import RPi.GPIO module
from time import sleep             # lets us have a delay
GPIO.setmode(GPIO.BCM)             # choose BCM or BOARD
GPIO.setup(17, GPIO.OUT)           # step
GPIO.setup(18, GPIO.OUT)           # Direction

try:
    while True:
        GPIO.output(18, GPIO.HIGH)
        GPIO.output(17, GPIO.HIGH)
        sleep(.001)                 # wait half a second
        GPIO.output(17, GPIO.LOW)
        sleep(.001)                 # wait half a second

except KeyboardInterrupt:          # trap a CTRL+C keyboard interrupt
    GPIO.cleanup()                 # resets all GPIO ports used by this program
