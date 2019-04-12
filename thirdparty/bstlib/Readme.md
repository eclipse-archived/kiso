# BST Library
----
## vx.x.x-0.4.1
* Readme is corrected for proxy setting

## vx.x.x-0.4.0
* Migrated to the new version control model for 3rd-party software BST Lib
* Migrated to the new build system

## Modifications in Older Versions
### BST Lib Release vx.x.x-v0.3.0**
* BME280 Driver library (v2.0.5) is updated from BST Github master


### BST Lib Release vx.x.x-v0.2.0**
* BST library is updated from BST Github master

----
# Notes

* BST library is added from GitHub using Git sub tree.
* Run the GithubUpdate.sh script to update the BST drivers.
* Currently all drivers are pulled from master branch from github.

**Refer the below Wiki page for proxy setting**

https://bcds01.de.bosch.com/wiki/display/BSE/Software+Setup+For+Testing#SoftwareSetupForTesting-ProxyConfigurations

**Update BST library:**

Run the below commands:

./GithubUpdate.sh <Repo Name> <Tag>

git push origin <branch name>

Example :

./GithubUpdate.sh BNO055_driver master

git push origin feature/BSTlibupdate
