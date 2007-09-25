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



#ifndef __BRAIN_SET_H__
#define __BRAIN_SET_H__

#include <QString>
#include <vector>

#include <QImage>
#include <QObject>
#include <QDateTime>

#include "BrainModelAlgorithmException.h"
#include "BrainModelBorderSet.h"
#include "BrainModelSurface.h"
#include "BrainSetNodeAttribute.h"
#include "Structure.h"
#include "NodeAttributeFile.h"
#include "PreferencesFile.h"
#include "SceneFile.h"
#include "SpecFile.h"
#include "TopologyFile.h"
#include "TransformationMatrixFile.h"
#include "VolumeFile.h"

// forward declarations avoid include files (fewer dependencies when compiling).
class QProgressDialog;
class QWidget;

class AreaColorFile;
class ArealEstimationFile;
class BorderColorFile;
class BorderFile;
class BrainModelContours;
class BrainModelSurfaceAndVolume;
class BrainModelSurfaceNodeColoring;
class BrainModelVolumeVoxelColoring;
class BrainModelVolume;
class BrainModelVolumeRegionOfInterest;
class CellColorFile;
class CellFile;
class CellProjectionFile;
class CocomacConnectivityFile;
class ContourCellFile;
class ContourCellColorFile;
class ContourFile;
class CutsFile;
class DeformationFieldFile;
class DeformationMapFile;
class DisplaySettingsArealEstimation;
class DisplaySettingsBorders;
class DisplaySettingsCells;
class DisplaySettingsContours;
class DisplaySettingsCuts;
class DisplaySettingsCoCoMac;
class DisplaySettingsDeformationField;
class DisplaySettingsFoci;
class DisplaySettingsGeodesicDistance;
class DisplaySettingsImages;
class DisplaySettingsMetric;
class DisplaySettingsModels;
class DisplaySettingsSection;
class DisplaySettingsSurface;
class DisplaySettingsPaint;
class DisplaySettingsProbabilisticAtlas;
class DisplaySettingsScene;
class DisplaySettingsRgbPaint;
class DisplaySettingsStudyMetaData;
class DisplaySettingsSurfaceShape;
class DisplaySettingsSurfaceVectors;
class DisplaySettingsTopography;
class DisplaySettingsVolume;
class DisplaySettingsWustlRegion;
class FociColorFile;
class FociFile;
class FociProjectionFile;
class GeodesicDistanceFile;
class ImageFile;
class LatLonFile;
class MetricFile;
class BrainModelIdentification;
class NodeAttributeFile;
class PaintFile;
class PaletteFile;
class ParamsFile;
class PreferencesFile;
class ProbabilisticAtlasFile;
class QTimer;
class RgbPaintFile;
class SectionFile;
class StudyMetaDataFile;
class SurfaceShapeFile;
class SurfaceVectorFile;
class TopographyFile;
class VocabularyFile;
class VtkModelFile;
class WustlRegionFile;

/**
 * Brain Set contains all Surfaces (topology and coordinates) and all
 * attribute data (paint files, metric files, etc).
 */
class BrainSet : public QObject {
   Q_OBJECT
   
   public:
      /// spec file read mode
      enum SPEC_FILE_READ_MODE {
         SPEC_FILE_READ_MODE_NORMAL,
         SPEC_FILE_READ_MODE_APPEND
      };
      
      /// Constructor (using only the caret main sets the primaryBrainSetFlagIn)
      BrainSet(const bool primaryBrainSetFlagIn = false);
      
      /// Read in the brain set contained in the spec file
      BrainSet(const QString& specFileNameIn,
               const bool readAllFilesInSpecFile,
               const bool primaryBrainSetFlagIn);
      
      /// Construct a brain set from a topology file and a coordinate file
      BrainSet(const QString& topoFileName,
               const QString& coordFileName);
       
      /// Construct a brain set from a vtk surface file
      BrainSet(const QString& vtkSurfaceFileName,
               const BrainModelSurface::SURFACE_TYPES surfaceType = BrainModelSurface::SURFACE_TYPE_UNKNOWN,
               const TopologyFile::TOPOLOGY_TYPES topologyType = TopologyFile::TOPOLOGY_TYPE_UNKNOWN);
      
      /// Destructor
      virtual ~BrainSet();
      
      /// initialize static stuff such as caretHomeDirectory and Preferences File
      /// this is normally called from the BrainSet constructor
      static void initializeStaticStuff();
      
      /// initialize data file static members
      static void initializeDataFileStaticMembers();

      /// set the random seed
      static void setRandomSeed(unsigned int randomSeed);
      
      /// get the web caret flag
      bool getWebCaretFlag() const { return webCaretFlag; }
      
      /// set the web caret flag
      void setWebCaretFlag(const bool flag) { webCaretFlag = flag; }
      
      /// get display cross for node
      int getDisplayCrossForNode() const { return displayCrossForNode; }
      
      /// get surface on which identification was made
      BrainModelSurface* getDisplayCrossSurface() const { return displayNoCrossForSurface; }
      
      /// set display cross for node
      void setDisplayCrossForNode(const int nodeNumber, BrainModelSurface* surface);
           
      /// add a brain model
      void addBrainModel(BrainModel* bm, const bool readingSpecFile = false);
      
      /// delete all brain models
      void deleteAllBrainModels();
      
      /// delete a brain model
      void deleteBrainModel(const BrainModel* bm);
            
      /// add a topology file
      void addTopologyFile(TopologyFile* tf);
      
      /// delete topology file
      void deleteTopologyFile(TopologyFile* tf);
      
      /// add a volume file of the specified type
      void addVolumeFile(const VolumeFile::VOLUME_TYPE vt, VolumeFile* vf,
                         const QString& name,
                         const bool append = true, const bool updateSpec = true);
      
      /// delete a volume file
      void deleteVolumeFile(const VolumeFile* vf);
      
      /// apply all projected files (foci, cells, borders)
      void applyAllProjectedFiles();
      
      /// Assign border colors to border projections and borders
      void assignBorderColors();
      
      /// Assign cell colors to cell and cell projection files
      void assignCellColors();
      
      /// Assign contour cell colors to contour cells
      void assignContourCellColors();

      /// Assign foci colors to foci and foci projection files
      void assignFociColors();
      
      /// add nodes to all surfaces at the origin
      void addNodes(const int numNodesToAdd);
      
      /// Clear all display list.
      void clearAllDisplayLists();

      /// classify the nodes as interior and edges
      void classifyNodes(TopologyFile* tf = NULL,
                         const bool onlyDoClassificationIfNeeded = false);
      
      /// find out if nodes have been classified
      bool getNodesHaveBeenClassified() const { return nodesHaveBeenClassified; }
      
      /// get the number of brain models
      int getNumberOfBrainModels() const { return brainModels.size(); }
      
      /// get the index for a brain model
      int getBrainModelIndex(const BrainModel* bm) const;
      
      /// get a brain model
      BrainModel* getBrainModel(const int modelIndex) { return brainModels[modelIndex]; }
      
      /// get a brain model (constant method)
      const BrainModel* getBrainModel(const int modelIndex) const { 
         return brainModels[modelIndex]; 
      }
      
      /// get a brain model contours (if negative index first one found is returned)
      BrainModelContours* getBrainModelContours(const int modelIndex = -1);
      
      /// get a brain model contours (const method) (if negative index first one found is returned)
      const BrainModelContours* getBrainModelContours(const int modelIndex = -1) const;

      /// get a brain model surface
      BrainModelSurface* getBrainModelSurface(const int modelIndex);
            
      /// get a brain model surface (const method)
      const BrainModelSurface* getBrainModelSurface(const int modelIndex) const;
            
      /// get a brain model surface with the specified file name (NULL if not found)
      BrainModelSurface* getBrainModelSurfaceWithFileName(const QString& fileName);
      
      /// get the index of the first brain model surface (returns -1 if not found)
      int getFirstBrainModelSurfaceIndex() const;
      
      /// set the "active" fiducial surface
      void setActiveFiducialSurface(BrainModelSurface* bms);
      
      /// get the "active" fiducial surface
      BrainModelSurface* getActiveFiducialSurface();
      
      /// delete a brain model surface 
      void deleteBrainModelSurface(BrainModelSurface* bms);
      
      /// delete all brain model surfaces
      void deleteAllBrainModelSurfaces();
      
      /// delete all topology files
      void deleteAllTopologyFiles();
      
      /// get a brain model volume (if negative index first one found is returned) 
      BrainModelVolume* getBrainModelVolume(const int modelIndex = -1);
            
      /// get a brain model volume  (if negative index first one found is returned) CONST method
      const BrainModelVolume* getBrainModelVolume(const int modelIndex = -1) const;
            
      /// get a brain model surface and volume (if negative index first one found is returned) 
      BrainModelSurfaceAndVolume* getBrainModelSurfaceAndVolume(const int modelIndex = -1);
            
