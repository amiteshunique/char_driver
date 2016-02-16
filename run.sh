#! /bin/bash
insert()
{ 
	if ( make )
	then
		echo "Do you want to insert the module? [y/n]"
		read choice 
		if [ $choice = 'y' ]
		then
			insmod modules/lkm.ko
			lsmod | grep lkm
			read
			dmesg
			read
		fi
	else
		echo "make failed.. please check for build errors"
		exit
	fi
}

remove()
{
	echo "Do you want to remove the module? [y/n]"
	read choice 
	if [ $choice = 'y' ]
	then
		lsmod | grep lkm
		read
		rmmod lkm
		dmesg
		read
		make clean
	fi
}

commit() 
{
	echo "Do you want to commit the module? [y/n]"
	read choice 
	if [ $choice = 'y' ]
	then
		git log
		read
		git add '*.c'
		git add '*.h'
		git status
		read
		echo "Please enter your comments for this commit: "
		read comment
		git commit -m $comment
		git log
		read
		git status
		read
	fi
}

edit() 
{
	echo "Do you want to edit this module? [y/n]"
	read choice 
	if [ $choice = 'y' ]
	then
		make clean
		git status
		read
		git add '*.c'
		git add '*.h'
		read
	fi
}


echo "Starting the script:"
echo "Making the module now.."
insert
remove 
commit
edit







