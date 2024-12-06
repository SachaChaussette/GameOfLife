#include "pch.h"
#include "Macro.h"

GAMEOFLIFELIB__API void Macro::Config()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	SetConsoleOutputCP(CP_UTF8);
	locale::global(locale("fr_FR"));
}

/* =============== System =============== */
void System::Sleep(const milliseconds& _duration)
{
	const auto _start = high_resolution_clock::now();
	sleep_for(_duration);
	const auto _end = high_resolution_clock::now();
	const duration<double, milli> _elapsed = _end - _start;
}

void System::MaximizeConsole()
{
	HWND Hwnd = GetForegroundWindow();
	int x = GetSystemMetrics(SM_CXSCREEN);
	int y = GetSystemMetrics(SM_CYSCREEN);
	LONG winstyle = GetWindowLong(Hwnd, GWL_STYLE);
	SetWindowLong(Hwnd, GWL_STYLE, (winstyle | WS_POPUP | WS_MAXIMIZE) & ~WS_CAPTION & ~WS_THICKFRAME & ~WS_BORDER);
	SetWindowPos(Hwnd, HWND_TOP, 0, 0, x, y, 0);
}


/* =============== Intéraction Utilisateur =============== */
int UserInteraction::GetInputByUser()
{
	int _key;
	if (_kbhit())
	{
		_key = _getch();
		return _key;
	}
	return 0;
}


/* =============== Random =============== */
int Random::GetRandomNumberInRange(const int _min, const int _max)
{
	random_device _rSeed; // Obtient un graine aléatoire
	mt19937 _gen(_rSeed()); // Initialise le générateur avec la graine
	uniform_int_distribution<> _distr(_min, _max); // Définit la distribution (les limites)

	return _distr(_gen); // Génération du nombre
}

Stream::StreamSystem::StreamSystem()
{
	openMode = ios_base::in;
	filePath = "None";
	stream = fstream(filePath,openMode);
}

/* =============== Flux =============== */
Stream::StreamSystem::StreamSystem(const string& _folderPath, const string& _filePath,
	const string& _fileExtension, const ios_base::openmode& _openMode)
{
	filePath = _folderPath + _filePath + "." + _fileExtension;
	openMode = _openMode;
	stream = fstream(filePath, openMode);
}

void Stream::StreamSystem::DisplayByRow(const int _rowsCount, const int _startIndex)
{
	if (!stream)
	{
		cerr << "ERREUR => Le fichier : " << filePath << " n'existe pas !\n";
		return;
	}

	string _line;
	for (int _index = 0; _index <= _startIndex + _rowsCount; _index++)
	{
		getline(stream, _line);

		if (_index >= _startIndex)
		{
			cout << _line << endl;
		}
	}
}

void Stream::StreamSystem::DisplayByChar(const int _charCount, const int _startIndex)
{
	char _char;

	if (!stream)
	{
		cerr << "ERREUR => Le fichier : " << filePath << " n'existe pas !\n";
		return;
	}

	stream.seekg(streampos(_startIndex));
	for (int _index = 0; _index < _charCount; _index++)
	{
		stream.get(_char);
		cout << _char << endl;
	}
}

void Stream::StreamSystem::WriteAtPosition(const string& _content, const streampos& _position)
{
	return WriteAtPosition(_content.c_str(), _content.size(), _position);
}

void Stream::StreamSystem::WriteAtPosition(const char* _content, const streamsize& _length, const streampos& _position)
{
	if (!IsValid()) return;

	string _remainingContent;
	if (_position != -1)
	{
		stream.seekp(_position);
		getline(stream, _remainingContent, '\0');

		stream.clear();
		stream.seekp(_position);
	}
	stream.write(_content, _length);

	if (!_remainingContent.empty())
	{
		stream.write(_remainingContent.c_str(), _remainingContent.size());
	}
}

string Stream::StreamSystem::ReadAtPosition(const streampos& _positionIndex, const streamsize& _lengthOfContent)
{
	if (!IsValid()) return "Erreur => Not Valid";


	string _content(_lengthOfContent, '\0');
	string _string(_lengthOfContent, '\0');

	if (_positionIndex != -1)
	{
		stream.seekg(_positionIndex);
	}

	while (getline(stream, _string))
	{
		for (u_int _index = 0; _index < _string.length(); _index++)
		{
			if (_index >= _lengthOfContent) break;

			_content[_index] += _string[_index];
		}

	}

	return _content;
}

void Stream::StreamSystem::Remove(const streampos& _position, const streamsize& _length)
{
	/*DISPLAY("Ne fonctionne pas", true);
	return;*/
	if (!IsValid())
	{
		DISPLAY("ERREUR => Remove : Not Valid", true);
		return;
	}

	string _remainingContent;

	stream.seekg(_position + _length);

	getline(stream, _remainingContent, '\0');

	stream.seekp(_position);
	stream.clear();
	stream = fstream(filePath, ios_base::out);

	if (!_remainingContent.empty())
	{
		stream.write(_remainingContent.c_str(), _remainingContent.size());
	}
}

u_int Stream::StreamSystem::ComputeLengthOfFile()
{
	stream.seekg(0, stream.end);
	u_int _length = u_int(stream.tellg());
	stream.seekg(0, stream.beg);
	return _length;
}

bool Stream::StreamSystem::IsValid()
{
	if (stream) return true;
	return false;
}

u_int Stream::StreamSystem::GetWordsNumber()
{
	if (!IsValid()) return 404;

	u_int _wordsCount = 0;
	string _string;

	while (getline(stream, _string))
	{
		for (u_int _index = 0; _index < _string.length(); _index++)
		{
			if (_string[_index] == ' ' || _string[_index] == '\n')
			{
				_wordsCount++;
			}
		}
	}

	return _wordsCount + 1; // +1 car à droite d'un espace il y a toujours un mot
}

u_int Stream::StreamSystem::GetLettersNumber()
{
	if (!IsValid()) return 404;

	u_int _lettersCount = 0;
	string _string;

	while (getline(stream, _string))
	{
		for (u_int _index = 0; _index < _string.length(); _index++)
		{
			if (_string[_index] != ' ')
			{
				_lettersCount++;
			}
		}
	}

	return _lettersCount;
}

void Stream::StreamSystem::Ignore()
{
	// change le mode d'ouverture en théorie
	stream.ignore();
}
