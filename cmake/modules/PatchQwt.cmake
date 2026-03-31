# PatchQwt.cmake - Cross-platform replacement for the sed-based Qwt patch step.
# Invoked by BuildQwt.cmake via: cmake -DSOURCE_DIR=... -DINSTALL_DIR=... -P PatchQwt.cmake

cmake_minimum_required(VERSION 3.16)

file(TO_CMAKE_PATH "${INSTALL_DIR}" INSTALL_DIR)

# --- qwtconfig.pri: disable DLL build, extras, and set install prefix ---
file(READ "${SOURCE_DIR}/qwtconfig.pri" content)

foreach(feature
    QwtDll QwtPolar QwtWidgets QwtSvg QwtOpenGL
    QwtDesigner QwtDesignerSelfContained
    QwtExamples QwtPlayground QwtTests)
  string(REGEX REPLACE
    "(QWT_CONFIG[ \t]*\\+=[ \t]*${feature})"
    "#\\1"
    content "${content}")
endforeach()

string(REGEX REPLACE
  "QWT_INSTALL_PREFIX[ \t]*=[ \t]*[^\n]*"
  "QWT_INSTALL_PREFIX = ${INSTALL_DIR}"
  content "${content}")

file(WRITE "${SOURCE_DIR}/qwtconfig.pri" "${content}")

# --- qwtbuild.pri: release-only, no build_all ---
file(READ "${SOURCE_DIR}/qwtbuild.pri" content)

string(REGEX REPLACE
  "(CONFIG[ \t]*\\+=[ \t]*)debug_and_release"
  "\\1release"
  content "${content}")

string(REGEX REPLACE
  "(CONFIG[ \t]*\\+=[ \t]*build_all)"
  "#\\1"
  content "${content}")

file(WRITE "${SOURCE_DIR}/qwtbuild.pri" "${content}")
