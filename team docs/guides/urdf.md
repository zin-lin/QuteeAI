# <center>URDF File Notes</center>

> URDF files ("Unified Robot Description Format") are used as a common language to describe robots based on XML format  
>
> Download URDF extension in VSCode 

---  

### <center>Table of Contents</center>  
|Item|Heading|Sub Contents|
|:---:|:---:|:---:|
| **1.** | [Reference Links](#reference-links) ||
| **2.** | [Overview](#overview) | [Common joint types](#common-joint-types) |
| **3.** | [Template With Description](#template-with-description) | |
| **4.** | [Including Xacro](#including-xacro) | [Main file](#main-file),<br>[Included file](#included-file),<br>[Placeholders](#placeholders),<br>[Xacro notes](#xacro-notes) |

<br>

[⬆ Table of Contents ⬆](#urdf-file-notes)    

---  

### <u>Reference Links</u>  

[ROS-Industrial Website: Intro to URDF](https://industrial-training-master.readthedocs.io/en/melodic/_source/session3/Intro-to-URDF.html)
[Wiki.ros.org/urdf](https://wiki.ros.org/urdf)  

[⬆ Table of Contents ⬆](#urdf-file-notes)    

---  

### <u>Overview</u>  

* A robot consists of `links` and `joints`  
* First break physical robot down into separate `links`  
* If two separate parts can move independently of each other, then they will need to be separate `links`. They should also be separate, if two parts make sense to be separate (e.g. removable modules or sensors)  
* Choose where origin for `link` should be, doesn't matter too much with exception of rotating links which should be at center of rotation  
* Connections between `links` are `joints` which describe where `links` are in space  
* Each `link` apart from the first one, will have a corresponding `joint` that says what other `link` its connected to (its parent), and how its connected to the parent  
* Each `link` can only have one parent, but can have many child `links`  

<br>  

* Describing joints, we need to specify what **type** of motion it has.  

##### Common joint types:  

|Type|Description|Example|
|:--|:--|:--|
| Revolute | Rotational motion about a point with a fixed start and stop angle (fixed limits) | Robotic arm |
| Continuous | Rotational motion about a point with no fixed limits (can spin freely forever) | A wheel or spinning gripper that can rotate continously |
| Prismatic | Linear translational motion | Linear actuator that moves along a rod |
| Fixed | Child link doesn't move relative to the parent link | Typically used for convenience where it makes more sense to keep things separated |  

[⬆ Table of Contents ⬆](#urdf-file-notes)  

---  

### <u>Template With Description:</u>  

`<?xml version="1.0"?>`  - xml declaration  
`<robot name="my_robots_name">` - root tag, [see including xacro](#including-xacro)  

<br>  

&emsp;- good practice to add suffex `_link` to name  
&emsp;`<link name="my_links_name_link">` - represents one link (**typically the first link will be empty and to the world**)  

&emsp;&emsp;`<visual>` - represents what is seen in sim (multiple `visual` tags can be specified per `link` tag for different aspects of link)  
&emsp;&emsp;&emsp;`<geometry>` - represents overall shape (box, cylinder, sphere or path to 3D mesh)  
&emsp;&emsp;&emsp;`<origin>` - offset for `geometry`, so it doesn't have to be centred around links origin  
&emsp;&emsp;&emsp;`<material>` - basically the colour  
&emsp;&emsp;`</visual>`  

&emsp;&emsp;`<collision>` - used for physics collision calculations, (multiple `collision` tags can be specified per `link` tag for different aspects of link)  
&emsp;&emsp;&emsp;- usually same as visual, but may be simpler for computational purposes  
&emsp;&emsp;&emsp;`<geometry>` - similar to in `visual`  
&emsp;&emsp;&emsp;`<origin>` - similar to in `visual`  
&emsp;&emsp;`<collision>`  

&emsp;&emsp;`<inertial>` - used in physics calculations, specifies how link responds to forces  
&emsp;&emsp;&emsp;`<mass>`  
&emsp;&emsp;&emsp;`<origin>` - centre of mass / gravity (balancing point)  
&emsp;&emsp;&emsp;`<inertia>` - rotational inertia matrix (how distribution of mass affects rotation of the link)  
&emsp;&emsp;`</inertial>`  

&emsp;`</link>`  

<br>

&emsp;- good practice to add suffex `_joint` to name  
&emsp;`<joint name="my_joints_name_joint" type="motion_type">` - represents one joint ([see join types](#common-joint-types))  

&emsp;&emsp;- which `links` this `joint` defines a relationship between  
&emsp;&emsp;- this joint is effectively defining the location of the child link with respect to the parent link  
&emsp;&emsp;`<parent link="parent_link_name" />`  
&emsp;&emsp;`<child link="child_link_name" />`  
&emsp;&emsp;`<origin xyz="0.25 0 0.15" rpy="0 0 0" />`  - defines relationship between the two links before any motion is applied  

&emsp;&emsp;- Following is not required for a fixed joint  
&emsp;&emsp;`<axis xyz="0 -1 0" />` - which axis the joint moves along or around  
&emsp;&emsp;`<limit lower="0" upper="${pi/2}" velocity="100" effort="100" />` - determines physical limitations of the joint (upper/lower in radians or meters, velocity in radians/sec or meters/sec, effort limit in newtons or newton meters)

&emsp;`</joint>`  

<br>  

&emsp;`<link name="another_links_name_link">`  
&emsp;`</link>`  

&emsp;`<joint name="another_joints_name_joint">`  
&emsp;`</joint>`  

&emsp;`...`  

`</robot>`   

[⬆ Table of Contents ⬆](#urdf-file-notes)  

---  

### <u>Including Xacro</u>  

> ROS provides a tool called Xacro  
> Makes URDF files easier to use and share  
> Allows files to be split up (e.g. core structure / sensors / materials / macros etc.)  
> Allows properties to be defined and then used by name  

<br>  

##### Main file:  
* Filename extension should be `filename.urdf.xacro`  

`?xml version="1.0"?>`  
`<robot xmlns:xacro="http//www.ros.org/wiki/xacro" name="my_robots_name">` - include xacro  

&emsp;`<xacro:include filename="file_to_include_path.xacro" />` - includes another file  

&emsp;`<link> ... </link>`  

&emsp;`<joint> ... </joint>`  

&emsp;`<link> ... </link>`  

&emsp;`<joint> ... </joint>`  

`</robot>`  

<br>  

##### Included file:  
* Filename extension should be `filename.xacro`  

`?xml version="1.0"?>`  
`<robot xmlns:xacro="http//www.ros.org/wiki/xacro">` - include xacro (name not required)  

&emsp;- example defining a material  
&emsp;`<material name="white">`  
&emsp;&emsp;`color rgba="1 1 1 1"/>`  
&emsp;`</material>`  

`</robot>`  

<br>  

##### Placeholders:  

> Placeholders can be used in a variety of ways  

* Properties:
`<xacro:property name="arm_radius" value="0.5." />` - define a property  
`...`  
`<cylinder radius="${arm_radius}" length="7" />` - using a property value by name  

<br>  

* Mathematical Operations:  
`<cylinder length="${4*arm_radius + pi}">` - Uses the value defined in the property (above)  

<br>  

* Macros:  
  * Define a template:
  `<xacro:macro name="inertial_box" params="mass x y z *origin">`  
  &emsp;`<inertial>`  
  &emsp;&emsp;`<xacro:insert_block name="origin" />`  
  &emsp;&emsp;`<mass value="${mass}" />`
  &emsp;&emsp;`<inertia ixx="${(1/12) * mass * (y*y+z*z)}"`  
  &emsp;&emsp;&emsp;*other entries skipped* `/>`  
  &emsp;`</iniertial>`  
  `</xacro:macro>`  
  * Use the template:  
  `<xacro:inertial_box mass="12" x="2" y="3" z="4">`  
  &emsp;`<origin xyz="0 2 4" rpy="0 0 0" />`  
  `</xacro:inertial_box>`  
  * Expands to:  
  `<inertial>`  
  &emsp;`<origin xyz="0 2 4" rpy="0 0 0" />`  
  &emsp;`<mass value="12" />`  
  &emsp;`<inertia ixx="25" />`  
  `</inertial>`  

<br>  

##### Xacro notes:

* When files include Xacro and we want to use the URDF file(s), need to first run xacro program  
* The single URDF output file can then be fed directly into the ROS `robot_state_publisher` which then will publish the URDF file on the `/robot_description` topic  
* This can be done with a launch file   

[⬆ Table of Contents ⬆](#urdf-file-notes)  

---  