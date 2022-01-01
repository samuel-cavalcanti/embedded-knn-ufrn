echo 'run test chosen'
gcc tests/test_chosen.c knn/chosen/*   -lm -o test_chosen
./test_chosen
rm test_chosen

echo 'run test knn'
gcc tests/test_knn.c knn/chosen/chosen.c knn/chosen/chosen.h  knn/knn.c knn/knn.h dataset/dataset.h -lm -o test_knn
./test_knn
rm test_knn

echo 'check compilation with avr'
avr-gcc knn/chosen/chosen.c knn/chosen/chosen.h  knn/knn.c knn/knn.h dataset/dataset.h main.c -lm hardware/hardware_abstraction_layer.h hardware/arduino_uno.c -o avr_test
rm avr_test

echo 'done'