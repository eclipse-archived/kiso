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
str_level="$1"

# reading the input parameter
case "$1" in
    "branch")           level=1;;
    "staging")          level=2;;
    "pullrequest")      level=3;;
    "master")           level=4;;
    "frontline")        level=5;;
esac

# assign common variables
STASH_PROJECT=bcds_shared
PACKAGENAME=Essentials
cd ../..
WORKSPACE="$PWD"
PACKAGE_HOME=$WORKSPACE/$STASH_PROJECT/$PACKAGENAME
CONTEXT=RefContexts

# assign plattform specific variables
if [ "$2" == "efm32" ]; then
    CONTEXT_PATH=$WORKSPACE/$STASH_PROJECT/$CONTEXT/efm32
    COM_PORT=$XDK_COM_PORT
    BCDS_MCU_FAMILY=efm32
elif [ "$2" == "stm32l4" ] || [ "$2" == "stm32" ] ; then # To remove once STM32 is truly deprecated
    CONTEXT_PATH=$WORKSPACE/$STASH_PROJECT/$CONTEXT/stm32
    COM_PORT=$BSE_COM_PORT
    BCDS_MCU_FAMILY=stm32l4
elif [ "$2" == "stm32f7" ]; then
    CONTEXT_PATH=$WORKSPACE/$STASH_PROJECT/$CONTEXT/stm32
    COM_PORT=$BSE_COM_PORT
    BCDS_MCU_FAMILY=stm32f7
elif [ "$2" == "ti_cc26xx" ]; then
    CONTEXT_PATH=$WORKSPACE/$STASH_PROJECT/$CONTEXT/ti_cc26xx
    COM_PORT=$BSE_COM_PORT
    BCDS_MCU_FAMILY=ti_cc26xx
else
	echo "$0: "
	echo "$0: Assignment of BOARD_TYPE failed."
	echo "$0:  TARGET parameter must be one of (stm32l4, stm32f7, efm32, ti_cc26xx), you passed '$2'"
	echo "$0: "
	echo "$0: "
	exit 1
fi

COMMON_MAKEFILE=$CONTEXT_PATH/common.mk
export TEST_APP_PATH=$CONTEXT_PATH/TestApp

if [ "$2" == "" ]; then
    echo "$0: "
    echo "$0: Assignment of BOARD_TYPE failed."
    echo "$0:  TARGET parameter must be one of (stm32, efm32), you passed '$2'"
    echo "$0: "
    echo "$0: "
    exit 1
fi


echo "$0: "
echo "$0: Start $STASH_PROJECT build with following parameters:"
echo "$0:   STASH_PROJECT:      $STASH_PROJECT "
echo "$0:   PACKAGENAME:        $PACKAGENAME "
echo "$0:   WORKSPACE:      $WORKSPACE "
echo "$0: "

# do not check out dependencies in case of frontline jobs
if [ $str_level != "frontline" ]
then
{
    {
	print_start_block "Getting RefContexts with spider" &&
    export BCDS_WORKSPACE_PATH=$WORKSPACE &&
	$PYTHON_34/python.exe -u -mspider sync -d $WORKSPACE/$STASH_PROJECT/$PACKAGENAME/$CONTEXT.dep -s $SHARED_COMMON_GIT_DIR &&

        print_done_block "Getting RefContexts with spider"
    } || {
        echo "$0: ### Getting RefContexts with spider failure" &&
        echo "BUILD FAILURE: failure while checking out RefContexts" &&
        exit 1
    }

    {
	print_start_block "Getting dependencies with spider" &&
    export BCDS_WORKSPACE_PATH=$WORKSPACE &&
	$PYTHON_34/python.exe -u -mspider sync -d $WORKSPACE/$STASH_PROJECT/RefContexts/platform.dep -s $SHARED_COMMON_GIT_DIR -e $PACKAGENAME &&

        print_done_block "Getting dependencies with spider"
    } || {
        echo "$0: ### Getting dependencies with spider failure" &&
        echo "BUILD FAILURE: failure while checking out dependencies" &&
        exit 1
    }
}
fi

{
    print_start_block "Debug build" &&

    cd $WORKSPACE/$STASH_PROJECT/$PACKAGENAME &&

    mingw32-make.exe clean BCDS_COMMON_MAKEFILE=$COMMON_MAKEFILE &&
    mingw32-make.exe debug BCDS_COMMON_MAKEFILE=$COMMON_MAKEFILE BCDS_MCU_FAMILY=$BCDS_MCU_FAMILY&&

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
    mingw32-make.exe release BCDS_COMMON_MAKEFILE=$COMMON_MAKEFILE BCDS_MCU_FAMILY=$BCDS_MCU_FAMILY&&

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
    mingw32-make.exe gtest  BCDS_COMMON_MAKEFILE=$COMMON_MAKEFILE BCDS_MCU_FAMILY=$BCDS_MCU_FAMILY&&

    cp debug/gtest/*.xml $TESTRESULTS &&

    mingw32-make.exe lcov_clean BCDS_COMMON_MAKEFILE=$COMMON_MAKEFILE &&
    mingw32-make.exe lcov BCDS_COMMON_MAKEFILE=$COMMON_MAKEFILE BCDS_MCU_FAMILY=$BCDS_MCU_FAMILY&&

    print_done_block "GTest/LCOV build"
} || {
    echo "$0: ### GTest/LCOV build failure" &&
    echo "BUILD FAILURE: failure while building the GTest/LCOV target" &&
    exit 1
}

{
    print_start_block "Doxygen generation" &&

    cd $WORKSPACE/$STASH_PROJECT/$PACKAGENAME&&

    mingw32-make.exe doxygen_clean BCDS_COMMON_MAKEFILE=$COMMON_MAKEFILE &&
	mingw32-make.exe doxygen BCDS_COMMON_MAKEFILE=$COMMON_MAKEFILE BCDS_MCU_FAMILY=$BCDS_MCU_FAMILY&&

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
   mingw32-make.exe lint BCDS_COMMON_MAKEFILE=$COMMON_MAKEFILE BCDS_MCU_FAMILY=$BCDS_MCU_FAMILY&&

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
        mingw32-make debug BCDS_TEST_SECTION_PATH=$BCDS_TEST_SECTION_PATH BCDS_MCU_FAMILY=$BCDS_MCU_FAMILY&&

        # running integration tests
        cd $WORKSPACE/$STASH_PROJECT/$PACKAGENAME/test/integration/TestCoordinator &&
        $PYTHON_34/python.exe -u TestExecutor_Essentials.py -c $COM_PORT -v $2 &&

        print_done_block "Integration test"
    } || {
        echo "$0: ### Integration test failure" &&
        echo "BUILD FAILURE: failure while performing integration tests" &&
        exit 1
}
fi