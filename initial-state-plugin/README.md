# Initial State Gazebo Plugin

This is a simple Gazebo plugin to set the initial state of a joint at simulation startup. 

## Getting started

Build the plugin:

```sh
cd build
cmake ../
make
```

Add the path of the plugin to the Gazebo plugin path:

```sh
export GAZEBO_PLUGIN_PATH=$<path_to_plugin>:$GAZEBO_PLUGIN_PATH
```

Add the plugin to your model. 

```xml
<plugin name="initial_plugin" filename="libinitial_plugin.so">
    <joint>NAME_OF_JOINT</joint>
    <position>VALUE</position>
</plugin>
```

## Usage

This plugin was created to support one companion tool of the FloorPlan DSL. The companion tool allows for modelling kinematic chains and their states, as well as their placement in the world. 

# Acknowledgement

This work is part of a project that has received funding from the European Union's Horizon 2020 research and innovation programme SESAME under grant agreement No 101017258.

<p align="center">
    <img src="images/EU.jpg" alt="drawing" height="100"/>
    <img src="images/SESAME.jpg" alt="drawing" height="100"/>
</p>