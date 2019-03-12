# Any subsequent(*) commands which fail will cause the shell script to exit immediately
set -e

if [ "$#" -lt 1 ]; then
    {
        echo "Missing parameter."
        echo "Usage: $0 <LEVEL>"
        echo "  LEVEL   build level (branch, staging, pullrequest, master, <empty>)"
    }  >&2
    exit 1
fi

function print_block {
echo " "
echo "$0: ###"
echo "$0: ### $1 $2"
echo "$0: ###"
echo " "
}

function print_start_block {
print_block "$1" "START"
}

function print_done_block {
print_block "$1" "DONE"
}

# setting the build level
level=0
# reading the input parameter
case "$1" in
    "branch")           level=1;;
    "staging")          level=2;;
    "pullrequest")      level=3;;
    "master")           level=4;;
esac

# assign common variables
STASH_PROJECT=bcds_shared
PACKAGENAME=Utils
cd ../..
WORKSPACE="$PWD"
CONTEXT=RefContexts
PACKAGE_HOME=$WORKSPACE/$STASH_PROJECT/$PACKAGENAME


# assign plattform specific variables
if [ "$2" == "efm32" ]; then
    CONTEXT_PATH=$WORKSPACE/$STASH_PROJECT/$CONTEXT/efm32
	SPIDER_DEP_FILE=Package.dep
	COM_PORT=$XDK_COM_PORT
elif [ "$2" == "stm32" ]; then
    CONTEXT_PATH=$WORKSPACE/$STASH_PROJECT/$CONTEXT/stm32
	SPIDER_DEP_FILE=Package.dep
	COM_PORT=$BSE_COM_PORT
elif [ "$2" == "ti_cc26xx" ]; then
	CONTEXT_PATH=$WORKSPACE/$STASH_PROJECT/$CONTEXT/ti_cc26xx
	SPIDER_DEP_FILE=Package.dep
else
	echo "$0: "
	echo "$0: Assignment of TARGET_PLATFORM failed."
	echo "$0:  TARGET parameter must be one of (stm32, efm32, ti_cc26xx), you passed '$2'"
	echo "$0: "
	echo "$0: "
	exit 1
fi

COMMON_MAKEFILE=$CONTEXT_PATH/common.mk
export TEST_APP_PATH=$CONTEXT_PATH/TestApp

echo "$0: "
echo "$0: Start $STASH_PROJECT build with following parameters:"
echo "$0:   STASH_PROJECT:      $STASH_PROJECT "
echo "$0:   PACKAGENAME:        $PACKAGENAME "
echo "$0:   WORKSPACE:      $WORKSPACE "
echo "$0: "

{
    print_start_block "Getting context with spider" &&

    export BCDS_WORKSPACE_PATH=$WORKSPACE &&
	$PYTHON_34/python.exe -u -mspider sync -d $WORKSPACE/$STASH_PROJECT/$PACKAGENAME/$CONTEXT.dep -s $SHARED_COMMON_GIT_DIR &&

    print_done_block "Getting context with spider"
} || {
    echo "$0: ### Getting context with spider failure" &&
    echo "BUILD FAILURE: failure while checking out context" &&
    exit 1
}

# getting package dependencies

{
    print_start_block "Getting dependencies with spider" &&
    # cloning dependencies
    export BCDS_WORKSPACE_PATH=$WORKSPACE &&
	# global build
	$PYTHON_34/python.exe -u -mspider sync -d $WORKSPACE/$STASH_PROJECT/RefContexts/platform.dep -s $SHARED_COMMON_GIT_DIR -e $PACKAGENAME &&
    
    print_done_block "Getting dependencies with spider"
} || {
    echo "$0: ### Getting dependencies with spider failure" &&
    echo "BUILD FAILURE: failure while checking out dependencies" &&
    exit 1
}

{
    print_start_block "Debug build" &&

    cd $WORKSPACE/$STASH_PROJECT/$PACKAGENAME &&

    mingw32-make.exe clean BCDS_COMMON_MAKEFILE=$COMMON_MAKEFILE &&
    mingw32-make.exe debug BCDS_COMMON_MAKEFILE=$COMMON_MAKEFILE &&

    print_done_block "Debug build"
} || {
    echo "$0: ### Debug build failure" &&
    echo "BUILD FAILURE: failure while building the debug target" &&
    exit 1
}

