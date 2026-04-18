#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
  const char* data;
  size_t count;
} sview;

sview sv(const char* Cstring)
{
  return (sview){ 
    .data = Cstring,
    .count = strlen(Cstring),
  };
}

void sview_chop_right(sview* sv, size_t n)
{
  if (sv->count == 0) return;
  sv->count -= n;
}

void sview_chop_left(sview* sv, size_t n)
{
  if (sv->count == 0) return;
  sv->count -= n;
  sv->data  += n;
}

void sview_trim_left(sview* sv)
{
  while (sv->count > 0 && isspace(sv->data[0])) {
    sview_chop_left(sv, 1);
  }
}

void sview_trim_right(sview* sv)
{
  while (sv->count > 0 && isspace(sv->data[sv->count-1])) {
    sview_chop_right(sv, 1); 
  }
}

void sview_trim(sview* sv)
{
  sview_trim_left(sv);
  sview_trim_right(sv);
}

sview slice(sview* sv, char separator)
{
  size_t i = 0;
  while (i < sv->count && sv->data[i] != separator) {
    ++i; 
  }
  if (i < sv->count) {
      sview result = {
        .data = sv->data,
        .count = i,
    };
    sview_chop_left(sv, i + 1);
    return result;
  }
  sview result = *sv;
  sview_chop_left(sv, sv->count);
  return result;
}
//TODO: Back converter - converts string view back to a Null terminated string for backward compatability
//      String view funtions:
//        casefold()
//        center(width, fillchar)
//        count(x, start, end)
//        encode(encoding, errors)
//        endswith(suffix, start, end)
//        expandtabs(tabsize)
//        find(sub, start, end)

sview Upper(sview sv)
{
  char buffer[512];
  strncpy(buffer, sv.data, sv.count);
  size_t i = 0;
  while (i < sv.count) {
    buffer[i] = toupper(sv.data[i]);
    ++i; 
  }
  sview result = {
    .data = buffer,
    .count = strlen(buffer),
  };
  return result;
}

sview Lower(sview sv)
{
  char buffer[512];
  strncpy(buffer, sv.data, sv.count);
  size_t i = 0;
  while (i < sv.count) {
    buffer[i] = tolower(sv.data[i]);
    ++i; 
  }
  sview result = {
    .data = buffer,
    .count = strlen(buffer),
  };
  return result;
}

void printsv(sview sv)
{
  #define STRING_VIEW_FORMATTING "%.*s"
  #define STRING_VIEW_ARGS(s) (s).count, (s).data
  printf(""STRING_VIEW_FORMATTING"\n", STRING_VIEW_ARGS(sv));
}

sview capitalize(sview str_view)
{
  char buffer[1024] = {};

  if (str_view.count > sizeof(buffer)) {
    sview error = sv("String are too long!"); 
    return error;
  }

  for (int i = 0; i < str_view.count; ++i) {
    if ((islower(str_view.data[i]) && isspace(str_view.data[i-1])) || 
        (islower(str_view.data[i]) && i == 0)) {
      buffer[i] = toupper(str_view.data[i]);
    }
    else {
      buffer[i] = str_view.data[i];
    }
  }

  sview result = {
    .data = buffer,
    .count = strlen(buffer),
  };
  return result;
}

char* converter(sview sv)
{
  char* result;
  return result;
}

sview center(sview sv, int width, char filler)
{
  char buffer[width];
  memset(buffer, filler, sizeof(buffer));
  buffer[width] = '\0';
  int base_center = sizeof(buffer) / 2;
  int sv_center = sv.count / 2;
  int true_center = base_center - sv_center;
  for (int i = 0; i < sv.count; ++i) {
    buffer[true_center + i] = sv.data[i];
  }
  sview result = {
    .data = buffer,
    .count = strlen(buffer),
  };
  return result;
}

sview left(sview sv, int width, char filler)
{
  char buffer[width];
  memset(buffer, filler, sizeof(buffer));
  buffer[width] = '\0';
  for (int i = 0; i < sv.count; ++i) {
    buffer[i] = sv.data[i];  
  }
  sview result = {
    .data = buffer,
    .count = strlen(buffer),
  };
  return result;
}

sview right(sview sv, int width, char filler)
{
  char buffer[width];
  memset(buffer, filler, sizeof(buffer));
  buffer[width] = '\0';
  int offset = width - sv.count;
  for (int i = 0; i < sv.count; ++i) {
    buffer[offset + i] = sv.data[i]; 
  }
  sview result = {
    .data = buffer,
    .count = sizeof(buffer),
  };
  return result;
}

int main(int argc, char* argv[])
{
  sview name = sv("Nick");
  printf("%s, its yours first clean C project, do you like it?\n", name.data);
  char buffer[64];
  fgets(buffer, sizeof(buffer),stdin);
  puts(buffer);
  sview test_slice = sv("This, and that");
  sview this = slice(&test_slice, ',');
  printf("|"STRING_VIEW_FORMATTING"|\n", STRING_VIEW_ARGS(this));
  sview upper = Upper(name);
  printf("|"STRING_VIEW_FORMATTING"|\n", STRING_VIEW_ARGS(upper));
  sview lower = Lower(name);
  printf("|"STRING_VIEW_FORMATTING"|\n", STRING_VIEW_ARGS(lower));
  printsv(lower);
  sview testSentance = sv("this is width string with multiple words that starts with lowercase words and this words will be capitalized after function call!");
  sview capitalized = capitalize(testSentance);
  printsv(capitalized);
  sview word = sv("uncomfortablylongwordlooooooooooonger");
  printsv(center(word, 50, '_'));
  printsv(left(word, 50, '_'));
  printsv(right(word, 50, '_'));
  return 0;
}
