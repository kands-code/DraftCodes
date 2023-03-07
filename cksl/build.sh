currPath=$(pwd)

# up to one argument
if [[ "$#" == "1" ]]; then
  if [[ "$1" == "clean" ]]; then
    echo "Clean the build"

    if [[ -d "$currPath/.cache" ]]; then
      echo "Remove build cache"
      rm -r "$currPath/.cache"
    fi

    if [[ -d "$currPath/buildDir" ]]; then
      echo "Remove build directory"
      rm -r "$currPath/buildDir"
    fi
  else
    echo "Wrong argument!"
    echo "Only 'clean' or no argument is accepted!"
    echo "You typed $1"
    exit 1
  fi
elif [[ "$#" == "0" ]]; then
  echo "Start build project"
  if [[ -d "$currPath/buildDir" ]]; then
    cmake --build "$currPath/buildDir"
    echo "Build finished!"
  else
    echo "Try to generate default config"
    if [[ -f "$currPath/configure.sh" ]]; then
      source "$currPath/configure.sh" linux D
      echo "Try build on default config"
      source "$currPath/build.sh"
    fi
  fi
else
  echo "Wrong argument!"
  echo "Only 'clean' or no argument is accepted!"
  echo "You typed $*"
  exit 1
fi
