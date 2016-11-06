# Try to find allegro 5
#
#  ALLEGRO5_PRIMITIVES_FOUND - system has allegro5
#  ALLEGRO5_PRIMITIVES_INCLUDE_DIR - the allrgo5 include directory
#  ALLEGRO5_PRIMITIVES_LIBRARIES - Link these to use allegro5
#

FIND_PATH(ALLEGRO5_PRIMITIVES_INCLUDE_DIR allegro5/allegro_primitives.h)

SET(ALLEGRO5_PRIMITIVES_NAMES ${ALLEGRO5_PRIMITIVES_NAMES} allegro_primitives allegro_primitives_static liballegro_primitives liballegro_primitives_static AllegroPrimitives-5.2 allegro_primitives-debug)
FIND_LIBRARY(ALLEGRO5_PRIMITIVES_LIBRARY NAMES ${ALLEGRO5_PRIMITIVES_NAMES} )

# handle the QUIETLY and REQUIRED arguments and set ALLEGRO5_PRIMITIVES_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ALLEGRO5_PRIMITIVES DEFAULT_MSG ALLEGRO5_PRIMITIVES_LIBRARY ALLEGRO5_PRIMITIVES_INCLUDE_DIR)

IF(ALLEGRO5_PRIMITIVES_FOUND)
  SET(ALLEGRO5_PRIMITIVES_LIBRARIES ${ALLEGRO5_PRIMITIVES_LIBRARY})
ENDIF(ALLEGRO5_PRIMITIVES_FOUND)

MARK_AS_ADVANCED(ALLEGRO5_PRIMITIVES_LIBRARY ALLEGRO5_PRIMITIVES_INCLUDE_DIR )
