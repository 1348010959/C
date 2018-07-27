/*
实现一个函数，将一个字符串中的每个空格替换成“%20”。
例如，当字符串为We Are Happy.则经过替换之后的字符串为We%20Are%20Happy。
*/

void replaceSpace(char *str, int length) {
	if (str == NULL || length < 0)
		return;
	int space = 0;
	int newlength = 0;
	for (int i = 0; str[i] != '\0'; ++i)
	{
		++newlength;
		if (str[i] == ' ')
			++space;
	}
	int len = newlength + 2 * space;
	char* str1 = str + newlength;
	char* str2 = str + len;
	while (str1 < str2)
	{
		if (*str1 == ' ')
		{
			*str2-- = '0';
			*str2-- = '2';
			*str2-- = '%';
		}
		else{
			*str2-- = *str1;
		}
		--str1;
	}
}