chmod +x install-caddy.sh gen-waitress.sh deploy-waitress.sh deploy-caddy.sh
if [ "$#" -eq 0 ]; then
    echo "No installations, straight to deploying"
else
    pip install waitress
    ./install-caddy.sh
fi

echo "Generating Waitress"
./gen-waitress.sh
./deploy-waitress.sh
./deploy-caddy.sh
echo "Caddy deployed"