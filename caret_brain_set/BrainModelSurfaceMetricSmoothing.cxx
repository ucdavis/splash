
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

#include <limits>
#include <QDateTime>

#include "BrainModelSurface.h"
#include "BrainModelSurfaceMetricSmoothing.h"
#include "GaussianComputation.h"
#include "MetricFile.h"
#include "StringUtilities.h"
#include "TopologyFile.h"
#include "TopologyHelper.h"

/**
 * Constructor.
 */
BrainModelSurfaceMetricSmoothing::BrainModelSurfaceMetricSmoothing(
                                                BrainSet* bs,
                                                BrainModelSurface* fiducialSurfaceIn,
                                                BrainModelSurface* gaussianSphericalSurfaceIn,
                                                MetricFile* metricFileIn,
                                                const SMOOTH_ALGORITHM algorithmIn,
                                                const int columnIn, 
                                                const int outputColumnIn,
                                                const QString& outputColumnNameIn,
                                                const float strengthIn,
                                                const int iterationsIn,
                                                const float gaussNormBelowCutoffIn,
                                                const float gaussNormAboveCutoffIn,
                                                const float gaussSigmaNormIn,
                                                const float gaussSigmaTangIn,
                                                const float gaussTangentCutoffIn)
   : BrainModelAlgorithm(bs)
{
   fiducialSurface = fiducialSurfaceIn;
   gaussianSphericalSurface = gaussianSphericalSurfaceIn;
   if (gaussianSphericalSurface == NULL) {
      gaussianSphericalSurface = fiducialSurface;
   }
   metricFile = metricFileIn;
   algorithm = algorithmIn;
   column = columnIn;
   outputColumn = outputColumnIn;
   outputColumnName = outputColumnNameIn;
   strength = strengthIn;
   iterations = iterationsIn;
   gaussNormBelowCutoff = gaussNormBelowCutoffIn;
   gaussNormAboveCutoff = gaussNormAboveCutoffIn;
   gaussSigmaNorm = gaussSigmaNormIn;
   gaussSigmaTang = gaussSigmaTangIn;
   gaussTangentCutoff = gaussTangentCutoffIn;
}

/**
 * Destructor.
 */
BrainModelSurfaceMetricSmoothing::~BrainModelSurfaceMetricSmoothing()
{
}

/**
 * execute the algorithm.
 */
