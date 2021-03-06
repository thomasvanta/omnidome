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

#ifndef OMNI_UI_GLVIEW_H_
#define OMNI_UI_GLVIEW_H_

#include <set>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QPointF>
#include <omni/visual/Session.h>

namespace omni
{
  class Session;

  namespace ui
  {
    class GLView : 
      public QOpenGLWidget,
      protected QOpenGLFunctions
    {
      Q_OBJECT
    public:
      explicit GLView(QWidget* _parent = nullptr);
      GLView(omni::Session*,QWidget* _parent = nullptr);
      virtual ~GLView();

      typedef std::set<GLView*> view_set_type;

      float aspect() const;
      Session const* session() const;
      void setSession(Session* _session);

      QPointF mousePosition() const;

      bool initialized() const;
    
    private slots:
      inline virtual void free() {}

    protected:
      void initializeGL();
      virtual void resizeGL(int _w, int _h);
      virtual void paintGL();

      virtual void mousePressEvent(QMouseEvent*);

      QPointF mousePosition_;
      std::unique_ptr<visual::Session> session_;

    private:
      virtual bool initialize() = 0;
      bool initialized_ = false;
    };
  }
}

#endif /* OMNI_UI_GLVIEW_H_ */
