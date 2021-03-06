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

#include <omni/ui/BrushPreview.h>

#include <QImage>
#include <QPainter>
#include <QPaintEvent>
#include <omni/BlendBrush.h>

using namespace omni::ui;

BrushPreview::BrushPreview(QWidget* _parent) :
  QWidget(_parent)
{
  update(brush_.feather(),brush_.invert());
  setMinimumSize(64 / devicePixelRatio(),64 / devicePixelRatio());
  setMaximumSize(128 / devicePixelRatio(),128 / devicePixelRatio());
}
      
void BrushPreview::update(float _feather, bool _invert)
{
  brush_.setBrush(std::min(width(),height()),_feather,_invert);
  image_ = brush_.buffer().toQImage();

  if (_invert)
    image_.invertPixels();

  QWidget::update();
}
      

void BrushPreview::resizeEvent(QResizeEvent* event)
{
  update(brush_.feather(),brush_.invert());
}

void BrushPreview::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);

  painter.drawImage(0, 0,  image_);
}

