#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one

// Includes
#include "GameConstants.h"
#include "windowOGL.h"
#include "cWNDManager.h"
#include "cColours.h"
#include "cShapes.h"
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
#include "spaceGame.h"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR cmdLine,
	int cmdShow)
{

	//Set our window settings
	const int windowWidth = 1024;
	const int windowHeight = 768;
	const int windowBPP = 16;

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

	//Adding Spheres for the solar system (Sun, Planet, moon)
	cSphere theSun(4, 40, 40);
	cSphere thePlanet(3, 30, 30);
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

	// Create Texture map for models
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

	//Adding textures for the star system

	cTexture planetTexture;
	planetTexture.createTexture("Images/ice.png");

	cTexture sunTexture;
	sunTexture.createTexture("Images/Sun.png");

	cTexture moonTexture;
	moonTexture.createTexture("Images/Moon.png");

	// the starfield
	cStarfield theStarField(starTexture.getTexture(), glm::vec3(50.0f, 50.0f, 50.0f));

	//Initialization of the planet and sun
	theSun.initialise(sunTexture.getTexture(), glm::vec3(0, 0, 40), glm::vec3(0, 0, 0));
	thePlanet.initialise(planetTexture.getTexture(), glm::vec3(10, 0, 20), glm::vec3(0, 0, 0));
	float planetRotSpeed = 3.0f;
	GLfloat planetOrbit = 0.0f;

	//Initialization of the moon of the planet
	theMoon.initialise(moonTexture.getTexture(), glm::vec3(10, 5, 8), glm::vec3(0, 0, 0));
	float moonRotSpeed = 5.0f;
	GLfloat moonOrbit = 0.0f;

	// Create Materials for lights
	cMaterial sunMaterial(lightColour4(0.0f, 0.0f, 0.0f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(0, 0, 0, 1.0f), 5.0f);
	cMaterial planetMaterial(lightColour4(0.2f, 0.2f, 0.2f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(0, 0, 0, 1.0f), 50.0f);
	cMaterial moonMaterial(lightColour4(0.1f, 0.1f, 0.1f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(0.2f, 0.2f, 0.2f, 1.0f), lightColour4(0, 0, 0, 1.0f), 10.0f);

	// Create Light where the sun to act as a light source for the scene
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

	// Load Sound for main theme and sounds					
	LPCSTR gameSounds[4] = { "Audio/Breaking Ground.wav", "Audio/shot007.wav", "Audio/explosion2.wav", "Audio/engine_2.wav" };

	theSoundMgr->add("Theme", gameSounds[0]);
	theSoundMgr->add("Shot", gameSounds[1]);
	theSoundMgr->add("Explosion", gameSounds[2]);
	theSoundMgr->add("Engine", gameSounds[3]);

	//Clear key buffers
	theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);


	// Model Loaders, object is assigned along with its texture
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

	// Player initizlation, the two managers (Input and Sound) are attached to allow key inputs to control the player, sound associated with player.
	cPlayer thePlayer;
	thePlayer.initialise(glm::vec3(1, 0, 70), 90.0f, glm::vec3(0.002, 0.002, 0.002), glm::vec3(0, 0,0), 1.0f, true);

	thePlayer.setMdlDimensions(spaceShipMdl.getModelDimensions());
	thePlayer.attachInputMgr(theInputMgr);
	thePlayer.attachSoundMgr(theSoundMgr);

	// First three models are created, intially there was going to be 3 gates to travel too like racing but this was scrapped
	cEnemy gate;
	gate.initialise(glm::vec3(1, 0, 60), 0.0f, glm::vec3(0.02, 0.02, 0.02), glm::vec3(0, 0, 0), 1.0f, true);
	gate.setMdlDimensions(spaceGateMdl.getModelDimensions());

	cEnemy gate1;
	gate1.initialise(glm::vec3(10, 0, 60), 0.0f, glm::vec3(0.02, 0.02, 0.02), glm::vec3(0, 0, 0), 1.0f, true);
	gate1.setMdlDimensions(spaceGateMdl1.getModelDimensions());

	cEnemy gate2;
	gate2.initialise(glm::vec3(20, 0, 60), 0.0f, glm::vec3(0.02, 0.02, 0.02), glm::vec3(0, 0, 0), 1.0f, true);
	gate2.setMdlDimensions(spaceGateMdl2.getModelDimensions());

	// A space station model, a larger example of model loading
	cEnemy station;
	station.initialise(glm::vec3(1, 0, 50), 0.0f, glm::vec3(0.02, 0.02, 0.02), glm::vec3(0, 0, 0), 1.0f, true);
	station.setMdlDimensions(spaceStation.getModelDimensions());

	// A satellite model initialization
	cEnemy satellite;
	satellite.initialise(glm::vec3(10, 0, 50), 0.0f, glm::vec3(0.002, 0.002, 0.002), glm::vec3(0, 0, 0), 1.0f, true);
	satellite.setMdlDimensions(spaceSatellite.getModelDimensions());

	//This creates the camera based on the cCamera class, its here that initial parameters are set like position and lookat
	cCamera theCamera;
	theCamera.setTheCameraPos(glm::vec3(thePlayer.getPosition().x, thePlayer.getPosition().y, 3.0f + thePlayer.getPosition().z));
	theCamera.setTheCameraLookAt(glm::vec3(thePlayer.getPosition().x, thePlayer.getPosition().y ,0.0f));
	theCamera.setTheCameraUpVector(glm::vec3(0.0f, 1.0f, 0.0f)); // pointing upwards in world space
	theCamera.setTheCameraAspectRatio(windowWidth, windowHeight);
	theCamera.setTheProjectionMatrix(45.0f, theCamera.getTheCameraAspectRatio(), 0.1f, 300.0f);
	theCamera.update();

	float tCount = 0.0f;

	//Outputmsg that can be passed to displaying text in the scene
	string outputMsg;

	// Starts the theme music and plays it in a loop
	theSoundMgr->getSnd("Theme")->playAudio(AL_LOOPING);

	// List and index for the lasers fired from player is declared here
	std::vector<cLaser*> laserList;
	std::vector<cLaser*>::iterator index;

	//Declaration of Variables for Fog
	GLuint filter;
	GLuint fogMode[] = { GL_EXP, GL_EXP2, GL_LINEAR };
	GLuint fogfilter = 0;
	GLfloat fogcolour[4] = { 0.5f, 0.5f, 0.5f, 1.0f };


	//This is the mainloop, we render frames until isRunning returns false
	while (pgmWNDMgr->isWNDRunning())
	{
		//Enter code here about what to render, create a variable that is boolean or int, render title scene if 0, render game scene if 1 and end scene if is 2
		//This may require some experimentation to succed like is it here where the code changes or 2 lines down after the elapsed time 
		pgmWNDMgr->processWNDEvents(); //Process any window events

		//We get the time that passed since the last frame
		float elapsedTime = pgmWNDMgr->getElapsedSeconds();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		theOGLWnd.initOGL(windowWidth, windowHeight);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glLoadMatrixf((GLfloat*)&theCamera.getTheViewMatrix());

		//Fog Code
		
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glFogi(GL_FOG_MODE, fogMode[fogfilter]);
		glFogfv(GL_FOG_COLOR, fogcolour);
		glFogf(GL_FOG_DENSITY, 0.05f);
		glFogf(GL_FOG_START, 1.0f);
		glFogf(GL_FOG_END, 5.0f);
		
		//Enable Fog

		glEnable(GL_FOG);

		//This allows the toggling of the main theme playing ON/OFF
		if (theInputMgr->isKeyDown(69) && thePlayer.soundToggle == false)
		{
			theSoundMgr->getSnd("Theme")->playAudio(AL_LOOPING);
		}
		if (theInputMgr->isKeyDown(82) && thePlayer.soundToggle == true)
		{
			theSoundMgr->getSnd("Theme")->stopAudio();
		
		}

		// Code that switches between the player 3rd person view and set view 
		if (thePlayer.cameraSwitch == true)
		{
			// Set view
			outputMsg = to_string(thePlayer.cameraSwitch);
			theCamera.setTheCameraPos(glm::vec3(10.0f, 3.0f, 70.0f));
			theCamera.setTheCameraLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
			theCamera.update();
		}

		if (thePlayer.cameraSwitch == false)
		{
			// return back to third person view
		outputMsg = to_string(thePlayer.cameraSwitch);
		theCamera.setTheCameraPos(glm::vec3(thePlayer.getPosition().x, thePlayer.getPosition().y, 3.0f + thePlayer.getPosition().z));
		theCamera.setTheCameraLookAt(glm::vec3(thePlayer.getPosition().x, thePlayer.getPosition().y, 0.0f));
		theCamera.update();
		}
	
		// render the Starfield
		theStarField.render(0.0f);

		//Rendering and setup of the star system planet,moon and sun
		theSun.prepare(0.0f);
		sunMaterial.useMaterial();
		sunLight.lightOn();
		lfLight.lightOn();
		rfLight.lightOn();
		cbLight.lightOn();
		theSun.render(theSun.getRotAngle());
	
		// Planets's orbit
		glPushMatrix();
		thePlanet.setRotAngle(thePlanet.getRotAngle() + (planetRotSpeed*elapsedTime));
		thePlanet.prepare(thePlanet.getRotAngle()); //Do any pre-rendering logic
		planetMaterial.useMaterial();				// Set the material for use
		thePlanet.render(thePlanet.getRotAngle()); //Render the scene

		// Moon's orbit
		glPushMatrix();
		theMoon.setRotAngle(theMoon.getRotAngle() + (moonRotSpeed*elapsedTime));
		theMoon.prepare(rotationAngle);
		moonMaterial.useMaterial();
		theMoon.render(theMoon.getRotAngle());
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();

		//Model Rendering for the spacegates, player fighter, satellite and station.
		spaceShipMdl.renderMdl(thePlayer.getPosition(), thePlayer.getRotation(), thePlayer.getScale());
		spaceGateMdl.renderMdl(gate.getPosition(), gate.getRotation(), gate.getScale());
		spaceGateMdl1.renderMdl(gate1.getPosition(), gate1.getRotation(), gate1.getScale());
		spaceGateMdl2.renderMdl(gate2.getPosition(), gate2.getRotation(), gate2.getScale());
		spaceStation.renderMdl(station.getPosition(), station.getRotation(), station.getScale());
		spaceSatellite.renderMdl(satellite.getPosition(), satellite.getRotation(), satellite.getScale());

		thePlayer.update(elapsedTime);
	//Iterator that creates multiple laser projectiles as the player presses the fire laser key
		for (vector<cLaser*>::iterator laserIterartor = theLasers.begin(); laserIterartor != theLasers.end(); ++laserIterartor)
		{
			if ((*laserIterartor)->isActive())
			{
				theLaser.renderMdl((*laserIterartor)->getPosition(), (*laserIterartor)->getRotation(), (*laserIterartor)->getScale());
				(*laserIterartor)->update(elapsedTime);
			}
		}

		glPushMatrix();
		theOGLWnd.setOrtho2D(windowWidth, windowHeight);

		//Generate the text overlay on screen
		theFontMgr->getFont("DrWho")->printText("Space Game", FTPoint(10, 35, 0.0f), colour3f(0.0f, 255.0f, 0.0f));
		theFontMgr->getFont("Space")->printText("C & V = camera", FTPoint(830, 35, 0.0f), colour3f(255.0f, 255.0f, 255.0f)); // uses c_str to convert string to LPCSTR
		theFontMgr->getFont("Space")->printText("E & R = Sound ON/OFF", FTPoint(830, 45, 0.0f), colour3f(255.0f, 255.0f, 255.0f));
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