      /// get a brain model surface and volume  (if negative index first one found is returned) CONST method
      const BrainModelSurfaceAndVolume* getBrainModelSurfaceAndVolume(const int modelIndex = -1) const;
            
      /// get a brain model surface of the specified type
      BrainModelSurface* getBrainModelSurfaceOfType(const BrainModelSurface::SURFACE_TYPES st);
      
      /// get the number of nodes
      int getNumberOfNodes() const;
      
      /// find a node with the specified morph row and column (returns -1 if not found)
      int getNodeWithMorphRowColumn(const int row, const int column,
                                    const int startAtNode = 0) const;
      
      /// get the identification object
      BrainModelIdentification* getBrainModelIdentification() { return brainModelIdentification; }
      
      /// get access to node coloring
      BrainModelSurfaceNodeColoring* getNodeColoring() { return nodeColoring; }
      
      /// get access to the voxel coloring
      BrainModelVolumeVoxelColoring* getVoxelColoring() { return voxelColoring; }
      
      /// get the volume region of interest controller
      BrainModelVolumeRegionOfInterest* getVolumeRegionOfInterestController()
                                      { return brainModelVolumeRegionOfInterest; }
                                      
      /// get the stereotaxic space
      QString getStereotaxicSpace() const { return stereotaxicSpace; }
      
      /// set the stereotaxic space
      void setStereotaxicSpace(const QString& s) { stereotaxicSpace = s; }
      
      /// get the subject
      QString getSubject() const { return subject; }
      
      /// set the subject
      void setSubject(const QString& s);
      
      /// get the species
      QString getSpecies() const { return species; }
      
      /// set the species
      void setSpecies(const QString& s);
      
      /// get the structure
      Structure getStructure() const {
         return structure; 
      }

      /// set the structure
      void setStructure(const Structure::STRUCTURE_TYPE s);
      
      /// set the structure
      void setStructure(const Structure& s);
      
      /// add a document file
      void addDocumentFile(const QString& documentFileName);
      
      /// get the areal estimation file
      ArealEstimationFile* getArealEstimationFile() { 
         return arealEstimationFile; 
      }
      
      /// get the cocomac file
      CocomacConnectivityFile* getCocomacFile() { return cocomacFile; }
      
      /// get the contour cell file
      ContourCellFile* getContourCellFile() { return contourCellFile; }
      
      /// get the contour cell color file
      ContourCellColorFile* getContourCellColorFile() { return contourCellColorFile; }
      
      /// get the cuts file
      CutsFile* getCutsFile() { return cutsFile; }

      /// get the name of the deformation map file
      QString getDeformationMapFileName() const { return deformationMapFileName; }
      
      /// set the deformation map file name
      void setDeformationMapFileName(const QString& name, const bool updateSpec);
      
      /// get the deformation field file
      DeformationFieldFile* getDeformationFieldFile() { return deformationFieldFile; }
      
      /// get the metric file
      MetricFile* getMetricFile() { return metricFile; }
      
      /// get the area color file
      AreaColorFile* getAreaColorFile() { return areaColorFile; }
      
      /// get the probabilistic atlas file
      ProbabilisticAtlasFile* getProbabilisticAtlasSurfaceFile() { return probabilisticAtlasSurfaceFile; }
      
      /// get the paint file
      PaintFile* getPaintFile() { return paintFile; }
      
      /// get the palette file
      PaletteFile* getPaletteFile() { return paletteFile; }
      
      /// get the parameters file
      ParamsFile* getParamsFile() { return paramsFile; }
      
      /// get the RGB Paint file
      RgbPaintFile* getRgbPaintFile() { return rgbPaintFile; }
      
      /// get the study metadata file
      StudyMetaDataFile* getStudyMetaDataFile() { return studyMetaDataFile; }
      
      /// get the vocabulary file
      VocabularyFile* getVocabularyFile() { return vocabularyFile; }
      
      /// get the wustl region file
      WustlRegionFile* getWustlRegionFile() { return wustlRegionFile; }
      
      /// get the geodesic distance file
      GeodesicDistanceFile* getGeodesicDistanceFile() { return geodesicDistanceFile; }
      
      /// get the surface shape file
      SurfaceShapeFile* getSurfaceShapeFile() { return surfaceShapeFile; }
      
      /// get the surface vector file
      SurfaceVectorFile* getSurfaceVectorFile() { return surfaceVectorFile; }
      
      /// get the number of topology files
      int getNumberOfTopologyFiles() const { return topologyFiles.size(); }

      /// get a topology file by its index
      TopologyFile* getTopologyFile(const int index) { return topologyFiles[index]; }
      
      /// get the topography file
      TopographyFile* getTopographyFile() { return topographyFile; }
      
      /// get the closed topology
      TopologyFile* getTopologyClosed() { return topologyClosed; }
      
      /// get the open topology
      TopologyFile* getTopologyOpen() { return topologyOpen; }
      
      /// get the cut topology
      TopologyFile* getTopologyCut() { return topologyCut; }
      
      /// get the lobar cut topology
      TopologyFile* getTopologyLobarCut() { return topologyLobarCut; }
      
      /// get the unknown topology
      TopologyFile* getTopologyUnknown() { return topologyUnknown; }
      
      /// get the transformation matrix file
      TransformationMatrixFile* getTransformationMatrixFile() { return transformationMatrixFile; }
      
      /// get the name of the cerebral hull file
      QString getCerebralHullFileName() const { return cerebralHullFileName; }
      
      /// generate the cerebral hull vtk file from the segmentation volume
      void generateCerebralHullVtkFile(const VolumeFile* segmentationVolume,
                                       const bool saveHullVolumeFileFlag)
                                           throw (BrainModelAlgorithmException);
      
      /// get the areal estimation settings
      DisplaySettingsArealEstimation* getDisplaySettingsArealEstimation() {
         return displaySettingsArealEstimation;
      }
      
      /// get the border display settings
      DisplaySettingsBorders* getDisplaySettingsBorders() {
         return displaySettingsBorders;
      }
      
      /// get the cell display settings
      DisplaySettingsCells* getDisplaySettingsCells() {
         return displaySettingsCells;
      }
      
      /// get the contour display settings
      DisplaySettingsContours* getDisplaySettingsContours() {
         return displaySettingsContours;
      }
      
      /// get the cuts display settings
      DisplaySettingsCuts* getDisplaySettingsCuts() {
         return displaySettingsCuts;
      };
      
      /// get the cocomac display settings
      DisplaySettingsCoCoMac* getDisplaySettingsCoCoMac() {
         return displaySettingsCoCoMac;
      }
      
      /// get the foci display settings
      DisplaySettingsFoci* getDisplaySettingsFoci() {
         return displaySettingsFoci;
      }
      
      /// get the deformation field display settings
      DisplaySettingsDeformationField* getDisplaySettingsDeformationField() {
         return displaySettingsDeformationField;
      }
      
      /// get the images display settings
      DisplaySettingsImages* getDisplaySettingsImages() {
         return displaySettingsImages;
      }
      
      /// get the metric display settings
      DisplaySettingsMetric* getDisplaySettingsMetric() { 
         return displaySettingsMetric;
      }
      
      /// get the model display settings
      DisplaySettingsModels* getDisplaySettingsModels() { 
         return displaySettingsModels;
      }
      
      /// get the node display settings
      DisplaySettingsSurface* getDisplaySettingsSurface() {
         return displaySettingsSurface;
      }
      
      /// get the section display settings
      DisplaySettingsSection* getDisplaySettingsSection() {
         return displaySettingsSection;
      }
      
      /// get the paint display settings
      DisplaySettingsPaint* getDisplaySettingsPaint() {
         return displaySettingsPaint;
      }
      
      /// get the prob atlas display settings
      DisplaySettingsProbabilisticAtlas* getDisplaySettingsProbabilisticAtlasSurface() { 
         return displaySettingsProbabilisticAtlasSurface;
      }
      
      /// get the rgb paint display settings
      DisplaySettingsRgbPaint* getDisplaySettingsRgbPaint() {
         return displaySettingsRgbPaint;
      }
      
      /// get the scene display settings
      DisplaySettingsScene* getDisplaySettingsScene() {
         return displaySettingsScene;
      }
      
      /// get the surface shape display settings
      DisplaySettingsSurfaceShape* getDisplaySettingsSurfaceShape() {
         return displaySettingsSurfaceShape;
      }
      
      /// get the study metadata display settings
      DisplaySettingsStudyMetaData* getDisplaySettingsStudyMetaData() {
         return displaySettingsStudyMetaData;
      }
      
      /// get the surface vector display settings
      DisplaySettingsSurfaceVectors* getDisplaySettingsSurfaceVectors() {
         return displaySettingsSurfaceVectors;
      };
      
      /// get the topography file display settings
      DisplaySettingsTopography* getDisplaySettingsTopography() {
         return displaySettingsTopography;
      }
      
      /// get the volume display settings
      DisplaySettingsVolume* getDisplaySettingsVolume() {
         return displaySettingsVolume;
      }
      
