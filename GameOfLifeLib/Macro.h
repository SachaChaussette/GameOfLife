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

namespace Iterator
{
	/// <summary>
	/// Retourne la localisation du fichier.
	/// </summary>
	/// <param name="_folderPath"></param>
	/// <param name="_filePath"></param>
	/// <param name="_fileExtension"></param>
	/// <returns></returns>
	static string ComputeFilePath(const string& _folderPath, const string& _filePath, const string& _fileExtension)
	{
		return _folderPath + _filePath + "." + _fileExtension;
	}

	template<class ValueType, class SizeType = u_int>
	class InflowIterator
	{
		ifstream stream;
		istream_iterator<ValueType> startIt;
		istream_iterator<ValueType> endIt;
	public:
		InflowIterator() = default;
		/// <summary>
		/// Constructeur pour lire le contenu d'un fichier.
		/// ValueType doit être 'string' pour afficher des mots et 'char' pour afficher des lettres 
		/// </summary>
		/// <param name="_filePath"></param>
		InflowIterator(const string& _filePath)
		{
			stream = ifstream(_filePath);
			startIt = stream;
		}
	public:
		/// <summary>
		/// Affiche tous les items dans le fichier entre _startIndex et _stopIndex.
		/// 'string' pour afficher des mots et 'char' pour afficher des lettres 
		/// </summary>
		/// <param name="_startIndex"></param>
		/// <param name="_stopIndex"></param>
		void DisplayItemInRangeOfFile(const SizeType& _startIndex, const SizeType& _stopIndex)
		{
			if (_startIndex > _stopIndex)
			{
				DISPLAY("ERREUR => DisplayItemInRangeOfFile : Ton stopIndex ne peut pas être plus petit que ton startIndex !!!", true);
				return;
			}

			// Afficher le Nième mot choisi par l'utilisateur d'un fichier
			istream_iterator<ValueType> _it = startIt;

			for (SizeType _currentIndex = 0;
				_currentIndex != _startIndex && _it != istream_iterator<ValueType>();
				_it++, _currentIndex++); // permet d'aller à l'index voulu sans corps

			for (SizeType _index = 0; _index < _stopIndex - _startIndex; _index++)
			{
				cout << *_it++ << endl;
			}
		}
		ValueType GetContentAtIndexOfFile(const SizeType& _colIndex, const SizeType& _rowIndex)
		{
			/*istream_iterator<ValueType> fileIterator(stream);

			advance(fileIterator, _index);

			return *fileIterator;*/

			string line;
			int current_row = 0;

			while (getline(stream, line))
			{
				// Si on est à la ligne voulue
				if (current_row == _rowIndex)
				{
					return line[_colIndex]; // Retourne le caractère à la colonne souhaitée
				}
				current_row++;
			}


		}
		/// <summary>
		/// Affiche tous les items de l'index _index jusqu'à la fin du fichier.
		/// 'string' pour afficher des mots et 'char' pour afficher des lettres 
		/// </summary>
		/// <param name="_index"></param>
		void DisplayItemAtIndexTileEndOfFile(const SizeType& _index = 0, const bool _endl = true)
		{
			// Afficher le Nième mot choisi par l'utilisateur d'un fichier
			istream_iterator<ValueType> _it = startIt;

			for (SizeType _currentIndex = 0;
				_currentIndex != _index && _it != istream_iterator<ValueType>();
				_it++, _currentIndex++); // permet d'aller à l'index voulu sans corps

			while (_it != endIt)
			{
				cout << *_it++;
				if (_endl) cout << endl;
			}
		}
		/// <summary>
		/// Renvoie le nombre d'items de l'index _index jusqu'à la fin du fichier.
		/// 'string' pour le nombre de mots et 'char' pour le nombre de lettres 
		/// </summary>
		/// <param name="_index"></param>
		/// <returns></returns>
		SizeType GetContentNumberAtIndexTileEndOfFile(const SizeType& _indexToAdd = 0)
		{
			istream_iterator<ValueType> _it = startIt;
			SizeType _wordsCount = 0;

			for (SizeType _currentIndex = 0;
				_currentIndex != _indexToAdd && _it != istream_iterator<ValueType>();
				_it++, _currentIndex++); // permet d'aller à l'index voulu sans corps

			while (_it != endIt)
			{
				_wordsCount++;
				_it++;
			}

			return _wordsCount;
		}
	};

