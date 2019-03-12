# Any subsequent(*) commands which fail will cause the shell script to exit immediately
set -e

# assign common variables
STASH_PROJECT=bcds_shared
PACKAGENAME=RefContexts

# assign plattform specific variables
if [ "$2" == "efm32" ]; then
    CONTEXT_PATH=$WORKSPACE/$STASH_PROJECT/$PACKAGENAME/efm32
elif [ "$2" == "stm32" ]; then
    CONTEXT_PATH=$WORKSPACE/$STASH_PROJECT/$PACKAGENAME/stm32
elif [ "$2" == "ti_cc26xx" ]; then
    CONTEXT_PATH=$WORKSPACE/$STASH_PROJECT/$PACKAGENAME/ti_cc26xx 
else
    echo "$0: "
    echo "$0: Assignment of TARGET_PLATFORM failed."
    echo "$0:  TARGET parameter must be one of (stm32, efm32, ti_cc26xx), you passed '$2'"
    echo "$0: "
    echo "$0: "
    exit 1
fi

export BCDS_PACKAGE_HOME=$WORKSPACE/$STASH_PROJECT/$PACKAGENAME
cd $WORKSPACE/$STASH_PROJECT/$PACKAGENAME
git clean -ffdx
git submodule init
git submodule sync
git submodule update --init --recursive
git submodule foreach --recursive git reset --hard
git submodule foreach --recursive git clean -ffdx
export BCDS_WORKSPACE_PATH=$WORKSPACE &&
$PYTHON_34/python -u -mspider sync -d Platform.dep -s $SHARED_COMMON_GIT_DIR


echo "$0: "
echo "$0: Start jenkins build with following parameter:"
echo "$0: TARGET PLATFOM: $2"
echo "$0: "
{
    echo "Doxygen generation for all BCDS Shared Packages" &&
    cd $BCDS_PACKAGE_HOME/doxygen &&
    export BCDS_SHARED_PATH=$WORKSPACE/$STASH_PROJECT
    $BCDS_DEVELOPMENT_TOOLS/doxygen/v1.8.8_32Bit/doxygen.exe > doxygen.log
} || {
        echo "$0: ### Doxygen generation failure" &&
        echo "BUILD FAILURE: failure while building the Doxygen target" &&
        exit 1
    }