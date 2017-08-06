unameOut="$(uname -s)"
case "${unameOut}" in
	Linux*)     machine=Linux;;
	Darwin*)    machine=Mac;;
	CYGWIN*)    machine=Cygwin;;
	MINGW*)     machine=MinGw;;
	*)          machine="UNKNOWN:${unameOut}"
esac
echo ${machine}

case "${machine}" in
	Linux)
		if !(dpkg -l | grep sdl) ;then
			sudo apt-get install libsdl2-dev
		fi
		;;
esac
