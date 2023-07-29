# C++ sample code written by Alexander Ken

This sample code presents C++ classes implementing dynamic multi-dimensional arrays with highly optimized ways of accessing and traversing elements or slices within these arrays.

## Main

Function "main" is located in the file Main.cpp. It invokes a serious of tests and usage examples of the presented classes.

## Tests

Test code is located in files TestArray.hpp and TestArray.cpp.

### Test cases

- Random access operator implemented via optimized helper access classes.
- Random access operator implemented via variadic function templates.
- Traversing arrays or array slices with passing a lambda (a functor) as an operation to be performed on the elements.

### Demonstration cases

- Using array view on memory managed outside of the array classes.
- Cloning

## Results

### Subscript operators
Write time: 1.02376 ns. - classics highly optimized access.

### Variadic function template
Write time: 0.94222 ns. - tends to be the faster one.

### Index visitor functor.
Write time: 1.02869 ns  - Very cool because it combines flexibility and speed.

### Expected output

		A small 3D array:
		0 1
		10 11
		20 21
		30 31
		40 41

		100 101
		110 111
		120 121
		130 131
		140 141

		Good copy.
		Good clone.

		Performance testing: number of iterations 100.
		### Testing array access method 1 (subscript operators).
		Number of dimensions: 4
		Array size: 100000000
		Method 1 write time: 1.02376 ns. (classics highly optimized)
		### Testing array access method 2 (variadic function template).
		Number of dimensions: 4
		Array size: 100000000
		Method 2 write time: 0.94222 ns. (tends to be the faster one)
		### Testing array access method 3 (index visitor functor).
		Number of dimensions: 4
		Array size: 100000000
		Method 3 write time: 1.02869 ns (very cool: flexibility and speed)
