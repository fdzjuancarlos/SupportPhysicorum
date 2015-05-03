#include "IntroState.h"
#include "PlayState.h"

template<> IntroState* Ogre::Singleton<IntroState>::msSingleton = 0;

void
IntroState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();
  _sceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, "SceneManager");
  _sceneMgr -> setAmbientLight(Ogre::ColourValue(0.6, 0.6, 0.6));
  _sceneMgr -> setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

  _camera = _sceneMgr->createCamera("IntroCamera");
  _camera -> lookAt(Vector3(15,15,0));
  _camera -> setOrientation(Ogre::Quaternion::IDENTITY);
  _camera -> setPosition(80,20,-200);
  _camera -> yaw(Ogre::Degree(-122));
  _camera -> pitch(Ogre::Degree(12));



  //Camaras para pruebas
//	_camera -> setPosition(60,1000,-270);
//	_camera -> pitch(Ogre::Degree(-90));
//	_camera -> setPosition(100,160,180);
//	_camera -> pitch(Ogre::Degree(-10));


  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
  _viewport->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
  
  Ogre::Entity* ent2 = _sceneMgr->createEntity("bottom", "Level1Mesh.mesh");
//  Entity *entity = _sceneMgr->createEntity("Level1Mesh.mesh");
//  ent2->setMaterialName("bottom");
  ent2->setCastShadows(true);
    Ogre::SceneNode* bottom = _sceneMgr->createSceneNode("bottom");
  bottom->attachObject(ent2);
  _sceneMgr->getRootSceneNode()->addChild(bottom);
  bottom->setScale(10,10,10);
  bottom->setPosition(0,0,0);

  //Robot Initialization
  Ogre::Entity* ent1 = _sceneMgr->createEntity("Robotillo", "RobotilloMesh.mesh");
  ent1->setCastShadows(true);
//  std::shared_ptr<SceneNode> player(_sceneMgr->createSceneNode("Player"));
  Ogre::SceneNode* player = _sceneMgr->createSceneNode("Player");
  player->attachObject(ent1);
  _sceneMgr->getRootSceneNode()->addChild(player);
  player->setScale(10,10,10);
  player->setPosition(170,9,-110);
  player->yaw(Ogre::Degree(36));


  //Inicializamos las luces
  Ogre::Light* light = _sceneMgr->createLight("Light1");
  light->setType(Ogre::Light::LT_SPOTLIGHT);
  light->setPosition(170,120,500);
//  light -> setDiffuseColour (1, 1, 1);
//  light -> setSpecularColour (1, 1, 1);
  light -> setPowerScale(60);
  light->setDirection(Ogre::Vector3(0,0,-1));
//  light->setSpotlightInnerAngle(Ogre::Degree(1.0f));
//  light->setSpotlightOuterAngle(Ogre::Degree(1.0f));
//  light->setSpotlightFalloff(10.0f);
//  light->setCastShadows(true);

  _overlayManager = OverlayManager::getSingletonPtr();
  Overlay *overlay = _overlayManager->getByName("Info");
  overlay->show();

  //Inicializamos animaciones
  _animState = _sceneMgr->getEntity("Robotillo")->getAnimationState("Standing");
  _animState->setEnabled(false);

  //Inicializamos menu
  renderer = &CEGUI::OgreRenderer::bootstrapSystem();
  createGUI();
  initMenu();

  _exitGame = false;
}

void
IntroState::exit()
{
  _sceneMgr->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();
}

void
IntroState::pause ()
{
}

void
IntroState::resume ()
{
}

bool
IntroState::frameStarted
(const Ogre::FrameEvent& evt) 
{
  _timeSinceLastFrame = evt.timeSinceLastFrame;
  CEGUI::System::getSingleton().injectTimePulse(_timeSinceLastFrame);

  if (!_animState->getEnabled()) {
    _animState->setTimePosition(0.0);
    _animState->setEnabled(true);
    _animState->setLoop(true);
  }

  if (_animState->getEnabled() && !_animState->hasEnded()) 
    _animState->addTime(_timeSinceLastFrame);

  return true;
}

bool
IntroState::frameEnded
(const Ogre::FrameEvent& evt)
{
  if (_exitGame)
    return false;
  
  return true;
}

