par=0
impar=0

contor=$#
cmd="echo "

while [ $contor -gt 0 ] ; do
cmd="$cmd \$$contor"
contor=`expr $contor - 1`
done

for i ; do
	if [ `expr $i % 2 ` -eq 0 ] ;then
		par=`expr $par + 1 `
	else 
		impar=`expr $impar + 1`
	fi
done

echo $cmd
eval $cmd

echo $par
echo $impar