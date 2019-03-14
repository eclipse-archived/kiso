pipeline
{
    agent // Define the agent to use
    {
        docker
        {
            label 'RT-Z0KHU'
            image 'rb-dtr.de.bosch.com/software-campus/cddk-toolchain:v0.1.1'
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
        pollSCM('*/5 * * * *') //  If new changes exist, the Pipeline will be re-triggered automatically. Check will be done every 6 minutes
    }

    stages
    {
        stage('Build Software')
        {
            steps // Define the steps of a stage
            {
                script // Run static analysis
                {
                    echo "Build the application"
                }
            }
        }
        stage('Run DoD')
        {
            parallel
            {
                stage('Static Analysis')
                {
                    steps
                    {
                        script // Run static analysis
                        {
                            echo "run static placeholder"
                        }
                    }
                }
                stage('UnitTests')
                {
                    steps
                    {
                        script // Run unittests
                        {
                            echo "run unit-tests placeholder"
                        }
                    }
                }
                stage('IntegrationTests')
                {
                    steps
                    {
                        script // Run unittests
                        {
                            echo "run integration-tests placeholder"
                        }
                    }
                }
                stage('Doxygen Documentation')
                {
                    steps
                    {
                        script // Run unittests
                        {
                            echo "Generate doxygen placeholder"
                        }
                    }
                }
            } // parallel
        } // stage('Run DoD')
    } // stages

    post // Called at very end of the script to notify developer and github about the result of the build
    {
        success
        {
            echo 'To do'
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
