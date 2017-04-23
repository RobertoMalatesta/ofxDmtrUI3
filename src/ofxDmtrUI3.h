/**********************************************************************************

 Copyright (C) 2016 Dimitre Lima (www.dmtr.org)

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

 **********************************************************************************/


//#define NOXML

#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include "ofxXmlSettings.h"
#include "ofxDmtrUIElements.h"

#if defined( TARGET_OF_IPHONE ) || defined( TARGET_OF_IOS ) || defined( TARGET_ANDROID )
	#define DMTRUI_TARGET_TOUCH
#endif


class ofxDmtrUI3 : public ofBaseApp
{
public:
	~ofxDmtrUI3();
	ofxDmtrUI3();
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void keyReleased(int key);

	void createFromText(string file);
	void createFromLine(string line);

	//auto getVal(string n);
	void save(string xml);
	void load(string xml);

	vector <string> textToVector(string file);
	vector <element*> elements;

	void onDraw(ofEventArgs &data);
	void onUpdate(ofEventArgs &data);

	void onMousePressed(ofMouseEventArgs &data);
	void onMouseDragged(ofMouseEventArgs &data);
	void onMouseReleased(ofMouseEventArgs &data);
	void onKeyPressed(ofKeyEventArgs &data);
	void onKeyReleased(ofKeyEventArgs &data);

//	void onMouseMoved(ofMouseEventArgs &data);
	void onExit(ofEventArgs &data);
	void onWindowResized(ofResizeEventArgs &data);

	void uiEvents(string & e);

	//ofPoint flow = ofPoint(10, 10);
	uiConfig settings;

	ofFbo fboUI;
//	bool onlyDrawOnRedraw = false;

	map <string,float>	pFloat;
	map <string,bool>	pBool;

	void reFlow();

};