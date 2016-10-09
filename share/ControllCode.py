import RPi.GPIO as GPIO            # import RPi.GPIO module
import time            # lets us have a delay
GPIO.setmode(GPIO.BCM)             # choose BCM or BOARD
GPIO.setup(17, GPIO.OUT)           # step
GPIO.setup(18, GPIO.OUT)           # Direction
GPIO.setup(36, GPIO.IN)           # button
GPIO.setup(38, GPIO.IN)           # button
GPIO.setup(40, GPIO.IN)           # button

GPIO.add_event_detect(36, GPIO.RISING)  # add rising edge detection on a channel
GPIO.add_event_detect(38, GPIO.RISING)  #for both buttons
GPIO.add_event_detect(40, GPIO.RISING)  #for both buttons

insertSteps = 23
pausePoint = 8 #how many steps to go back out before waiting
executionTime = 5 #seconds

pauseTime = executionTime/insertSteps
paused = False
currentDistance = 0
start = time.time()
direction = 1 #1 means we're going IN to the rat brain
stage = 0 #0 = insterting, 1 = exit stage 1 , 2= pause during exit, 3 = stopped

def move (inOut):
  GPIO.output(18, inOut)
  GPIO.output(17, GPIO.HIGH)
  time.sleep(.001)
  GPIO.output(17, GPIO.LOW)
  
def distanceChecker(distance):
  global stage
  global direction
  print(currentDistance)
  print('stage')
  print(stage)

  if stage == 0 and distance > insertSteps:
    print('pause')
    time.sleep(5)
    direction = 0
    stage = 1
  if stage == 1 and distance < (insertSteps - pausePoint):
    print('pause')
    time.sleep(5)
    stage = 2
  if stage == 2 and distance < 1:
    stage = 3

try:
  global currentDistance
  while True:
    if not paused:
      if stage == 0:
        currentDistance += 1
        move(direction)
        time.sleep(pauseTime)
      if stage == 1 or stage == 2:
        currentDistance -= 1
        move(direction)
        time.sleep(pauseTime / 3)
      if stage == 3:
        GPIO.cleanup()
        print('done!')
        break
      distanceChecker(currentDistance)


except KeyboardInterrupt:
  GPIO.cleanup()
