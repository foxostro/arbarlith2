#!/bin/sh
# Run this command from the make_a_mac_app_bundle directory to finish the
# partial app bundle. The end result is an app bundle that can be used on
# Mac OS X.
ditto -VV "../redist" "./Arbarlith2.app/Contents/Resources"