      /// get the volume display settings (const method)
      const DisplaySettingsVolume* getDisplaySettingsVolume() const {
         return displaySettingsVolume;
      }
      
      /// get the volume prob atlas display settings
      DisplaySettingsProbabilisticAtlas* getDisplaySettingsProbabilisticAtlasVolume() { 
         return displaySettingsProbabilisticAtlasVolume;
      }
      
      /// get the wustl region display settings
      DisplaySettingsWustlRegion* getDisplaySettingsWustlRegion() {
         return displaySettingsWustlRegion;
      }
      
      /// get the geodesic distance display settings
      DisplaySettingsGeodesicDistance* getDisplaySettingsGeodesicDistance() {
         return displaySettingsGeodesicDistance;
      }
      
      ///  Set the default scaling for all surfaces
      void setDefaultScaling(const float orthoRight, const float orthoTop);
      
      /// reset the Brain Surfaces
      void reset(const bool keepSceneData = false);
      
      /// reset all data files
      void resetDataFiles(const bool keepSceneData);
      
      /// reset all node attribute files
      void resetNodeAttributeFiles();
      
      /// create a spec file from all files in the selected scenes
      void createSpecFromScenes(const std::vector<int>& sceneIndices,
                                const QString& newSpecFileName,
                                const QString& newSceneFileName,
                                QString& errorMessageOut);
                                
      /// add a tag and file pair to the spec file
      void addToSpecFile(const QString& specFileTag, const QString& fileName,
                         const QString& fileName2 = "");
      
      /// get the spec file that keeps track of loaded files
      SpecFile* getLoadedFilesSpecFile()  { return &loadedFilesSpecFile; }
      
      /// get the spec file (const method)
      const SpecFile* getLoadedFilesSpecFile() const { return &loadedFilesSpecFile; }
      
      /// get the spec file name
      QString getSpecFileName() const { return specFileName; }
      
      /// set the spec file name
      void setSpecFileName(const QString& name);

      /// get the time the spec file was loaded
      QDateTime getSpecFileTimeOfLoading() const { return specFileTimeOfLoading; }
       
      /// read the spec file (returns true if reading was aborted by user)
      bool readSpecFile(const SpecFile& specFileIn, 
                        const QString& specFileNameIn,
                        QString& errorMessagesOut);
                        
      
      /// read the spec file (returns true if reading was aborted by user)
      bool readSpecFile(const SPEC_FILE_READ_MODE specReadMode,
                        const SpecFile& specFileIn, 
                        const QString& specFileNameIn,
                        std::vector<QString>& errorMessagesOut,
                        const TransformationMatrix* specTransformationMatrixIn,
                        QProgressDialog* progressDialog);
                        
      /// read the spec file (returns true if reading was aborted by user)
      bool readSpecFile(const SPEC_FILE_READ_MODE specReadMode,
                        const SpecFile& specFileIn, 
                        const QString& specFileNameIn,
                        QString& errorMessageOut,
                        const TransformationMatrix* specTransformationMatrixIn,
                        QProgressDialog* progressDialog);
                     
      /// delete all borders
      void deleteAllBorders();
      
      /// get the border color file
      BorderColorFile* getBorderColorFile() { return borderColorFile; }
      
      /// get the border set
      BrainModelBorderSet* getBorderSet() { return brainModelBorderSet; }
                  
      /// get the volume borders
      BorderFile* getVolumeBorderFile() { return brainModelBorderSet->getVolumeBorders(); }
      
      /// convert volume borders to cells
      void convertVolumeBordersToFiducialCells();
      
      /// get the volume borders const method
      const BorderFile* getVolumeBorderFile() const { return brainModelBorderSet->getVolumeBorders(); }
      
      /// cell color file
      CellColorFile* getCellColorFile() { return cellColorFile; }
      
      /// get the cell projection file
      CellProjectionFile* getCellProjectionFile() { return cellProjectionFile; }
      
      /// get the volume cell file
      CellFile* getVolumeCellFile() { return volumeCellFile; }
      
      /// get the volume cell file (const method)
      const CellFile* getVolumeCellFile() const { return volumeCellFile; }
      
      /// delete all cells and cell projections
      void deleteAllCells(const bool deleteCellProjections,
                          const bool deleteVolumeCells);
      
      /// delete all cell projections (including those in cell files)
      void deleteAllCellProjections();
      
      /// delete a cell
      void deleteCell(const int cellNumber);

      /// move foci study info to the study meta data file
      void moveFociStudyInfoToStudyMetaDataFile();
      
      /// get the foci color file
      FociColorFile* getFociColorFile() { return fociColorFile; }
      
      /// get the foci projection file
      FociProjectionFile* getFociProjectionFile() { return fociProjectionFile; }
      
      /// get a volume foci file
      FociFile* getVolumeFociFile() { return volumeFociFile; }
      
      /// set a volume foci file
      const FociFile* getVolumeFociFile() const { return volumeFociFile; }
      
      /// delete all foci and foci projections
      void deleteAllFoci(const bool deleteFociProjections,
                         const bool deleteVolumeFoci);
      
      /// delete all foci projections (including those in foci files)
      void deleteAllFociProjections();
      
      /// delete a focus
      void deleteFocus(const int focusNumber);

      /// get the lat/lon file
      LatLonFile* getLatLonFile() { return latLonFile; }
      
      /// get the section file
      SectionFile* getSectionFile() { return sectionFile; }
      
      /// get the scene file
      SceneFile* getSceneFile() { return sceneFile; };
      
      /// get the preferences file
      static PreferencesFile* getPreferencesFile() { return &preferencesFile; }
      
      /// get the preferences file name
      static QString getPreferencesFileName() { return preferencesFileName; }
      
      /// get the number of volume functional files
      int getNumberOfVolumeFunctionalFiles() const { return volumeFunctionalFiles.size(); }
      
      /// get the volume functional file
      VolumeFile* getVolumeFunctionalFile(const int index);
      
      /// get the volume functional file (const method)
      const VolumeFile* getVolumeFunctionalFile(const int index) const;
      
      /// get the volume functional files
      void getVolumeFunctionalFiles(std::vector<VolumeFile*>& files) { files = volumeFunctionalFiles; }
      
      /// get the number of paint volume files
      int getNumberOfVolumePaintFiles() const { return volumePaintFiles.size(); }
      
      /// get the volume paint file
      VolumeFile* getVolumePaintFile(const int index);
      
      /// get the volume paint file (const method)
      const VolumeFile* getVolumePaintFile(const int index) const;
      
      /// get the volume paint files
      void getVolumePaintFiles(std::vector<VolumeFile*>& files) { files = volumePaintFiles; }
      
      /// get the volume prob atlas file
      VolumeFile* getVolumeProbAtlasFile(const int index);
      
      /// get the volume prob atlas file (const method)
      const VolumeFile* getVolumeProbAtlasFile(const int index) const;
      
      /// get the volume prob atlas files
      void getVolumeProbAtlasFiles(std::vector<VolumeFile*>& files) { files = volumeProbAtlasFiles; }
      
      /// get the number of prob atlas volume files
      int getNumberOfVolumeProbAtlasFiles() const { return volumeProbAtlasFiles.size(); }
      
      /// get the number of rgb volume files
      int getNumberOfVolumeRgbFiles() const { return volumeRgbFiles.size(); }
      
      /// get the volume rgb file
      VolumeFile* getVolumeRgbFile(const int index);
      
      /// get the volume rgb file (const method)
      const VolumeFile* getVolumeRgbFile(const int index) const;
      
      /// get the volume rgb files
      void getVolumeRgbFiles(std::vector<VolumeFile*>& files) { files = volumeRgbFiles; }
      
      /// get the number of volume segmentation files
      int getNumberOfVolumeSegmentationFiles() const { return volumeSegmentationFiles.size(); }
      
      /// get the volume segmentation file
      VolumeFile* getVolumeSegmentationFile(const int index);
      
      /// get the volume segmentation file (const method)
      const VolumeFile* getVolumeSegmentationFile(const int index) const;
      
      /// get the volume segmentation files
      void getVolumeSegmentationFiles(std::vector<VolumeFile*>& files) { files = volumeSegmentationFiles; }
      
      /// get the number of volume anatomy files
      int getNumberOfVolumeAnatomyFiles() const { return volumeAnatomyFiles.size(); }
      
      /// get the volume anatomy file
      VolumeFile* getVolumeAnatomyFile(const int index);
      
      /// get the volume anatomy file (const method)
      const VolumeFile* getVolumeAnatomyFile(const int index) const;
      
      /// get the volume anatomy files
      void getVolumeAnatomyFiles(std::vector<VolumeFile*>& files) { files = volumeAnatomyFiles; }
      
      /// get the number of volume vector files
      int getNumberOfVolumeVectorFiles() const { return volumeVectorFiles.size(); }
      