void 
BrainModelSurfaceMetricSmoothing::execute() throw (BrainModelAlgorithmException)
{
   numberOfNodes = fiducialSurface->getNumberOfNodes();
   
   //
   // Check for valid input column
   //
   if ((metricFile->getNumberOfColumns() <= 0) || 
       (metricFile->getNumberOfNodes() <= 0)) {
      throw BrainModelAlgorithmException("Metric file contains no data.");
   }
   if ((column < 0) || (column >= metricFile->getNumberOfColumns())) {
      throw BrainModelAlgorithmException("Input metric column is invalid.");
   }
   
   //
   // Inverse of strength is applied to the node's current metric value
   //
   const float oneMinusStrength = 1.0 - strength;
   
   //
   // Create a new column if needed.
   //
   if ((outputColumn < 0) || (outputColumn >= metricFile->getNumberOfColumns())){
      metricFile->addColumns(1);
      outputColumn = metricFile->getNumberOfColumns() - 1;
   }
   metricFile->setColumnName(outputColumn, outputColumnName);
   
   //
   // Copy the input column to the output column
   //
   if (column != outputColumn) {
      std::vector<float> values;
      metricFile->getColumnForAllNodes(column, values);
      metricFile->setColumnForAllNodes(outputColumn, values);
   }
   
   //
   // column now being smoothed
   //
   const int smoothColumn = outputColumn;
   
   //
   // Allocate arrays for storing data of column being smoothed
   //
   float* inputValues = new float[numberOfNodes];
   float* outputValues = new float[numberOfNodes];
   
   //
   // Coordinates of all nodes
   //
   const CoordinateFile* coordinateFile = fiducialSurface->getCoordinateFile();
   
   //
   // Used if gaussian smoothing is being performed
   //
   GaussianComputation gauss(gaussNormBelowCutoff,
                             gaussNormAboveCutoff,
                             gaussSigmaNorm,
                             gaussSigmaTang,
                             gaussTangentCutoff);
   
   //
   // Determine the neighbors for each node
   //
   determineNeighbors();
   
   //
   // smooth the data for the specified number of iterations
   //
   for (int iter = 0; iter < iterations; iter++) {
      //
      // load arrays for smoothing data
      //
      //std::vector<float> columnValues(numberOfNodes);
      //getColumnForAllNodes(outputColumn, columnValues);
      metricFile->getColumnForAllNodes(smoothColumn, inputValues);        
      
      //
      // smooth all of the nodes
      //
      for (int i = 0; i < numberOfNodes; i++) {
         //
         // copy input to output in event this node is not smoothed
         //
         outputValues[i] = inputValues[i];

         const NeighborInfo& neighInfo = nodeNeighbors[i];
                  
         //
         // Does this node have neighbors
         //
         if (neighInfo.numNeighbors > 0) {
         
            float neighborSum = 0.0;
            
            switch (algorithm) {
               case SMOOTH_ALGORITHM_NONE:
                  break;
               case SMOOTH_ALGORITHM_AVERAGE_NEIGHBORS:
                  {
                     //
                     // smooth metric data for this node
                     //
                     for (int j = 0; j < neighInfo.numNeighbors; j++) {
                        //
                        // Note: outputColumn has output from last iteration of smoothing
                        //
                        neighborSum += inputValues[neighInfo.neighbors[j]];
                     }
                     neighborSum = neighborSum / static_cast<float>(neighInfo.numNeighbors);
                  };
                  break;
               case SMOOTH_ALGORITHM_GAUSSIAN:
                  {
                     //
                     // Get neighbor information for gaussian smoothing
                     //
                     std::vector<GaussianComputation::Point3D> points;
                     for (int j = 0; j < neighInfo.numNeighbors; j++) {
                        const int neigh = neighInfo.neighbors[j];
                        points.push_back(GaussianComputation::Point3D(
                                                      coordinateFile->getCoordinate(neigh),
                                                      inputValues[neigh]));
                     }
                     
                     //
                     // Evaluate the gaussian for the node and its neighbors
                     //
                     neighborSum = gauss.evaluate(coordinateFile->getCoordinate(i),
                                                  fiducialSurface->getNormal(i),
                                                  points);
                  }
                  break;
               case SMOOTH_ALGORITHM_WEIGHTED_AVERAGE_NEIGHBORS:
                  {
                     //
                     // Distances to each neighbor
                     //
                     std::vector<float> neighborDistance(neighInfo.numNeighbors);
                     float totalDistance = 0.0;
                     for (int j = 0; j < neighInfo.numNeighbors; j++) {
                        totalDistance += neighInfo.distanceToNeighbor[j];
                     }
                     if (totalDistance == 0.0) {
                        totalDistance = 1.0;
                     }
                     
                     //
                     // compute neighbor weighted average
                     //
                     std::vector<float> neighborWeights(neighInfo.numNeighbors);
                     float totalWeight = 0.0;
                     for (int j = 0; j < neighInfo.numNeighbors; j++) {
                        neighborWeights[j] =  1.0 - (neighInfo.distanceToNeighbor[j]
                                                     / totalDistance);
                        totalWeight += neighborWeights[j];
                     }
                     if (totalWeight == 0.0) {
                        totalWeight = 1.0;
                     }
                     
                     //
                     // compute neighbor weighted average
                     //
                     for (int j = 0; j < neighInfo.numNeighbors; j++) {
                        const float weight = neighborWeights[j] / totalWeight;
                        neighborSum += inputValues[neighInfo.neighbors[j]] * weight;
                     }
                  }
                  break;
            }
            
            
            //
            // Apply smoothing to the node
            //
            outputValues[i] = (inputValues[i] * oneMinusStrength)
                            + (neighborSum * strength);
         }
      }
      
      //
      // Copy the smoothed values to the output column
      //
      metricFile->setColumnForAllNodes(smoothColumn, outputValues);
   }
   
   //
   // Add comments describing smoothing
   //
   QString smoothComment(metricFile->getColumnComment(smoothColumn));
   if (smoothComment.isEmpty() == false) {
      smoothComment.append("\n");
   }
   switch (algorithm) {
      case SMOOTH_ALGORITHM_NONE:
         smoothComment.append("Invalid smoothing algorithm: \n");
         break;
      case SMOOTH_ALGORITHM_AVERAGE_NEIGHBORS:
         smoothComment.append("Average Neighbors Smoothing: \n");
         break;
      case SMOOTH_ALGORITHM_GAUSSIAN:
         smoothComment.append("Gaussian Smoothing: \n");
         smoothComment.append("   Norm Below Cutoff: ");
         smoothComment.append(StringUtilities::fromNumber(gaussNormBelowCutoff));
         smoothComment.append("\n");
         smoothComment.append("   Norm Above Cutoff: ");
         smoothComment.append(StringUtilities::fromNumber(gaussNormAboveCutoff));
         smoothComment.append("\n");
         smoothComment.append("   Sigma Norm: ");
         smoothComment.append(StringUtilities::fromNumber(gaussSigmaNorm));
         smoothComment.append("\n");
         smoothComment.append("   Sigma Tang: ");
         smoothComment.append(StringUtilities::fromNumber(gaussSigmaTang));
         smoothComment.append("\n");
         smoothComment.append("   Tangend Cutoff: ");
         smoothComment.append(StringUtilities::fromNumber(gaussTangentCutoff));
         smoothComment.append("\n");
         break;
      case SMOOTH_ALGORITHM_WEIGHTED_AVERAGE_NEIGHBORS:
         smoothComment.append("Weighted Average Neighbors Smoothing: \n");
         break;
   }
   smoothComment.append("   Stength/Iterations: ");
   smoothComment.append(StringUtilities::fromNumber(strength));
   smoothComment.append(" ");
   smoothComment.append(StringUtilities::fromNumber(iterations));
   smoothComment.append("\n");
   metricFile->setColumnComment(smoothColumn, smoothComment);
   
   delete[] inputValues;
   delete[] outputValues;
}

