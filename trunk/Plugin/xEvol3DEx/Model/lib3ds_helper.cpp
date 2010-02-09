#include "../stdafx.h"
#include "lib3ds/lib3ds.h"

static long wfileio_seek_func(void *self, long offset, Lib3dsIoSeek origin)
{
	FILE *f = (FILE*)self;
	int o;
	switch (origin) {
		case LIB3DS_SEEK_SET:
			o = SEEK_SET;
			break;

		case LIB3DS_SEEK_CUR:
			o = SEEK_CUR;
			break;

		case LIB3DS_SEEK_END:
			o = SEEK_END;
			break;

		default:
			assert(0);
			return(0);
	}
	return (fseek(f, offset, o));
}


static long wfileio_tell_func(void *self) 
{
	FILE *f = (FILE*)self;
	return(ftell(f));
}


static size_t wfileio_read_func(void *self, void *buffer, size_t size) 
{
	FILE *f = (FILE*)self;
	return(fread(buffer, 1, size, f));
}


static size_t wfileio_write_func(void *self, const void *buffer, size_t size) 
{
	FILE *f = (FILE*)self;
	return(fwrite(buffer, 1, size, f));
}

Lib3dsFile* lib3ds_file_wopen(const wchar_t * filename)
{
	FILE *f;
	Lib3dsFile *file;
	Lib3dsIo io;

	f = _wfopen(filename, L"rb");
	if (!f)
	{
		return NULL;
	}
	file = lib3ds_file_new();
	if (!file) 
	{
		fclose(f);
		return NULL;
	}

	memset(&io, 0, sizeof(io));
	io.self = f;
	io.seek_func  = wfileio_seek_func;
	io.tell_func  = wfileio_tell_func;
	io.read_func  = wfileio_read_func;
	io.write_func = wfileio_write_func;
	io.log_func = NULL;

	if (!lib3ds_file_read(file, &io)) {
		fclose(f);
		free(file);
		return NULL;
	}
	fclose(f);
	return file;
}