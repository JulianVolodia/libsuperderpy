# Try to find allegro 5
#
#  ALLEGRO5_COLOR_FOUND - system has allegro5
#  ALLEGRO5_COLOR_INCLUDE_DIR - the allrgo5 include directory
#  ALLEGRO5_COLOR_LIBRARIES - Link these to use allegro5
#

FIND_PATH(ALLEGRO5_COLOR_INCLUDE_DIR allegro5/allegro_color.h)

SET(ALLEGRO5_COLOR_NAMES ${ALLEGRO5_COLOR_NAMES} allegro_color allegro_color_static liballegro_color liballegro_color_static AllegroColor-5.2 allegro_color-debug)
FIND_LIBRARY(ALLEGRO5_COLOR_LIBRARY NAMES ${ALLEGRO5_COLOR_NAMES} )

# handle the QUIETLY and REQUIRED arguments and set ALLEGRO5_COLOR_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ALLEGRO5_COLOR DEFAULT_MSG ALLEGRO5_COLOR_LIBRARY ALLEGRO5_COLOR_INCLUDE_DIR)

IF(ALLEGRO5_COLOR_FOUND)
  SET(ALLEGRO5_COLOR_LIBRARIES ${ALLEGRO5_COLOR_LIBRARY})
  SET(ALLEGRO5_LIBS ${ALLEGRO5_LIBS} ${ALLEGRO5_COLOR_LIBRARIES})
ENDIF(ALLEGRO5_COLOR_FOUND)

MARK_AS_ADVANCED(ALLEGRO5_COLOR_LIBRARY ALLEGRO5_COLOR_INCLUDE_DIR )
