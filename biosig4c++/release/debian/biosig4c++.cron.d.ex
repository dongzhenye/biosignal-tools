#
# Regular cron jobs for the biosig4c++ package
#
0 4	* * *	root	[ -x /usr/bin/biosig4c++_maintenance ] && /usr/bin/biosig4c++_maintenance
