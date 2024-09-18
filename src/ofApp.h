#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		const static int WINDOW_WIDTH = 1000;
		const static int WINDOW_HEIGHT = 1000;
		int nBalls = 10;
		ofApp(int nBalls)
		{
			this->nBalls = nBalls;
		}

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void mousePressed(int x, int y, int button);

		struct LineCircleCollisionResult
		{
			bool collision;
			float cx;
			float cy;
			float dist;
		};

		ofxPanel gui;
		ofxIntSlider houseWidth;
		ofxIntSlider houseHeight;
		ofxFloatSlider houseScale;
		
};
