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
   
   https://www.youtube.com/watch?v=hkU9chTgqK8
   
   Added 3 Dec 2020
   ================
   The chimes_sound version also provides a colourful display and hourly annunciation.   
   
   On the hour and half hour a LED pattern is displayed for 20 seconds
   On the quarter hour a different pattern is displayed, for 10 seconds
   
   On the hour a 'chime' sequence is played, but this is inhibited at night
   (defined by low detected light level) and a series of tones sounded according 
   to the current hour. During the night these are very much shorter to make them less 
   intrusive. For silent operation, simply unplug the sound generator unit! 

   The sound generator used was an old earpiece of about 1K ohm. 
   It was plugged into a 3.5mm audio jack wired to +5 and to the soundPin. 

   The pushbutton connects bewteen setupPin and )v with a pullup resistor (10K-ish) 
   bewteen soundPin and +5.
   
   https://www.youtube.com/watch?v=xTUISuY6ftM&feature=youtu.be
  
   
   Added 9th Jan 2021
   ==================
   Software dated 22Dec uploaded. Minor improvements:
   - the hourly tones are now split into groups of 3 so you don't need to count them!
   - as the minutes pass the half hour the hour hand moves on by 1/24th of a revoloution.
   
