find . -type f -exec bash -c 'mv "$1" "${1/B1/MuStopped}"' bash {} \;