      /// get the volume vector file
      VolumeFile* getVolumeVectorFile(const int index);
      
      /// get the volume vector file (const method)
      const VolumeFile* getVolumeVectorFile(const int index) const;
      
      /// get the volume vector files
      void getVolumeVectorFiles(std::vector<VolumeFile*>& files) { files = volumeVectorFiles; }
      
      /// delete all image files
      void deleteAllImageFiles();
      
      /// delete an image file
      void deleteImageFile(ImageFile* img);
      
      /// add an image file
      void addImageFile(ImageFile* img);
      
      /// get number of image files
      int getNumberOfImageFiles() const { return imageFiles.size(); }
      
      /// get an image file
      ImageFile* getImageFile(const int fileNum);
      
      /// get an image file based upon the image's name
      ImageFile* getImageFile(const QString& filename);
      
      /// see if an image file is valid
      bool getImageFileValid(const ImageFile* img) const;
      
      /// delete all of the VTK model files
      void deleteAllVtkModelFiles();

      /// delete a VTK model file
      void deleteVtkModelFile(VtkModelFile* vmf);

      /// add a VTK model file
      void addVtkModelFile(VtkModelFile* vmf);
      
      /// get the number of vtk model files
      int getNumberOfVtkModelFiles() const { return vtkModelFiles.size(); }
      
      /// get a vtk model file
      VtkModelFile* getVtkModelFile(const int modelNum);
      
      /// assign transformation data file colors
      void assignTransformationDataFileColors();
      
      /// get the number of transformation data files
      int getNumberOfTransformationDataFiles() const 
                             { return transformationDataFiles.size(); }
                             
      /// get a transformation data file
      AbstractFile* getTransformationDataFile(const int indx)
                             { return transformationDataFiles[indx]; }
      
      /// get a transformation data file (const method)
      const AbstractFile* getTransformationDataFile(const int indx) const
                             { return transformationDataFiles[indx]; }
      
      /// get have transformation data cell files
      bool getHaveTransformationDataCellFiles() const;
      
      /// get have transformation data contour files
      bool getHaveTransformationDataContourFiles() const;
      
      /// get have transformation data contour cell files
      bool getHaveTransformationDataContourCellFiles() const;
      
      /// get have transformation data foci files
      bool getHaveTransformationDataFociFiles() const;
      
      /// get have transformation data vtk files
      bool getHaveTransformationDataVtkFiles() const;
      
      /// get caret's home directory
      static QString getCaretHomeDirectory() { return caretHomeDirectory; }
      
      /// get the common node attributes for a node
      BrainSetNodeAttribute* getNodeAttributes(const int nodeNum);
            
      /// get the common node attributes for a node (const method)
      const BrainSetNodeAttribute* getNodeAttributes(const int nodeNum) const {
         return &nodeAttributes[nodeNum];
      }
      
      /// set the visited flag on all node attributes
      void setAllNodesVisited(const bool value);
      
      /// read the areal estimation data file 
      void readArealEstimationFile(const QString& name, const bool append,
                                   const bool updateSpec) throw (FileException);
      
      /// write the ArealEstimation data file
      void writeArealEstimationFile(const QString& name) throw (FileException);
      
      /// read a volume border file
      void readVolumeBorderFile(const QString& name,
                                const bool append,
                                const bool updateSpec) throw (FileException);
      
      /// write a volume border file
      void writeVolumeBorderFile(const QString& name,
                                 const bool removeDuplicates = false) throw (FileException);
                                 
      /// read the border data file 
      void readBorderFile(const QString& name, const BrainModelSurface::SURFACE_TYPES st,
                          const bool append,
                          const bool updateSpec) throw (FileException);
      
      /// write the Border data file
      void writeBorderFile(const QString& name,
                           const BrainModelSurface* bms,
                           const BrainModelSurface::SURFACE_TYPES borderFileType,
                           const QString& commentText,
                           const QString& pubMedID,
                           const bool removeDuplicates = false) throw (FileException);
      
      /// read the border color data file 
      void readBorderColorFile(const QString& name, const bool append,
                               const bool updateSpec) throw (FileException);
      
      /// write the BorderColor data file
      void writeBorderColorFile(const QString& name) throw (FileException);
      
      /// read the border projection data file 
      void readBorderProjectionFile(const QString& name, 
                                    const bool append,
                                    const bool updateSpec) throw (FileException);
      
      /// write the BorderProjection data file
      void writeBorderProjectionFile(const QString& name,
                                     const QString& commentText,
                                     const QString& pubMedID,
                                     const bool removeDuplicates = false) throw (FileException);
      
      /// read the cell data file 
      void readCellFile(const QString& name, 
                        const bool append,
                        const bool updateSpec) throw (FileException);
      
      /// read the volume cell data file 
      void readVolumeCellFile(const QString& name, 
                        const bool append,
                        const bool updateSpec) throw (FileException);
      
      /// write the Cell data file
      void writeCellFile(const QString& name,
                         const BrainModelSurface* bms,
                         const AbstractFile::FILE_FORMAT fileFormat,
                         const QString& commentText) throw (FileException);
      
      /// write the Volume Cell data file
      void writeVolumeCellFile(const QString& name) throw (FileException);
      
      /// read the cell color data file 
      void readCellColorFile(const QString& name, const bool append,
                             const bool updateSpec) throw (FileException);
      
      /// write the CellColor data file
      void writeCellColorFile(const QString& name) throw (FileException);
      
      /// read the cell projection data file 
      void readCellProjectionFile(const QString& name, 
                                  const bool append,
                                  const bool updateSpec) throw (FileException);
     
      /// write the CellProjection data file
      void writeCellProjectionFile(const QString& name) throw (FileException);
      
      /// read the cocomac connectivity file
      void readCocomacConnectivityFile(const QString& name, const bool append,
                                       const bool updateSpec) throw (FileException);
      
      /// write the CocomacConnectivity data file
      void writeCocomacConnectivityFile(const QString& name) throw (FileException);
      
      /// read a contour file
      void readContourFile(const QString& name, 
                           const bool append,
                           const bool updateSpec) throw (FileException);
                          
      /// write the Contour data file
      void writeContourFile(const QString& name, ContourFile* cf) throw (FileException);
      
      /// read a contour cell file
      void readContourCellFile(const QString& name, 
                               const bool append,
                               const bool updateSpec) throw (FileException);
                          
      /// read a contour cell color file
      void readContourCellColorFile(const QString& name, 
                               const bool append,
                               const bool updateSpec) throw (FileException);
                          
      /// write the Contour Cell data file
      void writeContourCellFile(const QString& name) throw (FileException);
      
      /// write the Contour Cell Color data file
      void writeContourCellColorFile(const QString& name) throw (FileException);
      
      /// read the coordinate data file file
      void readCoordinateFile(const QString& name, const BrainModelSurface::SURFACE_TYPES st,
                              const bool readingSpecFile,
                              const bool append,
                              const bool updateSpec) throw (FileException);
      
      /// write the coordinate data file
      void writeCoordinateFile(const QString& name, 
                               const BrainModelSurface::SURFACE_TYPES st,
                               CoordinateFile* cf,
                               const bool updateSpecFile = true) throw (FileException);
      
      /// read the surface data file file
      void readSurfaceFile(const QString& name, const BrainModelSurface::SURFACE_TYPES st,
                              const bool readingSpecFile,
                              const bool append,
                              const bool updateSpec) throw (FileException);
      
      /// write the surface data file
      void writeSurfaceFile(const QString& name, 
                            const BrainModelSurface::SURFACE_TYPES st,
                            BrainModelSurface* bms,
                            const bool updateSpecFile = true,
                            const AbstractFile::FILE_FORMAT fileFormat = AbstractFile::FILE_FORMAT_XML) throw (FileException);
      
      /// read the cuts file
      void readCutsFile(const QString& name, const bool append,
                        const bool updateSpec) throw (FileException);
      
      /// write the Cuts data file
      void writeCutsFile(const QString& name) throw (FileException);
      
      /// read the foci data file file
      void readFociFile(const QString& name, 
                        const bool append,
                        const bool updateSpec) throw (FileException);
      
      /// Read the volume foci data file.
      void readVolumeFociFile(const QString& name, 
                              const bool append,
                              const bool updateSpec) throw (FileException);
                       
      /// write the Foci data file
      void writeFociFile(const QString& name,
                         const BrainModelSurface* leftBms,
                         const BrainModelSurface* rightBms,
                         const AbstractFile::FILE_FORMAT fileFormat,
                         const QString& commentText) throw (FileException);
      
      /// write the Foci data file
      void writeFociFileOriginalCoordinates(const QString& name,
                                            const AbstractFile::FILE_FORMAT fileFormat,
                                            const QString& commentText) throw (FileException);
      
      /// Write the volume foci data file.
      void writeVolumeFociFile(const QString& name) throw (FileException);

      /// read the foci color data file file
      void readFociColorFile(const QString& name, const bool append,
                             const bool updateSpec) throw (FileException);
      
