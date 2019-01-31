# darpasubt
Subterenean challenge n00bs

## Things to note.
This repository contains everything for the challenge (heck google does it too - one repo for everything). The root of this repository in itself is a catkin workspace.
The organization of this repo is as follows:

````
/ui - contains the GUI for path tracking of entities. [WIP] Maintained by @marcus-pzj
/firmware - contains the source of our UWB firmware. Maintained by @lekook
/src - contains ROS packages. Maintained by @arjo129
````

## Relevant ROS packages

The `src` folder contains a number of packages. These packages include:
 - `octomap_*` - These packages are for 3D mapping. They are taken from octomap_mapping
 - `generic_msgs` - These contain our custom message. For instance UWB, etc... 
 - `uwb_package` - Contains ROS side UWB driver. Maintained by @lws803
 - `optical_flow` - [??] Optical flow stuff from @pdotdeep
 - `subt_seed` - for localization related stuff (TODO: Rename to localization_daemon). Maintained by @arjo129.
 - `ynav` - Navigation package. Maintained by @LeeYiyuan.

