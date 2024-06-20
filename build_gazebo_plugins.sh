#!/bin/bash

# Function to install cmake if not installed
install_cmake() {
    if ! command -v cmake &> /dev/null; then
        echo "cmake not found, installing..."
        sudo apt-get update
        sudo apt-get install -y cmake
        echo "cmake installed successfully."
    else
        echo "cmake is already installed."
    fi
}

sudo apt-get install ros-jazzy-gz-*-vendor

# Function to build a plugin
build_plugin() {
    plugin_dir=$1
    echo "Building plugin in $plugin_dir..."
    mkdir -p $plugin_dir/build || { echo "Failed to create build directory in $plugin_dir"; exit 1; }
    cd $plugin_dir/build || { echo "Failed to enter directory: $plugin_dir/build"; exit 1; }
    cmake ../ || { echo "CMake configuration failed for $plugin_dir"; exit 1; }
    make || { echo "Make failed for $plugin_dir"; exit 1; }
    cd - > /dev/null
    echo "Plugin in $plugin_dir built successfully."
}

# Function to add plugin path to Gazebo plugin path
add_plugin_path() {
    plugin_path=$1
    echo "Adding $plugin_path to Gazebo plugin path..."
    export GAZEBO_PLUGIN_PATH=$plugin_path:$GAZEBO_PLUGIN_PATH
    echo "GAZEBO_PLUGIN_PATH updated successfully."
}

# Install cmake if not installed
install_cmake

# Find all plugin directories ending with "-plugin"
plugin_dirs=($(find . -type d -name "*-plugin"))

# Build each plugin
for plugin_dir in "${plugin_dirs[@]}"; do
    build_plugin $plugin_dir
done

# Add each plugin to the Gazebo plugin path
for plugin_dir in "${plugin_dirs[@]}"; do
    add_plugin_path "$PWD/$plugin_dir/build"
done

echo "All plugins built and paths added to GAZEBO_PLUGIN_PATH."

