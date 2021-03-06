/* Copyright (c) 2014-2015 "Omnidome" by cr8tr
 * Dome Mapping Projection Software (http://omnido.me).
 * Omnidome was created by Michael Winkelmann aka Wilston Oreo (@WilstonOreo)
 * 
 * This file is part of Omnidome.
 * 
 * Omnidome is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <omni/ui/GLView3D.h>

#include <QMouseEvent>
#include <omni/visual/util.h>

namespace omni
{
  namespace ui
  {
    GLView3D::GLView3D(QWidget* _parent) :
      GLView(_parent)
    {
    }
    
    GLView3D::~GLView3D()
    {
    }

    bool GLView3D::initialize()
    {
      if (!session() || initialized() || !context()) return false;

      using namespace visual;
      float _radius = session()->canvas() ? session()->canvas()->radius() : 5.0;

      camera_ = Camera(
                  Tracker(QVector3D(0,0,0), PolarVec(-45.0,45.0,_radius * 5.0)));

      lights_[0] = Light(
                     Tracker(QVector3D(0.0,0.0,0.0),
                             PolarVec(-45.0,45.0,_radius * 10.0)),1.0);

      lights_[1] = Light(
                     Tracker(QVector3D(0.0,0.0,0.0),
                             PolarVec(45.0,45.0,_radius * 10.0)),0.2);

      lights_[2] = Light(
                     Tracker(QVector3D(0.0,0.0,0.0),
                             PolarVec(45.0,-45.0,_radius * 10.0)),0.2);

      updateLight();
      this->session_->update();
      return true;
    }
      
    void GLView3D::setupCamera()
    {
      camera_.setup(30.0,aspect());
    }
 
    void GLView3D::updateLight()
    {
      GLuint _index = GL_LIGHT0;
      for (auto& _light : lights_)
        _light.setup(++_index);
    }

    void GLView3D::paintGL()
    {
      if (!session()) return;

      this->session_->update();

      makeCurrent();
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
      visual::viewport(this);

      setupCamera();
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      updateLight();
      this->session_->drawCanvas(mapping::OutputMode::MAPPED_INPUT,displayInput_);
      this->session_->drawProjectors();
      this->session_->drawCanvasWithFrustumIntersections();
      this->session_->drawProjectorHalos();
    }

    void GLView3D::wheelEvent(QWheelEvent* event)
    {
      if (!session()) return;

      float _r = event->delta()/100.0;
      camera_.track( 0, 0, _r );
      
      if (session()->canvas())
      {
        auto _r = session()->canvas()->radius();
        camera_.limitDistance(_r*0.1,_r*10.0);
      }

      update();
    }

    void GLView3D::keyPressEvent(QKeyEvent* event)
    {
    }

    void GLView3D::mouseMoveEvent(QMouseEvent *event)
    {
      if (event->buttons() & Qt::LeftButton)
      {
        if( event->modifiers() & Qt::ShiftModifier )
        {
          camera_.strafe((event->pos().x() - mousePosition().x())/20.0);
          camera_.lift((event->pos().y() - mousePosition().y())/20.0);
        }
        else
        {
          if( event->modifiers() & Qt::ControlModifier )
          {
            for (auto& _light : lights_)
            {
              _light.track( event->pos().x() - mousePosition().x(), - event->pos().y() + mousePosition().y(), 0 );
            }
            updateLight();
          }
          if( !(event->modifiers() & Qt::ControlModifier) )
            camera_.track( event->pos().x() - mousePosition().x(), event->pos().y() - mousePosition().y(), 0 );
        }
        update();
      }

      this->mousePosition_ = event->pos();
    }
      
    bool GLView3D::displayInput() const
    {
      return displayInput_;
    }

    void GLView3D::setDisplayInput(bool _displayInput)
    {
      displayInput_ = _displayInput;
      update();
    }

    void GLView3D::changeZoom(int _value)
    {
      camera_.setDistance(_value/5.0);
      update();
    }
  }
}

