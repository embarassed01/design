#include "common-vc.h"

int w_get_file_size(const wchar_t * in_file)
{
	int file_size = 0;
	FILE * f_in;
	
	f_in = _wfopen(in_file, _T("rb"));
	if (f_in == NULL)
	{
		fprintf(stderr, "error: open the file\n");
		return -1;
	}
	fseek(f_in, 0L, SEEK_END);
	file_size = ftell(f_in);
	fseek(f_in, 0L, SEEK_SET);
	fclose(f_in);

	return file_size;
}

void w_read_file(const wchar_t * in_file, u8 * out_buf, const int out_buf_size)
{//文件大小不超过2G
	FILE * f_in;
	int file_size = 0;
	int read_len = 0;

	// open file for in:
	f_in = _wfopen(in_file, _T("rb"));
	if (f_in == NULL)
	{
		fprintf(stderr, "error: open the file\n");
		return;
	}

	// get file size:
	fseek(f_in, 0L, SEEK_END);
	file_size = ftell(f_in);
	fseek(f_in, 0L, SEEK_SET);
	if (file_size >= out_buf_size)
	{
		fprintf(stderr, "error: file size large than buf size\n");
		fclose(f_in);
		return;
	}

	// read file:
	read_len = fread(out_buf, 1, file_size, f_in);
	if (read_len != file_size)
	{
		fprintf(stderr, "error: real read size no-equal file size\n");
		fclose(f_in);
		return;
	}

	// detect is or not file-end:
	//	bool feof()
	if ( !feof(f_in) )
	{
		fprintf(stderr, "error: fail to read until file-end\n");
		fclose(f_in);
		return;
	}

	// detect is or not file-error:
	//   bool ferror()
	if ( ferror(f_in) )
	{
		fprintf(stderr, "error: read file occur fatal error\n");
		fclose(f_in);
		return;
	}

	// close fp:
	fclose(f_in);

	return;
}

void w_write_file(const wchar_t * out_file, const u8 * in_buf, const int towrite_size)
{//文件大小不超过2G
	FILE * f_out;
	int write_len = 0;

	// open file for out:
	f_out = _wfopen(out_file, _T("wb"));
	if (f_out == NULL)
	{
		fprintf(stderr, "error: open the file\n");
		return;
	}

	// write file:
	write_len = fwrite(in_buf, 1, towrite_size, f_out);
	if (write_len != towrite_size)
	{
		fprintf(stderr, "error: real write size no-equal towrite buf size\n");
		fclose(f_out);
		return;
	}

	// detect is or not file-error:
	//   bool ferror()
	if ( ferror(f_out) )
	{
		fprintf(stderr, "error: write file occur fatal error\n");
		fclose(f_out);
		return;
	}

	// close fp:
	fclose(f_out);

	return;
}