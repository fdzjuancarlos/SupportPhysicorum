/*********************************************************************
 * Módulo 1. Curso de Experto en Desarrollo de Videojuegos
 * Autor: David Vallejo Fernández    David.Vallejo@uclm.es
 *
 * Código modificado a partir de Managing Game States with OGRE
 * http://www.ogre3d.org/tikiwiki/Managing+Game+States+with+OGRE
 * Inspirado en Managing Game States in C++
 * http://gamedevgeek.com/tutorials/managing-game-states-in-c/
 *
 * You can redistribute and/or modify this file under the terms of the
 * GNU General Public License ad published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * and later version. See <http://www.gnu.org/licenses/>.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.  
 *********************************************************************/

#ifndef PlayState_H
#define PlayState_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include <InputHandler.h>
#include <AnimationUpdater.h>

#include "GameState.h"

#include <OgreBulletDynamicsRigidBody.h>
#include <Shapes/OgreBulletCollisionsStaticPlaneShape.h>
#include <Shapes/OgreBulletCollisionsBoxShape.h>
#include <Shapes/OgreBulletCollisionsSphereShape.h>

#include "TrackManager.h"
#include "SoundFXManager.h"

#define PLAYER 1 << 0  // Mascara para el escenario
#define CUBE1 1 << 1  // Mascara para objetos de tipo 1
#define CUBE2 1 << 2  // Mascara para objetos de tipo 2

class PlayState : public Ogre::Singleton<PlayState>, public GameState
{
 public:
  PlayState () {}

  void enter ();
  void exit ();
  void pause ();
  void resume ();

  void keyPressed (const OIS::KeyEvent &e);
  void keyReleased (const OIS::KeyEvent &e);

  void mouseMoved (const OIS::MouseEvent &e);
  void mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id);
  void mouseReleased (const OIS::MouseEvent &e, OIS::MouseButtonID id);

  bool frameStarted (const Ogre::FrameEvent& evt);
  bool frameEnded (const Ogre::FrameEvent& evt);

  double getTimeSinceLastTime();

  // Heredados de Ogre::Singleton.
  static PlayState& getSingleton ();
  static PlayState* getSingletonPtr ();

 protected:
  Ogre::Root* _root;
  Ogre::SceneManager* _sceneMgr;
  Ogre::Viewport* _viewport;
  Ogre::Camera* _camera;
  //AnimationBlender *_animBlender;

  std::shared_ptr<InputHandler> _inputHandler;
  std::shared_ptr<AnimationUpdater> _animationUpdater;
  std::shared_ptr<SceneNode> _player;
  std::shared_ptr<SceneNode> _coordVisor;
  OgreBulletDynamics::RigidBody *rigidBoxPlayer;

  OgreBulletDynamics::DynamicsWorld * _world;
  std::deque <OgreBulletDynamics::RigidBody *>         _bodies;
  std::deque <OgreBulletCollisions::CollisionShape *>  _shapes;

  double _lastTime;
  int _changes;

  bool _exitGame;

  //Movement Booleans
  bool _forward;
  bool _back;
  bool _left;
  bool _right;

  bool _ball;
  bool _firstperson;
  bool _leftShooting;

  bool _win;

  double _newtons;
  double _cameraZoom;
  
  TrackManager* _pTrackManager;
  SoundFXManager* _pSoundFXManager;
  TrackPtr _mainTrack;
  SoundFXPtr _simpleEffect;
  
  OverlayManager* _overlayManager;
  
  bool initSDL ();


};

#endif
