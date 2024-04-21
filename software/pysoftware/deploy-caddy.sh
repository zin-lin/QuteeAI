#!/usr/bin/bash

sudo systemctl daemon-reload
sudo systemctl enable caddy
sudo systemctl restart caddy 
sudo cp Caddyfile /etc/caddy/Caddyfile
sudo systemctl reload caddy
