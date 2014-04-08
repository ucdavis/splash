#!/bin/sh

#
# This script copies the mac icon into
# into the Mac App's Resources directory.
#

exeName=$1
iconName=$2

frameworksDirName=${buildType}${exeName}.app/Contents/Frameworks

for buildType in Debug/ Release/ RelWithDebInfo/ MinRelSize/ ""
do
   echo "BUILD TYPE ${buildType}"
   appName=${buildType}${exeName}.app/Contents/MacOS/${exeName}
   echo "App ${appName}"

   if [ -f ${appName} ] ; then
      echo "App ${appName} EXISTS!"

      if [ ! -d ${frameworksDirName} ] ; then
         echo "Frameworks do not exist, running macdeployqt"
         macdeployqt ${buildType}${exeName}.app
      fi
   fi
done


