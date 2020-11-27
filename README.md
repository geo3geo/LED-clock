# LED-clock
   A very simple Arduino base 12 hr / 60 min clock based on a 24LED circle
   Fine if you just need the approx time! As there are 60 minutes but only 24 LEDs the minutes area displayed only to a resolution of 60/24 minutes (see code)
   The 'sweep hand' brights up the 3/6/9/12 positions to give better reading of the time.
   
   Uses a single pushbutton for changing the time, with a pull-up of about 10K to +5
   Provides auto dimming for night/day compensation using a photresistor connected to an analogue input, with a pull-up resistor of about 10K to +5.   

   Press the Setup button to enter setup mode, the sweep hand will stop, the hour led will flash, 
   press th and sweep will restart.e button to increment hours. no press for 3 seconds causes hours LED to stop flashing and the minutes LED to flash, 
   press button to increment minutes. No press for 3 seconds causes end of exit mode and the sweep will restart.
   
   There are various clock facias on Google images so print out whichever takes your fancy or design your own!
   
   
