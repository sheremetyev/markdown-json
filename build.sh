#!/bin/bash
xcodebuild -project markdown.xcodeproj -configuration Release
cp build/Release/markdown ./
