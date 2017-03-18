#compile
make

#run tests
./testfile

# check test status
testcode=$?
if (( $testcode == 0))
then
#run main if tests passed
./main
fi