/**
 * determine neighbors for each node.
 */
void 
BrainModelSurfaceMetricSmoothing::determineNeighbors()
{
   //
   // Clear the neighbors
   //
   nodeNeighbors.clear();
   
   //
   // Get the topology helper
   //
   const TopologyFile* topologyFile = fiducialSurface->getTopologyFile();
   const TopologyHelper* topologyHelper = 
                      topologyFile->getTopologyHelper(false, true, false);
   
   //
   // Coordinate file and maximum distance cutoff
   //
   CoordinateFile* cf = fiducialSurface->getCoordinateFile();
   float maxDistanceCutoff = std::numeric_limits<float>::max();
   switch (algorithm) {
      case SMOOTH_ALGORITHM_AVERAGE_NEIGHBORS:
      case SMOOTH_ALGORITHM_WEIGHTED_AVERAGE_NEIGHBORS:
         cf = fiducialSurface->getCoordinateFile();
         break;
      case SMOOTH_ALGORITHM_GAUSSIAN:
         cf = gaussianSphericalSurface->getCoordinateFile();
         maxDistanceCutoff = std::max(std::max(gaussNormBelowCutoff,
                                               gaussNormAboveCutoff),
                                      gaussTangentCutoff);
         break;
      case SMOOTH_ALGORITHM_NONE:
         break;
   }
   
   //
   // Loop through the nodes
   //
   QTime timer;
   timer.start();
   for (int i = 0; i < numberOfNodes; i++) {
      std::vector<int> neighbors;
      
      switch (algorithm) {
         case SMOOTH_ALGORITHM_AVERAGE_NEIGHBORS:
         case SMOOTH_ALGORITHM_WEIGHTED_AVERAGE_NEIGHBORS:
            {
               //
               // Get the neighbors for the node
               //
               topologyHelper->getNodeNeighbors(i, neighbors);
            }
            break;
         case SMOOTH_ALGORITHM_GAUSSIAN:
            {
               //
               // Get the neighbors for the node to the specified depth
               //
               topologyHelper->getNodeNeighborsToDepth(i, 5, neighbors);
            }
            break;
         case SMOOTH_ALGORITHM_NONE:
            break;
      }
      
      //
      // add to all neighbors
      //
      nodeNeighbors.push_back(NeighborInfo(cf, i, neighbors, maxDistanceCutoff));
   }
   const float elapsedTime = timer.elapsed() * 0.001;
   if (DebugControl::getDebugOn()) {
      std::cout << "Time to determine neighbors: " << elapsedTime << " seconds." << std::endl;
   }
}      

//***************************************************************************************

/**
 * Constructor.
 */
BrainModelSurfaceMetricSmoothing::NeighborInfo::NeighborInfo(const CoordinateFile* cf,
                                                    const int myNodeNumber,
                                                    const std::vector<int>& neighborsIn,
                                                    const float maxDistanceCutoff)
{
   const int numNeighborsIn = static_cast<int>(neighborsIn.size());
   for (int i = 0; i < numNeighborsIn; i++) {
      const int node = neighborsIn[i];
      const float dist = cf->getDistanceBetweenCoordinates(myNodeNumber, node);
      if (dist <= maxDistanceCutoff) {
         neighbors.push_back(node);
         distanceToNeighbor.push_back(dist);
      }
   }
   numNeighbors = static_cast<int>(neighbors.size());
}

/**
 * Destructor.
 */
BrainModelSurfaceMetricSmoothing::NeighborInfo::~NeighborInfo()
{
}