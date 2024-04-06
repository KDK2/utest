#!/bin/bash

# Execute build command and check result
make -j4 -C /home/test/Robot/KeenOn/utest/Debug || { echo "Build failed, not committing or pushing."; exit 1; }

# If the build is successful, execute Git commands
git -C /home/test/Robot/KeenOn/utest add .
git -C /home/test/Robot/KeenOn/utest update-index --assume-unchanged KeenOnControl.pro.user
git -C /home/test/Robot/KeenOn/utest commit -m "Successful build"
git -C /home/test/Robot/KeenOn/utest push
