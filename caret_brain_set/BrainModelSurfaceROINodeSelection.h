
#ifndef __BRAIN_MODEL_SURFACE_ROI_NODE_SELECTION_H__
#define __BRAIN_MODEL_SURFACE_ROI_NODE_SELECTION_H__

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

#include <QString>

#include "FileException.h"

class Border;
class BrainModelBorderSet;
class BrainModelSurface;
class BrainSet;
class LatLonFile;
class MetricFile;
class NodeRegionOfInterestFile;
class PaintFile;
class SurfaceShapeFile;
class TopologyHelper;

/// class for selecting nodes in a region of interest
class BrainModelSurfaceROINodeSelection {
   public:
      /// logical selection (and, or, etc)
      enum SELECTION_LOGIC {
         /// normal selection (all existing selections are reset)
         SELECTION_LOGIC_NORMAL,
         /// logically AND (intersection) new and existing selections
         SELECTION_LOGIC_AND,
         /// logically OR (union) new and existing selections
         SELECTION_LOGIC_OR,
         /// new selections are AND'ed with opposite of existing selections
         SELECTION_LOGIC_AND_NOT
      };

      // constructor
      BrainModelSurfaceROINodeSelection(BrainSet* brainSetIn);
      
      // destructor
      ~BrainModelSurfaceROINodeSelection();
      
      // see if any nodes are selected
      bool anyNodesSelected() const;
      
      // get the selection status of a node
      bool getNodeSelected(const int nodeNumber) const;
      
      // set the selection status of a node
      void setNodeSelected(const int nodeNumber,
                           const bool nodeSelectedFlag);
              
      // get the number of nodes selected
      int getNumberOfNodesSelected() const;
      
      // get node in ROI flags
      void getNodesInROI(std::vector<bool>& nodesAreInROI) const;
      
      // set the ROI selection into file
      void setRegionOfInterestIntoFile(NodeRegionOfInterestFile& nroi) const;
      
      // get the ROI selection from file
      void getRegionOfInterestFromFile(const NodeRegionOfInterestFile& nroi) throw (FileException);
      
      // get the tiles that have at least one node in the ROI
      void getTilesInRegionOfInterest(const BrainModelSurface* bms,
                                      std::vector<int>& tilesInROI) const;
                                      
      // get the nodes selection types and names
      static void getNodeSelectionTypesAndNames(std::vector<SELECTION_LOGIC>& typesOut,
                                                std::vector<QString>& namesOut);
                                                
      // get display selected nodes
      bool getDisplaySelectedNodes() const { return displaySelectedNodes; }
      
      // set display selected nodes
      void setDisplaySelectedNodes(const bool b) { displaySelectedNodes = b; }
      
      // deselect all nodes
      void deselectAllNodes();
      
      // invert selected nodes
      QString invertSelectedNodes(const BrainModelSurface* selectionSurface);
      
      // get the extent of the selection
      void getExtentOfSelectedNodes(const BrainModelSurface* bms,
                                    float extentOut[6]) const;
      
      // get the center of gravity of the selection
      void getCenterOfGravityOfSelectedNodes(const BrainModelSurface* bms,
                                             float cogOut[3]) const;
      
      // get the nodes that have min/max x/y/z values
      void getNodesWithMinMaxXYZValues(const BrainModelSurface* bms,
                                       int& xMostMedial,
                                       int& xMostLateral,
                                       int& minXNode,
                                       int& maxXNode,
                                       int& minYNode,
                                       int& maxYNode,
                                       int& minZNode,
                                       int& maxZNode,
                                       int& absMinXNode,
                                       int& absMaxXNode,
                                       int& absMinYNode,
                                       int& absMaxYNode,
                                       int& absMinZNode,
                                       int& absMaxZNode) const;
      
      // boundary only (keeps nodes in ROI that have at least one neighbor NOT in ROI)
      void boundaryOnly(const BrainModelSurface* bms);
      
      // dilate the selected nodes
      void dilate(const BrainModelSurface* selectionSurface,
                  int numberOfIterations);
      
      // dilate but only add nodes with selected paint
      void dilatePaintConstrained(const BrainModelSurface* selectionSurface,
                  const PaintFile* paintFile,
                  const int paintColumnNumber,
                  const QString& paintName,
                  const int numberOfIterations);
                  
      // erode the selected nodes
      void erode(const BrainModelSurface* selectionSurface,
                 int numberOfIterations);
      
      // erode the selected nodes but maintain a connection between two nodes
      void erodeButMaintainNodeConnection(const BrainModelSurface* selectionSurface,
                                          int numberOfIterations,
                                          const int node1,
                                          const int node2);
                                          
