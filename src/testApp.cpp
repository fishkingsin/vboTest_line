#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	lines = new RunningLine[GRID_WIDTH*GRID_HEIGHT];
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofEnableSmoothing();
	bPause	   = false;
	restLength = 3.0;
	space      = 200;
	
	center.x   = ((GRID_WIDTH-1)  * space) / 2;
	center.y   = ((GRID_HEIGHT-1) * space) / 2;
	
	for (int i=0; i<GRID_WIDTH; i++) {
		for (int j=0; j<GRID_HEIGHT; j++) {
			
			int x	  = (i * space) - center.x;
			int y     = (j * space) - center.y;
			int z     = 0;
			int index = (j*GRID_WIDTH+i) * LENGTH;
			
			// now we are at each line
			for (int k=0; k<LENGTH; k++) {
				
				pos[index + k].x = x;
				pos[index + k].y = y;
				pos[index + k].z = z;
				color[index + k] = ofFloatColor::white;
			}
			
			
		}
	}
	
	total = GRID_WIDTH*GRID_HEIGHT*LENGTH;
	vbo.setVertexData(pos, total, GL_DYNAMIC_DRAW);
	vbo.setColorData(color, total, GL_DYNAMIC_DRAW);
	ofSetVerticalSync(false);
	ofEnableAlphaBlending();
	
#ifndef TARGET_LINUX_ARM
	post.init(ofGetWidth(), ofGetHeight());
	post.createPass<FxaaPass>()->setEnabled(true);
    post.createPass<BloomPass>()->setEnabled(true);
    post.createPass<DofPass>()->setEnabled(true);
#else
	shader.setGeometryInputType(GL_LINES);
	shader.setGeometryOutputType(GL_TRIANGLE_STRIP);
	shader.setGeometryOutputCount(LENGTH);
#ifdef TARGET_OPENGLES
	shader.load("shaders/vertGLES.glsl", "shaders/fragGLES.glsl", "shaders/geomGLES.glsl");
#else
	shader.load("shaders/vert.glsl", "shaders/frag.glsl", "shaders/geom.glsl");
#endif
	//	printf("Maximum number of output vertices support is: %i\n", shader.getGeometryMaxOutputCount());
#endif
	glEnable(GL_DEPTH_TEST);
}

//--------------------------------------------------------------
void testApp::update(){
	
	if(!bPause) {
		ofVec3f vec;
		float   r = 0.3;
		int lineCount = 0;
		for (int i=0; i<GRID_WIDTH; i++) {
			for (int j=0; j<GRID_HEIGHT; j++) {
				
				
				int index = (j*GRID_WIDTH+i) * LENGTH;
				lines[lineCount].update();
				int x	  = (i * space) - center.x+lines[lineCount].output.x - (lines[lineCount].rect.width*0.5);
				int y     = (j * space) - center.y+lines[lineCount].output.y - (lines[lineCount].rect.height*0.5);
				int z     = lines[lineCount].output.z+lines[lineCount].z;
				
				
				for (int k=LENGTH-1; k>=1; k--) {
					vec		 =  pos[index + k] - pos[index + (k-2)];
					float d  = vec.length();
					
					pos[index + k] = pos[index + k-1];
					
//					if(lines[lineCount].direction==0)
//					{
//						pos[index + k].z = lines[lineCount].z;
//					}
//					else
//					{
//						pos[index + k].z += lines[lineCount].v.z;
//					}
				}
				
				pos[index].set(x, y, z);
				pos[index+1].set(x, y, z);
				if(abs(z-pos[index + (LENGTH-1)].z)>1000)
				{
					for (int k=0; k<LENGTH; k++) pos[index+k].z=lines[lineCount].z;
					
				}
				for (int k=0; k<LENGTH; k++)
				{
					pos[index + k].z += lines[lineCount].v.z;
					color[index + k] = ofFloatColor(1,1,1,ofMap(z, -5000, 1000, 0, 1));
				}
				
				
				lineCount++;
				
			}
		}
	}
	
	
	//	zoom += (zoomTarget-zoom) * 0.04;
	
}

//--------------------------------------------------------------
void testApp::draw() {
		ofBackgroundGradient( ofColor::black , ofColor(10,10,10));
	if(doShader) {
#ifndef TARGET_LINUX_ARM
		// copy enable part of gl state
		glPushAttrib(GL_ENABLE_BIT);
		
		// setup gl state
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		post.begin();
#else
	shader.begin();

	// set thickness of ribbons
	shader.setUniform1f("thickness", 3);

	// make light direction slowly rotate
		shader.setUniform3f("lightDir", 0,0,1);//sin(ofGetElapsedTimef()/10), cos(ofGetElapsedTimef()/10),1);
#endif
	}
	ofBackgroundGradient( ofColor::black , ofColor(10,10,10));
	ofPushMatrix();
	ofTranslate(ofGetWidth()/2, ofGetHeight()/2, zoom);
	
	
	
	
	// the lines
	ofEnableAlphaBlending();
	ofSetColor(255, 255, 255);
	glLineWidth( 5.0f );
	vbo.bind();
	vbo.updateVertexData(pos, total);
	vbo.updateColorData(color, total);
	
	for (int i=0; i<GRID_WIDTH; i++) {
		for (int j=0; j<GRID_HEIGHT; j++) {
			int index = (j*GRID_WIDTH+i) * LENGTH;
			
			vbo.draw(GL_LINE_STRIP, index, LENGTH);
		}
	}
	
	vbo.unbind();
	
	ofPopMatrix();
	if(doShader) {

#ifndef TARGET_LINUX_ARM
		post.end();

		// set gl state back to original
		glPopAttrib();
#else
				shader.end();
#endif
		
	}
	
	ofSetColor(90, 90, 90);
	ofDrawBitmapString(ofToString(ofGetFrameRate(), 1), 5, 20);
	
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if( key == 's' ){
		doShader = !doShader;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	//zoomTarget = ofMap(x, 0.0, ofGetWidth(), 100, 500);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	bPause = !bPause;
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
	
}