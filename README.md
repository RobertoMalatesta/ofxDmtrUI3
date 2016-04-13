# ofxDmtrUI

Made to be really simple and lightweight.  
It is my first public openFrameworks addon.  
Requires oF 0.9+  
Suggestions and code improvements will be greatly appreciated.  
PS: Not for production use.  
http://dmtr.org/  

## Features
Load interface layouts from text files.


## Dependencies
ofxXmlSettings  

# TODO
Detail: Slider steps in integers.
Bang button triggering an event
radio button shortcuts for integers or floats (bang)
radio buttons rectangles according to width.
pass pointer reference of pFloat, pBool to update functions of classes.  
radio images for presets.
add namespace to classes.  
multiple toggle object  
vertical sliders  
toggle wall grids
toggle shortcut for sliders

integration with ofxMidi for midi controllers use
port to lib cinder 

# Ideas
Performance : maybe check inside panel rectangle if mouse event is inside before check in each panel element.

## 2016 04 12
radio buttons flowing correctly
dirtiest working too

## 2016 04 08
radio buttons first draft.

## 2016 04 07
slider integer working OK

## 2016 04 06  
multiple columns now possible
interface building from text file
min max and def working on sliders.

## 2016 04 05  
possibility of creating multiple columns  
Solved drag to outside slider  
ofClamp  
github first draft  
example working inside addon folder  
addon_config.mk added  
ready added.  

## 2016 04 04  
primeiro rascunho da primeira versao do ofxDmtrUI  
toggle, toggle drag event  
package inside an addon and bind events
load save presets in xml
objects containers of panels. 
os eventos de mouse só entram em cada painel primeiramente.  
talvez fazer subcolunas tb.