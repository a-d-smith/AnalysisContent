# Analysis Content
Pandora algorithms for Andrew Smith's analysis

## Install guide

```
cd AnalysisContent
mkdir build
cd build
cmake -DCMAKE_MODULE_PATH="$MY_TEST_AREA/PandoraPFA/cmakemodules;$ROOTSYS/etc/cmake" -DPANDORA_MONITORING=ON -DPandoraSDK_DIR=$MY_TEST_AREA/PandoraSDK -DPandoraMonitoring_DIR=$MY_TEST_AREA/PandoraMonitoring -DEigen3_DIR=$MY_TEST_AREA/Eigen3/share/eigen3/cmake/ -DLArContent_DIR=$MY_TEST_AREA/LArContent ..
make -j4 install
```
