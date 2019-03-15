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
LEVEL=0
str_level="$1"
# reading the input parameter
case "$1" in
    "branch")           LEVEL=1;;
    "staging")          LEVEL=2;;
    "pullrequest")      LEVEL=3;;
    "master")           LEVEL=4;;
    "longduration")     LEVEL=5;;
    "frontline")        level=6;;
esac


# assign common variables
STASH_PROJECT=bcds_shared
PACKAGENAME=Cellular
cd ../..
WORKSPACE="$PWD"
PACKAGE_HOME=$WORKSPACE/$STASH_PROJECT/$PACKAGENAME

CONTEXT=RefContexts

# assign common variables
STASH_PROJECT=bcds_shared
if [ "$2" == "UBLOX" ]; then
        BCDS_TARGET_PLATFORM="efm32"
        CONTEXT_PATH=$WORKSPACE/$STASH_PROJECT/$CONTEXT/efm32
        HARDWARE_PLATFORM="XDK"
        DEVICE_TYPE="EFM32GG"
        DEVICE_ID="EFM32GG390F1024"
        CELLULAR_PLATFORM="ublox"
        COM_PORT=$XDK_MODEM_UBLOX_COM_PORT
        JLINK_DEVICE=$XDK_MODEM_UBLOX_JLINK_DEVICE
        if [ -n "${UBLOX_NETWORK_APN}" ]
        then
            export BCDS_NETWORK_APN=$UBLOX_NETWORK_APN
        fi
elif [ "$2" == "QUECTEL" ]; then
        BCDS_TARGET_PLATFORM="efm32"
        CONTEXT_PATH=$WORKSPACE/$STASH_PROJECT/$CONTEXT/efm32
        HARDWARE_PLATFORM="XDK"
        DEVICE_TYPE="EFM32GG"
        DEVICE_ID="EFM32GG390F1024"
        CELLULAR_PLATFORM="quectel"
        COM_PORT=$XDK_MODEM_QUECTEL_COM_PORT
        JLINK_DEVICE=$XDK_MODEM_QUECTEL_JLINK_DEVICE
        if [ -n "${QUECTEL_NETWORK_APN}" ]
        then
            export BCDS_NETWORK_APN=$QUECTEL_NETWORK_APN
        fi
else
    echo "$0: "
    echo "$0: Assignment of BCDS_TARGET_PLATFORM failed."
    echo "$0: TARGET parameter, expected one of (UBLOX, QUECTEL), you passed '$2'"
    echo "$0: "
    echo "$0: "
    exit 1
fi

COMMON_MAKEFILE=$CONTEXT_PATH/common.mk
export TEST_APP_PATH=$CONTEXT_PATH/TestApp
export BCDS_CELLULAR_PLATFORM=$CELLULAR_PLATFORM

# Set number of parallel jobs
if [ "$(command -v getconf _NPROCESSORS_ONLN)" ]; then
    _NPROC=$(getconf _NPROCESSORS_ONLN)
else
    _NPROC=9
fi

# Convert server and network parameters
if [ -n "${GSM_ECHO_SERVER_IP}" ]
then
    IFS=. read -r a b c d <<< "$GSM_ECHO_SERVER_IP"
    export BCDS_ECHO_SERVER_IP="$((a * 256 ** 3 + b * 256 ** 2 + c * 256 + d))"
fi

if [ -n "${GSM_ECHO_SERVER_PORT}" ]
then
    export BCDS_ECHO_SERVER_PORT=$GSM_ECHO_SERVER_PORT
fi

if [ "$(command -v git)" ]; then
    GIT_CMD=git
else
    GIT_CMD="C:\Program Files\Git\cmd\git.exe"
fi

echo "$0: "
echo "$0: Start $PACKAGENAME build with following parameters:"
echo "$0:     STASH_PROJECT:      $STASH_PROJECT "
echo "$0:     WORKSPACE:          $WORKSPACE "
echo "$0:     GIT_CMD:            $GIT_CMD "
echo "$0:     TARGET_PLATFORM:    $BCDS_TARGET_PLATFORM "
echo "$0:     HARDWARE_PLATFORM:  $HARDWARE_PLATFORM "
echo "$0:     CELLULAR_PLATFORM:  $CELLULAR_PLATFORM "
echo "$0:     COM_PORT:           $COM_PORT "
echo "$0:     JLINK_DEVICE:       $JLINK_DEVICE "
echo "$0:     ECHO_SERVER_IP:     $GSM_ECHO_SERVER_IP "
echo "$0:     ECHO_SERVER_PORT:   $GSM_ECHO_SERVER_PORT "
echo "$0:     NETWORK_APN:        $BCDS_NETWORK_APN "
echo "$0: "
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

    mingw32-make.exe -j$_NPROC clean BCDS_COMMON_MAKEFILE=$COMMON_MAKEFILE &&
    mingw32-make.exe -j$_NPROC debug BCDS_COMMON_MAKEFILE=$COMMON_MAKEFILE &&

    print_done_block "Debug build"
} || {
    echo "$0: ### Debug build failure" &&
    echo "BUILD FAILURE: failure while building the debug target" &&
    exit 1
}

{
    print_start_block "Release build" &&

    cd $WORKSPACE/$STASH_PROJECT/$PACKAGENAME &&

    mingw32-make.exe -j$_NPROC clean BCDS_COMMON_MAKEFILE=$COMMON_MAKEFILE &&
    mingw32-make.exe -j$_NPROC release BCDS_COMMON_MAKEFILE=$COMMON_MAKEFILE &&

    print_done_block "Release build"
} || {
    echo "$0: ### Release build failure" &&
    echo "BUILD FAILURE: failure while building the release target" &&
    exit 1
}

