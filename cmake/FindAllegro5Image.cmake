# Try to find allegro 5
#
#  ALLEGRO5_IMAGE_FOUND - system has allegro5
#  ALLEGRO5_IMAGE_INCLUDE_DIR - the allrgo5 include directory
#  ALLEGRO5_IMAGE_LIBRARIES - Link these to use allegro5
#

FIND_PATH(ALLEGRO5_IMAGE_INCLUDE_DIR allegro5/allegro_image.h)

SET(ALLEGRO5_IMAGE_NAMES ${ALLEGRO5_IMAGE_NAMES} allegro_image allegro_image_static liballegro_image liballegro_image_static AllegroImage-5.2 allegro_image-debug)
FIND_LIBRARY(ALLEGRO5_IMAGE_LIBRARY NAMES ${ALLEGRO5_IMAGE_NAMES} )

# handle the QUIETLY and REQUIRED arguments and set ALLEGRO5_IMAGE_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ALLEGRO5_IMAGE DEFAULT_MSG ALLEGRO5_IMAGE_LIBRARY ALLEGRO5_IMAGE_INCLUDE_DIR)

IF(ALLEGRO5_IMAGE_FOUND)
  SET(ALLEGRO5_IMAGE_LIBRARIES ${ALLEGRO5_IMAGE_LIBRARY})
ENDIF(ALLEGRO5_IMAGE_FOUND)

MARK_AS_ADVANCED(ALLEGRO5_IMAGE_LIBRARY ALLEGRO5_IMAGE_INCLUDE_DIR )
