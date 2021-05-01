# ONNX Library

This is a simple library with functionblocks to load an [ONNX](http://onnx.ai) file via C-API from [onnxruntime](https://www.onnxruntime.ai/).
For now only one functionblock class (UnityMLAgent) is provided, that supports to perform inference (execute the ONNX file) based on following requirements:
* 2 float vector inputs named `vector_observation` and `action_masks`
* 1 float vector output named `action`
* The file is loaded by setting the FilePath input parameter.
* Windows 32bit

Matching onnx files can be created by [Unity ML-Agents Toolkit](https://github.com/Unity-Technologies/ml-agents).
An [example ONNX file](doc/example.onnx) is included in doc folder comprising the following visualized structure via [netron app](https://netron.app/):

![Example ONNX file visualized via netron](doc/example.svg)