#define SVIEW_IMPLEMENTATION
#include "sview.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
  sview name = sv("Nick");
  printf("%s, its yours first clean C project, do you like it?\n", name.data);
  char buffer[64];
  fgets(buffer, sizeof(buffer),stdin);
  puts(buffer);
  sview test_slice = sv("This, and that");
  sview this = slice(&test_slice, ',');
  sview upr = upper(name);
  sview lwr = lower(name);
  printsv(upr);
  sview testSentance = sv("this is width string with multiple words that starts with lowercase words and this words will be capitalized after function call!");
  sview capitalized = capitalize(testSentance);
  printsv(capitalized);
  sview titled = title(testSentance);
  printsv(titled);
  sview word = sv("uncomfortablylongwordlooooooooooonger");
  printsv(center(word, 50, '_'));
  printsv(left(word, 50, '_'));
  printsv(right(word, 50, '_'));

  sview qwer = sv("qwer");
  sview asdf = sv("as");
  sview zxcv = sv("zxcvbibos");
  sview zxcv1 = sv("zxcvhaha");
  sview zxcv2 = sv("zxcvbeniz");
  sview zxcv3 = sv("zxcv");
  sview_array_t words = sv_array(&qwer, &asdf, &zxcv, &zxcv1, &zxcv2, &zxcv3);

  sview joined = join(&words, ","); 
  printsv(joined);

  sview src = sv("converTED");
  printf("Word:\t%s\nLenght:\t%i\n", src.data, src.count);
  sview suffix = sv("TED");
  bool endresult = endswith(src, suffix, 0, strlen(src.data));
  printf("%i\n", endresult);

  sview suffix2 = sv("son");
  bool startresult = startswith(src, suffix2, 1, strlen(src.data));
  printf("%i\n", startresult);
  sview findstr = sv("some Find some some substringsome");
  sview sub = sv("some");
  int index = find(findstr, sub, 0);
  printf("%i\n", index);
  int counter = count(findstr, sub, 0);
  printf("%i\n", counter);
  sview conv= sv("test");
  printf("Word:\t%s\nLenght:\t%i\n", src.data, src.count);

  char converted1[joined.count];
  converter(&joined, converted1, joined.count);
  printf(converted1);
  return 0;
}
