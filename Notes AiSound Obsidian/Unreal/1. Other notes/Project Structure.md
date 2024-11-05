C++ scripts are split into .cpp files and .h files.

.h are headers that define for the compiler what they will find in the .cpp file, where the actual code is.

![[Pasted image 20241105113851.png]]
- .h files 
	- That define things that should only be visible inside a module should be in a private folder
	- That define things that should be visible outside a singular module should be in a public folder
- All .cpp files should be in private folders