void
IntroState::keyPressed
(const OIS::KeyEvent &e)
{

  CEGUI::System::getSingleton().injectKeyDown(e.key);
  CEGUI::System::getSingleton().injectChar(e.text);

  // Transición al siguiente estado.
  // Espacio --> PlayState
  if (e.key == OIS::KC_SPACE) {
//	if (_initGameControl == true){
//		CEGUI::MouseCursor::getSingleton().hide( );
		//CEGUI::WindowManager::getSingletonPtr()->destroyAllWindows();
//		CEGUI::WindowManager::getSingletonPtr()->destroyWindow("MenuWin");
		//PlayState::getSingletonPtr()->newGame(true);
		changeState(PlayState::getSingletonPtr());
	}
//  }
}

void
IntroState::keyReleased
(const OIS::KeyEvent &e )
{
  CEGUI::System::getSingleton().injectKeyUp(e.key);
  if (e.key == OIS::KC_ESCAPE) {
    _exitGame = true;
  }
}

void IntroState::mouseMoved (const OIS::MouseEvent &e)
{
	CEGUI::System::getSingleton().injectMouseMove(e.state.X.rel, e.state.Y.rel);
}

void IntroState::mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().injectMouseButtonDown(convertMouseButton(id));
}

void IntroState::mouseReleased (const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().injectMouseButtonUp(convertMouseButton(id));
}

CEGUI::MouseButton IntroState::convertMouseButton(OIS::MouseButtonID id)
{
	CEGUI::MouseButton ceguiId;
	switch(id)
	{
		case OIS::MB_Left:
			ceguiId = CEGUI::LeftButton;
			break;
		case OIS::MB_Right:
			ceguiId = CEGUI::RightButton;
			break;
		case OIS::MB_Middle:
			ceguiId = CEGUI::MiddleButton;
			break;
		default:
			ceguiId = CEGUI::LeftButton;
	}
	return ceguiId;
}

IntroState*
IntroState::getSingletonPtr ()
{
return msSingleton;
}

IntroState&
IntroState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}

void IntroState::createGUI(){
	//CEGUI

	//renderer = &CEGUI::OgreRenderer::bootstrapSystem();
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");

	CEGUI::SchemeManager::getSingleton().create("Physigui.scheme");
	CEGUI::ImagesetManager::getSingleton().create("Physigui.imageset");
	//CEGUI::System::getSingleton().setDefaultFont("tenby-five");
	if(! CEGUI::FontManager::getSingleton().isDefined( "tenby-five" ) )
	CEGUI::FontManager::getSingleton().createFreeTypeFont( "tenby-five", 17, true, "tenby-five.otf", "Fonts" );
	CEGUI::System::getSingleton().setDefaultFont( "tenby-five" );
	CEGUI::System::getSingleton().setDefaultMouseCursor("Physigui","MouseArrow");
	CEGUI::MouseCursor::getSingleton().show( );
}

void IntroState::initMenu(){
	//Sheet
	CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow","MenuWin");

	//Config Window
	CEGUI::Window* formatWin = CEGUI::WindowManager::getSingleton().loadWindowLayout("MenuInit.layout");

	//Game Window
	//CEGUI::Window* gameButton = CEGUI::WindowManager::getSingleton().getWindow("FormatWin/GameButton");
	//gameButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&IntroState::initGame, this));

	//Load Window
	//CEGUI::Window* loadButton = CEGUI::WindowManager::getSingleton().getWindow("FormatWin/LoadButton");
	//loadButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&IntroState::load, this));

	//Option Window
	//CEGUI::Window* optionButton = CEGUI::WindowManager::getSingleton().getWindow("FormatWin/OptionButton");
	//optionButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&IntroState::option, this));

	//Exit Window
	CEGUI::Window* exitButton = CEGUI::WindowManager::getSingleton().getWindow("FormatWin/ExitButton");
	exitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&IntroState::quit, this));
	
	//Attaching buttons
	sheet->addChildWindow(formatWin);
	CEGUI::System::getSingleton().setGUISheet(sheet);
}

bool IntroState::quit(const CEGUI::EventArgs &e){
	_exitGame = true;
	return true;
}
