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
		//void keyReleased(int key);
		//void mouseMoved(int x, int y );
		//void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		//void mouseReleased(int x, int y, int button);
		//void mouseEntered(int x, int y);
		//void mouseExited(int x, int y);
		//void windowResized(int w, int h);
		//void dragEvent(ofDragInfo dragInfo);
		//void gotMessage(ofMessage msg);

		struct LineCircleCollisionResult
		{
			bool collision;
			float cx;
			float cy;
			float dist;
		};

		//bool lineCircle(float x1, float y1, float x2, float y2, float cx, float cy, float r);
		//bool pointCircle(float px, float py, float cx, float cy, float r);
		//bool circleCircle(float c1x, float c1y, float c1r, float c2x, float c2y, float c2r);
		//bool linePoint(float x1, float y1, float x2, float y2, float px, float py);
		//bool lineLine(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);

		ofxPanel gui;
		ofxIntSlider houseWidth;
		ofxIntSlider houseHeight;
		ofxFloatSlider houseScale;
		
};