{
    print_start_block "GTest/LCOV build" &&

    cd $WORKSPACE/$STASH_PROJECT/$PACKAGENAME &&

    mingw32-make.exe -j$_NPROC gtest_clean BCDS_COMMON_MAKEFILE=$COMMON_MAKEFILE &&
    mingw32-make.exe -j$_NPROC gtest  BCDS_COMMON_MAKEFILE=$COMMON_MAKEFILE &&
    cp debug/gtest/*.xml $TESTRESULTS &&
    mingw32-make.exe -j$_NPROC lcov_clean BCDS_COMMON_MAKEFILE=$COMMON_MAKEFILE &&
    mingw32-make.exe -j$_NPROC lcov BCDS_COMMON_MAKEFILE=$COMMON_MAKEFILE &&

    print_done_block "GTest/LCOV build"
} || {
    echo "$0: ### GTest/LCOV build failure" &&
    echo "BUILD FAILURE: failure while building the GTest/LCOV target" &&
    exit 1
}

{
    print_start_block "Doxygen generation" &&

    cd $WORKSPACE/$STASH_PROJECT/$PACKAGENAME&&

    mingw32-make.exe -j$_NPROC doxygen_clean BCDS_COMMON_MAKEFILE=$COMMON_MAKEFILE &&
    mingw32-make.exe -j$_NPROC doxygen BCDS_COMMON_MAKEFILE=$COMMON_MAKEFILE &&

    print_done_block "Doxygen generation"
} || {
    echo "$0: ### Doxygen generation failure" &&
    echo "BUILD FAILURE: failure while building the Doxygen target" &&
    exit 1
}

{
   print_start_block "Lint build" &&

   cd $WORKSPACE/$STASH_PROJECT/$PACKAGENAME &&

   mingw32-make.exe -j$_NPROC lint_clean BCDS_COMMON_MAKEFILE=$COMMON_MAKEFILE &&
   mingw32-make.exe -j$_NPROC lint BCDS_COMMON_MAKEFILE=$COMMON_MAKEFILE &&

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

# Integration testing is enabled for build jobs with level > 1 (staging and higher).
if [ $LEVEL -gt 1 ]
then
    {
		# Generate TestSuites.py
        TESTPATH=$WORKSPACE/$STASH_PROJECT/$PACKAGENAME/test/integration
        SCRIPTNAME=`basename "$0"`
        echo "# AUTOGENERATED BY $SCRIPTNAME, DO NOT EDIT" > $TESTPATH/TestCoordinator/TestSuites.py
        sed -nr 's/#define (.*) (.*)/\1 = \2/p' $TESTPATH/Testling/TestSuites/TestSuite.h >> $TESTPATH/TestCoordinator/TestSuites.py
	
        print_start_block "Integration test" &&

        cd $TEST_APP_PATH &&
        
        BCDS_TEST_SECTION_PATH=$WORKSPACE/$STASH_PROJECT/$PACKAGENAME/test/integration/testling/TestSection &&
        mingw32-make.exe -j$_NPROC clean_libraries BCDS_TEST_SECTION_PATH=$BCDS_TEST_SECTION_PATH &&
        mingw32-make.exe -j$_NPROC clean BCDS_TEST_SECTION_PATH=$BCDS_TEST_SECTION_PATH &&
        mingw32-make.exe -j$_NPROC release BCDS_TEST_SECTION_PATH=$BCDS_TEST_SECTION_PATH &&

        # running integration tests
        sleep 5
    
        # Run smoke tests for build level 2 (staging)
        if [ $LEVEL -eq 2 ]
            then
            {
                cd $WORKSPACE/$STASH_PROJECT/$PACKAGENAME/test/integration/TestCoordinator &&
                echo -e "\n\nLaunching Smoke test suite:\n  python -u CellularTestExecutor.py -c $COM_PORT -j $JLINK_DEVICE -p $DEVICE_ID -t SMOKE_TEST"
                $PYTHON_34/python.exe -u CellularTestExecutor.py -c $COM_PORT -j $JLINK_DEVICE -p $DEVICE_ID -t SMOKE_TEST
            }
        fi

        # Run integration test suite for build level 3 or 4 (pullrequest, master)
        if [ $LEVEL -eq 3 ] || [ $LEVEL -eq 4 ]
            then
            {
                cd $WORKSPACE/$STASH_PROJECT/$PACKAGENAME/test/integration/TestCoordinator &&
                echo -e "\n\nLaunching integration tests:\n  python -u CellularTestExecutor.py -c $COM_PORT -j $JLINK_DEVICE -p $DEVICE_ID"
                $PYTHON_34/python.exe -u CellularTestExecutor.py -c $COM_PORT -j $JLINK_DEVICE -p $DEVICE_ID
            }
        fi

        # Run integration test suite multiple times for build level 5  ("longduration")
        if [ $LEVEL -gt 4 ]
            then
            {
                cd $WORKSPACE/$STASH_PROJECT/$PACKAGENAME/test/integration/TestCoordinator &&
                echo -e "\n\nLaunching integration tests (6 iterations):\n  python -u CellularTestExecutor.py -n 6 -c $COM_PORT -j $JLINK_DEVICE -p $DEVICE_ID"
                $PYTHON_34/python.exe -u CellularTestExecutor.py -n 6 -c $COM_PORT -j $JLINK_DEVICE -p $DEVICE_ID
            }
        fi
        print_done_block "Integration test"
    } || {
        echo "$0: ### Integration test failure" &&
        echo "BUILD FAILURE: failure while performing integration tests" &&
        exit 1
    }
fi


