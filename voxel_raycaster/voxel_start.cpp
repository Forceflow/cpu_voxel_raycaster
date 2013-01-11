#include <TriMesh.h>
#include <vector>
#include <GL/glut.h>
#include <iostream>
#include <limits>
#include <windows.h>

#include "RenderContext.h"
#include "util.h"
#include "binvox_tools.h"
#include "avox_tools.h"
#include "mavox_tools.h"
#include "octree_generation.h"
#include "DiffuseOctreeRenderer.h"
#include "BaseOctreeRenderer.h"
#include "WorkOctreeRenderer.h"
#include "DepthRenderer.h"
#include "DebugRenderer.h"
#include "NormalRenderer.h"
#include <AntTweakBar.h>

using namespace std;

// viewpoint
Camera camera;
Frustrum frustrum;
RenderContext render_context;

// renderer
vector<Renderer*> renderers;
int current_r = 0;
string rendername;

// lightselector
int lightselector = 0;

// our voxel grid
Octree* octree = NULL;

// generated render
unsigned char* data;

// OpenGL
GLuint texid;

// Draw fullsize quad, regardless of camera standpoint
void drawFullsizeQuad()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);		glVertex3f(-1.0, -1.0, 0.0);
		glTexCoord2f(1.0, 0.0);		glVertex3f(1.0, -1.0, 0.0);
		glTexCoord2f(1.0, 1.0);		glVertex3f(1.0, 1.0, 0.0);
		glTexCoord2f(0.0, 1.0);		glVertex3f(-1.0, 1.0, 0.0);
	glEnd();
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void generateTexture(){
   glBindTexture(GL_TEXTURE_2D, texid);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,render_context.n_x,render_context.n_y,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
   glEnable(GL_TEXTURE_2D);
}

void setupTexture(){
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_FLAT);
   glGenTextures(1, &texid);
   glBindTexture(GL_TEXTURE_2D, texid);
   glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   generateTexture();
}

void display(void)
{
	Timer t = Timer();
	rendername = renderers.at(current_r)->getName();
	camera.computeUVW(camera.e_,camera.g_,camera.t_);

	memset(data,0,render_context.n_x*render_context.n_y*4);
	renderers.at(current_r)->Render(render_context,octree,data);

	generateTexture();
	drawFullsizeQuad();
	TwDraw();
	//glPopMatrix();
	glutSwapBuffers(); // necessary?

	std::stringstream out;
	out << "Voxelicious v0.2 | Rendertime: " << t.getTimeMilliseconds() << " ms | FPS: " << 1000/t.getTimeMilliseconds();
	string s = out.str();
	glutSetWindowTitle(s.c_str());
}

void reshape(int w, int h)
{
	// Set our viewport to the size of our window  
	glViewport(0, 0, (GLsizei)w, (GLsizei)h); 
	// Switch to the projection matrix so that we can manipulate how our scene is viewed  
	glMatrixMode(GL_PROJECTION); 
	// Reset the projection matrix to the identity matrix so that we don't get any artifacts (cleaning up)  
	glLoadIdentity(); 
	// Set the Field of view angle (in degrees), the aspect ratio of our window, and the new and far planes 
	gluPerspective(60, (GLfloat)w / (GLfloat)h, 1.0, 100.0); 
	// Switch back to the model view matrix, so that we can start drawing shapes correctly 
	glMatrixMode(GL_MODELVIEW);  
	TwWindowSize(w, h);
}

void loadRenderers(){
	renderers.push_back(new DiffuseOctreeRenderer());
	renderers.push_back(new BaseOctreeRenderer());
	renderers.push_back(new WorkOctreeRenderer());
	renderers.push_back(new NormalRenderer());
	renderers.push_back(new DepthRenderer());
	renderers.push_back(new DebugRenderer());
	rendername = renderers.at(current_r)->getName();
}

void switchRenderer(){
	current_r = (current_r+1) % renderers.size();
}

