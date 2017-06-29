# ofxDmtrUI3

## 2017 june 29
First version is ready to go. all the basics working and building my first project using it.
~~Still not for production use. Handle with care.  
Just float slider and bool toggle working now.~~

## 2017 apr 17
It has been one year since the first version of ofxDmtrUI came out.
Now it is time to commemorate and write another UI, with similar ideas and look, more concise code.

I'll use temporarly this name to test ideas, concepts developed after original ofxDmtrUI, and Cinder-DmtrUI and if it works great and backwards compatible I'll make it the official ofxDmtrUI.

## Example
Example features autoflow, try resizing window.
Press keys from 1 to 4 to load presets
Holding command they save presets.

## Performance.
This ui has been made with two things in mind: ease of use, create and update elements, change order and layout all of them. automatize repetitive tasks. 
And performance in mind. It only redraws the updated element when needed and most of the time is an static FBO texture with a picture of the elements.
There are no animation on elements, scrolling or any features that can slow down your software, eat cpu cycles or battery time. only the raw things needed to work.

## Improvisation
As I've started this project as a music project, you have a lot of freedom interacting with elements, with only one mousedown and drag you can activate or toggle lots of radios.
Of course if it is not the intended way of functioning you can set to work as a normal UI.

## Extendability
Addons were designed in the top of ofxDmtrUI (Original) which extended the functionality to remote mirroring in devices via OSC (Desktop and mobile), even getting accelerometer data from mobile and using a midi controller linked to the UI. 
The intention is to port or make compatible with this new UI too.

## Collaboration
As I'm still learning C++, suggestions are welcome and pull requests too. 

## Share your work
Share your screenshots and pictures featuring this UI on social networks tagging #ofxDmtrUI

Thank you,

Dimitre Lima  
http://dmtr.org/  