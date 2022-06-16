#!/bin/sh

WEBSERV_DIR=${ls | grep webserv}
if [ ! $1 ]
then
	echo "Specify the webserv project directory."
	exit
elif [ $1 != webserv ]
fi

echo "tatata"