	/// <summary>
	/// Ecrit le contenu '_content' dans le flux '_stream' avec une delimitation '_delim' entre chaque contenu
	/// </summary>
	/// <param name="_content"></param>
	/// <param name="_stream"></param>
	/// <param name="_delim"></param>
	template<typename ContentType, typename DelimType>
	void WriteContent(const string& _filePath, const ContentType& _content, const DelimType* const _delim = nullptr)
	{
		ofstream _stream = ofstream(_filePath);
		ostream_iterator<ContentType> _it = ostream_iterator<ContentType>(_stream, _delim);
		*_it = _content;
	}
	/// <summary>
	/// Ecrit une collection de contenu comme vector<string>
	/// </summary>
	/// <typeparam name="Alloc"></typeparam>
	/// <param name="_collection"></param>
	template<template<typename, typename> typename Collection, typename ContentType, typename Alloc = allocator<ContentType>, typename DelimType = char>
	void WriteContents(const string& _filePath, const Collection<ContentType, Alloc>& _collection, const DelimType* const _delim = nullptr)
	{
		ofstream _stream = ofstream(_filePath);
		ostream_iterator<ContentType> _it(_stream, _delim);

		const u_int& _size = _collection.size();

		for (u_int _index = 0; _index < _size; _index++)
		{
			*_it = _collection[_index];
		}
	}

}

namespace Stream
{
	class GAMEOFLIFELIB__API StreamSystem
	{
		fstream stream;
		ios_base::openmode openMode;
		string filePath;
	public:
		string GetFilePath() const
		{
			return filePath;
		}
	public:
		StreamSystem() = default;
		StreamSystem(const string& _folderPath, const string& _filePath,
			const string& _fileExtension, const ios_base::openmode&
			_openMode = ios_base::in | ios_base::out);

	public:
		/// <summary>
		/// Affiche un nombre _rowsCount de ligne contenus dans un fichier à partir du charactère _startIndex 
		/// </summary>
		/// <param name="_filePath"></param>
		/// <param name="_rowsCount"></param>
		/// <param name="_startIndex"></param>
		void DisplayByRow(const int _rowsCount, const int _startIndex);
		/// <summary>
		/// Affiche un nombre _charCount de charactère contenus dans un fichier à partir du charactère _startIndex 
		/// </summary>
		/// <param name="_filePath"></param>
		/// <param name="_charCount"></param>
		/// <param name="_startIndex"></param>
		void DisplayByChar(const int _charCount, const int _startIndex);
		/// <summary>
		/// Ecrit _content à partir de la position à la position _positionIndex
		/// </summary>
		/// <param name="_content"></param>
		/// <param name="_positionIndex"></param>
		/// <returns></returns>
		void WriteAtPosition(const string& _content, const streampos& _positionIndex);
		/// <summary>
		/// Ecrit _content à partir de la position à la position _positionIndex
		/// </summary>
		/// <param name="_content"></param>
		/// <param name="_lengthOfContent"></param>
		/// <param name="_position"></param>
		/// <returns></returns>
		void WriteAtPosition(const char* _content, const streamsize& _lengthOfContent, const streampos& _position);
		/// <summary>
		/// Retourne un string de taille _lengthOfContent à partir de la position _positionIndex
		/// </summary>
		/// <param name="_positionIndex"></param>
		/// <param name="_lengthOfContent"></param>
		/// <returns></returns>
		string ReadAtPosition(const streampos& _positionIndex, const streamsize& _lengthOfContent);
		void Remove(const streampos& _position, const streamsize& _length);
		/// <summary>
		/// Retourne la taille d'un fichier
		/// </summary>
		/// <returns></returns>
		u_int ComputeLengthOfFile();
		/// <summary>
		/// Vérifie si l'on peut ouvrir un fichier
		/// </summary>
		/// <returns></returns>
		bool IsValid();
		/// <summary>
		/// Retourne le nombre de mots dans un fichier
		/// </summary>
		/// <returns></returns>
		u_int GetWordsNumber();
		/// <summary>
		/// Retourne le nombre de lettres dans un fichier
		/// </summary>
		/// <returns></returns>
		u_int GetLettersNumber();

		void Ignore();

	};
}