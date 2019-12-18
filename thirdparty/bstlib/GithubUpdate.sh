#!/bin/sh
REPO_NAME=$1
BRANCH_TAG_NAME=$2
declare -a EXPECTED_REPOS=('BMA2x2_driver' 'BME280_driver' 'BMG160_driver' 'BMI160_driver' 'BMM050_driver' 'BMP280_driver' 'BMP180_driver' 'BNO055_driver')
FLAG=0
for REPO in "${EXPECTED_REPOS[@]}"
do	
	if [ ${REPO} == ${REPO_NAME} ]
	then
		FLAG=1
	fi
done
if [ ${FLAG} != 1 ]
then
	echo "Given REPO is not a valid repository"
	exit 1
else	
	echo "Entered else condition"
	OUTPUT=$(git remote)
	#declare -a REMOTE_REPOS_LIST
	mapfile -t REMOTE_REPOS_LIST <<< ${OUTPUT}
	echo ${REMOTE_REPOS_LIST}
	FLAG=0
	if  echo ${REMOTE_REPOS_LIST} | grep -q ${REPO_NAME}
	then
        echo "git subtree pull"
		git subtree pull --prefix=BST_Github/${REPO_NAME}/ ${REPO_NAME} ${BRANCH_TAG_NAME}
	else
        echo "git subtree add"
		git remote add ${REPO_NAME} https://github.com/BoschSensortec/${REPO_NAME}.git
		git subtree add --prefix=BST_Github/${REPO_NAME}/ ${REPO_NAME} ${BRANCH_TAG_NAME}
		git subtree pull --prefix=BST_Github/${REPO_NAME}/ ${REPO_NAME} ${BRANCH_TAG_NAME}
	fi
fi
	
