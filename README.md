# FloorPlan DSL Gazebo Plugins

This repository host three plugins for dynamic behaviour of articulated objects:
* [Initial Joint State Plugin](#initial-joint-state-plugin): sets an initial joint position at the beginning of the simulation
* [Time-Based Triggered Joint Position Plugin](#time-based-triggered-joint-position-plugin): changes the joint position at specified times
* [Distance-Based Triggered Joint Position Plugin](#distance-based-triggered-joint-position-plugin) changes the joint position when the robot is at a specified distance 

## Getting started

## Requirements

- Gazebo Harmonic
- [ROS Jazzy](https://docs.ros.org/en/jazzy/Installation.html)

## Installation

1. Clone this package to your colcon workspace and [build it](https://docs.ros.org/en/jazzy/Tutorials/Beginner-Client-Libraries/Colcon-Tutorial.html#build-the-workspace)

2. [Source your workspace](https://docs.ros.org/en/jazzy/Tutorials/Beginner-Client-Libraries/Colcon-Tutorial.html#build-the-workspace)

3. Add the plugins to your Gazebo world as defined in the section [Usage](#Usage) or by generating the Gazebo world model using the [scenery_builder](https://github.com/secorolab/scenery_builder)

4. Launch or start your Gazebo world

## Usage

These plugins were created to support the execution of sceneries defined with the FloorPlan DSL.
The objects model objects' kinematic chains and their states, as well as their placement in the world.

### Initial Joint State Plugin

To set the initial state of a joint, add this plugin inside a `<model>` tag:

```xml
<plugin name="initial_plugin" filename="libinitial_plugin.so">
    <joint></joint>
    <position></position>
</plugin>
```

Where:
* `<joint>`: string, name of the joint that will have a position set up
* `<position>`: float, position of the joint. If the joint is revolute it is interpreted as radians, and if it is prismatic it is interpreted as meters. 

### Time-Based Triggered Joint Position Plugin

This plugin enables the change of joint positions at sampled intervals. 
To add this plugin to an object, add the template below inside a `<model>` tag:

```xml
<plugin name="dynamic_joint_pluginss" filename="libdynamic_joint_plugin.so">
    <joint></joint>
    <uri></uri>
</plugin>
```

Where:
* `<joint>`: string, name of the joint that will have a position set up.
* `<uri>`: string, path to a json file that specifies the positions and timestamps at which the joint moves. If one sets `GZ_SIM_RESOURCE_PATH`, the URI can use relative paths that start with `file://`.

The json file has this format:

```json
{
    "keyframes": [
        {
            "position": 0,
            "time": 10.0
        }
    ]
}
```

Just like the `<position>` tag, if the joint is revolute the float value is interpreted as radians, and if it is prismatic it is interpreted as meters. The time is specified in seconds. 

### Distance-Based Triggered Joint Position Plugin

This plugin enables an object to change its joint position based when another object (e.g., a robot) reaches a distance to the base link.
For example, to open/close a door when a robot is near it.
To add this plugin to an object, add the template below inside a `<model>` tag:

```xml
<plugin name="adversarial_joint_plugin" filename="libadversarial_joint_plugin.so">
    <joint></joint>
    <start_joint_angle></start_joint_angle>
    <trigger_dist></trigger_dist>
    <end_joint_angle></end_joint_angle>
</plugin>
```

Where:
* `<joint>`: string, name of the joint that will have a position set up.
* `<start_joint_angle>`: float, similar to joint_position.
* `<trigger_dist>`: float, distance at which the joint goes from the before_trigger position to the after_trigger position, in meters.
* `<end_joint_angle>`: float, similar to joint_position.


# Acknowledgement

This work is part of a project that has received funding from the European Union's Horizon 2020 research and innovation programm SESAME under grant agreement No 101017258.

<p align="center">
    <img src="images/EU.jpg" alt="drawing" height="100"/>
    <img src="images/SESAME.jpg" alt="drawing" height="100"/>

</p>

