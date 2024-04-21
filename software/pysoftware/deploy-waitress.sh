#!/usr/bin/bash

sudo cp webtech.service /etc/systemd/system/
sudo systemctl daemon-reload
sudo systemctl enable webtech.service
sudo systemctl restart webtech.service

if [ $? -eq 0 ]
then
    echo "Waitress service deployed successfully"
else
    echo "Something went wrong"
fi
