#!/bin/bash
xcodebuild -project markdown-json.xcodeproj -configuration Release
cp build/Release/markdown-json ./
