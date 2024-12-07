#! /bin/bash

NM_CONFIGURATION=Hotspot
NM_DEVICE=wlan0

hotspot_start()
{
	sudo systemctl start hotspot.up@${NM_CONFIGURATION}
}

hotspot_stop()
{
	sudo systemctl start hotspot.down@${NM_DEVICE}
}

hotspot_enable()
{
	sudo systemctl start hotspot.down@${NM_DEVICE}.timer
	sudo systemctl start hotspot.up@${NM_CONFIGURATION}.timer
	hotspot_start
}

hotspot_disable()
{
	sudo systemctl stop hotspot.up@${NM_CONFIGURATION}.timer
	sudo systemctl stop hotspot.down@${NM_DEVICE}.timer
	hotspot_stop
}

wifi_status()
{
	echo hotspot status:	
	nmcli connection show | grep ${NM_CONFIGURATION}
	echo
	echo device status:
	nmcli connection show | grep ${NM_DEVICE}
	echo
	echo timers status:
	systemctl list-timers hotspot* -all
	echo
	echo commands are start/stop/disable

}

case $1 in
	start)
		hotspot_enable
		;;
	stop)
		hotspot_stop
		;;
	disable)
		hotspot_disable
		;;
	*)
		hotspot_status
		;;
esac
