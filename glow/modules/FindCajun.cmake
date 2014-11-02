# - Try to find the c++ JSON library CAJUN
# Once done this will define
# Cajun_FOUND - System has Cajun
# Cajun_INCLUDE_DIR - The CAJUN include directory

# Include dir
find_path(Cajun_INCLUDE_DIR
  NAMES json/elements.h
  PATHS /usr/local/include /usr/include/cajun
)

set(Cajun_INCLUDE_DIRS ${Cajun_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(Cajun DEFAULT_MSG Cajun_INCLUDE_DIR)

mark_as_advanced(Cajun_INCLUDE_DIR)