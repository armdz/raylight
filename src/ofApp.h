#pragma once

#include "ofMain.h"
#include "ofxGui.h"

struct Obstacle{
  ofVec2f pos;
  float   size;
  int     resolution;
  ofMesh   mesh;
};

struct LightPoint
{
  ofVec2f   pos;
  ofVec2f   ray_point;
  float     distance;
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
  
  private:
    void  generate_scene();
    void  make_poly(Obstacle  &_obstacle);
    //  gui
    ofxPanel      gui;
    ofxIntSlider  obstacles_count;
    ofxIntSlider  min_poly_size;
    ofxIntSlider  max_poly_size;
    ofxColorSlider  obstacle_color;
    ofxButton     do_generate_scene;
    ofxIntSlider  ray_count;
    ofxIntSlider  light_radius;
  
    //
    ofVec2f   point;
    vector<Obstacle>   obstacles;
    vector<LightPoint> light_points;
    float     radius;
		
};
