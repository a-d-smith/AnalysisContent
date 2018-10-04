# Analysis Content
Pandora algorithms for Andrew Smith's analysis

## Install guide

Follow the instructions to [install Pandora](https://github.com/PandoraPFA/Documentation/blob/master/README.md).

```
# Make sure you have already set the MY_TEST_AREA environment variable

# Clone and install analysis content
git clone https://github.com/a-d-smith/AnalysisContent.git
cd AnalysisContent
mkdir build
cd build

cmake -DCMAKE_MODULE_PATH="$MY_TEST_AREA/PandoraPFA/cmakemodules;$ROOTSYS/etc/cmake" -DPANDORA_MONITORING=ON -DPandoraSDK_DIR=$MY_TEST_AREA/PandoraSDK -DPandoraMonitoring_DIR=$MY_TEST_AREA/PandoraMonitoring -DEigen3_DIR=$MY_TEST_AREA/Eigen3/share/eigen3/cmake/ -DLArContent_DIR=$MY_TEST_AREA/LArContent ..
make -j4 install

# Update the client app so it knows about the analysis content
cd $MY_TEST_AREA/LArReco
git remote add asmith https://github.com/a-d-smith/LArReco.git
git fetch
git checkout feature/AnalysisContent_asmith

cd build
cmake -DCMAKE_MODULE_PATH="$MY_TEST_AREA/PandoraPFA/cmakemodules;$ROOTSYS/etc/cmake" -DPANDORA_MONITORING=ON -DPandoraSDK_DIR=$MY_TEST_AREA/PandoraSDK/ -DPandoraMonitoring_DIR=$MY_TEST_AREA/PandoraMonitoring/ -DLArContent_DIR=$MY_TEST_AREA/LArContent/ -DAnalysisContent_DIR=$MY_TEST_AREA/AnalysisContent/ ..
make -j4 install
```
