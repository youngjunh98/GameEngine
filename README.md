# GameEngine

혼자서 만드는 게임 엔진


- 그래픽 예시

<img src="https://user-images.githubusercontent.com/8470484/99874502-d5603500-2c2b-11eb-8677-285991c3e95e.png"
width="30%" height="30%" title="PBR Dielectric" alt="PBR Dielectric"></img>
<img src="https://user-images.githubusercontent.com/8470484/99874503-d6916200-2c2b-11eb-872d-6b7db963df23.png"
width="30%" height="30%" title="PBR Metal" alt="PBR Metal"></img>



## Feature

### Supported Asset Formats
- 3D: FBX, OBJ
- Audio: WAV
- Texture: DDS, JPG, PNG, TGA, BMP, PSD, GIF, HDR, PIC


### Engine
- Cross-platform Frameowrk
- Component Based Game Object
- Modular System


### Editor
- GUI Editor using Dear ImGUI
- Real Time Component Editting
- Wireframe Mode

### Audio
- Audio thread
- WASAPI (Windows)


### Physics
- PhysX SDK
- Box, Sphere Collider
- Rigidbody


### Rendering
- PBR
- Directional, Point, Spot Light
- Normal Mapping
- Tessellation
- Shadow Mapping
- Customizable Rendering Pipeline
- Shader Code Reflection


### Other
- CMake를 이용한 빌드



## Roadmap

### Working
- Thread Job
- Particle effect
- Asset Manager 개선하기.
- Global Renderer, Render Pipeline 개선하기.
- String, WString 클래스.
- Game 모듈.


### Future
- Memory manager
- Skinned mesh animation
- Cascaded shadow maps
- OpenGL support
- DirectX 12 support
- Vulkan supoort
- Object Type Reflection
- XML support



## Third Party Libraries

- ### Autodesk FBX SDK 2019.0
This software contains Autodesk® FBX® code developed by Autodesk, Inc. Copyright 2018 Autodesk, Inc. All rights, reserved. 
Such code is provided "as is" and Autodesk, Inc. disclaims any and all warranties, whether express or implied, including without 
limitation the implied warranties of merchantability, fitness for a particular purpose or non-infringement of third party rights. In 
no event shall Autodesk, Inc. be liable for any direct, indirect, incidental, special, exemplary, or consequential damages 
(including, but not limited to, procurement of substitute goods or services; loss of use, data, or profits; or business interruption) 
however caused and on any theory of liability, whether in contract, strict liability, or tort (including negligence or otherwise) 
arising in any way out of such code.


- ### NVIDIA PhysX SDK 4.1
Copyright (c) 2019 NVIDIA Corporation. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following 
conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
disclaimer.
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
disclaimer in the documentation and/or other materials provided with the distribution.
Neither the name of NVIDIA CORPORATION nor the names of its contributors may be used to endorse or promote 
products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, 
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
POSSIBILITY OF SUCH DAMAGE.


- ### stb
Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


- ### Dear ImGui
The MIT License (MIT)

Copyright (c) 2014-2020 Omar Cornut

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


- ### JSON for Modern C++
MIT License 

Copyright (c) 2013-2020 Niels Lohmann

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.