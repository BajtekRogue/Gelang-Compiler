make
./build/ge ./baseExamples/x.imp ./compiledExamples/x.mr
if [ $? -eq 0 ]; then
    ./build/vm/vm ./compiledExamples/x.mr
fi