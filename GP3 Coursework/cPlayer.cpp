#include "cPlayer.h"
#include "GameConstants.h"

cPlayer::cPlayer() : cModel()
{

}

void cPlayer::attachInputMgr(cInputMgr* inputMgr)
{
	m_InputMgr = inputMgr;
}

void cPlayer::update(float elapsedTime)
{
	if (m_InputMgr->isKeyDown(VK_RIGHT))
	{
		rotationAngle += 1.0f;
	}
	if (m_InputMgr->isKeyDown(VK_LEFT))
	{
		rotationAngle -= 1.0f;
	}
	if (m_InputMgr->isKeyDown(VK_UP))
	{
		translationZ += 1.0f;
		// Play the engine sound when moving
		m_SoundMgr->getSnd("Engine")->playAudio(AL_TRUE);
	}
	if (m_InputMgr->isKeyDown(VK_DOWN))
	{
		translationZ -= 1.0f;
		// Play the engine sound when moving
		m_SoundMgr->getSnd("Engine")->playAudio(AL_TRUE);
	}

	//Keys to capture to allow camera switching
	if (m_InputMgr->isKeyDown(67)) // C Key
	{
		cameraSwitch = true;
	}

	if (m_InputMgr->isKeyDown(86)) // V Key
	{
		cameraSwitch = false;
	}

	//Keys to capture the sound switching
	if (m_InputMgr->isKeyDown(69)) // E key
	{
		soundToggle = true;
	}

	if (m_InputMgr->isKeyDown(82)) // R key
	{
		soundToggle = false;
	}

	//Fire the laser
	if (m_InputMgr->isKeyDown(VK_SPACE))
	{
		glm::vec3 mdlLaserDirection;
		mdlLaserDirection.x = -(float)glm::sin(glm::radians(this->getRotation()));
		mdlLaserDirection.y = 0.0f;
		mdlLaserDirection.z = (float)glm::cos(glm::radians(this->getRotation()));
		mdlLaserDirection *= -1;

		// Add new bullet sprite to the vector array
		theLasers.push_back(new cLaser);
		int numLasers = theLasers.size() - 1;
		theLasers[numLasers]->setDirection(mdlLaserDirection);
		theLasers[numLasers]->setRotation(0.0f);
		theLasers[numLasers]->setScale(glm::vec3(1, 1, 1));
		theLasers[numLasers]->setSpeed(5.0f);
		theLasers[numLasers]->setPosition(this->getPosition() + mdlLaserDirection);
		theLasers[numLasers]->setIsActive(true);

		//theLasers[numLasers]->setMdlDimensions(theLaser.getModelDimensions());

		theLasers[numLasers]->update(elapsedTime);

		// play the firing sound
		m_SoundMgr->getSnd("Shot")->playAudio(AL_TRUE);
	}

	/*
	==============================================================
	| Check for collisions
	==============================================================
	*/
	for (vector<cLaser*>::iterator laserIterartor = theLasers.begin(); laserIterartor != theLasers.end(); ++laserIterartor)
	{
		(*laserIterartor)->update(elapsedTime);
		for (vector<cEnemy*>::iterator enemyIterator = theEnemy.begin(); enemyIterator != theEnemy.end(); ++enemyIterator)
		{
			if ((*enemyIterator)->SphereSphereCollision((*laserIterartor)->getPosition(), (*laserIterartor)->getMdlRadius()))
			{
				// if a collision set the bullet and spaceship to false
				(*enemyIterator)->setIsActive(false);
				(*laserIterartor)->setIsActive(false);
				// play the explosion sound.
				m_SoundMgr->getSnd("Explosion")->playAudio(AL_TRUE);
			}
		}
	}

	vector<cLaser*>::iterator laserIterartor = theLasers.begin();
	while (laserIterartor != theLasers.end())
	{
		if ((*laserIterartor)->isActive() == false)
		{
			laserIterartor = theLasers.erase(laserIterartor);
		}
		else
		{
			++laserIterartor;
		}
	}

	// Find out what direction we should be thrusting, using rotation.
	glm::vec3 mdlVelocityAdd;
	mdlVelocityAdd.x = -(float)glm::sin(glm::radians(m_mdlRotation));  // Remember to adjust for radians
	mdlVelocityAdd.y = 0.0f;
	mdlVelocityAdd.z = -(float)glm::cos(glm::radians(m_mdlRotation));

	m_mdlRotation -= rotationAngle;


	mdlVelocityAdd *= translationZ;

	m_mdlDirection += mdlVelocityAdd;

	m_mdlPosition += m_mdlDirection * m_mdlSpeed *elapsedTime;
	m_mdlDirection *= 0.95f;

	rotationAngle = 0;

	translationZ = 0;
}

cPlayer::~cPlayer()
{

}