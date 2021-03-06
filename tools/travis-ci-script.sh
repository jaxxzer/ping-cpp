#!/usr/bin/env bash

echo "Generate messages.."
git submodule update --init
src/generate/generate-message.py --output-dir src/message

echo "Run test.."
if ! g++ test/test-message.cpp -std=c++11 -pedantic -Wall -Werror -Wshadow -Wnarrowing -Wconversion; then
    echo " Failed to compile test-message!"
    exit 1
fi
if ! ./a.out; then
    echo " Failed to run test-message!"
    exit 1
fi

# because these files are gitignored, we have to force-add and commit them
# in order to move them to the deployment branch. every other approach I tried
# clobbered the files
git add src/message -f
git commit -m temporary-commit
git checkout deployment
git checkout HEAD@{1} src/message
git diff --staged
git commit -m "update autogenerated files"

if [ ! -z ${TRAVIS_PULL_REQUEST} ] && [ ${TRAVIS_PULL_REQUEST} == "false" ]; then
    echo "Updating deployment branch.."
    git remote set-url origin https://${GITHUB_TOKEN}@github.com/bluerobotics/ping-cpp
    git push origin
else
    echo "PR detected, no deployment will be done."
fi