      /// write the FociColor data file
      void writeFociColorFile(const QString& name) throw (FileException);
      
      /// read the foci projection data file file
      void readFociProjectionFile(const QString& name, 
                                  const bool append,
                                  const bool updateSpec) throw (FileException);
      
      /// write the FociProjection data file
      void writeFociProjectionFile(const QString& name) throw (FileException);
      
      /// read the geodesic distance data file file
      void readGeodesicDistanceFile(const QString& name, const bool append,
                                    const bool updateSpec) throw (FileException);
      
      /// read the geodesic distance data file file (only selected columns)
      void readGeodesicDistanceFile(const QString& name, 
                          const std::vector<int>& columnDestination,
                          const std::vector<QString>& fileBeingReadColumnNames,
                          const AbstractFile::FILE_COMMENT_MODE fcm,
                          const bool updateSpec) throw (FileException);
      
      /// write the geodesic distance data file
      void writeGeodesicDistanceFile(const QString& name) throw (FileException);
      
      /// read the lat lon data file file
      void readLatLonFile(const QString& name, const bool append,
                          const bool updateSpec) throw (FileException);
      
      /// read the lat lon data file file (only selected columns)
      void readLatLonFile(const QString& name, 
                          const std::vector<int>& columnDestination,
                          const std::vector<QString>& fileBeingReadColumnNames,
                          const AbstractFile::FILE_COMMENT_MODE fcm,
                          const bool updateSpec) throw (FileException);
      
      /// write the LatLon data file
      void writeLatLonFile(const QString& name) throw (FileException);
      
      /// read the deformation field file
      void readDeformationFieldFile(const QString& name, const bool append,
                          const bool updateSpec) throw (FileException);
      
      /// read the deformation field file (only selected columns)
      void readDeformationFieldFile(const QString& name, 
                          const std::vector<int>& columnDestination,
                          const std::vector<QString>& fileBeingReadColumnNames,
                          const AbstractFile::FILE_COMMENT_MODE fcm,
                          const bool updateSpec) throw (FileException);
      
      /// write the deformation field data file
      void writeDeformationFieldFile(const QString& name) throw (FileException);
      
      /// read the metric data file file
      void readMetricFile(const QString& name, const bool append,
                          const bool updateSpec) throw (FileException);
      
      /// read the metric data file file (only selected columns)
      void readMetricFile(const QString& name, 
                          const std::vector<int>& columnDestination,
                          const std::vector<QString>& fileBeingReadColumnNames,
                          const AbstractFile::FILE_COMMENT_MODE fcm,
                          const bool updateSpec) throw (FileException);
      
      /// write the Metric data file
      void writeMetricFile(const QString& name) throw (FileException);
      
      /// read the area color data file file
      void readAreaColorFile(const QString& name, const bool append,
                             const bool updateSpec) throw (FileException);
      
      /// write the NodeColor data file
      void writeAreaColorFile(const QString& name) throw (FileException);
      
      /// read the paint data file file (only selected columns)
      void readPaintFile(const QString& name, 
                         const std::vector<int>& columnDestination,
                         const std::vector<QString>& fileBeingReadColumnNames,
                         const AbstractFile::FILE_COMMENT_MODE fcm,
                         const bool updateSpec) throw (FileException);
      
      /// read the paint data file file
      void readPaintFile(const QString& name, const bool append,
                         const bool updateSpec) throw (FileException);
      
      /// write the Paint data file
      void writePaintFile(const QString& name) throw (FileException);
      
      /// read the study metadata file
      void readStudyMetaDataFile(const QString& name,
                                 const bool append,
                                 const bool updateSpec) throw (FileException);
                  
      /// write the study metadata file
      void writeStudyMetaDataFile(const QString& name) throw (FileException);
      
      /// read the vocabulary file
      void readVocabularyFile(const QString& name,
                              const bool append,
                              const bool updateSpec) throw (FileException);
                              
      /// write the vocabulary file
      void writeVocabularyFile(const QString& name) throw (FileException);
      
      /// read the wustl region file
      void readWustlRegionFile(const QString& name,
                               const bool append,
                               const bool updateSpec) throw (FileException);
                               
      /// write the wustl region file
      void writeWustlRegionFile(const QString& name) throw (FileException);
      
      /// read the palette data file file
      void readPaletteFile(const QString& name, const bool append,
                           const bool updateSpec) throw (FileException);
      
      /// write the Palette data file
      void writePaletteFile(const QString& name) throw (FileException);
      
      /// read the params data file file
      void readParamsFile(const QString& name, const bool append,
                           const bool updateSpec) throw (FileException);
      
      /// write the Params data file
      void writeParamsFile(const QString& name) throw (FileException);
      
      /// read the prob atlas data file file
      void readProbabilisticAtlasFile(const QString& name, const bool append,
                                      const bool updateSpec) throw (FileException);
      
      /// write the ProbabilisticAtlas data file
      void writeProbabilisticAtlasFile(const QString& name) throw (FileException);
      
      /// read the rgb paint data file file
      void readRgbPaintFile(const QString& name, const bool append,
                            const bool updateSpec) throw (FileException);
      
      /// write the RgbPaint data file
      void writeRgbPaintFile(const QString& name) throw (FileException);
      
      /// read the scene data file file
      void readSceneFile(const QString& name, const bool append,
                           const bool updateSpec) throw (FileException);
      
      /// write the scene data file
      void writeSceneFile(const QString& name) throw (FileException);
      
      /// read the section data file file
      void readSectionFile(const QString& name, const bool append,
                           const bool updateSpec) throw (FileException);
      
      /// write the Section data file
      void writeSectionFile(const QString& name) throw (FileException);
      
      /// read the image data file
      void readImageFile(const QString& name, const bool append,
                            const bool updateSpec) throw (FileException);
      
      /// write the image data file
      void writeImageFile(const QString& name,
                             ImageFile* img) throw (FileException);
      
      /// read the vtk model data file
      void readVtkModelFile(const QString& name, const bool append,
                            const bool updateSpec) throw (FileException);
      
      /// write the vtk model data file
      void writeVtkModelFile(const QString& name,
                             VtkModelFile* vmf) throw (FileException);
      
      /// read the surface shape data file file (only selected columns)
      void readSurfaceShapeFile(const QString& name, 
                                const std::vector<int>& columnDestination,
                                const std::vector<QString>& fileBeingReadColumnNames,
                                const AbstractFile::FILE_COMMENT_MODE fcm,
                                const bool updateSpec) throw (FileException);
      
      /// read the surface shape data file file
      void readSurfaceShapeFile(const QString& name, const bool append,
                                const bool updateSpec) throw (FileException);
      
      /// write the SurfaceShape data file
      void writeSurfaceShapeFile(const QString& name) throw (FileException);
      
      /// read the surface vector data file file (only selected columns)
      void readSurfaceVectorFile(const QString& name, 
                                const std::vector<int>& columnDestination,
                                const std::vector<QString>& fileBeingReadColumnNames,
                                const AbstractFile::FILE_COMMENT_MODE fcm,
                                const bool updateSpec) throw (FileException);
      
      /// read the surface vector data file file
      void readSurfaceVectorFile(const QString& name, const bool append,
                                const bool updateSpec) throw (FileException);
      
      /// write the SurfaceVector data file
      void writeSurfaceVectorFile(const QString& name) throw (FileException);
      
      /// read the topography data file file
      void readTopographyFile(const QString& name, const bool append,
                              const bool updateSpec) throw (FileException);
      
      /// write the Topography data file
      void writeTopographyFile(const QString& name) throw (FileException);
      
      /// read the topology data file file
      void readTopologyFile(const QString& name, const TopologyFile::TOPOLOGY_TYPES tt,
                            const bool append,
                            const bool updateSpec) throw (FileException);
      
      /// write theTopology  data file
      void writeTopologyFile(const QString& name,
                             const TopologyFile::TOPOLOGY_TYPES tt,
                             TopologyFile* tf) throw (FileException);
      
      /// read the transformation matrix file
      void readTransformationMatrixFile(const QString& name, const bool append,
                                        const bool updateSpec) throw (FileException);
                                 
      /// write the transformation matrix file
      void writeTransformationMatrixFile(const QString& name) throw (FileException);
      
      /// read the transformation data file
      void readTransformationDataFile(const QString& name, const bool append,
                                        const bool updateSpec) throw (FileException);
                                 
      /// read volume data file
      void readVolumeFile(const QString& name, const VolumeFile::VOLUME_TYPE vt,
                          const bool append, const bool updateSpec) throw (FileException);
                    
      /// write the volume data file
      void writeVolumeFile(const QString& name,
                           const VolumeFile::FILE_READ_WRITE_TYPE writeFileType,
                           const VolumeFile::VOLUME_TYPE volumeType,
                           VolumeFile* vf,
                           const VolumeFile::VOXEL_DATA_TYPE voxelDataTypeToWrite
                                            = VolumeFile::VOXEL_DATA_TYPE_UNKNOWN,
                           const bool zipAfniVolumeFile = false) throw (FileException);
      
