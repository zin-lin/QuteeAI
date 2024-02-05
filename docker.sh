#!/bin/bash

# run docker commands
docker build -t cypress:latest .
docker run  -it --rm cypress:latest