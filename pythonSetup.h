#include <Python.h>
wchar_t *p = NULL;

void setupInterpreter(char *argv[])
{
	wchar_t *program = Py_DecodeLocale(argv[0], NULL);
	if (program == NULL)
	{
		printf("Fatal error: Cannot decode argv[0]\n");
		exit(1);
	}
	Py_SetProgramName(argv[0]);
	Py_Initialize();
}

void shutdownInterpreter()
{
	Py_Finalize();
}
