langdetect
================================================================

Este proyecto es un programa simple para identificar el idioma de un texto. Usa palabras comúnes para identificarlo. Estas palabras se llaman "stop words" y usan por buscadores para eliminar palabras sin importancia a la búsqueda. En Español, palabras como "el" y "la" son "stop words". He incluido listas para muchos idiomas. 

El objectivo para este proyecto es para ganar experiencia con la lengua C y para aprender como crear y matener un programa de sistemas diferences (como Windows, Linux, OSX, etc.). 

El idea para este proyecto originalmente era del subreddit /r/dailyprogrammer. Sin embargo, he creado un programa gráfico que funciona en muchos sistemas diferentes. Puede funcionar en Windows, OS X, Linux, y BeOS/Haiku. 

Para ejecutarse el programa, necesitas tener CMake. También, necesitas tener GTK+ 2 y pkg-config si tienes Linux o otros sistemas Unix (no es necesario para OS X). 

Como ejecutarse
==================================================================

de la raíz de la proyecto:

	mkdir build && cd build
	cmake ..
	cmake --build .

sin embargo, para OS X, haz:

	mkdir build && cd build
	cmake -GXcode ..
	cmake --build .

si no, el programa no se ejecutará


(Me disculpo por errores de la tradución) 
