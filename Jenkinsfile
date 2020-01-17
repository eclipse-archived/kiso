pipeline
{
    agent // Define the agent to use
    {
        docker
        {
            label 'RT-Z0KHU'
            image 'rb-dtr.de.bosch.com/software-campus/kiso-toolchain:v0.4.3'
            registryUrl 'https://rb-dtr.de.bosch.com'
            registryCredentialsId 'docker-registry'
        }
    }
    options
    {
        timeout(time: 60, unit: 'MINUTES') // Define a timeout of 60 minutes
                timestamps() // Use a timestamp notation
                disableConcurrentBuilds() // We don't want to have concurrent build issues here
    }
    triggers // Define how many times the job will be triggered
    {
        pollSCM('*/5 * * * *') //  If new changes exist, the Pipeline will be re-triggered automatically. Check will be done every 5 minutes
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
                                def reports = findFiles(glob: 'builddir-formatting/**/*_format.xml')
                                sh "python3 ci/clang-format-to-junit.py ${reports.join(' ')} -o builddir-formatting/clang-format.xml -p builddir-formatting -s _format.xml"
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
                            sh 'cmake . -Bbuilddir-static -G"Unix Makefiles" -DKISO_BOARD_NAME=CommonGateway -DENABLE_STATIC_CHECKS=1 -DENABLE_ALL_FEATURES=1 -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON'
                            sh 'cmake --build builddir-static 2> builddir-static/clang-report.txt'
                            sh 'if [ ! -s builddir-static/clang-report.txt ]; then  echo "Good, all tests have been passed w/o findings" > builddir-static/clang-report.txt; fi;'
                            sh 'cat builddir-static/clang-report.txt | python ci/thirdparty/clangTidyToJunit/clang-tidy-to-junit.py `pwd` > builddir-static/clang-report.xml'
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
                            sh 'cmake . -Bbuilddir-unittests -G"Ninja" -DENABLE_TESTING=1 -DENABLE_ALL_FEATURES=1'
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
                            echo "run integration-tests placeholder"
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
                body: "Please go to ${env.BUILD_URL}, check it out AND fix it...",
                recipientProviders: [[$class: 'CulpritsRecipientProvider'],
                                    [$class: 'DevelopersRecipientProvider'],
                                    [$class: 'RequesterRecipientProvider']])
}

def notifyAbort()
{
    emailext (subject: "Job '${env.JOB_NAME}' (${env.BUILD_NUMBER}) was aborted",
                body: "Please go to ${env.BUILD_URL}, check what happened.",
                recipientProviders: [[$class: 'CulpritsRecipientProvider'],
                                    [$class: 'DevelopersRecipientProvider'],
                                    [$class: 'RequesterRecipientProvider']])
}