      /// Write the multi-volume file data.
      void writeMultiVolumeFile(const QString& name, 
                          const VolumeFile::VOLUME_TYPE volumeType,
                          const QString& comment,
                          std::vector<VolumeFile*>& subVolumes,
                          const VolumeFile::VOXEL_DATA_TYPE voxelDataTypeToWrite
                                            = VolumeFile::VOXEL_DATA_TYPE_UNKNOWN,
                          const bool zipAfniVolumeFile = false) throw (FileException);
                             
      /// reset node attributes (ensures number of attributes == num nodes)
      void resetNodeAttributes();
      
      /// clear the node attributes
      void clearNodeAttributes();

      /// clear all node highlight symbols
      void clearNodeHighlightSymbols();

      /// disconnect the nodes for the specified topology file.  All coordinate files using this
      /// topology will have disconnected nodes moved to the origin.
      void disconnectNodes(TopologyFile* tf, const std::vector<bool>& nodesToDisconnect);
      
      /// disconnect the nodes with the specified paint names in the specified paint column
      void disconnectNodes(TopologyFile* tf, const std::vector<QString>& paintNames,
                           const int paintColumn);
      
      /// disconnect the nodes with the specified paint indices in the specified paint column
      void disconnectNodes(TopologyFile* tf, const std::vector<int>& paintIndices,
                           const int paintColumn);
                            
      /// set the parent for progress dialogs (if this is called with a non-null value, progress
      /// dialogs will be displayed during "longish" operations.
      void setProgressDialogParent(QWidget* parent) { progressDialogParent = parent; }
      
      /// get the progress dialog parent
      QWidget* getProgressDialogParent() { return progressDialogParent; }
      
      /// import MD Plot file
      void importMDPlotFile(const QString& filename,
                            const bool importPointsAsContourCells,
                            const bool importLinesAsContours,
                            const bool appendToExistingContours,
                            const bool appendToExistingContourCells) throw (FileException);
                            
      /// import Neurolucida file
      void importNeurolucidaFile(const QString& filename,
                            const bool importMarkersAsCells,
                            const bool importContours,
                            const bool appendToExistingContours,
                            const bool appendToExistingContourCells) throw (FileException);
                            
      /// import brain voyager file
      void importBrainVoyagerFile(const QString& filename,
                                  const bool importCoordinates,
                                  const bool importTopology,
                                  const bool importColors,
                                  const BrainModelSurface::SURFACE_TYPES surfaceType = BrainModelSurface::SURFACE_TYPE_FIDUCIAL,
                                  const TopologyFile::TOPOLOGY_TYPES topologyType = TopologyFile::TOPOLOGY_TYPE_CLOSED) throw (FileException);

      /// import byu surface file
      void importByuSurfaceFile(const QString& filename,
                                const bool importCoordinates,
                                const bool importTopology,
                                const BrainModelSurface::SURFACE_TYPES surfaceType,
                                const TopologyFile::TOPOLOGY_TYPES topologyType) throw (FileException);
      
      /// Export to a free surfer ascii surface file.
      void exportFreeSurferAsciiSurfaceFile(BrainModelSurface* bms,
                                            const QString& filename) throw (FileException);
      
      /// import a free surfer ascii surface file
      void importFreeSurferSurfaceFile(const QString& filename,
                                       const bool importCoordinates,
                                       const bool importTopology,
                                       const AbstractFile::FILE_FORMAT fileFormat = AbstractFile::FILE_FORMAT_ASCII,
                                       const BrainModelSurface::SURFACE_TYPES surfaceType = BrainModelSurface::SURFACE_TYPE_FIDUCIAL,
                                       const TopologyFile::TOPOLOGY_TYPES topologyType = TopologyFile::TOPOLOGY_TYPE_CLOSED) throw (FileException);
                                            
      /// import stl surface file
      void importStlSurfaceFile(const QString& filename,
                                const bool importCoordinates,
                                const bool importTopology,
                                const BrainModelSurface::SURFACE_TYPES surfaceType,
                                const TopologyFile::TOPOLOGY_TYPES topologyType) throw (FileException);
      
      /// import vtk surface file
      void importVtkSurfaceFile(const QString& filename,
                                const bool importCoordinates,
                                const bool importTopology,
                                const bool importColors,
                                const BrainModelSurface::SURFACE_TYPES surfaceType,
                                const TopologyFile::TOPOLOGY_TYPES topologyType) throw (FileException);
      
      /// import vtk XML surface file
      void importVtkXmlSurfaceFile(const QString& filename,
                                   const bool importCoordinates,
                                   const bool importTopology,
                                   const bool importColors,
                                   const BrainModelSurface::SURFACE_TYPES surfaceType,
                                   const TopologyFile::TOPOLOGY_TYPES topologyType) throw (FileException);
      
      /// Import vtk type files (vtk, stl, byu)
      void importVtkTypeFileHelper(const QString& filename,
                                   vtkPolyData* polyData, 
                                   const bool importCoordinates,
                                   const bool importTopology,
                                   const bool importColors,
                                   const BrainModelSurface::SURFACE_TYPES surfaceType,
                                   const TopologyFile::TOPOLOGY_TYPES topologyType) throw (FileException);
      
      /// Import Analyze volume file
      void importAnalyzeVolumeFile(const QString& filename,
                                   const VolumeFile::VOLUME_TYPE volumeType) throw (FileException);
                                
      /// Export Analyze volume file
      void exportAnalyzeVolumeFile(VolumeFile* vf,
                                   const QString& filename) throw (FileException);
                                
      /// Export MINC volume file
      void exportMincVolumeFile(VolumeFile* vf,
                                const QString& filename) throw (FileException);
                                
      /// Import Raw volume file
      void importRawVolumeFile(const QString& filename,
                               const VolumeFile::VOLUME_TYPE volumeType,
                               const int dimensions[3],
                               const VolumeFile::VOXEL_DATA_TYPE voxelDataType,
                               const bool byteSwap) throw (FileException);
                                
      /// Import MINC volume file
      void importMincVolumeFile(const QString& filename,
                                const VolumeFile::VOLUME_TYPE volumeType) throw (FileException);
                                
      /// Import VTK structured points volume file
      void importVtkStructuredPointsVolumeFile(const QString& filename,
                                const VolumeFile::VOLUME_TYPE volumeType) throw (FileException);
                                
      /// Export VTK structured points volume file
      void exportVtkStructuredPointsVolumeFile(VolumeFile* vf,
                                               const QString& filename) throw (FileException);
                                
      /// export byu surface file
      void exportByuSurfaceFile(const BrainModelSurface* bms,
                                const QString& filename) throw (FileException);
      
      /// export inventor surface file
      void exportInventorSurfaceFile(const BrainModelSurface* bms,
                                     const QString& filename) throw (FileException);
      
      /// export stl surface file
      void exportStlSurfaceFile(const BrainModelSurface* bms,
                                const QString& filename) throw (FileException);
      
      /// export 3dStudio ASE surface file
      void export3DStudioASESurfaceFile(const BrainModelSurface* bms,
                                 const QString& filename) throw (FileException);
      
      /// export vrml surface file
      void exportVrmlSurfaceFile(const BrainModelSurface* bms,
                                 const QString& filename) throw (FileException);
      
      /// export wavefront object surface file
      void exportWavefrontSurfaceFile(const BrainModelSurface* bms,
                                      const QString& filename) throw (FileException);
      
      /// Convert a BrainModelSurface to a vtkPolyData file with option of node colors
      vtkPolyData* convertToVtkPolyData(const BrainModelSurface* bms,
                                        const bool useNodeColors);     
      
      /// export vtk surface file
      void exportVtkSurfaceFile(const BrainModelSurface* bms,
                                const QString& filename,
                                const bool exportColors) throw (FileException);
      
      /// export vtk XML surface file
      void exportVtkXmlSurfaceFile(const BrainModelSurface* bms,
                                   const QString& filename,
                                   const bool exportColors) throw (FileException);
      
      /// request that a brain model be displayed and drawn by the user of this brain set
      void drawBrainModel(const int brainModelIndex, const int currentAlgorithmIteration = -1);
      
      /// request that a brain model be displayed and drawn by the user of this brain set
      void drawBrainModel(const BrainModel* bm, const int currentAlgorithmIteration = -1);
      
      /// See if this iteration is one in which the brain model should be redrawn.
      bool isIterationUpdate(const int currentAlgorithmIteration) const;   
      
      /// Simplify the surface (this also removed all other surfaces and volumes)
      void simplifySurface(const BrainModelSurface* bms,
                           const int maxPolygons) throw (FileException);
      
      /// Initalize things after reading a spec file or initial surface into caret
      void postSpecFileReadInitializations();

      /// get display splash image
      bool getDisplaySplashImage() const { return displaySplashImage; }
      
