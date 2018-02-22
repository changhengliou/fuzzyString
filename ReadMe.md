# FuzzyString
### A C++ Library for fuzzy string matching

### C# Usage
In order to use it in C#, you need to call it from dll library, So first build the project in Visual Studio. In **Solution Explorer**, right click on project, under project default choose **Dynamic library** as your **Configuration Type**.

Finally, if you want to compile code in X64 mode, be sure to open your C# project and select **Build**, and unchecked **Prefer-32bit**
In your C# code, do the following to import fuzzy string matching function.

```C#
[DllImport(DllPath, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Auto)]
static extern double jaro_distance([MarshalAs(UnmanagedType.LPStr)] string x, [MarshalAs(UnmanagedType.LPStr)] string y, int l1, int l2);
```

### Result
1 means fully matched, 0 is fully unmatched. NORMALIZED LEVENSHTEIN DISTANCE is derived from `1 - edit_distance / max(str1.size(), str2.size())`
```
ENTER A STR: 12345
ENTER ANOTHER STR: 54321
JARO DISTANCE = 0.466666666666667
JARO WRINKLER DISTANCE = 0.52
LEVENSHTEIN DISTANCE = 4
NORMALIZED LEVENSHTEIN DISTANCE = 0.2
```
