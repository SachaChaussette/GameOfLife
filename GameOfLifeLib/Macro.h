#pragma once

#ifdef GAMEOFLIFELIB_EXPORTS
#define GAMEOFLIFELIB__API __declspec(dllexport)
#else
#define GAMEOFLIFELIB__API __declspec(dllimport)
#endif

#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <random>
#include <conio.h>
#include <thread>
#include "Color.h"
#include <vector>

#pragma region Define

#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define DISPLAY(_value, _isEndl)    cout << _value;\
                                    if (_isEndl) cout << endl;
#define CLEAR  cin.clear();\
               cin.ignore((numeric_limits<streamsize>::max)(), '\n');

#pragma endregion

using namespace std;
using namespace std::chrono_literals;
using namespace chrono;
using namespace this_thread;

#pragma region TypeDef

typedef unsigned int u_int;
typedef const unsigned int c_u_int;

#pragma endregion

namespace Macro
{
	GAMEOFLIFELIB__API void Config();
};

namespace System
{
	/// <summary>
	/// /!\ ATTENTION FONCTIONS BLOQUANTE /!\ Stop le programme pendant _duration millisecondes 
	/// </summary>
	/// <param name="_duration"></param>
	GAMEOFLIFELIB__API void Sleep(const milliseconds& _duration);

	GAMEOFLIFELIB__API void MaximizeConsole();
};

namespace UserInteraction
{
	/// <summary>
	/// Retourne l'input de l'utilisateur si il presse une touche
	/// </summary>
	/// <returns></returns>
	GAMEOFLIFELIB__API int GetInputByUser();


	/// <summary>
	/// Retourne un type de donnée donné par l'utilisateur avec une question
	/// </summary>
	/// <param name="_question"></param>
	/// <returns></returns>
	template<typename ReturnType>
	ReturnType Retrieve(const string& _question)
	{
		DISPLAY(_question, true);
		ReturnType _result;
		cin >> _result;
		CLEAR;
		return _result;
	}
}

namespace Random
{
	/// <summary>
	/// Retourne un nombre aléatoire en _min et _max
	/// </summary>
	/// <param name="_min"></param>
	/// <param name="_max"></param>
	/// <returns></returns>
	GAMEOFLIFELIB__API int GetRandomNumberInRange(const int _min, const int _max);
}

namespace Stream
{
	class StreamSystem
	{
		fstream stream;
		std::ios_base::openmode openMode;
		std::string filePath;
	public:
		GAMEOFLIFELIB__API string GetFilePath() const
		{
			return filePath;
		}
	public:
		GAMEOFLIFELIB__API StreamSystem();
		GAMEOFLIFELIB__API StreamSystem(const string& _folderPath, const string& _filePath,
			const string& _fileExtension, const ios_base::openmode&
			_openMode = ios_base::in | ios_base::out);

	public:
		/// <summary>
		/// Affiche un nombre _rowsCount de ligne contenus dans un fichier à partir du charactère _startIndex 
		/// </summary>
		/// <param name="_filePath"></param>
		/// <param name="_rowsCount"></param>
		/// <param name="_startIndex"></param>
		GAMEOFLIFELIB__API void DisplayByRow(const int _rowsCount, const int _startIndex);
		/// <summary>
		/// Affiche un nombre _charCount de charactère contenus dans un fichier à partir du charactère _startIndex 
		/// </summary>
		/// <param name="_filePath"></param>
		/// <param name="_charCount"></param>
		/// <param name="_startIndex"></param>
		GAMEOFLIFELIB__API void DisplayByChar(const int _charCount, const int _startIndex);
		/// <summary>
		/// Ecrit _content à partir de la position à la position _positionIndex
		/// </summary>
		/// <param name="_content"></param>
		/// <param name="_positionIndex"></param>
		/// <returns></returns>
		GAMEOFLIFELIB__API void WriteAtPosition(const string& _content, const streampos& _positionIndex);
		/// <summary>
		/// Ecrit _content à partir de la position à la position _positionIndex
		/// </summary>
		/// <param name="_content"></param>
		/// <param name="_lengthOfContent"></param>
		/// <param name="_position"></param>
		/// <returns></returns>
		GAMEOFLIFELIB__API void WriteAtPosition(const char* _content, const streamsize& _lengthOfContent, const streampos& _position);
		/// <summary>
		/// Retourne un string de taille _lengthOfContent à partir de la position _positionIndex
		/// </summary>
		/// <param name="_positionIndex"></param>
		/// <param name="_lengthOfContent"></param>
		/// <returns></returns>
		GAMEOFLIFELIB__API string ReadAtPosition(const streampos& _positionIndex, const streamsize& _lengthOfContent);
		GAMEOFLIFELIB__API void Remove(const streampos& _position, const streamsize& _length);
		/// <summary>
		/// Retourne la taille d'un fichier
		/// </summary>
		/// <returns></returns>
		GAMEOFLIFELIB__API u_int ComputeLengthOfFile();
		/// <summary>
		/// Vérifie si l'on peut ouvrir un fichier
		/// </summary>
		/// <returns></returns>
		GAMEOFLIFELIB__API bool IsValid();
		/// <summary>
		/// Retourne le nombre de mots dans un fichier
		/// </summary>
		/// <returns></returns>
		GAMEOFLIFELIB__API u_int GetWordsNumber();
		/// <summary>
		/// Retourne le nombre de lettres dans un fichier
		/// </summary>
		/// <returns></returns>
		GAMEOFLIFELIB__API u_int GetLettersNumber();

		GAMEOFLIFELIB__API void Ignore();

	};
}