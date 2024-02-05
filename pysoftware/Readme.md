# Damian's way for deploying https and http -VM name/VM IP with Caddy and Flask/wsgi-waitress

- Step 0: make sure `your_repo` in `gen-waitress.sh` is `your repo address/folder` and `Description=Your App Name` is actually `Description= whatever_you_want_to_name_it` and also in your Caddyfile change `xx` to `your napier machine no. example 23`
- Step 1: run 
```powershell
chmod +x run.sh
```
- Step 2: run the file
if installation needed/ run for the first time
```powershell
./run.sh install
```
else run after changes
```
./run.sh
```
