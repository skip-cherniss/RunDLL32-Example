
#include <iostream>
#include <string>
#include "myCode.h"

namespace myCode {
    using namespace std;

    void sayHello()
    {
        //using namespace std;
        cout << "Hello, world!" << endl;
    }
}

namespace myStrings {
    using namespace std;
    
    /// <summary>
    /// return the number of delimiters in the string
    /// </summary>
    /// <param name="str">the string to search</param>
    /// <param name="delimeter">the delimiter to search for</param>
    /// <returns>the # of delimiters</returns>
    int delimiter_count(string str, char delimeter)
    {
        int count = 0;
        for (int i = 0; i < str.length(); i++)
        {
            if (str[i] == delimeter)
                count++;
        }
        return count;
    }

    /// <summary>
    /// This function will split the string by the supplied delimiter. Make sure to initialize the return array to size.
    /// </summary>
    /// <param name="str">the string to split</param>
    /// <param name="delimiter">the delimiter</param>
    /// <param name="results"></param>
    void split(string str, char delimiter, string *results)
    {
        int currIndex = 0, i = 0;
        int startIndex = 0, endIndex = 0;
        while (i <= str.length())
        {
            if (str[i] == delimiter || i == str.length())
            {
                endIndex = i;
                string subStr = "";
                subStr.append(str, startIndex, endIndex - startIndex);
                results[currIndex] = subStr;
                currIndex += 1;
                startIndex = endIndex + 1;
            }
            i++;
        }
    }

}    // namespace myCode
