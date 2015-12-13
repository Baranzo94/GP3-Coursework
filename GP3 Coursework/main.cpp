#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one


//#include <windows.h>
#include "GameConstants.h"
#include "windowOGL.h"
#include "cWNDManager.h"
#include "cColours.h"
#include "cShapes.h"
#include "cPyramid.h"
#include "cCube.h"
#include "cSphere.h"
#include "cMaterial.h"
#include "cLight.h"
#include "cStarfield.h"
#include "cFontMgr.h"
#include "cCamera.h"
#include "cInputMgr.h"
#include "cSoundMgr.h"
#include "cModelLoader.h"
#include "cModel.h"
#include "cPlayer.h"
#include "cEnemy.h"
#include "cLaser.h"
#include "tardisWarsGame.h"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR cmdLine,
	int cmdShow)
{

	//Set our window settings
	const int windowWidth = 1024;
	const int windowHeight = 768;
	const int windowBPP = 16;

	//Make a variable like intitialModelRotation

	

	//This is our window
	static cWNDManager* pgmWNDMgr = cWNDManager::getInstance();

	// This is the input manager
	static cInputMgr* theInputMgr = cInputMgr::getInstance();

	// This is the Font manager
	static cFontMgr* theFontMgr = cFontMgr::getInstance();

	// This is the sound manager
	static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();

	//The example OpenGL code
	windowOGL theOGLWnd;

	//Attach our example to our window
	pgmWNDMgr->attachOGLWnd(&theOGLWnd);

	//Adding Spheres for the solar system (Sun, Earth, moon)
	cSphere theSun(4, 40, 40);
	cSphere theEarth(3, 30, 30);
	cSphere theMoon(1, 20, 20);

	// Attach the keyboard manager
	pgmWNDMgr->attachInputMgr(theInputMgr);

	//Attempt to create the window
	if (!pgmWNDMgr->createWND(windowWidth, windowHeight, windowBPP))
	{
		//If it fails

		MessageBox(NULL, "Unable to create the OpenGL Window", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
		return 1;
	}

	if (!theOGLWnd.initOGL(windowWidth, windowHeight)) //Initialize our example
	{
		MessageBox(NULL, "Could not initialize the application", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
		return 1;
	}
	// Create Texture map
	cTexture spaceShipTexture;
	spaceShipTexture.createTexture("Models/SpaceFighter/mat_ship.png");
	cTexture spaceGateTexture;
	spaceGateTexture.createTexture("Models/SpaceGate/mat_gate.png");
	cTexture spaceStationTexture;
	spaceStationTexture.createTexture("Models/SpaceStation/mat_stat.png");
	cTexture spaceSatelliteTexture;
	spaceSatelliteTexture.createTexture("Models/SpaceSatellite/mat_sate.png");


	cTexture laserTexture;
	laserTexture.createTexture("Models/laser.tga");
	cTexture starTexture;
	starTexture.createTexture("Images/star.png");

	//Adding textures for the solar system

	cTexture earthTexture;
	earthTexture.createTexture("Images/Earth.png");

	cTexture sunTexture;
	sunTexture.createTexture("Images/Sun.png");

	cTexture moonTexture;
	moonTexture.createTexture("Images/Moon.png");

	// the starfield
	cStarfield theStarField(starTexture.getTexture(), glm::vec3(50.0f, 50.0f, 50.0f));

	theSun.initialise(sunTexture.getTexture(), glm::vec3(0, 0, 40), glm::vec3(0, 0, 0));
	theEarth.initialise(earthTexture.getTexture(), glm::vec3(10, 0, 20), glm::vec3(0, 0, 0));
	float earthRotSpeed = 3.0f;
	GLfloat earthOrbit = 0.0f;

	theMoon.initialise(moonTexture.getTexture(), glm::vec3(10, 5, 8), glm::vec3(0, 0, 0));
	float moonRotSpeed = 5.0f;
	GLfloat moonOrbit = 0.0f;

	// Create Materials for lights
	cMaterial sunMaterial(lightColour4(0.0f, 0.0f, 0.0f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(0, 0, 0, 1.0f), 5.0f);
	cMaterial earthMaterial(lightColour4(0.2f, 0.2f, 0.2f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(0, 0, 0, 1.0f), 50.0f);
	cMaterial moonMaterial(lightColour4(0.1f, 0.1f, 0.1f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(0.2f, 0.2f, 0.2f, 1.0f), lightColour4(0, 0, 0, 1.0f), 10.0f);

	// Create Light
	cLight sunLight(GL_LIGHT0, lightColour4(0, 0, 0, 1), lightColour4(1, 1, 1, 1), lightColour4(1, 1, 1, 1), glm::vec4(0, 0, 20, 1),
		glm::vec3(0.0, 0.0, 1.0), 0.0f, 180.0f, 1.0f, 0.0f, 0.0f);
	cLight lfLight(GL_LIGHT1, lightColour4(0, 0, 0, 1), lightColour4(1, 1, 1, 1), lightColour4(1, 1, 1, 1), glm::vec4(30, 0, 100, 1),
		glm::vec3(0.0, 0.0, 1.0), 0.0f, 180.0f, 1.0f, 0.0f, 0.0f);
	cLight rfLight(GL_LIGHT2, lightColour4(0, 0, 0, 1), lightColour4(1, 1, 1, 1), lightColour4(1, 1, 1, 1), glm::vec4(-30, 0, 100, 1),
		glm::vec3(0.0, 0.0, 1.0), 0.0f, 180.0f, 1.0f, 0.0f, 0.0f);
	cLight cbLight(GL_LIGHT3, lightColour4(0, 0, 0, 1), lightColour4(1, 1, 1, 1), lightColour4(1, 1, 1, 1), glm::vec4(0, 0, -100, 1),
		glm::vec3(0.0, 0.0, 1.0), 0.0f, 180.0f, 1.0f, 0.0f, 0.0f);

	//Define Ambient light for scene
	GLfloat g_Ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, g_Ambient);

	// Load Fonts
	LPCSTR gameFonts[3] = { "Fonts/digital-7.ttf", "Fonts/space age.ttf", "Fonts/doctor_who.ttf" };

	theFontMgr->addFont("SevenSeg", gameFonts[0], 24);
	theFontMgr->addFont("Space", gameFonts[1], 12);
	theFontMgr->addFont("DrWho", gameFonts[2], 48);

	// Load Sound						-Find a suitable theme-
	LPCSTR gameSounds[3] = { "Audio/Short Change Hero.wav", "Audio/shot007.wav", "Audio/explosion2.wav" };

	theSoundMgr->add("Theme", gameSounds[0]);
	theSoundMgr->add("Shot", gameSounds[1]);
	theSoundMgr->add("Explosion", gameSounds[2]);

	// Create a camera, this code has been pushed later so that it can retreive the varables of the player 
	/*cCamera theCamera;
	theCamera.setTheCameraPos(glm::vec3(0.0f, 0.0f, 75.0f));
	
	//theCamera.setTheCameraPos(glm::vec3(thePlayer.getPosition().x,0.0f,75.0f);
	theCamera.setTheCameraLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
	theCamera.setTheCameraUpVector(glm::vec3(0.0f, 1.0f, 0.0f)); // pointing upwards in world space
	theCamera.setTheCameraAspectRatio(windowWidth, windowHeight);
	theCamera.setTheProjectionMatrix(45.0f, theCamera.getTheCameraAspectRatio(), 0.1f, 300.0f);
	theCamera.update();*/
	/*
	GLfloat cameraRotRadius = 80.0f;
	GLfloat cameraRotationAngle = 0.0f;
	*/

	//Clear key buffers
	theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);

	cModelLoader spaceShipMdl;
	spaceShipMdl.loadModel("Models/SpaceFighter/spaceship01.obj",spaceShipTexture);

	cModelLoader spaceGateMdl;
	spaceGateMdl.loadModel("Models/SpaceGate/gate.obj", spaceGateTexture);

	cModelLoader spaceGateMdl1;
	spaceGateMdl1.loadModel("Models/SpaceGate/gate.obj", spaceGateTexture);

	cModelLoader spaceGateMdl2;
	spaceGateMdl2.loadModel("Models/SpaceGate/gate.obj", spaceGateTexture);

	cModelLoader spaceStation;
	spaceStation.loadModel("Models/SpaceStation/station.obj", spaceStationTexture);

	cModelLoader spaceSatellite;
	spaceSatellite.loadModel("Models/SpaceSatellite/satellite.obj", spaceSatelliteTexture);

	cModelLoader theLaser;
	theLaser.loadModel("Models/laser.obj", laserTexture);

	//Other models to be loaded here, 3 more are required

	/*for (int loop = 0; loop < 5; loop++)
	{
		theEnemy.push_back(new cEnemy);
		theEnemy[loop]->randomise();
		theEnemy[loop]->setMdlDimensions(spaceShipMdl.getModelDimensions());
		theEnemy[loop]->setScale(glm::vec3(5, 5, 5));
	}*/

	cPlayer thePlayer;
	thePlayer.initialise(glm::vec3(1, 0, 70), 90.0f, glm::vec3(0.002, 0.002, 0.002), glm::vec3(0, 0,0), 1.0f, true);
	//thePlayer.setRotation(mdlRotation.x = 5);
	//thePlayer.setMdlDimensions(tardisMdl.getModelDimensions());
	thePlayer.setMdlDimensions(spaceShipMdl.getModelDimensions());
	thePlayer.attachInputMgr(theInputMgr);
	thePlayer.attachSoundMgr(theSoundMgr);

	//cModel spaceGate;
	cEnemy test;
	test.initialise(glm::vec3(1, 0, 60), 0.0f, glm::vec3(0.02, 0.02, 0.02), glm::vec3(0, 0, 0), 1.0f, true);
	test.setMdlDimensions(spaceGateMdl.getModelDimensions());

	cEnemy test1;
	test1.initialise(glm::vec3(10, 0, 60), 0.0f, glm::vec3(0.02, 0.02, 0.02), glm::vec3(0, 0, 0), 1.0f, true);
	test1.setMdlDimensions(spaceGateMdl1.getModelDimensions());

	cEnemy test2;
	test2.initialise(glm::vec3(20, 0, 60), 0.0f, glm::vec3(0.02, 0.02, 0.02), glm::vec3(0, 0, 0), 1.0f, true);
	test2.setMdlDimensions(spaceGateMdl2.getModelDimensions());

	cEnemy test3;
	test3.initialise(glm::vec3(1, 0, 50), 0.0f, glm::vec3(0.02, 0.02, 0.02), glm::vec3(0, 0, 0), 1.0f, true);
	test3.setMdlDimensions(spaceStation.getModelDimensions());

	cEnemy test4;
	test4.initialise(glm::vec3(10, 0, 50), 0.0f, glm::vec3(0.002, 0.002, 0.002), glm::vec3(0, 0, 0), 1.0f, true);
	test4.setMdlDimensions(spaceSatellite.getModelDimensions());

	cCamera theCamera;
	//theCamera.setTheCameraPos(glm::vec3(0.0f, 0.0f, 75.0f));
	theCamera.setTheCameraPos(glm::vec3(thePlayer.getPosition().x, thePlayer.getPosition().y, 3.0f + thePlayer.getPosition().z));
	theCamera.setTheCameraLookAt(glm::vec3(thePlayer.getPosition().x, thePlayer.getPosition().y ,0.0f));
	//theCamera.setTheCameraLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
	theCamera.setTheCameraUpVector(glm::vec3(0.0f, 1.0f, 0.0f)); // pointing upwards in world space
	theCamera.setTheCameraAspectRatio(windowWidth, windowHeight);
	theCamera.setTheProjectionMatrix(45.0f, theCamera.getTheCameraAspectRatio(), 0.1f, 300.0f);
	theCamera.update();

	

	float tCount = 0.0f;
	string outputMsg;

	theSoundMgr->getSnd("Theme")->playAudio(AL_LOOPING);
	
	/*
	if (thePlayer.soundToggle == false)
	{
	theSoundMgr->getSnd("Theme")->playAudio(AL_LOOPING);
	}
	if (thePlayer.soundToggle == true)
	{
		theSoundMgr->getSnd("Theme")->stopAudio();
	}*/


	std::vector<cLaser*> laserList;
	std::vector<cLaser*>::iterator index;

//Declaration of Variables for Fog
	bool gp; 
	GLuint filter;
	GLuint fogMode[] = { GL_EXP, GL_EXP2, GL_LINEAR };
	GLuint fogfilter = 0;
	GLfloat fogcolour[4] = { 0.5f, 0.5f, 0.5f, 1.0f };		
	
		//Fog Code
		/*glClearColor(0.5f, 0.0f, 0.5f, 1.0f);
		glFogi(GL_FOG_MODE, fogMode[fogfilter]);
		glFogfv(GL_FOG_COLOR, fogcolour);
		glFogf(GL_FOG_DENSITY, 0.35f);
		glHint(GL_FOG_HINT, GL_DONT_CARE);
		glFogf(GL_FOG_START, 1.0f);
		glFogf(GL_FOG_END, 5.0f);
		
		//Enable Fog
		glEnable(GL_FOG);*/

	//This is the mainloop, we render frames until isRunning returns false
	while (pgmWNDMgr->isWNDRunning())
	{
		//Enter code here about what to render, create a variable that is boolean or int, render title scene if 0, render game scene if 1 and end scene if is 2
		//This may require some experimentation to succed like is it here where the code changes or 2 lines down after the elapsed time 
		pgmWNDMgr->processWNDEvents(); //Process any window events

		//We get the time that passed since the last frame
		float elapsedTime = pgmWNDMgr->getElapsedSeconds();

		// Lab code goes here
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		theOGLWnd.initOGL(windowWidth, windowHeight);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glLoadMatrixf((GLfloat*)&theCamera.getTheViewMatrix());

		//thePlayer.soundToggle = true;


		//outputMsg = to_string(theEnemy.size()); // convert float to string

		if (thePlayer.soundToggle == false)
		{
			theSoundMgr->getSnd("Theme")->playAudio(AL_LOOPING);
		}
		if (thePlayer.soundToggle == true)
		{
			theSoundMgr->getSnd("Theme")->stopAudio();
		
		}
		if (thePlayer.cameraSwitch == true)
		{
			outputMsg = to_string(thePlayer.cameraSwitch);
			theCamera.setTheCameraPos(glm::vec3(1.0f, 0.0f, 70.0f));
			theCamera.setTheCameraLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
			theCamera.update();
		}

		if (thePlayer.cameraSwitch == false)
		{
		outputMsg = to_string(thePlayer.cameraSwitch);
		theCamera.setTheCameraPos(glm::vec3(thePlayer.getPosition().x, thePlayer.getPosition().y, 3.0f + thePlayer.getPosition().z));
		theCamera.setTheCameraLookAt(glm::vec3(thePlayer.getPosition().x, thePlayer.getPosition().y, 0.0f));
		theCamera.update();
		}
		//Code before camera switch
		/*
		theCamera.setTheCameraPos(glm::vec3(thePlayer.getPosition().x, thePlayer.getPosition().y, 3.0f + thePlayer.getPosition().z));
		theCamera.setTheCameraLookAt(glm::vec3(thePlayer.getPosition().x, thePlayer.getPosition().y, 0.0f));
		theCamera.update();
		*/

		theStarField.render(0.0f);

		theSun.prepare(0.0f);
		sunMaterial.useMaterial();
		sunLight.lightOn();
		lfLight.lightOn();
		rfLight.lightOn();
		cbLight.lightOn();
		theSun.render(theSun.getRotAngle());
	
		// Earth's orbit
		glPushMatrix();
		theEarth.setRotAngle(theEarth.getRotAngle() + (earthRotSpeed*elapsedTime));
		theEarth.prepare(theEarth.getRotAngle()); //Do any pre-rendering logic
		earthMaterial.useMaterial();				// Set the material for use
		theEarth.render(theEarth.getRotAngle()); //Render the scene
		// Moon's orbit
		glPushMatrix();
		theMoon.setRotAngle(theMoon.getRotAngle() + (moonRotSpeed*elapsedTime));
		theMoon.prepare(rotationAngle);
		moonMaterial.useMaterial();
		theMoon.render(theMoon.getRotAngle());
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();
		
		/*for (vector<cEnemy*>::iterator enemyIterator = theEnemy.begin(); enemyIterator != theEnemy.end(); ++enemyIterator)
		{
			if ((*enemyIterator)->isActive())
			{
				spaceShipMdl.renderMdl((*enemyIterator)->getPosition(), (*enemyIterator)->getRotation(), (*enemyIterator)->getScale());
				(*enemyIterator)->update(elapsedTime);
			}
		}*/

		//Model Rendering
		spaceShipMdl.renderMdl(thePlayer.getPosition(), thePlayer.getRotation(), thePlayer.getScale());
		spaceGateMdl.renderMdl(test.getPosition(), test.getRotation(), test.getScale());
		spaceGateMdl1.renderMdl(test1.getPosition(), test1.getRotation(), test1.getScale());
		spaceGateMdl2.renderMdl(test2.getPosition(), test2.getRotation(), test2.getScale());
		spaceStation.renderMdl(test3.getPosition(), test3.getRotation(), test3.getScale());
		spaceSatellite.renderMdl(test4.getPosition(), test4.getRotation(), test4.getScale());

		thePlayer.update(elapsedTime);

		for (vector<cLaser*>::iterator laserIterartor = theTardisLasers.begin(); laserIterartor != theTardisLasers.end(); ++laserIterartor)
		{
			if ((*laserIterartor)->isActive())
			{
				theLaser.renderMdl((*laserIterartor)->getPosition(), (*laserIterartor)->getRotation(), (*laserIterartor)->getScale());
				(*laserIterartor)->update(elapsedTime);
			}
		}
		//outputMsg = to_string(theEnemy.size()); // convert float to string

		glPushMatrix();
		theOGLWnd.setOrtho2D(windowWidth, windowHeight);
		theFontMgr->getFont("DrWho")->printText("Space Game", FTPoint(10, 35, 0.0f), colour3f(0.0f, 255.0f, 0.0f));
		theFontMgr->getFont("Space")->printText("C & V = camera", FTPoint(850, 35, 0.0f), colour3f(255.0f, 255.0f, 255.0f)); // uses c_str to convert string to LPCSTR
		theFontMgr->getFont("Space")->printText("E & R = Sound ON/OFF", FTPoint(850, 45, 0.0f), colour3f(255.0f, 255.0f, 255.0f));
		glPopMatrix();

		
		pgmWNDMgr->swapBuffers();

		tCount += elapsedTime;

		//Clear key buffers
		theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);
	}
	theOGLWnd.shutdown(); //Free any resources
	pgmWNDMgr->destroyWND(); //Destroy the program window

	return 0; //Return success
}
