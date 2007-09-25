######################################################################
# Automatically generated by qmake (1.04a) Tue Jan 14 11:58:13 2003
######################################################################

TEMPLATE = app
CONFIG   += console
INCLUDEPATH += .
#QT	-= gui

include(../caret_qmake_include.pro)

win32 {
   debug {
      LIBS += \
         ..\caret_brain_set\debug\libCaretBrainSet.a \
         ..\caret_vtk4_classes\debug\libCaretVtk4Classes.a \
         ..\caret_files\debug\libCaretFiles.a \
         ..\caret_uniformize\debug\libCaretUniformize.a \
         ..\caret_statistics\debug\libCaretStatistics.a \
         ..\caret_common\debug\libCaretCommon.a 
   }

   release {
      LIBS += \
         ..\caret_brain_set\release\libCaretBrainSet.a \
         ..\caret_vtk4_classes\release\libCaretVtk4Classes.a \
         ..\caret_files\release\libCaretFiles.a \
         ..\caret_uniformize\release\libCaretUniformize.a \
         ..\caret_statistics\release\libCaretStatistics.a \
         ..\caret_common\release\libCaretCommon.a 
   }

   contains ( DEFINES, HAVE_ITK ) {
      LIBS += $$ITK_LIBS
   }
   
   LIBS += $$VTK_LIBS
   
   contains( DEFINES, HAVE_MINC ) {
      LIBS += $$NETCDF_LIBS
   }
}


win32::debug {
   CONFIG += console
}

macx {
   CONFIG -= app_bundle
   LIBS += -L../caret_brain_set -lCaretBrainSet \
           -L../caret_vtk4_classes -lCaretVtk4Classes \
	   -L../caret_files -lCaretFiles \
           -L../caret_uniformize -lCaretUniformize \
           -L../caret_statistics -lCaretStatistics \
	   -L../caret_common -lCaretCommon 

   contains ( DEFINES, HAVE_ITK ) {
      LIBS += $$ITK_LIBS
   }
   
   LIBS += $$VTK_LIBS
   

   contains( DEFINES, HAVE_MINC ) {
      LIBS += $$NETCDF_LIBS
   }
}

unix:!macx {
   LIBS += -L../caret_brain_set -lCaretBrainSet \
	   -L../caret_files -lCaretFiles \
           -L../caret_uniformize -lCaretUniformize \
           -L../caret_statistics -lCaretStatistics \
	   -L../caret_common -lCaretCommon 

   contains ( DEFINES, HAVE_ITK ) {
      LIBS += $$ITK_LIBS
   }
   
   LIBS += $$VTK_LIBS
   
   contains( DEFINES, HAVE_MINC ) {
      LIBS += $$NETCDF_LIBS
   }

   # FreeBSD 5.3 uses vtk 4.3, which does not have separate libraries
   # for jpeg, png, and expat.
   # FreeBSD also does not have libdl, but integrates its functions
   # into libc.
   !exists( /etc/rc.conf ) { 
      LIBS += -ldl 
   }       
           
   # for SGI systems
   exists( /usr/lib/libgen.so ) {
      LIBS       += -lgen -LD_LAYOUT:lgot_buffer=500
   }
}

# Input
#HEADERS += 

SOURCES += main.cxx