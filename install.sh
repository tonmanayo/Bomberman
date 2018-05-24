#!/usr/bin/env bash
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
		if !(dpkg -l | grep libglfw3) ;then
            sudo apt-get install libglfw3
        else
            echo libglfw3 already installed
		fi
		if !(dpkg -l | grep pkg-config) ;then
                    sudo apt-get install pkg-config
        else
            echo pkg-config already installed
        fi
        if !(dpkg -l | grep cmake) ;then
                    sudo apt-get install cmake
        else
            echo cmake already installed
        fi
        if !(dpkg -l | grep libglew-dev) ;then
                    sudo apt-get install libglew-dev
        else
            echo glew already installed
        fi
        if !(dpkg -l | grep libglm-dev) ;then
                    sudo apt-get install libglm-dev
        else
            echo libglm-dev already installed
        fi
        if !(dpkg -l | grep libfreetype6-dev) ;then
                    sudo apt-get install libfreetype6-dev
        else
            echo libfreetype6-dev already installed
        fi

		;;
esac

case "${machine}" in
	Mac)
		echo Checking if Homebrew is installed
        if which -s brew > /dev/null ; then
            echo Homebrew already installed
            echo Installing dependencies
            if brew ls --versions pkg-config  > /dev/null; then
                echo pkg-config already installed
            else
                echo Installing pkg-config
                brew install pkg-config
            fi

            if brew ls --versions cmake > /dev/null; then
                echo cmake already installed
            else
                echo Installing cmake
                brew install cmake
            fi
            if brew ls --versions glfw3 > /dev/null; then
                echo glfw3 already installed
            else
                echo Installing glfw3
                brew install glfw3
            fi
            if brew ls --versions glew > /dev/null; then
                echo glew already installed
            else
                echo Installing glew
                brew install glew
            fi
            if brew ls --versions glm > /dev/null; then
                echo glm already installed
            else
                echo Installing glm
                brew install glm
            fi
            if brew ls --versions freetype > /dev/null; then
                echo freetype already installed
            else
                echo Installing freetype
                brew install freetype
            fi
        else
            echo Installing Homebrew
            sh -c "$(curl -fsSL https://raw.githubusercontent.com/Tolsadus/42homebrewfix/master/install.sh)"
            echo Please open a new terminal and run "make" again.
        fi

		;;
esac