{
    print_start_block "Release build" &&

    cd $WORKSPACE/$STASH_PROJECT/$PACKAGENAME &&

    mingw32-make.exe clean BCDS_COMMON_MAKEFILE=$COMMON_MAKEFILE &&
    mingw32-make.exe release BCDS_COMMON_MAKEFILE=$COMMON_MAKEFILE &&

    print_done_block "Release build"
} || {
    echo "$0: ### Release build failure" &&
    echo "BUILD FAILURE: failure while building the release target" &&
    exit 1
}

{
    print_start_block "GTest/LCOV build" &&

    cd $WORKSPACE/$STASH_PROJECT/$PACKAGENAME &&

    mingw32-make.exe gtest_clean BCDS_COMMON_MAKEFILE=$COMMON_MAKEFILE &&
    mingw32-make.exe gtest  BCDS_COMMON_MAKEFILE=$COMMON_MAKEFILE &&

    cp debug/gtest/*.xml $TESTRESULTS &&

    mingw32-make.exe lcov_clean BCDS_COMMON_MAKEFILE=$COMMON_MAKEFILE &&
    mingw32-make.exe lcov BCDS_COMMON_MAKEFILE=$COMMON_MAKEFILE &&

    print_done_block "GTest/LCOV build"
} || {
    echo "$0: ### GTest/LCOV build failure" &&
    echo "BUILD FAILURE: failure while building the GTest/LCOV target" &&
    exit 1
}

# Doxygen Generation
{
    print_start_block "Doxygen generation" &&

    cd $WORKSPACE/$STASH_PROJECT/$PACKAGENAME&&

    mingw32-make.exe doxygen_clean BCDS_COMMON_MAKEFILE=$COMMON_MAKEFILE &&
	mingw32-make.exe doxygen BCDS_COMMON_MAKEFILE=$COMMON_MAKEFILE &&

    print_done_block "Doxygen generation"
} || {
	echo "$0: ### Doxygen generation failure" &&
	echo "BUILD FAILURE: failure while building the Doxygen target" &&
	exit 1
}

{
   print_start_block "Lint build" &&

   cd $WORKSPACE/$STASH_PROJECT/$PACKAGENAME &&

   mingw32-make.exe lint_clean BCDS_COMMON_MAKEFILE=$COMMON_MAKEFILE &&
   mingw32-make.exe lint BCDS_COMMON_MAKEFILE=$COMMON_MAKEFILE &&

   print_done_block "Lint build"
} || {
	echo "$0: ### Lint build failure" &&
	echo "BUILD FAILURE: failure while building the Lint target" &&
	exit 1
}

{
    print_start_block "report generation" &&

    # generate cobertura xml coverage report
    $PYTHON_34/python.exe $GCOVR_HOME/gcovr --root=$WORKSPACE/$STASH_PROJECT/$PACKAGENAME/ --gcov-exclude=".*gtest.h" --gcov-exclude=".*_unittest.*" --gcov-exclude=".*_th.*" --xml -o $WORKSPACE/coverage.xml &&


    # generate CCM report
    ccm.exe $WORKSPACE/$STASH_PROJECT/$PACKAGENAME /xml >$WORKSPACE/$STASH_PROJECT_$PACKAGENAME_ccm.xml

    print_done_block "report generation"
} || {
    echo "$0: ### Report generation FAILURE" &&
    echo "BUILD FAILURE: report generation failed" &&
    exit 1
}

if [ $level -gt 1 ] && [ "$2" == "efm32" ]
    then
{
    print_start_block "Integration test" &&

    cd $TEST_APP_PATH &&
        
    BCDS_TEST_SECTION_PATH=$WORKSPACE/$STASH_PROJECT/$PACKAGENAME/test/integration/testling/TestSection &&
    mingw32-make clean_libraries BCDS_TEST_SECTION_PATH=$BCDS_TEST_SECTION_PATH &&
    mingw32-make clean BCDS_TEST_SECTION_PATH=$BCDS_TEST_SECTION_PATH &&
    mingw32-make release BCDS_TEST_SECTION_PATH=$BCDS_TEST_SECTION_PATH &&
    # running integration tests
    cd $WORKSPACE/$STASH_PROJECT/$PACKAGENAME/test/integration/TestCoordinator &&
    $PYTHON_34/python.exe -u UtilsTestExecutor.py -c $COM_PORT -v $2 &&

    print_done_block "Integration test"
} || {
        echo "$0: ### Integration test failure" &&
        echo "BUILD FAILURE: failure while performing integration tests" &&
        exit 1
}
fi