      /// set display splash image
      void setDisplaySplashImage(const bool b) { displaySplashImage = b; }
      
      /// get the splash image
      QImage* getSplashImage() { return &splashImage; }
      
      /// get the display all nodes flag
      bool getDisplayAllNodes() const { return displayAllNodesFlag; }
      
      /// set the display all nodes flag
      void setDisplayAllNodes(const bool dan) { displayAllNodesFlag = dan; }
      
      /// Set node display flags based upon sections and other criteria.
      void updateNodeDisplayFlags();      
      
      /// apply a scene (set display settings)
      void showScene(const SceneFile::Scene* ss, 
                     const bool checkSpecFlag,
                     QString& errorMessage);
      
      /// apply a scene (set display settings)
      void showScene(const int sceneIndex, 
                     QString& errorMessage);

      /// Get the model for a window from a scene.
      BrainModel* showSceneGetBrainModel(const int sceneIndex,
                                         const int viewingWindowNumberIn,
                                         int geometryOut[4],
                                         int glWidthHeightOut[2],
                                         QString& errorMessageOut);

      /// Get the model for a window from a scene.
      BrainModel* showSceneGetBrainModel(const SceneFile::Scene* scene,
                                         const int viewingWindowNumberIn,
                                         int geometryOut[4],
                                         int glWidthHeightOut[2],
                                         QString& errorMessageOut);
      
      /// create a scene (read display settings)
      void saveScene(SceneFile* sf,
                     const std::vector<SceneFile::SceneClass>& mainWindowSceneClasses,
                     const QString& sceneName, const bool onlyIfSelectedFlag,
                     QString& errorMessageOut);
      
      /// insert after scene (read display settings)
      void insertScene(SceneFile* sf,
                       const int insertAfterIndex,
                       const std::vector<SceneFile::SceneClass>& mainWindowSceneClasses,
                       const QString& sceneName, const bool onlyIfSelectedFlag,
                       QString& errorMessageOut);
      
      /// replace a scene (read display settings)
      void replaceScene(SceneFile* sf,
                        const int sceneIndex,
                        const std::vector<SceneFile::SceneClass>& mainWindowSceneClasses,
                        const QString& sceneName, const bool onlyIfSelectedFlag,
                        QString& errorMessageOut);
      
      /// Save the model for a window from a scene
      void saveSceneForBrainModelWindow(const int viewingWindowNumber,
                                        const int geometry[4],
                                        const int glWidthHeight[2],
                                        const BrainModel* bm,
                                        SceneFile::SceneClass& sceneClass);
       
      /// convert displayed borders into a VTK model.
      void convertDisplayedBordersToVtkModel(const BrainModelSurface* bms);
      
      /// convert displayed cells into a VTK model.
      void convertDisplayedCellsToVtkModel(const BrainModelSurface* bms);
      
      /// convert displayed foci into a VTK model.
      void convertDisplayedFociToVtkModel(const BrainModelSurface* bms);
      
      /// get the displayed model index for a window
      int getDisplayedModelIndexForWindow(const BrainModel::BRAIN_MODEL_VIEW_NUMBER windowNumber) const;
      
      /// set the displayed model index for a window
      void setDisplayedModelIndexForWindow(const BrainModel::BRAIN_MODEL_VIEW_NUMBER windowNumber,
                                           const int modelIndex);
      
      /// update the default file naming prefix
      void updateDefaultFileNamePrefix(); 
           
      /// set when reading coordinate files DO NOT use topology file in coord file header
      void setIgnoreTopologyFileInCoordinateFileHeaderFlag(const bool b) {
         ignoreTopologyFileInCoordinateFileHeaderFlag = b;
      }
      
   public slots:
      /// clear the file
      void clearAreaColorFile();

      /// clear the file
      void clearArealEstimationFile();

      /// clear the file
      void clearBorderColorFile();

      /// clear the file
      void clearCellColorFile();

      /// clear the file
      void clearCocomacConnectivityFile();

      /// clear the file
      void clearContourCellColorFile();

      /// clear the file
      void clearContourCellFile();

      /// clear the file
      void clearContourFile(const bool deleteBrainModelContoursFlag = true);

      /// clear the file
      void clearCutsFile();

      /// clear the file
      void clearDeformationFieldFile();

      /// clear the file
      void clearDeformationMapFile();

      /// clear the file
      void clearFociColorFile();

      /// clear the file
      void clearGeodesicDistanceFile();

      /// clear the file
      void clearLatLonFile();

      /// clear the file
      void clearMetricFile();

      /// clear the file
      void clearPaintFile();

      /// clear the file
      void clearPaletteFile();

      /// clear the file
      void clearParamsFile();

      /// clear the file
      void clearProbabilisticAtlasFile();

      /// clear the file
      void clearRgbPaintFile();

      /// clear the file
      void clearSceneFile();

      /// clear the file
      void clearSectionFile();

      /// clear the study metadata file
      void clearStudyMetaDataFile();
      
      /// clear the file
      void clearSurfaceShapeFile();

      /// clear the file
      void clearSurfaceVectorFile();
      
      /// clear the file
      void clearTopographyFile();

      /// clear the file
      void clearTransformationMatrixFile();

      /// clear the vocabulary file
      void clearVocabularyFile();
      
      /// clear the file
      void clearWustlRegionFile();
   
   signals:
      /// signal that requests a brain model be displayed and drawn
      void signalDisplayBrainModel(int brainModelIndex);
      
      /// signal that the brain set has changed
      void signalBrainSetChanged();
      
      /// signal that graphics windows should be redrawn
      void signalGraphicsUpdate(BrainSet* bs);
      
   protected slots:
      // called when cross timer timesout
      void slotDisplayCrossTimerTimeout();
      
   private:
      /// index of model in each window
      int displayedModelIndices[BrainModel::NUMBER_OF_BRAIN_MODEL_VIEW_WINDOWS];
      
      /// reading a spec file flag (do not update the spec file)
      bool readingSpecFileFlag;
      
      /// the web caret flag
      bool webCaretFlag;
      
      /// splash image
      QImage splashImage;
    
      /// display splash image
      bool displaySplashImage;
       
      /// parent for progress dialogs
      QWidget* progressDialogParent;
      
      /// node 
      std::vector<BrainSetNodeAttribute> nodeAttributes;
      
      /// the spec file that keeps track of loaded files for this brain set
      SpecFile loadedFilesSpecFile;
      
      /// name of the spec file
      QString specFileName;
      
      /// time of spec file loading
      QDateTime specFileTimeOfLoading;
      
      /// a transformation matrix that is applied to "fiducial" files while reading a spec file
      TransformationMatrix specDataFileTransformationMatrix;
      
      /// storage for all surfaces
      std::vector<BrainModel*> brainModels;
      
      /// the active fiducial surface
      BrainModelSurface* activeFiducialSurface;
      
      /// the identification object
      BrainModelIdentification* brainModelIdentification;
      
      /// node coloring class
      BrainModelSurfaceNodeColoring* nodeColoring;
      
      /// voxel coloring class
      BrainModelVolumeVoxelColoring* voxelColoring;
      
      /// volume region of interest controller
      BrainModelVolumeRegionOfInterest* brainModelVolumeRegionOfInterest;
      
      /// areal estimation file
      ArealEstimationFile* arealEstimationFile;
      
      /// cocomac file
      CocomacConnectivityFile* cocomacFile;
      
      /// contour cell file
      ContourCellFile* contourCellFile;
      
      /// contour cell color file
      ContourCellColorFile* contourCellColorFile;
      
      /// cuts file (cuts are same as borders)
      CutsFile* cutsFile;

      /// name of deformation map file
      QString deformationMapFileName;
      
      /// deformation field file
      DeformationFieldFile* deformationFieldFile;
      
      /// lat/lon files
      LatLonFile* latLonFile;
      
      /// scene file
      SceneFile* sceneFile;
      
      /// section file
      SectionFile* sectionFile;
      
      /// metric file
      MetricFile* metricFile;
      
      /// area color file
      AreaColorFile* areaColorFile;
      
      //// probabilistic atlas file
      ProbabilisticAtlasFile* probabilisticAtlasSurfaceFile;
      
      /// paint file
      PaintFile* paintFile;
      
      /// Palette File
      PaletteFile* paletteFile;
      
      /// study metadata file
      StudyMetaDataFile* studyMetaDataFile;
      
      /// vocabulary file
      VocabularyFile* vocabularyFile;
      
      /// wustl region file
      WustlRegionFile* wustlRegionFile;
      
      /// geodesic distance file
      GeodesicDistanceFile* geodesicDistanceFile;
      
      /// Params File
      ParamsFile* paramsFile;
      
      /// RGB Paint file
      RgbPaintFile* rgbPaintFile;
      
      /// Surface Shape file
      SurfaceShapeFile* surfaceShapeFile;
      
      /// Surface vector file
      SurfaceVectorFile* surfaceVectorFile;
      
