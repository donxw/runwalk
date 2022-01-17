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
Power on device.  No default is set so on first boot follow these steps:
* connect to wireless AP broadcast by the ESP32:  SSID: runwalk / Password: 12345678
* navigate to webpage 192.168.10.2
* fill in the desired run and walk cycle times
* ![runwalk_webpage](https://user-images.githubusercontent.com/31633408/149831297-ac54bf85-75d3-43a2-b175-71692c70dc11.png)
* click submit - on submit the ESP32 will store the settings in EEPROM for future default settings
* disconnect from the wireless AP

Screen will show how much time you have run or walked, total cumulative run/walk time, icon to show current walk or run cycle and a bar chart to show how much time is left in the current cycle.
![fullview (Small)](https://user-images.githubusercontent.com/31633408/149832136-0254f9f4-8c5d-4b21-9566-c85df9cf81ec.JPEG)

Piezo speaker will create a low-high tone when it is time to switch to running, 
![run (Small)](https://user-images.githubusercontent.com/31633408/149831785-96f403c7-1e53-49bb-bdad-b1cc973cbb3c.JPEG)

and a high-low tone when it is time to walk.
![walk (Small)](https://user-images.githubusercontent.com/31633408/149831801-0520bf64-e152-4495-970b-30c22a2afd0c.JPEG)


