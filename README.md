# LightStudio

## Overview

A study project to implement and test different lightning models. 

In this application, we can:
- Choose the shape to be presented;
- Choose the lightning model to be used;
- Setup a maximum of 4 lights in the scene;
- Setup the material of the shape;

The shapes supported currently are a cube and a icosahedron

The lightning models implemented are:
- Flat shading;
- Gouraud shading;
- Phong shading;
- Deferred Phong shading;

For each light the user can:
- Turn on/off the light;
- Setup ambient, diffuse and specular colors;
- Define the light as directional or point light;
- For directional light, the user can define a direction;
- For point light, the user can define the attenuation coefficients;

For the material the user can setup the ambient, diffuse and specular colors, and the shininess as well.

## Input

The user can do the following actions in the app:
- Interact in the UI to setup the scene parameters;
- Hold the right mouse button to rotate the camera;

## Building this project

To build this project you only need to have git and CMake installed. The libraries used in this project are installed via vcpkg. To setup the project, do the following steps:
- Clone this repo;
- Run Instal.bat
- Run Build.bat
