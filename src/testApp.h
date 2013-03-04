#pragma once

#include "ofMain.h"
#define GRID_WIDTH  8
#define GRID_HEIGHT 8
#define LENGTH		2 * 32
#define TARGET_LINUX_ARM
#ifndef TARGET_LINUX_ARM
#include "ofxPostProcessing.h"
#endif
class RunningLine : public ofVec3f{
public:
	RunningLine()
	{
		int dirX = ofRandom(0, 1);
		direction = int (ofRandom(0,3));
		
		set(0,0,ofRandom(-5000,100));
		
		if(dirX)
		{
				int dir = ofRandom(0, 1);
			v.set(((dir>0.5)?-1:1)*ofRandom(2.5,5),0,ofRandom(0.5,1.5));
		}
		else
		{
			int dir = ofRandom(0, 1);
			v.set(0,((dir>0.5)?-1:1)*ofRandom(2.5,5),ofRandom(0.5,1.5));
			
		}
		
		rect.set( 0,0,ofRandom(70,150),ofRandom(70,150));
	}
	void update()
	{
		
		if(x+v.x > rect.width || x+v.x <0)
		{
			if(x>rect.width)
				x = rect.width;
			else if (x < 0)
				x = 0;
			
			v.y = v.x;
			
			if(y+v.y > rect.height || y+v.y< 0)
				v.y *= -1;
			
			v.x = 0;
			
		}
		else if(y+v.y > rect.height || y+v.y< 0)
		{
			if(y>rect.height)
				y = rect.height;
			else if (y < 0)
				y = 0;
			
			v.x = v.y;
			
			if(x+v.x > rect.width || x+v.x <0)
				v.x *= -1;
			
			v.y = 0;
		}
		
		x += v.x;
		y += v.y;
		z += v.z;
		
		if(direction==0)
		{
			output.x = x;
			output.y = y;
			output.z = 0;
		}
		
		else if(direction==1)
		{
			output.x = x;
			output.y = 0;
			output.z =y;
		}
		else if(direction==2)
		{
			output.x = 0;
			output.y = y;
			output.z = x;
			
		}
		
		
		if(z+v.z>1000)
		{
			z = -2000;
//			output.z = -5000;
		}
//		ofLogVerbose("RunningLine") << *this;
	}
	int direction;
	
	ofVec3f v;
	ofVec3f	output;
	ofRectangle rect;
};
class testApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	ofVbo vbo;
	ofVec3f pos[GRID_WIDTH*GRID_HEIGHT*LENGTH];
	ofFloatColor color[GRID_WIDTH*GRID_HEIGHT*LENGTH];
	RunningLine *lines;
	ofVec3f center;
	
	float restLength;
	int total;
	int space;
	int count;
	bool bPause;
	int zoom;
	int zoomTarget;

	bool doShader;
#ifndef TARGET_LINUX_ARM

	ofxPostProcessing post;
#else
		ofShader shader;
#endif
};
