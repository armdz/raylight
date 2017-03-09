#include "ofApp.h"

//  8/3 #niunamenos #vivaslasqueremos
//  macri gato

//--------------------------------------------------------------
void ofApp::setup(){

  gui.setup();
  
  obstacles_count.setup("Obstacles Count",50,10,200);
  min_poly_size.setup("Min Obstacle Size",10,10,200);
  max_poly_size.setup("Max Obstacle Size",10,10,200);
  obstacle_color.setup("Obstacle Color",ofColor(255),ofColor(0),ofColor(255));
  do_generate_scene.setup("Regenerate",false);
  ray_count.setup("Ray Count",50,4,500);
  light_radius.setup("Light Radius",10,10,ofGetWidth()/2);
  
  gui.add(&obstacles_count);
  gui.add(&min_poly_size);
  gui.add(&max_poly_size);
  gui.add(&obstacle_color);
  gui.add(&do_generate_scene);
  gui.add(&ray_count);
  gui.add(&light_radius);

  gui.loadFromFile("settings.xml");
  
  ofSetBackgroundAuto(true);
  ofBackground(0);
  generate_scene();

}

void ofApp::make_poly(Obstacle  &_obstacle)
{
  _obstacle.mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
  for(int i=0;i<_obstacle.resolution+1;i++)
  {
    float delta = (float)i/(float)_obstacle.resolution;
    float angle = delta*TWO_PI;
    float x = cos(angle)*_obstacle.size;
    float y = sin(angle)*_obstacle.size;
    _obstacle.mesh.addVertex(ofVec2f(x,y));
    _obstacle.mesh.addColor((ofColor)obstacle_color);
  }
  
}

void  ofApp::generate_scene()
{
  
  obstacles.clear();
  
  for(int i=0;i<obstacles_count;i++)
  {
    Obstacle  obstacle;
    obstacle.pos.x = ofRandomWidth();
    obstacle.pos.y = ofRandomHeight();
    obstacle.size = ofRandom(min_poly_size,max_poly_size);
    obstacle.resolution = ofRandom(4,16);
    make_poly(obstacle);
    obstacles.push_back(obstacle);
  }
  
}

//--------------------------------------------------------------
void ofApp::update(){
  
  ofSetWindowTitle(ofToString(ofGetFrameRate(),2));
  
  if(do_generate_scene)
  {
    generate_scene();
  }
  
  light_points.clear();

  for(int i=0;i<ray_count;i++)
  {
    float delta = (float)i/(float)ray_count;
    float angle = delta*TWO_PI;
    float dis_x = point.x+ofGetWidth();
    float dis_y = point.y+ofGetHeight();
    float ray_x = point.x+cos(angle)*light_radius;
    float ray_y = point.y+sin(angle)*light_radius;
    ofPoint ray_point = ofPoint(ray_x,ray_y);
    
    float     intersec_dist = 999999;
    bool      intersected = false;
    ofPoint*  intersected_point = NULL;
    
    for(int o=0;o<obstacles.size();o++)
    {
      Obstacle*  cur_obstacle = &obstacles.at(o);
      float distance_with_light = ofDist(cur_obstacle->pos.x, cur_obstacle->pos.y, point.x, point.y);
      
      if(distance_with_light < light_radius){
        
        ofMesh* cur_mesh = &cur_obstacle->mesh;

        for(int v_index=1;v_index<cur_mesh->getNumVertices();v_index++)
        {
          ofVec2f vertex_0 = cur_obstacle->pos+cur_mesh->getVertices().at(v_index-1);
          ofVec2f vertex_1 = cur_obstacle->pos+cur_mesh->getVertices().at(v_index);

          ofPoint _intersection_point;
          bool    intersects = ofLineSegmentIntersection(point, ray_point, vertex_0, vertex_1, _intersection_point);
          
          if(intersects){
            intersected = true;
            float dist = ofDist(point.x,point.y,_intersection_point.x,_intersection_point.y);
            if(dist < intersec_dist){
              intersec_dist = dist;
              intersected_point = new ofPoint(_intersection_point.x,_intersection_point.y);
            }
          }
          
        }
        
      }
      
    }
    
    LightPoint  light_point;
    
    light_point.pos.set(point.x, point.y);
    
    if(intersected_point != NULL){
      light_point.ray_point = ofVec2f(intersected_point->x,intersected_point->y);
    }else{
      light_point.ray_point = ofVec2f(ray_point.x,ray_point.y);
    }
    light_points.push_back(light_point);
    
    delete intersected_point;
    
  }

}

//--------------------------------------------------------------
void ofApp::draw(){

  
  
  //  avoid to check if the light source is inside of a osbtacle-mesh
  for(int i=0;i<obstacles.size();i++)
  {
    ofPushMatrix();
    ofTranslate(obstacles.at(i).pos);
    obstacles.at(i).mesh.draw();
    ofPopMatrix();
  }
  
  
  ofMesh  mesh;
  
  mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
  mesh.addVertex(point);
  mesh.addColor(ofFloatColor(1.0,1.0,1.0,1.0));
  
  ofFloatColor  first_color;

  for(int i=0;i<light_points.size();i++)
  {
    LightPoint  light_point = light_points.at(i);
    ofVec2f l_pos = light_point.pos;
    ofVec2f r_pos = light_point.ray_point;
    
    float ray_dis = ofDist(point.x, point.y, r_pos.x, r_pos.y);
    float dis_dif = ray_dis/(float)light_radius;
 
    ofFloatColor  color;
    color.setHsb((float)i/(float)light_points.size(), 1.0, 1.0);
    color.a = 0.0f;
    mesh.addColor(color);
    if(i == 0)
      first_color = color;
    
    mesh.addVertex(r_pos);
  }
  
  mesh.addColor(first_color);
  mesh.addVertex(mesh.getVertex(1));
  
  ofPushStyle();
  ofEnableBlendMode(OF_BLENDMODE_ADD);
  mesh.draw();
  ofPopStyle();

  
  gui.draw();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
  point.x = x;
  point.y = y;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
