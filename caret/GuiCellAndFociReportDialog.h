
#ifndef __GUI_CELL_AND_FOCI_REPORT_DIALOG_H__
#define __GUI_CELL_AND_FOCI_REPORT_DIALOG_H__

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

#include <vector>

#include "QtDialog.h"

class QCheckBox;

class CellFile;

/// dialog for cell and foci reports
class GuiCellAndFociReportDialog : public QtDialog {
   Q_OBJECT
   
   public:
      // constructor
      GuiCellAndFociReportDialog(QWidget* parent, 
                                 const bool fociFlag);
                                 
      // destructor
      ~GuiCellAndFociReportDialog();
      
   protected slots:
   
   protected:
      // called when ok/cancel button pressed
      virtual void done(int r);
      
      // create the cell/foci section
      QWidget* createCellFociSection(const QString& typeString);
                                 
      // create the paint section
      QWidget* createPaintSection();
      
      // determine if a check box is shown and checked
      bool checked(const QCheckBox* cb) const;
      
      /// number check box
      QCheckBox* numberCheckBox;
      
      /// name check box
      QCheckBox* nameCheckBox;
      
      /// position check box
      QCheckBox* positionCheckBox;
      
      /// geography check box
      QCheckBox* geographyCheckBox;
      
      /// area check box
      QCheckBox* areaCheckBox;
      
      /// size check box
      QCheckBox* sizeCheckBox;
      
      /// statistic check box
      QCheckBox* statisticCheckBox;
      
      /// comment check box
      QCheckBox* commentCheckBox;
      
      /// hemisphere check box
      QCheckBox* hemisphereCheckBox;
      
      /// class check box
      QCheckBox* classCheckBox;
      
      /// study check box
      QCheckBox* studyCheckBox;
      
      /// paint name check boxes
      std::vector<QCheckBox*> paintNameCheckBoxes;
      
      /// file type string (cell or foci)
      QString typeString;
      
      /// foci flag
      bool fociFlag;
};

#endif // __GUI_CELL_AND_FOCI_REPORT_DIALOG_H__
