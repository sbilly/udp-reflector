#!/bin/bash

# Start the physical channels, each in their own console window
gnome-terminal -x physical_channel/Debug/physical_channel p_ccpch
gnome-terminal -x physical_channel/Debug/physical_channel s_ccpch
gnome-terminal -x physical_channel/Debug/physical_channel d_dpcch
gnome-terminal -x physical_channel/Debug/physical_channel hs_scch

# Start the transport channels, each in their own console window
gnome-terminal -x transport_channel/Debug/transport_channel bch
gnome-terminal -x transport_channel/Debug/transport_channel pch
gnome-terminal -x transport_channel/Debug/transport_channel fach
gnome-terminal -x transport_channel/Debug/transport_channel dch
gnome-terminal -x transport_channel/Debug/transport_channel hs_dsch

# Start the logical channels, each in their own console window
gnome-terminal -x logical_channel/Debug/logical_channel bcch
gnome-terminal -x logical_channel/Debug/logical_channel pcch
gnome-terminal -x logical_channel/Debug/logical_channel ccch
gnome-terminal -x logical_channel/Debug/logical_channel ctch
gnome-terminal -x logical_channel/Debug/logical_channel dcch
gnome-terminal -x logical_channel/Debug/logical_channel dtch
gnome-terminal -x logical_channel/Debug/logical_channel mcch
gnome-terminal -x logical_channel/Debug/logical_channel mtch



