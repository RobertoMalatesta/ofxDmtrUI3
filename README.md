# ofxDmtrUI

Made to be really simple and lightweight.  
It is my first public openFrameworks addon.  
Requires oF 0.9+  
Suggestions and code improvements will be greatly appreciated.  
PS: Not for production use, yet.  
Tested on OS X. Let me know if you are using it. Examples will be created under demand  
http://dmtr.org/ofxDmtrUI  

![Example](http://hifolio.com/media/1/Screen_Shot_2016-04-15_at_12.45.11_PM.png)


## Features
Load interface layouts from text files.
See section Text File Format in this README for more info

## Motivation
A C++ exercise for me  
first open source project, hope to get collaboration from skilled programmers  
lead an interface project from my own needs  
a way of quickly design a prototype interface and spend time in what matters  
a personal substitute for the beloved and deprecated ofxUI  
An UI that is not constantly redrawing on screen. each time one element is refreshed the screen is saved into an FBO. CPU usage is very low. 

## Dependencies
ofxXmlSettings  

# TODO
Different events for loading xml files and updating with the mouse.
./Change some variables named in portuguese  
radio buttons rectangles according to width.  
radio images for presets.
2d Sliders  
add namespace to classes.  
test events in IOs
integration with ofxMidi for midi controllers use  
port to lib cinder  

# Future Ideas
Performance : maybe check inside panel rectangle if mouse event is inside before check in each panel element.
Implement the class Element and make pointers to member functions of draw, check mouse, get val etc.

## 2016 05 04
Flow fixes

## 2016 04 29
“margin” affects marginx and marginy
Presets working. load and save presets with thumbnail images.

## 2016 04 27
Slider2d first sketches
Color first sketch

## 2016 04 18
License added. 

## 2016 04 17
toggle matrix elements
```
togglematrix	checkers	5 3
```

## 2016 04 15
Bang! button, only triggering an event  
radio button shortcuts for integers or floats (bang)  
vertical sliders  
create object from a line
Areia, arrays of toggles.
shortcut to float and int sliders

## 2016 04 14
manual for ui text files 
Pointers to pString working on Radio items and DirList.
Pointers to pBool on toggles
Pointers to pInt and pFloat on Sliders
ui.keepSettings = false; ~ a way of automatically loading and saving xml files from ui on setup and exit.  
parameter “auto fit” for dimensioning rectangles.
pass pointer reference of pFloat, pBool to update functions of classes.    

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




## Text File Format
Text files are a custom format like TSV <tab separated values>
```
label	ofxDmtrUI
label	Code Dmtr.org 2016

marginy	100
label	First Column
float	r	0 255 127
float	g	0 255 127
float	b	0 255 127
radio	speed2shortcut	1 2 4 8

float	DivNoiseX	0 1 .2
float	DivNoiseY	0 1 .2
bool	toggle1
bool	toggle2
bool	toggle3
bool	ok	1
bool	ok2	1
```

Setup values, values that doesn’t display anything but changes the variables in UI instance.
rect<tab>x y width height
```
rect	240 20 840 940
```
defines the interface rectangle, useful when using more than one UI instance, distributing rectangles on the screen.
```
marginx	20
marginy	280
sliderWidth	240
```
changes the rectangle margins and redefine sliderWidth
```
label	Fita 1
float	posicao1	0 1 0
int	divisao1	1 8 1
float	pan1	-1 1 -.5
float	vol1	0 1 1
```
create a label “Fita 1”
a float variable called “posicao1” with min = 0, max = 1, def = 0
```
newcol
label	Fita 2
float	posicao2	0 1 0
```
newcol jumps to a new column to add more widgets.

```
radio	speed2shortcut	1 2 4 8
```

Radio create a radio with n parameters separated by space. useful to list numbers or words in the software
accessible in the map ui.pString[name]


```
dirlist	fita1	/Users/dd/Desktop/WORK_DMTR/AREIA_AUDIO
```
dirlist creates a radio with files listed in a specific directory, relative or absolute path.

