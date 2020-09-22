pipeline
{
    agent
    {
        kubernetes
        {
            containerTemplate
            {
                name 'kiso-build-env'
                image 'eclipse/kiso-build-env:v0.0.8'
                ttyEnabled true
                resourceRequestCpu '2'
                resourceLimitCpu '2'
                resourceRequestMemory '8Gi'
                resourceLimitMemory '8Gi'
            }
        }
    }

    stages
    {
        stage('Build Software')
        {
            steps // Define the steps of a stage
            {
                script // Run build
                {
                    echo "Build the application"
                    sh 'cmake . -Bbuilddir-debug -G"Ninja"'
                    sh 'cmake --build builddir-debug'
                }
            }
        }
        stage('Run DoD')
        {
            parallel
            {
                stage('Format Checks')
                {
                    steps
                    {
                        script
                        {
                            echo "enforce formatting rules"
                            sh 'cmake . -Bbuilddir-formatting -G"Ninja" -DENABLE_FORMAT_CHECKS=1 -DSKIP_FORMAT_REPORTS=0'
                            script {
                                def reports = sh(script: "find builddir-formatting/ -name *_format.xml | tr '\n' ' '",  returnStdout: true)
                                sh "python3 ci/clang-format-to-junit.py ${reports} -o builddir-formatting/clang-format.xml -p builddir-formatting -s _format.xml"
                            }
                        }
                    }
                }
                stage('Static Analysis')
                {
                    steps
                    {
                        script
                        {
                            echo "run static analysis"
                            sh 'cmake . -Bbuilddir-static -G"Unix Makefiles" -DPROJECT_CONFIG_PATH=ci/testing_config -DENABLE_STATIC_CHECKS=1 -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON'
                            sh 'cmake --build builddir-static 2> builddir-static/clang-report.txt'
                            sh 'if [ ! -s builddir-static/clang-report.txt ]; then echo "Good, all tests have been passed w/o findings" > builddir-static/clang-report.txt; fi;'
                            sh 'cat builddir-static/clang-report.txt | python ci/thirdparty/clangTidyToJunit/clang-tidy-to-junit.py `pwd` > builddir-static/clang-report.xml'
                            sh 'cp core/.clang-tidy builddir-static/clang-tidy-config.txt' // export clang-tidy config with '.txt' ending, so it can be displayed on Jenkins Dashboard
                        }
                    }
                }
                stage('Unit Tests')
                {
                    steps
                    {
                        script
                        {
                            echo "run unit-tests"
                            sh 'cmake . -Bbuilddir-unittests -G"Ninja" -DENABLE_TESTING=1 -DPROJECT_CONFIG_PATH=ci/testing_config -DKISO_STATIC_CONFIG=1'
                            sh 'cmake --build builddir-unittests'
                            sh 'cd builddir-unittests && ctest -T test -V --no-compress-output' // Produce test results xml
                            sh 'cmake --build builddir-unittests --target coverage -- -j1' // Produce coverage output (single-threaded because of ninja)
                        }
                    }
                }
                stage('Integration Tests')
                {
                    steps
                    {
                        script
                        {
                            echo "build integration-tests"
                            sh 'cmake . -Bbuilddir-integration -G"Ninja" -DENABLE_INTEGRATION_TESTING=1 -DKISO_INTEGRATION_TEST_ENTRY_PATH="core/essentials/test/integration" -DKISO_BOARD_NAME="NucleoF767"'
                            sh 'cmake --build builddir-integration'
                        }
                    }
                }
                stage('Doxygen Documentation')
                {
                    steps
                    {
                        script
                        {
                            echo "Generate Doxygen"
                            sh 'cmake --build builddir-debug --target docs'
                        }
                    }
                }
                stage('Hugo Documentation')
                {
                    steps
                    {
                        script
                        {
                            echo "Generate Hugo Website"
                            sh 'rm -rf docs/website/themes/learn/'
                            sh 'git clone --depth 1 --branch 2.5.0 https://github.com/matcornic/hugo-theme-learn.git docs/website/themes/learn/'
                            sh 'hugo -s docs/website'
                        }
                    }
                }
            } // parallel
        } // stage('Run DoD')
    } // stages

    post // Called at very end of the script to notify developer and github about the result of the build
    {
        always
        {
            archiveArtifacts (
                artifacts: 'builddir-unittests/Testing/**/*.xml',
                fingerprint: true
            )
            archiveArtifacts (
                artifacts: 'builddir-static/clang-report.txt',
                fingerprint: true
            )
            archiveArtifacts (
                artifacts: 'builddir-formatting/**/*_format.xml',
                fingerprint: true
            )
            archiveArtifacts (
                artifacts: 'builddir-static/clang-tidy-config.txt',
                fingerprint: true
            )
            junit (
                allowEmptyResults: true,
                testResults: 'builddir-static/clang-report.xml'
            )
            junit (
                allowEmptyResults: true,
                testResults: 'builddir-formatting/clang-format.xml'
            )
        }
        success
        {
            archiveArtifacts (
                artifacts: 'builddir-debug/docs/doxygen/**, builddir-unittests/*_cov/**, docs/website/public/**/*',
                fingerprint: true
            )
            cleanWs()
        }
        unstable
        {
            notifyFailed()
        }
        failure
        {
            notifyFailed()
        }
        aborted
        {
            notifyAbort()
        }
    }
} // pipeline


def notifyFailed()
{
    emailext (subject: "Job '${env.JOB_NAME}' (${env.BUILD_NUMBER}) is failing",
                body: "Oups, something went wrong with ${env.BUILD_URL}... We are looking forward for your fix!",
                recipientProviders: [[$class: 'CulpritsRecipientProvider'],
                                    [$class: 'DevelopersRecipientProvider'],
                                    [$class: 'RequesterRecipientProvider']])
}

def notifyAbort()
{
    emailext (subject: "Job '${env.JOB_NAME}' (${env.BUILD_NUMBER}) was aborted",
                body: "Oups, something went wrong with ${env.BUILD_URL}... We are looking forward for your fix!",
                recipientProviders: [[$class: 'CulpritsRecipientProvider'],
                                    [$class: 'DevelopersRecipientProvider'],
                                    [$class: 'RequesterRecipientProvider']])
}
