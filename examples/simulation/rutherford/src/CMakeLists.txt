 ################################################################################
 #    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    #
 #                                                                              #
 #              This software is distributed under the terms of the             # 
 #         GNU Lesser General Public Licence version 3 (LGPL) version 3,        #  
 #                  copied verbatim in the file "LICENSE"                       #
 ################################################################################
# Create a library called "libFairRutherford" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES}
  ${CMAKE_SOURCE_DIR}/examples/simulation/rutherford/src
  ${CMAKE_SOURCE_DIR}/examples/common/mcstack
)

Include_Directories(${INCLUDE_DIRECTORIES})
Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

Set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
) 

Link_Directories(${LINK_DIRECTORIES})

Set(SRCS
  FairRutherford.cxx
  FairRutherfordContFact.cxx
  FairRutherfordGeo.cxx
  FairRutherfordGeoPar.cxx
  FairRutherfordPoint.cxx
)


Set(HEADERS )
Set(LINKDEF FairRutherfordLinkDef.h)
Set(LIBRARY_NAME FairRutherford)
Set(DEPENDENCIES Base MCStack GeoBase ParBase VMC Physics Core)

GENERATE_LIBRARY()
