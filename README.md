# Tor Cell Capture
Capture and logging of Tor cells.

## Overview
The Tor source code is modified to directly emit cell sequences to a logger using UDP packets for use in website fingerprinting experiments. Tor is run in docker and restarted for each website capture to completely reset any existing circuits.

## Requirements
### python modules
```
pip3 install -r requirements.txt
```
### geckodriver
```
wget https://github.com/mozilla/geckodriver/releases/download/v0.31.0/geckodriver-v0.31.0-linux64.tar.gz
tar xvzf geckodriver-v0.31.0-linux64.tar.gz
chmod +x geckodriver
mv geckodriver /usr/local/bin
```
### docker
follow installation directions according to [https://docs.docker.com/engine/install/](https://docs.docker.com/engine/install/)
## Setup
### Modified Tor Instance
The modified instance of Tor is used to emit cell sequences of a website trace to the cell logger. Tor is run in docker for this experiment.
#### Build docker container (run from inside tor directory)
```
docker build -t tor .
```
### Tor Cell Logger
The cell logger captures and stores the cells emitted from Tor for each trace in a .cell file delimited by a tab.
#### Build cell logger (run from inside cell-logger directory)
```
make
```
### Extensions
The controller will look for any extensions implemented using [Mozilla Browser Extensions](https://developer.mozilla.org/en-US/docs/Mozilla/Add-ons/WebExtensions) in the ./visit/addon/ directory. An example is [here]() which implements half duplex communication for the Walkie-Talkie and Tail Time website fingerprinting defenses. To add any extensions to the capture, place their .xpi file in the ./visit/addon/ directory.
### Dataset
The websites captured consists of those listed in the websites.txt file. A sample is provided. In the original work, the top 100 websites from the [Alexa Top Sites](http://s3.amazonaws.com/alexa-static/top-1m.csv.zip) (this link will start a download) are used after removing duplicate and localized domains. 
## Trace Capture
The controller will do 100 captures in batches of 25 for each website in the websites.txt file. It will do this for each extension in the ./visit/addon/ directory as well as once for normal traffic without any extensions.
```
bash controller.sh
```
## Sorting Traces
The traces need to be sorted since they are collected using UDP packets.
```
python3 sort-traces.py
```
## Reference
J. Liang, C. Yu, K. Suh and H. Han, "Tail Time Defense Against Website Fingerprinting Attacks," in IEEE Access, vol. 10, pp. 18516-18525, 2022, doi: [10.1109/ACCESS.2022.3146236](10.1109/ACCESS.2022.3146236).
