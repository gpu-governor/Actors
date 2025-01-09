#!/bin/bash

echo "Installing dependencies..."

# Update package lists
# sudo apt update

# Install Python 3
echo "Installing Python 3..."
sudo apt install -y python3 python3-pip

# Install Pygame using pip
echo "Installing Pygame..."
sudo apt install python3-pygame

echo "All dependencies have been installed successfully!"
