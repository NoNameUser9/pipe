#include <iostream>
#include <cstdio>
#include <cstdarg>
#include <filesystem>

class Truba
{
	FILE *_pipe;
public:
	Truba() = default;
	~Truba()
	{
		flush();
		pclose(_pipe);
	}

	void
	pipe(const std::string &in)
	{
		try
		{
			if (!popen(in.c_str(), "w"))
				throw std::runtime_error("pipe doesn't created");

			_pipe = popen(in.c_str(), "w");
		}
		catch (std::exception &ex)
		{
			ex.what();
		}
	}

	int
	printf(const char *in, ...)
	{
		if(!_pipe)
			return 0;

		va_list args;
		va_start(args, in);
		int result = vfprintf(_pipe, in, args);
		va_end(args);

		return result;
	}

	void
	flush()
	{
		fflush(_pipe);
	}
};

int
main()
{
	Truba tube{};

//	std::filesystem::path dp = R"(C:\Users\User\PycharmProjects\vvv)";
//	std::filesystem::current_path(dp);
//	std::cout << std::filesystem::current_path();

	tube.pipe("cmd ../venv/Scripts/activate.bat\n");
	tube.printf("pip install -r ../requirements.txt\n");
	tube.printf("python ../main.py\n");

	auto c = "4\n4\n"
			 "0\n1\n"
			 "0\n2\n"
			 "0\n3\n"
			 "1\n2\n";
	tube.printf(c);

	tube.flush();

	return 0;
}
