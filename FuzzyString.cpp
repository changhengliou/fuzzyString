#include "stdafx.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#define NOMINMAX
#include <windows.h>

/**
 * Jaro distance
 */
extern "C" __declspec(dllexport) double __cdecl jaro_distance(LPSTR s1, LPSTR s2, const int l1, const int l2)
{
	if (l1 == 0 && l2 == 0)
		return 1;
	if (l1 == 0 ^ l2 == 0)
		return 0;

	const int match_distance = std::max(l1, l2) / 2 - 1;

	const auto s1_matches = new bool[l1];
	auto s2_matches = new bool[l2];

	std::fill(s1_matches, s1_matches + l1, false);
	std::fill(s2_matches, s2_matches + l2, false);

	auto matches = 0;

	for (auto i = 0; i < l1; i++)
	{
		const int end = std::min(i + match_distance + 1, l2);
		for (auto k = std::max(0, i - match_distance); k < end; k++)
			if (!s2_matches[k] && s1[i] == s2[k])
			{
				s1_matches[i] = true;
				s2_matches[k] = true;
				matches++;
				break;
			}
	}

	if (matches == 0)
		return 0.0;

	double t = 0.0;
	unsigned int k = 0;

	for (unsigned int i = 0; i < l1; i++)
		if (s1_matches[i])
		{
			while (!s2_matches[k]) k++;
			if (s1[i] != s2[k]) t += 0.5;
			k++;
		}

	const double m = matches;

	delete[] s1_matches, s2_matches;
	return (m / l1 + m / l2 + (m - t) / m) / 3.0;
}

/**
 * Jaro-Wrinkler Distance
 */
extern "C" __declspec(dllexport) double __cdecl jaro_wrinkler_distance(LPSTR s1, LPSTR s2, const int l1, const int l2)
{
	double l = 0;
	double jaro_dis = jaro_distance(s1, s2, l1, l2);

	/* calculate common string prefix up to 4 chars */
	for (int i = 0; i < std::min(std::min(l1, l2), 4); i++)
		if (s1[i] == s2[i])
			l++;
	// scaling factor = 0.1
	jaro_dis = jaro_dis + l * 0.1 * (1.0 - jaro_dis);
	return jaro_dis;
}

/**
 * Levenshtein Distance
 */
extern "C" __declspec(dllexport) int __cdecl levenshtein_distance(LPSTR s1, LPSTR s2, int l1, const int l2)
{
	const auto column_start = static_cast<decltype(l1)>(1);

	const auto column = new decltype(l1)[l1 + 1];
	std::iota(column + column_start - 1, column + l1 + 1, column_start - 1);

	for (auto x = column_start; x <= l2; x++)
	{
		column[0] = x;
		auto last_diagonal = x - column_start;
		for (auto y = column_start; y <= l1; y++)
		{
			auto old_diagonal = column[y];
			auto possibilities = {
				column[y] + 1,
				column[y - 1] + 1,
				last_diagonal + (s1[y - 1] == s2[x - 1] ? 0 : 1)
			};
			column[y] = std::min(possibilities);
			last_diagonal = old_diagonal;
		}
	}
	auto result = column[l1];
	delete[] column;
	return result;
}

/**
 * Normalized levenshtein distance
 * normalize foumula => 1 - (edit distance / length of the larger of the two strings)
 */
extern "C" __declspec(dllexport) double __cdecl normalized_levenshtein_distance(
	LPSTR s1, LPSTR s2, const int l1, const int l2)
{
	if (l1 == 0 && l2 == 0) return 1;
	return 1.0 - static_cast<double>(levenshtein_distance(s1, s2, l1, l2)) / static_cast<double>(std::max(l1, l2));
}

extern "C" __declspec(dllexport) double __cdecl print(LPSTR str)
{
	std::cout << str << " " << strlen(str) << std::endl;
	return 3.14159;
}

void test(const std::string& x, const std::string& y)
{
	using namespace std;
	LPSTR a = const_cast<char *>(x.c_str()), b = const_cast<char *>(y.c_str());
	int l1 = x.size(), l2 = y.size();
	cout << "JARO DISTANCE = " << jaro_distance(a, b, l1, l2) << endl;
	cout << "JARO–WINKLER DISTANCE = " << jaro_wrinkler_distance(a, b, l1, l2) << endl;
	cout << "LEVENSHTEIN DISTANCE = " << levenshtein_distance(a, b, l1, l2) << endl;
	cout << "NORMALIZED LEVENSHTEIN DISTANCE = " << normalized_levenshtein_distance(a, b, l1, l2) << endl;
}

int main()
{
	using namespace std;
	while (true)
	{
		string x, y;
		cout << "ENTER A STR: ";
		cin >> x;
		if (x == "exit")
			return 0;
		cout << "ENTER ANOTHER STR: ";
		cin >> y;
		test(x, y);
	}
}
