#include "ofApp.h"


ofImage image;
//--------------------------------------------------------------
void ofApp::setup(){
	u.createSoftwareFromText("u.txt");
	for (auto & thisui : u.allUIs) {
		ofAddListener(thisui->settings.uiEvent,this, &ofApp::uiEvents);
	}
	u.loadMaster();
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::desenha() {
	ofSetColor(ui->pColor["color"]);
	
	if (*cena == "bolas") {
		float multTimeX = uiC->pEasy["multTimeX"];
		float multTimeY = uiC->pEasy["multTimeY"];
		for (int a=0; a<uiC->pInt["numero"]; a++) {
			float qual = a/(float)uiC->pInt["numero"];
			float raio = uiC->pEasy["raio"] + qual*uiC->pEasy["raioMax"];
			float x = ofNoise(ofGetElapsedTimef() * multTimeX, a+1) * fbo->getWidth();
			float y = ofNoise(ofGetElapsedTimef() * multTimeY, a*1.2+1) * fbo->getHeight();
			raio = shaper(raio, uiC->pEasy["raio"], uiC->pEasy["raioMax"], uiC->pFloat["shaper"]);
			ofDrawEllipse(x, y, raio, raio);
		}
	}
	
	else if (*cena == "defasa") {
		float defasay = uiC->pEasy["defasaY"];
		float w = uiC->pEasy["w"] + updown * uiC->pFloat["wAudio"];
		for (int y = 0; y<fbo->getHeight(); y++) {
			float x = ofMap(sin(ofGetElapsedTimef() * uiC->pEasy["multTime"] + y * defasay), -1, 1, 0, fbo->getWidth() - w);
			ofDrawRectangle(x, y, w, 1);
			//ofDrawLine(x, y, x+w, y);
		}
	}

	else if (*cena == "image") {
		if (uiC->pImage["image"].isAllocated()) {
			int x = uiC->pInt["offX"];
			int y = uiC->pInt["offY"];
			float scale = uiC->pFloat["scale"];
			int w = uiC->pImage["image"].getWidth() * scale;
			int h = uiC->pImage["image"].getHeight() * scale;
			uiC->pImage["image"].draw(x,y,w,h);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(40);
	fbo->begin();
	ofClear(ui->pColor["bg"]);
	desenha();
	fbo->end();
	float w = fbo->getWidth() * u.pFloat["fboScale"];
	float h = fbo->getHeight() * u.pFloat["fboScale"];
	fbo->draw(u.pInt["fboX"], u.pInt["fboY"], w, h);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::uiEvents(uiEv & e) {

}
