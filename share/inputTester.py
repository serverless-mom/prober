import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BCM)
GPIO.setup(36, GPIO.IN, pull_up_down = GPIO.PUD_DOWN)
GPIO.setup(38, GPIO.IN, pull_up_down = GPIO.PUD_UP)
while True:
    if(GPIO.input(36) == 1):
        print('button1')
    if(GPIO.input(38) == 0):
        print('button2')
    GPIO.cleanup()