// Keyboard
void keyboardfunc(unsigned char key, int x, int y)
{
	TwEventKeyboardGLUT(key,x,y);
	switch (key) {
		case '0':
			render_context.lights[lightselector].position = render_context.lights[lightselector].position + vec3(0,-0.2,0);
			break;
		case '2':
			render_context.lights[lightselector].position = render_context.lights[lightselector].position + vec3(0,0.2,0);
			break;
		case '1':
			render_context.lights[lightselector].position = render_context.lights[lightselector].position + vec3(-0.2,0,0);
			break;
		case '3':
			render_context.lights[lightselector].position = render_context.lights[lightselector].position + vec3(0.2,0,0);
			break;
		case '9':
			camera.e_ = camera.e_ + vec3(0,0,-0.2);
			break;
		case '7':
			camera.e_ = camera.e_ + vec3(0,0,0.2);
			break;
		case '6':
			camera.e_ = camera.e_ + vec3(0.2,0,0);
			break;
		case '4':
			camera.e_ = camera.e_ + vec3(-0.2,0,0);
			break;
		case '8':
			camera.e_ = camera.e_ + vec3(0,0.2,0);
			break;
		case '5':
			camera.e_ = camera.e_ + vec3(0,-0.2,0);
			break;
		case 'w':
			camera.g_ = camera.g_ + vec3(0.2,0,0);
			break;
		case 's':
			camera.g_ = camera.g_ + vec3(-0.2,0,0);
			break;
		case 'a':
			camera.g_ = camera.g_ + vec3(0,0.2,0);
			break;
		case 'd':
			camera.g_ = camera.g_ + vec3(0,-0.2,0);
			break;
		case 'n':
			lightselector = (lightselector+1) % (render_context.lights.size());
			cout << "light selector:" << lightselector << endl;
			break;
		case 'p':
			switchRenderer();
			break;
		case 'i':
			{string filename = "image"+getTimeString()+"";
			writePPM(render_context.n_x,render_context.n_y, data, filename);
			std::cout << "Image file written: " << filename << ".ppm" << std::endl;}
			break;
		default:
			glutPostRedisplay();
	}
	camera.computeUVW(camera.e_,camera.g_,camera.t_);
	glutPostRedisplay();
}

void printInfo(){
	std::cout << "Voxel Renderer Proof Of Concept" << std::endl;
	std::cout << "Jeroen Baert - 2012" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "jeroen.baert@cs.kuleuven.be" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "I'll be using " << omp_get_num_procs() << " CPU cores for rendering" << std::endl << std::endl;
}

void printControls(){
	std::cout << "Controls (on numeric keypad):" << std::endl;
	std::cout << "-----------------------------" << std::endl;
	std::cout << "4-6: Camera left-right" << std::endl;
	std::cout << "8-5: Camera up-down" << std::endl;
	std::cout << "7-9: Camera nearer-further" << std::endl;
	std::cout << "i: save screenshot in .PPM format" << std::endl;
	std::cout << "p: toggle work rendering" << std::endl;
	std::cout << "-----------------------------" << std::endl;
}

void printInvalid(){
	std::cout << "Not enough or invalid arguments, please try again.\n" << endl; 
	std::cout << "At the bare minimum, I need a path to a data file (binvox/avox)" << endl; 
	std::cout << "For Example: voxelraycaster.exe -f /home/jeroen/bunny256.avox" << endl;
}

void parseParameters(int argc, char **argv, string& datafile, string& moctreefile, FileFormat& inputformat, unsigned int& rendersize){
	if(argc < 2){ printInvalid(); exit(0);}
	for (int i = 1; i < argc; i++) {
		if (string(argv[i]) == "-f") {
			datafile = argv[i + 1]; 
			size_t check_binvox = datafile.find(".binvox");
			size_t check_avox = datafile.find(".avox");
			size_t check_mavox = datafile.find(".mavox");
			if(check_binvox != string::npos){
				inputformat = BINVOX;
				moctreefile = datafile.substr(0,check_binvox).append(".moctree");
			} else if(check_avox != string::npos){
				inputformat = AVOX;
				moctreefile = datafile.substr(0,check_avox).append(".moctree");
			} else if(check_mavox != string::npos){
				inputformat = MAVOX;
				moctreefile = datafile.substr(0,check_mavox).append(".moctree");
			} else{ 
				cout << "Data filename does not end in .binvox/.avox/.mavox - I only support those file formats." << endl; 
				printInvalid();
				exit(0);
			}
			i++;
		} else if (string(argv[i]) == "-m") {
			moctreefile = argv[i + 1]; 
			size_t check_moctree = moctreefile.find(".moctree");
			if(check_moctree == string::npos){ 
				cout << "Filename does not end in .moctree - I only support .moctree files." << endl; 
				printInvalid();
				exit(0);
			}
			i++;
		} else if (string(argv[i]) == "-s") {
			rendersize = atoi(argv[i + 1]); 
			i++;
		} else {printInvalid(); exit(0);}
	}
}

