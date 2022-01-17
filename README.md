# runwalk timer

Timer to help in trainging for marathons.  Set run vs walk time.  Ratio of run to walk is set by web page hosted by the ESP32.

## parts ##

1.  Heltec WifiKit32 with built in OLED screen
2.  400 mAh lipo battery
3.  Switch
4.  Piezo Buzzer

## wiring ##
![runwalk_wiring](https://user-images.githubusercontent.com/31633408/149828180-8af3a792-5669-4ec4-a0bd-a1322456ff0b.png)

## operation ##
Power on device.  Default setting is a cycle of 120 min walking with 60 min of running.  Piezo speaker will create a low-high tone when it is time to switch to running and a high-low tone when it is time to walk.  Screen will show how much time you have run or walked, total cumulative run/walk time, icon to show current walk or run cycle and a bar chart to show how much time is left in the current cycle.

