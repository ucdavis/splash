
/*LICENSE_START*/
/*
 *  Copyright 1995-2002 Washington University School of Medicine
 *
 *  http://brainmap.wustl.edu
 *
 *  This file is part of CARET.
 *
 *  CARET is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  CARET is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with CARET; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
/*LICENSE_END*/

#include <QWidget>

#include "QtWidgetGroup.h"

/**
 * constructor.
 */
QtWidgetGroup::QtWidgetGroup(QWidget* parent)
   : QObject(parent)
{
}

/**
 * destructor.
 */
QtWidgetGroup::~QtWidgetGroup()
{
   //
   // Note: Do not "delete" the widgets !!!!
   //
   widgets.clear();
}

/**
 * add a widget to the group.
 */
void 
QtWidgetGroup::addWidget(QWidget* w)
{
   widgets.push_back(w);
}

/**
 * enable the group's widgets.
 */
void 
QtWidgetGroup::setEnabled(bool enable)
{
   for (int i = 0; i < widgets.size(); i++) {
      widgets.at(i)->setEnabled(enable);
   }
}

/**
 * disable the group's widgets.
 */
void 
QtWidgetGroup::setDisabled(bool disable)
{
   for (int i = 0; i < widgets.size(); i++) {
      widgets.at(i)->setDisabled(disable);
   }
}

/**
 * make the group's widgets visible.
 */
void 
QtWidgetGroup::setVisible(bool visible)
{
   for (int i = 0; i < widgets.size(); i++) {
      widgets.at(i)->setVisible(visible);
   }
}

/**
 * make the group's widgets hidden.
 */
void 
QtWidgetGroup::setHidden(bool hidden)
{
   for (int i = 0; i < widgets.size(); i++) {
      widgets.at(i)->setHidden(hidden);
   }
}

/**
 * block signals.
 */
void 
QtWidgetGroup::blockSignals(bool blockTheSignals)
{
   for (int i = 0; i < widgets.size(); i++) {
      widgets.at(i)->blockSignals(blockTheSignals);
   }
}
      
