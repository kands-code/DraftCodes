# must have two arguments
if [[ "$#" != "2" ]]; then
  echo "Wrong argument number!"
  echo "Need 2 arguments, passed $# arguments"
  echo "Usage:"
  echo "  ./configure.sh [arch] [type]"
  echo "    arch: 'win' or 'linux'"
  echo "    type: 'D' for debug, 'R' for release"
  exit 1
else
  echo "Start configure project"
fi

currPath=$(pwd)
buildArch=$1
buildType=$2

case "$buildArch" in
win)
  buildGen="MinGW Makefiles"
  ;;
linux)
  buildGen="Unix Makefiles"
  ;;
*)
  echo "Wrong build arch!"
  echo "Only 'win' or 'linux' supported!"
  echo "You typed $buildArch"
  exit 1
  ;;
esac

case "$buildType" in
D*)
  buildType="Debug"
  ;;
R*)
  buildType="Release"
  ;;
*)
  echo "Wrong build type!"
  echo "Only 'R' or 'D' supported!"
  echo "You typed $buildType"
  exit 1
  ;;
esac

if [[ -d "$currPath/src/" ]] && [[ -f "$currPath/src/CMakeLists.txt" ]]; then
  echo "Generate build resources"
  cmake -B "$currPath/buildDir" -S "$currPath/src/" \
    -DCMAKE_BUILD_TYPE="$buildType" \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE \
    -Wdev -Wdeprecated \
    -G "$buildGen"
else
  echo "Not a CMake project!"
  exit 1
fi

if [[ -f "$currPath/buildDir/compile_commands.json" ]]; then
  echo "Copy compile_commands.json to project root"
  mv "$currPath/buildDir/compile_commands.json" "$currPath/"
else
  echo "Configure failed!"
  exit 1
fi

echo "Configure finished!"
