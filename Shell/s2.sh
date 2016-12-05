nrfl=0
if [ $2 -eq 1 2>/dev/null ]; then

	dirs=($( ls -1d */ 2>/dev/null ))
	nrfl=$( ls -al | grep "^-" | wc -l )
	for i in "${dirs[@]}"; do
		pushd $i > /dev/null
		bash $0 $i 1
		nrfl=`expr $nrfl + $?`
		popd > /dev/null 
	done

	exit $nrfl
else
	shdir=$(pwd)
	pushd $1 > /dev/null
	bash $shdir/$0 $1 1
	nrfl=`expr $nrfl + $?`
	popd > /dev/null
fi

echo "Sunt $nrfl fisiere in arborele de directoare!"