      /// Topography File
      TopographyFile* topographyFile;
      
      /// topology files
      std::vector<TopologyFile*> topologyFiles;
      
      /// active closed topology file
      TopologyFile* topologyClosed;
      
      /// active open topology file
      TopologyFile* topologyOpen;
      
      /// active cut topology file
      TopologyFile* topologyCut;
      
      /// active lobar cut topology file
      TopologyFile* topologyLobarCut;
      
      /// active unknown topology file
      TopologyFile* topologyUnknown;
      
      /// functional volume files
      std::vector<VolumeFile*> volumeFunctionalFiles;
      
      /// paint volume files
      std::vector<VolumeFile*> volumePaintFiles;
      
      /// prob atlas volume files
      std::vector<VolumeFile*> volumeProbAtlasFiles;
      
      /// rgb paint volume files
      std::vector<VolumeFile*> volumeRgbFiles;
      
      /// segmentation volume files
      std::vector<VolumeFile*> volumeSegmentationFiles;
      
      /// anatomy volume files
      std::vector<VolumeFile*> volumeAnatomyFiles;
      
      /// vector volume files
      std::vector<VolumeFile*> volumeVectorFiles;
      
      /// transformation matrix file
      TransformationMatrixFile* transformationMatrixFile;
      
      /// the species
      QString species;
      
      /// the subject
      QString subject;
      
      /// the stereotaxic space
      QString stereotaxicSpace;
      
      /// hemisphere
      Structure structure;
      
      /// preferences file
      static PreferencesFile preferencesFile;
      
      /// preferences file's name
      static QString preferencesFileName;
      
      /// initialize static stuff flag
      static bool staticStuffInitialized;
      
      /// path of Caret's home directory
      static QString caretHomeDirectory;
      
      /// Areal Estimation display settings
      DisplaySettingsArealEstimation* displaySettingsArealEstimation;
      
      /// Border display settings
      DisplaySettingsBorders* displaySettingsBorders;
      
      /// Cell display settings
      DisplaySettingsCells* displaySettingsCells;
      
      /// CoCoMac display settings
      DisplaySettingsCoCoMac* displaySettingsCoCoMac;
      
      /// Contour display settings
      DisplaySettingsContours* displaySettingsContours;
      
      /// Cuts display settings
      DisplaySettingsCuts* displaySettingsCuts;
      
      /// Foci display settings
      DisplaySettingsFoci* displaySettingsFoci;
      
      /// node display settings
      DisplaySettingsSurface* displaySettingsSurface;
      
      /// section display settings
      DisplaySettingsSection* displaySettingsSection;
      
      /// deformation field display settings
      DisplaySettingsDeformationField* displaySettingsDeformationField;
      
      /// images display settings
      DisplaySettingsImages* displaySettingsImages;
      
      /// Metric display settings
      DisplaySettingsMetric* displaySettingsMetric;
      
      /// Models display settings
      DisplaySettingsModels* displaySettingsModels;
      
      /// Paint settings
      DisplaySettingsPaint* displaySettingsPaint;
      
      /// Probabilistic Atlas settings for surface
      DisplaySettingsProbabilisticAtlas* displaySettingsProbabilisticAtlasSurface;
      
      /// RGB Paint display settings
      DisplaySettingsRgbPaint* displaySettingsRgbPaint;
      
      /// Scene display settings
      DisplaySettingsScene* displaySettingsScene;
      
      /// study metadata display settings
      DisplaySettingsStudyMetaData* displaySettingsStudyMetaData;
      
      /// Surface shape display settings
      DisplaySettingsSurfaceShape* displaySettingsSurfaceShape;
      
      /// Surface vector display settings
      DisplaySettingsSurfaceVectors* displaySettingsSurfaceVectors;
      
      /// Topography display settings
      DisplaySettingsTopography* displaySettingsTopography;
      
      /// Volume display settings
      DisplaySettingsVolume* displaySettingsVolume;
      
      /// Wustl Region Display Settings
      DisplaySettingsWustlRegion* displaySettingsWustlRegion;
      
      /// Geodesic distance file Display Settings
      DisplaySettingsGeodesicDistance* displaySettingsGeodesicDistance;
      
      /// Probabilistic Atlas settings for volume
      DisplaySettingsProbabilisticAtlas* displaySettingsProbabilisticAtlasVolume;
      
      /// Border color file
      BorderColorFile* borderColorFile;
      
      /// the border set
      BrainModelBorderSet* brainModelBorderSet;
      
      /// Cell color file
      CellColorFile* cellColorFile;
      
      /// Cell Projection File
      CellProjectionFile* cellProjectionFile;
      
      /// volume cell file
      CellFile* volumeCellFile;
      
      /// foci color file
      FociColorFile* fociColorFile;
      
      /// foci projection file
      FociProjectionFile* fociProjectionFile;
      
      /// volume foci file
      FociFile* volumeFociFile;
      
      /// the image files
      std::vector<ImageFile*> imageFiles;
      
      /// the vtk model files
      std::vector<VtkModelFile*> vtkModelFiles;
      
      /// the transformation matrix data files
      std::vector<AbstractFile*> transformationDataFiles;
      
      /// number of nodes message when reading files
      QString numNodesMessage;

      /// name of cerebral hull file name
      QString cerebralHullFileName;
      
      /// nodes have been classified flag
      bool nodesHaveBeenClassified;
      
      /// display all nodes flag
      bool displayAllNodesFlag;
      
      /// primary brain set flag (allows setting of AbstractFile default names)
      bool primaryBrainSetFlag;
      
      /// when reading coordinate files DO NOT use topology file in coord file header
      bool ignoreTopologyFileInCoordinateFileHeaderFlag;
      
      /// display a cross for this node
      int displayCrossForNode;
      
      /// surface on which no cross should be shown
      BrainModelSurface* displayNoCrossForSurface;
      
      /// display cross timer
      QTimer* displayCrossTimer;
      
      /// update displayed model indices
      void updateDisplayedModelIndices();
      
      /// save/replace scene helper
      void saveReplaceSceneHelper(SceneFile::Scene& scene,
                                  const std::vector<SceneFile::SceneClass>& mainWindowSceneClasses,
                                  const bool onlyIfSelectedFlag,
                                  QString& errorMessageOut);
                                  
      /// check node attribute columns for columns with same name
      void checkNodeAttributeFilesForDuplicateColumnNames(QString& errorMessageOut);
      
      /// check for duplicate column names in a mode attribute file
      void nodeAttribteDuplicateNamesHelper(const QString& fileTypeName,
                                            NodeAttributeFile* naf,
                                            QString& errorMessageOut) const;

      /// check for duplicate column names in a mode attribute file
      void niftiNodeDataFileDuplicateNamesHelper(const QString& fileTypeName,
                                            GiftiNodeDataFile* naf,
                                            QString& errorMessageOut) const;

                     
      /// read an image (returns true if read successfully)
      bool readImage(const QString& filename, const QString& format, QImage& image);
      
      /// construct the brain set
      void constructBrainSet();

      /// create a brain model surface and volume
      void createBrainModelSurfaceAndVolume();
      
      /// delete surface that are of the specified type
      void deleteSurfacesOfType(const BrainModelSurface::SURFACE_TYPES st);
      
      /// Read in border files
      bool readBorderFiles(const SpecFile::Entry& borderFilesToRead,
                           const BrainModelSurface::SURFACE_TYPES bt,
                           std::vector<QString>& errorMessages,
                           int& progressFileCounter,
                           QProgressDialog* progressDialog);
      
      /// Read in coordinate file group
      bool readCoordinateFileGroup(const SpecFile::Entry& coordFile,
                           const BrainModelSurface::SURFACE_TYPES surfaceType,
                           std::vector<QString>& errorMessages,
                           int& progressFileCounter,
                           QProgressDialog* progressDialog);
      
      /// Read in surface file group
      bool readSurfaceFileGroup(const SpecFile::Entry& surfaceFile,
                           const BrainModelSurface::SURFACE_TYPES surfaceType,
                           std::vector<QString>& errorMessages,
                           int& progressFileCounter,
                           QProgressDialog* progressDialog);
      
      /// Update the file reading progress dialog
      bool updateFileReadProgressDialog(const QString& filename,
                                        int& progressFileCounter,
                                        QProgressDialog* progressDialog);
                                          
      /// set the selected topology files
      void setSelectedTopologyFiles();
      
      /// if section file empty, look for sections in topology files
      void getSectionsFromTopology();
      
      /// Update all display settings.
      void updateAllDisplaySettings();
};

// initialize static members
#ifdef __BRAIN_SET_MAIN__
PreferencesFile BrainSet::preferencesFile;
bool BrainSet::staticStuffInitialized = false;
QString BrainSet::preferencesFileName = "";
QString BrainSet::caretHomeDirectory  = "";
#endif // __BRAIN_SET_MAIN__

#endif // __BRAIN_SET_H__