void generateLightTWBars(TwBar* bar){
	TwStructMember lightMembers[] = // array used to describe tweakable variables of the Light structure
	{
		{ "Active",    TW_TYPE_BOOL32, offsetof(Light, active),"" },
		{ "Position",    TW_TYPE_DIR3F, offsetof(Light, position),"" },  
		{ "Diffuse",     TW_TYPE_COLOR3F, offsetof(Light, diffuse),"" },       
		{ "Specular",    TW_TYPE_COLOR3F,   offsetof(Light, specular),"" },
		{ "Constant Attenuation", TW_TYPE_FLOAT, offsetof(Light, CONSTANT_ATTENUATION), "" },
		{ "Linear Attenuation", TW_TYPE_FLOAT, offsetof(Light, LINEAR_ATTENUATION), "" },
		{ "Quadratic Attenuation", TW_TYPE_FLOAT, offsetof(Light, QUADRATIC_ATTENUATION), "" },
		{ "Shininess", TW_TYPE_FLOAT, offsetof(Light, SHININESS), "" }
	};
	TwType lightType = TwDefineStruct("Light", lightMembers, 7, sizeof(Light), NULL, NULL);

	for(int i = 0; i<render_context.lights.size(); i++){
		std::stringstream name;
		name << "Light " << i+1;
		TwAddVarRW(bar, name.str().c_str(), lightType, &render_context.lights[i], "group='Lights'"); // Add a lightType variable and group it into the 'Edit lights' group
		TwSetParam(bar, name.str().c_str(), "label", TW_PARAM_CSTRING, 1, name.str().c_str()); // Set label
	}
}

void initRenderSystem(unsigned int render_x, unsigned int render_y){
	camera = Camera(vec3(0,0,0),vec3(vec3(0,0,-1)),vec3(0,1,0));
	float aspect_ratio = render_x/render_y;
	frustrum = Frustrum(30,aspect_ratio,1,100);
	render_context = RenderContext(&camera,&frustrum,render_x,render_y);
	Light mylight = Light(vec3(-4,0,-3.0f), vec3(0.8,0.0,0.0));
	Light mylight2 = Light(vec3(4,0,-3.0f), vec3(0.0,0.0,0.8));
	Light mylight3 = Light(vec3(0,0,-3.0f), vec3(0.0,0.8,0.0));
	render_context.lights.push_back(mylight);
	render_context.lights.push_back(mylight2);
	render_context.lights.push_back(mylight3);
}

int main(int argc, char **argv) {
	printInfo();
	printControls();

	// Input argument validation
	string moctreefile = "";
	string datafile = "";
	unsigned int rendersize = 640;
	FileFormat inputformat;
	parseParameters(argc,argv,datafile,moctreefile,inputformat,rendersize);
	

	// Initialize render system
	unsigned int render_x = rendersize;
	unsigned int render_y = rendersize;
	initRenderSystem(render_x,render_y);
	loadRenderers();

	// Get morton array (describing the octree)
	bool* morton_array; size_t gridlength; VoxelData mydata;
	if(inputformat == BINVOX){
		readMoctreeFile(moctreefile,gridlength,&morton_array);
		if(!readBinvoxFile(datafile,gridlength,mydata)){
			cout << "Error reading binvox file..." << endl; exit(0);
		}
		generateOctree(gridlength,&mydata,morton_array,&octree);
	}
	else if(inputformat == AVOX){
		readMoctreeFile(moctreefile,gridlength,&morton_array);
		if(!readAvoxFile(datafile,gridlength,mydata)){
			cout << "Error reading avox file..." << endl; exit(0);
		}
		generateOctree(gridlength,&mydata,morton_array,&octree);
	}
	else if(inputformat == MAVOX){
		if(!readMavoxFile(datafile,octree)){
			cout << "Error reading mavox file..." << endl; exit(0);
		}
	}
	
	octree->min = vec3(0,0,-2);
	octree->max = vec3(2,2,-4);
	octree->size = vec3(2,2,2);

	const int rgba_amount = render_x*render_y*4;
	data = new unsigned char[rgba_amount]; // put this on heap, it's too big, captain

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(render_x, render_y);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Voxel Ray Caster");
	glutKeyboardFunc(keyboardfunc);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	TwInit(TW_OPENGL, NULL);
	glutMouseFunc((GLUTmousebuttonfun)TwEventMouseButtonGLUT);
	glutMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
	glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
	TwGLUTModifiersFunc(glutGetModifiers);
	TwBar *bar;
	bar = TwNewBar("VoxelRaycaster");
	TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with GLUT and OpenGL.' "); // Message added to the help bar.
	TwDefine(" VoxelRaycaster size='200 200' color='150 150 150' iconified=true fontsize=1"); // change default tweak bar size and color
	TwAddVarRO(bar, "RendererName", TW_TYPE_STDSTRING, &rendername, 
			   " label='Renderer Name' group=Renderer help='Current renderer' ");
	TwAddVarRW(bar, "Eye", TW_TYPE_DIR3F, &camera.e_, 
			   " label='Eye' group=Camera help='Camera eye position' ");
	TwAddVarRW(bar, "Gaze", TW_TYPE_DIR3F, &camera.g_, 
			   " label='Gaze' group=Camera help='Camera gaze direction' ");
	TwAddVarRW(bar, "Top", TW_TYPE_DIR3F, &camera.t_, 
			   " label='Top' group=Camera help='Camera top direction' ");
	generateLightTWBars(bar);

	setupTexture();
	glutMainLoop();
}