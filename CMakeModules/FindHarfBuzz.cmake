find_library(HARFBUZZ_LIBRARY NAMES harfbuzz)
find_path(HARFBUZZ_INCLUDE_DIR NAMES hb.h PATH_SUFFIXES harfbuzz)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(HarfBuzz DEFAULT_MSG HARFBUZZ_LIBRARY HARFBUZZ_INCLUDE_DIR)

mark_as_advanced(HARFBUZZ_LIBRARY HARFBUZZ_INCLUDE_DIR)
