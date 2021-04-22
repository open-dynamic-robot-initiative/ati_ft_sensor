# ati_ft_sensor
<img src="doc/images/ati_mini_40.jpg" width="400"><br>  
Interface using internet connexion to a ATI force torque sensor.

### Installation:

#### Standard dependencies:

This package is based on [ROS2](https://docs.ros.org/).
Currently based on ROS2 dashing release.

#### Download the pacakge:

Use the `git clone` or [treep](https://gitlab.is.tue.mpg.de/amd-clmc/treep)
using the [treep_machines_in_motion](https://github.com/machines-in-motion/treep_machines_in_motion) configuration.

In both case do not forget to **register your ssh public key in your settings**:

In short you can either:
  - use git directly:
  ```
  mkdir -p ~/devel/workspace/src
  cd ~/devel/workspace/src
  git clone git@github.com:open-dynamic-robot-initiative/ati_ft_sensor.git
  ```
  - or use treep:
  ```
  mkdir -p ~/devel
  pip install treep
  cd ~/devel
  git clone git@github.com:machines-in-motion/treep_machines_in_motion.git
  treep --clone ati_ft_sensor
  ```

#### Build the package

We use [colcon](https://github.com/machines-in-motion/machines-in-motion.github.io/wiki/use_colcon)
to build this package:
```
cd mkdir -p ~/devel/workspace
colcon build
```

### Usage:

#### Usage with rt_preempt

The ATI FT sensor is connected to the control PC via ethernet. To access the ATI sensor from the computer, the network connection to the ATI sensor must be configured.

- On Ubuntu, open the application "Network connections"
- Note: Please use ifconfig to identify the port to which the ethernet cable is connected.  To verify if you have identified the right port check if the Hwaddress in Ifconfig matches with the number in the General tab in "Network connection".
- If you configure the wrong port, the computer will not be able to connect with the internet. In which case, undo your changes and run "sudo service network-manager restart".
- Select the network connection which connects to the ATI sensor. For this, you might want to use the command ifconfig on the terminal to see the mac addresses of all network devices.
- In the "IPv4 Settings" tab, specify the network connection parameters manually by switching "Method" to "Manual" and put in the values as shown in the screenshot  
<img src="doc/images/editing_wired_connection.png" width="600"><br>  

##### Note
> :warning: In case you installed an LDAP account on the computer be aware that configuring the network card as described above might cause problems when rebooting the PC. Particularly, Ubuntu tries to connect to LDAP using the network settings from the hardcoded network connection, which will not work. In case you experience such issues at the first logging after a reboot, remove the network connection to the ATI sensor and restart the computer. Once you manged to login, connect the ATI sensor again.

> :warning: In case after restart you are unable to login to you LDAP account, please login to administrator(if you do not know the login credentials, please ask Vincent), and run "sudo service network-manager restart" and then restart the computer if necessary.  

> :warning: In case of socket error: ```cannot bind socket, error: 98, Address already in usehardware communication loop started```  
Make sure all of the processes for dynamic_graph and ATI sensor are killed.

#### Demos/Examples

### Hardware specs
You can find more detail in below links:
- [Datasheet](doc/mini_40_datasheet.pdf)
- [Technical Drawing Sensor](doc/mini_40_drawing.pdf)
- [Thecnical Drawing Net Box](doc/net_box_9105_a.jpg)
- [Quote](20170925_angebot_schunk_ati_sensor.PDF)
- [Schunk - Exclusive Distributor](20170925_schreiben_exklusivvertrieb_schunk.pdf)
- [Quick Start Guide](9610-05-1022%20Quick%20Start.pdf)  
These information is downloaded from the manufacturer's website, and we are not responsible for any information that they contain.

### Authors

- Ludovic Righetti
- Alexander Herzog
- Maximilien Naveau
- Elham Daneshmand

### License and Copyrights

License BSD-3-Clause
Copyright (c) 2019, New York University and Max Planck Gesellschaft.
