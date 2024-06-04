
# FloorPlan DSL Gazebo Plugins

## Initial State Gazebo Plugin

This is a simple Gazebo plugin to set the initial state of a joint at simulation startup.

## Getting started

Build the plugin:

```sh
cd build

# Gazebo Plugins

This repository host three plugins for dynamic behaviour of articulated objects:
* Initial Joint Pose: sets an initial joint position at the beginning of the simulation.
* Time-Based Dynamic Joint: changes the joint position at specified times.
* Trigger-Based Dynamic Joint: changes the joint position when the robot is at a specified distance. 


## Getting started

Build each the plugin:

```sh
cd plugin_to_build/build

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

export GAZEBO_PLUGIN_PATH=$<path_to_plugin_build_dir>:$GAZEBO_PLUGIN_PATH
```

Add the plugin to your model. 


**Initial Joint Pose**
```
<plugin name="initial_plugin" filename="libinitial_plugin.so">
    <joint>JOINT_NAME</joint>
    <position>JOINT_POSITION</position>
</plugin>
```
Where:
* JOINT_NAME: string, name of the model joint that will have a position set up.
* JOINT_POSITION: float, position of the joint. If the joint is revolute it is interpreted as radians, and if it is prismatic it is interpreted as meters. 


**Time-Based Dynamic Joint**
```
<plugin name="dynamic_joint_pluginss" filename="libdynamic_joint_plugin.so">
    <joint>JOINT_NAME</joint>
    <uri>PATH_TO_JSON_FILE</uri>
</plugin>
```
Where:
* JOINT_NAME: string, name of the model joint that will have a position set up.
* PATH_TO_JSON_FILE: string, path to a json file that specifies the positions and timestamps at which the joint moves. 

The json file has this format: 
```
{
    "keyframes": [
        {
            "pose": 0,
            "time": 10.0
        }
    ]
}
```
Similar to JOINT_POSITION,  If the joint is revolute the float value is interpreted as radians, and if it is prismatic it is interpreted as meters. The time is specified in seconds. 

**Trigger-Based Dynamic Joint**
```
<plugin name="adversarial_joint_plugin" filename="libadversarial_joint_plugin.so">
    <joint>JOINT_NAME</joint>
    <x>JOINT_POSITION_BEFORE_TRIGGER</x>
    <near>DISTANCE_TO_TRIGGER</near>
    <y>JOINT_POSITION_AFTER_TRIGGER</y>
</plugin>
```
Where:
* JOINT_NAME: string, name of the model joint that will have a position set up.
* JOINT_POSITION_BEFORE_TRIGGER: float, similar to JOINT_POSITION.
* DISTANCE_TO_TRIGGER: float, distance at which the joint goes from the before_trigger position to the after_trigger position, in meters.
* JOINT_POSITION_AFTER_TRIGGER: float, similar to JOINT_POSITION.


## Usage

This plugin was created to support one companion tool of the FloorPlan DSL. The companion tool allows for modelling kinematic chains and their states, as well as their placement in the world. 


# Acknowledgement

This work is part of a project that has received funding from the European Union's Horizon 2020 research and innovation programme SESAME under grant agreement No 101017258.

<p align="center">
    <img src="images/EU.jpg" alt="drawing" height="100"/>
    <img src="images/SESAME.jpg" alt="drawing" height="100"/>

</p>
</p>

