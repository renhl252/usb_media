
extern int scan_folder_files(char * path);
int main(int argc, char *argv[])
{
	scan_folder_files(argv[1]);
	return 0;
}
