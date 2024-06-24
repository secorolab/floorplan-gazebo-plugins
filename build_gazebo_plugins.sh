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

# Install ROS packages for Gazebo
sudo apt-get install ros-jazzy-gz-*-vendor

if ! grep -q "source /opt/ros/jazzy/setup.bash" ~/.bashrc; then
    echo "source /opt/ros/jazzy/setup.bash" >> ~/.bashrc
    echo "GAZEBO_PLUGIN_PATH added to ~/.bashrc"
else
    echo "GAZEBO_PLUGIN_PATH is already in ~/.bashrc"
fi

# Function to build a plugin
build_plugin() {
    plugin_src=$1
    plugin_name=$(basename "$plugin_src" .cc)
    build_dir="build/$plugin_name"
    echo "Building plugin $plugin_name from $plugin_src..."
    mkdir -p $build_dir || { echo "Failed to create build directory $build_dir"; exit 1; }
    cd $build_dir || { echo "Failed to enter directory: $build_dir"; exit 1; }
    cmake ../.. || { echo "CMake configuration failed for $plugin_src"; exit 1; }
    make || { echo "Make failed for $plugin_src"; exit 1; }
    cd - > /dev/null
    echo "Plugin $plugin_name built successfully."
}

# Function to add plugin path to Gazebo plugin path
add_plugin_path() {
    build_dir=$1
    echo "Adding $build_dir to Gazebo plugin path..."
    GAZEBO_PLUGIN_PATH=$build_dir:$GAZEBO_PLUGIN_PATH
    echo "GAZEBO_PLUGIN_PATH updated successfully."
    echo "GAZEBO_PLUGIN_PATH is $GAZEBO_PLUGIN_PATH."
}

# Function to update .bashrc with GAZEBO_PLUGIN_PATH if not already present
update_bashrc() {
    if ! grep -q "export GAZEBO_PLUGIN_PATH=" ~/.bashrc; then
        echo "export GAZEBO_PLUGIN_PATH=$GAZEBO_PLUGIN_PATH" >> ~/.bashrc
        echo "GAZEBO_PLUGIN_PATH added to ~/.bashrc"
    else
        echo "GAZEBO_PLUGIN_PATH is already in ~/.bashrc"
    fi
}

# Install cmake if not installed
install_cmake

# Find all plugin .cc files in the src directory
plugin_files=($(find ./src -type f -name "*.cc"))

# Build each plugin
for plugin_file in "${plugin_files[@]}"; do
    build_plugin $plugin_file
done

# Add each plugin build directory to the Gazebo plugin path
for plugin_file in "${plugin_files[@]}"; do
    plugin_name=$(basename "$plugin_file" .cc)
    build_dir="$PWD/build/$plugin_name"
    add_plugin_path "$build_dir"
done

update_bashrc

source ~/.bashrc

echo "All plugins built and paths added to GAZEBO_PLUGIN_PATH."