      // expand the ROI so that these nodes are within the ROI and connected
      void expandSoNodesAreWithinAndConnected(const BrainModelSurface* selectionSurface,
                                              const int node1,
                                              const int node2);
            
      // limit the extent of the ROI
      void limitExtent(const BrainModelSurface* selectionSurface,
                       const float extent[6]);
      
      // select all nodes (returns error message)
      QString selectAllNodes(const BrainModelSurface* selectionSurface);
      
      // select nodes with paint (returns error message)
      QString selectNodesWithPaint(const SELECTION_LOGIC selectionLogic,
                                   const BrainModelSurface* selectionSurface,
                                   const PaintFile* pf,
                                   const int paintFileColumnNumber,
                                   const QString& paintName);
                                
      // select nodes within border (returns error message)
      QString selectNodesWithinBorder(const SELECTION_LOGIC selectionLogic,
                                      const BrainModelSurface* selectionSurface,
                                      const BrainModelSurface* flatSurface,
                                      const BrainModelBorderSet* bmbs,
                                      const QString& borderName);
                                   
      // select nodes within border (returns error message)
      QString selectNodesWithinBorder(const SELECTION_LOGIC selectionLogic,
                                      const BrainModelSurface* selectionSurface,
                                      const Border* border,
                                      const bool surface3DFlag,
                                      const float zMinimum = 0.0);
                                   
      // select nodes within lat/long range (returns error message)
      QString selectNodesWithLatLong(const SELECTION_LOGIC selectionLogic,
                                     const BrainModelSurface* selectionSurface,
                                     const LatLonFile* latLonFile,
                                     const int latLonFileColumnNumber,
                                     const float minimumLatitude,
                                     const float maximumLatitude,
                                     const float minimumLongitude,
                                     const float maximumLongitude);
                                  
      // select nodes within metric range (returns error message)
      QString selectNodesWithMetric(const SELECTION_LOGIC selectionLogic,
                                    const BrainModelSurface* selectionSurface,
                                    const MetricFile* metricFile,
                                    const int metricFileColumnNumber,
                                    const float minimumMetricValue,
                                    const float maximumMetricValue);
                                 
      // select nodes within metric range and connected to specified node (returns error message)
      QString selectConnectedNodesWithMetric(const SELECTION_LOGIC selectionLogic,
                                             const BrainModelSurface* selectionSurface,
                                             const MetricFile* metricFile,
                                             const int metricFileColumnNumber,
                                             const float minimumMetricValue,
                                             const float maximumMetricValue,
                                             const int connectedToNodeNumber);
                                 
      // select nodes within surface shape range (returns error message)
      QString selectNodesWithSurfaceShape(const SELECTION_LOGIC selectionLogic,
                                          const BrainModelSurface* selectionSurface,
                                          const SurfaceShapeFile* surfaceShapeFile,
                                          const int surfaceShapeFileColumnNumber,
                                          const float minimumShapeValue,
                                          const float maximumShapeValue);
                                 
      // select nodes within surface shape range and connected to specified node (returns error message)
      QString selectConnectedNodesWithSurfaceShape(const SELECTION_LOGIC selectionLogic,
                                                   const BrainModelSurface* selectionSurface,
                                                   const SurfaceShapeFile* surfaceShapeFile,
                                                   const int surfaceShapeFileColumnNumber,
                                                   const float minimumShapeValue,
                                                   const float maximumShapeValue,
                                                   const int connectedToNodeNumber);
                                 
      // select nodes that are crossovers (returns error message)
      QString selectNodesThatAreCrossovers(const SELECTION_LOGIC selectionLogic,
                                           const BrainModelSurface* selectionSurface);
      
      // update (usually called if number of nodes changes)
      void update();
      
   protected:
      // see if two nodes are connected in the ROI
      bool areNodesConnected(const BrainModelSurface* bms,
                             const std::vector<int>& connectionFlags,
                             const int node1,
                             const int node2);
                             
      // get the topolgy helper for selection surface (returns NULL if unavailable)
      const TopologyHelper* getSelectionSurfaceTopologyHelper(const BrainModelSurface* selectionSurface,
                                                              QString& errorMessageOut) const;
                                                              
      // process the new node selections (returns error message)
      QString processNewNodeSelections(const SELECTION_LOGIC selectionLogic,
                                       const BrainModelSurface* selectionSurface,
                                       std::vector<int>& newNodeSelections);

      // the brain set
      BrainSet* brainSet;
      
      // the node selection flags (ints should be faster than bools since bools packed into bits)
      std::vector<int> nodeSelectedFlags;
      
      // display selected nodes
      bool displaySelectedNodes;
};

#endif // __BRAIN_MODEL_SURFACE_ROI_NODE_SELECTION_H__

