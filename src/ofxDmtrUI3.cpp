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

/*

ofxDmtrUI.h
Created by Dimitre Lima on 04/06/2016.
http://dmtr.org/

*/

#include "ofxDmtrUI3.h"

//--------------------------------------------------------------
void ofxDmtrUI3::setup() {
	int y = 10;
	for (int a=0; a<6; a++) {
		elements.push_back(slider("slider" + ofToString(a), 0.5, 10, y));
		y += 30;
	}
	for (int a=0; a<6; a++) {
		elements.push_back(toggle("toggle" + ofToString(a), 0.5, 10, y));
		y += 30;
	}

	int h = 60;
	for (auto & e : elements) {
		e.setColor(ofColor::fromHsb(h%255, 255, 255));
		h += 10;
	}
}
// END SETUP

//--------------------------------------------------------------
void ofxDmtrUI3::update() {
}

//--------------------------------------------------------------
void ofxDmtrUI3::draw() {
	for (auto & e : elements) {
		e.draw();
	}
}
