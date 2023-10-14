# VRTrackerReader #

This project can read tracking data from OpenVR and sends it with UDP messages.

## How to use ##

Use flag -c to generate a configuration file: VRTrackerReader.ini
Change this configuration file to your needs.
Entrys with -1 will be ignored.
Currently supported are 3 trackers, so index 0, 1 and 2 are available.
This limit is arbitrary and can be easily modified.

The VRTrackerReader works with a modified version of https://github.com/is-centre/udp-ue4-plugin-win64
Just change the FUDPData data structure to match the entries:

- id
- position_x
- position_y
- position_z
- roll
- pitch
- yaw
- tracking_result

## How to build ##

```
git submodule update --init
```
Open Visual Studio.

If your run an older/newer version right-click the project -> properties -> general -> platform toolset and change it to your needs.

Build the application and run it.
