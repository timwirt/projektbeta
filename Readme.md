# Projekt Beta

## Projekt bauen
Um das Projekt zu bauen, soll einer der folgenden Befehlsketten im Projektverzeichnis ausgeführt werden.
In Linux Mint verwendet man
```
cmake -S . -B build && cmake --build build && cmake --install build
```
Auf den Rechnern der CIP Pools ist der Aufruf leider ein wenig länger
```
export PROJ_PATH=`pwd` && cmake -S $PROJ_PATH -B $PROJ_PATH/build && cmake --build $PROJ_PATH/build && cmake --install $PROJ_PATH/build
```
Falls das Projekt so nicht baut, liegen entweder Implementierungsfehler vor oder es fehlen externe Softwarepakete.

## Spiel vorbereiten

Zum Spielen werden mindestens drei Terminals/Konsolen gebraucht. Diese müssen alle im Verzeichnis `./projektbeta/extra`.

Als nächstes muss in einem Terminal der Server gestartet werden mit:
```
uvicorn bs_server:bs_server --port 8000 --reload
```
Nun können jeweils die Clients gestartet werden mit:
```
python3 bs_client.py
```

Das Spiel kann nun, wie im [Wiki](https://gitlab.informatik.uni-bonn.de/poose_locke_musa_wirtz/projektbeta/-/wikis/Schiffe-versenken-spielen) näher beschrieben, gespielt werden. Viel Spaß!
